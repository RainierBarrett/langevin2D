#include "langevin2D.hpp"
#include <math.h>
#include <limits>
#include <fstream>

namespace langevin2D {


  
  void Langevin::set_epsilon(double new_eps){
    epsilon = new_eps;
    return;
  }

  void Langevin::set_sigma(double new_sig){
    sigma = new_sig;
    return;
  }

  double Langevin::calc_force(double dist, double sig12, double sig6, double coeff){
    double dist3 = pow(dist, 3);
    double dist6 = dist3*dist3;
    double dist7 = dist6*dist;
    double dist12 = dist6*dist6;
    double dist13 = dist12*dist;
    return(coeff * (2 * sig12 / dist13 - sig6 / dist7));
  }
  
  void Langevin::tabulate_forces(){
    static double inf = std::numeric_limits<double>::infinity();
    double coeff = -24.0 * epsilon;
    double sig3 = pow(sigma, 3);
    double sig6 = sig3*sig3;
    double sig12 = sig6*sig6;
    double current_dist = 0;
    table_dists[0] = 0.0;
    table_forces[0] = inf;
    int i = 1;
    for(i; i < 1024; i++){
      current_dist = (double)(i/1024.0) * r_cut;
      table_dists[i] = current_dist;
      table_forces[i] = calc_force(current_dist, sig12, sig6, coeff);
    }
  }

  void Langevin::read_params(std::string name){
    using namespace std;
    ifstream pfile;
    pfile.open(name.c_str());
    pfile >> this->T >> lambda >> dt >> tot_time >> potential_file_name >> particle_file_name;
    if(!pfile.is_open()){printf("THAT IS NOT GOOD \n");}
    printf("hi there, this is a test %f\n", this->T);
    pfile.close();
  }
  
}
