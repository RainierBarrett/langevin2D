#include "langevin2D.hpp"

namespace langevin2D {


  
  void Langevin::set_epsilon(double new_eps){
    epsilon = new_eps;
    return;
  }

  void Langevin::set_sigma(double new_sig){
    sigma = new_sig;
    return;
  }
  
}
