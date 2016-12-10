#ifndef _LANGEVIN_HPP_
#define _LANGEVIN_HPP_
#include <string>
#include <fstream>


namespace langevin2D {
  const int MAX_AXIS_SIZE = 256;//trying to overcome a bad_alloc error I keep getting 
  const int MAX_GRID_SIZE = 256*256;//trying to overcome a bad_alloc error I keep getting


  class Langevin {

    public:

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

      void tabulate_forces();//tabulates the LJ forces for a range of distances
      double calc_force(double dist, double sig12, double sig6, double coeff);//calculates LJ tabulation forces 
      /* calc_force takes in sig12 and sig6 as input because they'll never change
       * throughout the loop it's called in
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


    Langevin() : sigma(1.0), epsilon(1.0), table_dists(), table_forces(), r_cut(2.5), x_axis(), y_axis(), f_x(), f_y() {}
    };

}
#endif // _LANGEVIN_HPP_
