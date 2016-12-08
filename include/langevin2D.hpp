#ifndef _LANGEVIN_HPP_
#define _LANGEVIN_HPP_
#include <string>
#include <fstream>


namespace langevin2D {

    class Langevin {

    public:
      double** axes;//the array to hold the two axes we'll use
      double* x_axis;//the array of x-axis values
      double* y_axis;//the array of y-axis values

      //Simulation grid parameters      
      double dx, dy;//dx and dy can differ, but I only allow 'regular' intervals
      
      //Simulation grid parameter setters
      void set_dx();//sets dx. Called during initial setup only
      void set_dy();//sets dy. Called during initial setup only

      //Langevin parameters
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
       * calculation, since I only need two operations (distance, angle) to find x and y
       * projections among each particle pair.
       */
      double r_cut;//the cutoff distance we'll be using for our neighbor list. Defaults to 2.5
      double table_dists[1024];//the 'x' values of our force table 
      double  table_forces[1024];//the 'y' values of our force table

      void tabulate_forces();//tabulates the LJ forces for a range of distances
      double calc_force(double dist, double sig12, double sig6, double coeff);//calculates LJ tabulation forces 

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
      void read_params(std::string param_file);


      Langevin() : sigma(1.0), epsilon(1.0), table_dists(), table_forces(), r_cut(2.5) {}
    };

}
#endif // _LANGEVIN_HPP_
