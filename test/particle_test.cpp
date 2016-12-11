#include "langevin2D.hpp"
#include "catch.hpp"
#include <iomanip>

using namespace langevin2D;

SCENARIO( "Can update particle values.", "[particle]"){

  GIVEN( "A particle with initial values."){
    Particle test_particle(0.5, -0.5, 1.0, 2.5);
    double EPSILON = 0.000005;//for double "comparisons"
    
    WHEN("We call the Particle class setters."){
      test_particle.set_x(0.1);
      test_particle.set_y(0.3);
      test_particle.set_v_x(-1.5);
      test_particle.set_v_y(0.0);
      
      THEN( "The values should be set."){
	REQUIRE(abs(test_particle.x - 0.1) < EPSILON);
	REQUIRE(abs(test_particle.y - 0.3) < EPSILON);
	REQUIRE(abs(test_particle.v_x + 1.5) < EPSILON);
	REQUIRE(abs(test_particle.v_y) < EPSILON);
      }
    }
  }
}
