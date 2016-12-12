#include "langevin2D.hpp"
#include "catch.hpp"
#include <iomanip>

using namespace langevin2D;

SCENARIO( "Can get x and y force projections on a pair of particles.", "[forces]"){

  GIVEN( "An integrator (for the table) and a pair of particles."){
    Langevin integrator;
    double EPSILON = 0.000005;//for double "comparisons"
    double eps = 1.0;//the LJ epsilon value.
    double sig = 1.0;
    Particle* p1 = new Particle(1.0, 1.0, 0.0, 0.0);
    Particle* p2 = new Particle(2.0, 1.0, 0.0, 0.0);
    Particle* p3 = new Particle(1.0, 2.0, 0.0, 0.0);
    
    WHEN("We call the force_x and force_y functions."){
      integrator.set_epsilon(eps);
      integrator.set_sigma(sig);
      integrator.tabulate_forces();//this will tabulate the LJ force table
      double distance1_2 = integrator.calc_dist(p1, p2);
      double distance1_3 = integrator.calc_dist(p1, p3);
      double force1_2 = integrator.read_force(distance1_2);
      double force1_3 = integrator.read_force(distance1_3);

      THEN( "The values should be correct."){
	REQUIRE(abs(force1_2 - integrator.calc_x_force(p1, p2, force1_2, distance1_2)) < EPSILON);//all in x
	REQUIRE(integrator.calc_y_force(p1, p2, force1_2, distance1_2) < EPSILON);//should be 0 in y direction
	REQUIRE(abs(integrator.calc_y_force(p1, p3, force1_3, distance1_3) - force1_3) < EPSILON);//all in y
	REQUIRE(integrator.calc_x_force(p1, p3, force1_3, distance1_3) < EPSILON);//should be 0 in x direction
      }
    }
  }
}
