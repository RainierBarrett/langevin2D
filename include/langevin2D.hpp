#ifndef _LANGEVIN_HPP_
#define _LANGEVIN_HPP_
#include <string>
#include <fstream>
#include <boost/random.hpp>
#include <boost/random/normal_distribution.hpp>
#include <vector>


namespace langevin2D {
  const int MAX_AXIS_SIZE = 256;//trying to overcome a bad_alloc error I keep getting 
  const int MAX_GRID_SIZE = 256*256;//trying to overcome a bad_alloc error I keep getting
  const int MAX_NUM_PARTICLES = 256;//with the grid limit, I likewise limit num_particles

  class Particle{
  public:
    double x;//the particle's x-position
    double y;//the particle's y-position
    double v_x;//the particle's x-velocity
    double v_y;//the particle's y-velocity

    //Setters
    void set_x(double new_x, double min = -10.0, double max = 10.0);
    void set_y(double new_y, double min = -10.0, double max = 10.0);
    void set_v_x(double new_v_x);
    void set_v_y(double new_v_y);

    //Constructor from input values
    Particle(double x_init, double y_init, double v_x_init, double v_y_init){
      x = x_init;
      y = y_init;
      v_x = v_x_init;
      v_y = v_y_init;
    }
    
  };


  class Langevin {
    /*This is the main class for this project. It is the integrator and grid keeper class.
     *It tracks the list of all particles in a given simulation with an array of pointers to Particle's.
     *It is also responsible for reading input.
     */

  public:
    //The grid itself
    double x_axis[MAX_AXIS_SIZE];//the array of x-axis values 
    double y_axis[MAX_AXIS_SIZE];//the array of y-axis values 
    double f_x[MAX_GRID_SIZE];//the 1D x-forces array-- need to get 1D indices for each lookup
    double f_y[MAX_GRID_SIZE];//the 1D y-forces array-- need to get 1D indices for each lookup

    //Simulation grid parameters      
    double dx, dy;//dx and dy can differ, but I only allow 'regular' intervals
    int nx, ny;//the number of x and y gridpoints, and the total amount.
    //no setters since they're filled directly from ifstream
    int ntot;//the total number of grid points
      
    //Simulation grid parameter setters
    void set_dx();//sets dx. Called during initial setup only
    void set_dy();//sets dy. Called during initial setup only
    void set_ntot(int new_ntot);//sets total number of gridpoints

    //The list of particles within the simulation
    Particle* particles[MAX_NUM_PARTICLES];

    //The number of particles present
    int num_particles;

    //Langevin parameters-- none of these have setters because they're read in directly
    double T;//The simulation temperature
    double lambda;//The lambda value for the integrator
    double dt;//the timestep size
    double time;//the amount of time the sim has progressed
    double tot_time;//the total time over which the sim will run

    //LJ parameters
    double epsilon;//the epsilon value for the Lennard-Jones particle-particle interactions
    double sigma;//the sigma value for the Lennard-Jones particle-particle interactions
    void set_sigma(double new_sig);//setter for sigma
    void set_epsilon(double new_eps);//setter for epsilon

    /* Here I treat the LJ particle interactions using an efficient table lookup with
     * cheap interpolation for value-finding. If I just calculate a whole bunch of values
     * when I initialize the simulation, the whole thing goes faster in the long run.
     * I just chose 1024 to have 'a lot' of points on the table. Note that the force table
     * is calculated based on total distance, i.e., using inter-particle radii. This is
     * addressed by projecting along the x and y axes at runtime. Saves on the LJ potential
     * calculation, since I only need two values (distance, angle) to find x and y
     * force projections among each particle pair, and they're symmetric, so I only need to calculate
     * and lookup once per pair considered.
     */
    double r_cut;//the cutoff distance we'll be using for our neighbor list. Defaults to 2.5 
    double table_dists[1024];//the 'x' values of our force table 
    double  table_forces[1024];//the 'y' values of our force table
    /*The neighbor list is an array of vectors of ints. Each vector corresponds to a particle
     *based on its index in the particles[] array. Each entry in a given vector is an index of
     *a particle that is a neighbor with the "owner particle" of that array, i.e. the one
     *whose index matches that of the vector. I chose vectors because it allows me to only
     *consider the particles in a given particle's neighbor list in an intuitive way, by just
     *removing the ones that are no longer in the neighbor list.
     */
    std::vector<int> neighbor_list[MAX_NUM_PARTICLES];

    void make_neighbor_lists();//this function actually generates the neighbor lists.
    void update_neighbor_lists();//this updates the neighbor lists at each necessary step.

    void tabulate_forces();//tabulates the LJ forces for a range of distances
    double calc_force(double dist, double sig12, double sig6, double coeff);//calculates LJ tabulation forces 
    /* calc_force takes in sig12 and sig6 as input because they'll never change
     * throughout the loop it's called in
     */

    //given two particles + the radial force and distance  between them, returns the x-projection
    double calc_x_force(Particle* p1, Particle* p2, double force_tot, double dist);
    //this does the same as calc_x_force, but for the y-projection
    double calc_y_force(Particle* p1, Particle* p2, double force_tot, double dist);

    //given a particle index, sums up all its external x-forces from other particles
    //need to add in force field contribution also
    double get_tot_force_x(int idx);

    //given a particle index, sums up all its external y-forces from other particles
    //need to add in force field contribution also
    double get_tot_force_y(int idx);

    
    double calc_dist(Particle* p1, Particle* p2);//calculates the Euclidean distance between two Particles

    //The force table reading function. Takes in particle distances, returns the force.
    double read_force(double dist);

    /*
      //these don't work right now for an unknown reason. I'm moving on without dealing with it
    //retrieves the x-index of a particle's x-position on the grid (rounds down)
    int get_x_idx(Particle* p);

    //retrieves the y-index of a particle's y-position on the grid (rounds down)
    int get_y_idx(Particle* p);
    */

    /* 
     * Here's the stuff for reading input files
     */
    //The name of the parameter file, and its fstream
    std::string param_file_name;
    std::ifstream param_file;

    //The name of the potential file, and its fstream
    std::string potential_file_name;
    std::ifstream potential_file;

    //The name of the particle list file, and its fstream
    std::string particle_file_name;
    std::ifstream particle_file;

    //this is the function to read the parameter file
    void read_params(std::string name);

    //this is the function to read the potential file
    void read_potential(std::string name);

    //this is the function to read the particle file
    void read_particles(std::string name);

    //these are the RNG-related things. Eta is the Gaussian process for the Langevin dynamics.
    boost::mt19937 rng;
    double eta();
    void seed_rng();


    Langevin() : sigma(1.0), epsilon(1.0), table_dists(), table_forces(), r_cut(2.5), x_axis(), y_axis(), f_x(), f_y(), num_particles(1) {}
  };

}
#endif // _LANGEVIN_HPP_
 
