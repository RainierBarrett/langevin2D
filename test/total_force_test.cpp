#include "langevin2D.hpp"
#include "catch.hpp"
#include <iomanip>
#include <string>
#include <vector>

using namespace langevin2D;

SCENARIO( "Can get total (external) forces on particles using neighbor lists.", "[tot_forces]"){

  GIVEN( "The 3-particle input file, and an integrator."){
    Langevin integrator;
    double EPSILON = 0.005;//for double "comparisons"
    
    WHEN("We call the get_tot_force_x() and get_tot_force_y() methods."){
      std::string param_file = "test_params_3_part.txt";
      integrator.read_params(param_file);//now we should have 3 particles and neighbor lists
      double fx0 = integrator.get_tot_force_x(0);//this takes in the particle index
      double fy0 = integrator.get_tot_force_y(0);
      double fx1 = integrator.get_tot_force_x(1);
      double fy1 = integrator.get_tot_force_y(1);
      double fx2 = integrator.get_tot_force_x(2);
      double fy2 = integrator.get_tot_force_y(2);

      THEN( "The forces should be correct."){
	REQUIRE(integrator.particles[0]->x < EPSILON);
	REQUIRE(abs(fx0 - 24 * ((float)1/pow(2.0, 13) - (float)1/pow(2.0, 7))) < EPSILON);
	REQUIRE(abs(fx2 + fx0 + fx1) < EPSILON);//forces should sum to 0 in this system
      }
    }
  }
}
