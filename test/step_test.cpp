#include "langevin2D.hpp"
#include "catch.hpp"
#include <iomanip>
#include <string>
#include <vector>

using namespace langevin2D;

SCENARIO( "Can call the step() method to update all the particles.", "[step]"){

  GIVEN( "Our integrator and some particles."){
    Langevin integrator;
    double EPSILON = 0.000005;//for double "comparisons"
    
    WHEN("We call the integrate() method."){
      std::string param_file = "test_params.txt";
      integrator.read_params(param_file);
      //to track changes:
      double x1 = integrator.particles[0]->x;
      double y1 = integrator.particles[0]->y;
      double vx1 = integrator.particles[0]->v_x;
      double vy1 = integrator.particles[0]->v_y;
      double x50 = integrator.particles[49]->x;
      double y50 = integrator.particles[49]->y;
      double vx50 = integrator.particles[49]->v_x;
      double vy50 = integrator.particles[49]->v_y;
      integrator.run();//should call step a short 100 times


      THEN( "The particle should have moved."){
	REQUIRE((x1 != integrator.particles[0]->x));
	REQUIRE((y1 != integrator.particles[0]->y));
	REQUIRE((vx1 != integrator.particles[0]->v_x));
	REQUIRE((vy1 != integrator.particles[0]->v_y));
	REQUIRE((x50 != integrator.particles[49]->x));
	REQUIRE((y50 != integrator.particles[49]->y));
	REQUIRE((vx50 != integrator.particles[49]->v_x));
	REQUIRE((vy50 != integrator.particles[49]->v_y));
      }
    }
  }
}

