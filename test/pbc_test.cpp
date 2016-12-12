#include "langevin2D.hpp"
#include "catch.hpp"
#include <iomanip>

using namespace langevin2D;

SCENARIO( "Can properly handle PBC x and y setting.", "[PBCs]"){

  GIVEN( "A grid and a particle."){
    Langevin integrator;
    double EPSILON = 0.000005;//for double "comparisons"
    std::string testfile = "test_params_1_part.txt";
    
    WHEN("We set the particle's position outside the bounds."){
      integrator.read_params(testfile);
      integrator.particles[0]->set_x(7.0, -5.0, 5.0);//these are out of bounds
      integrator.particles[0]->set_y(-6.0, -5.0, 5.0);

      THEN( "The particle position should 'loop'."){
	REQUIRE(abs(integrator.particles[0]->x + 3.0) < EPSILON);
	REQUIRE(abs(integrator.particles[0]->y - 4.0) < EPSILON);
      }
    }
  }
}
