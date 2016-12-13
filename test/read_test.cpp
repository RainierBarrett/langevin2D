#include "langevin2D.hpp"
#include "catch.hpp"
#include <iomanip>
#include <string>

using namespace langevin2D;

SCENARIO( "Can properly parse our input file.", "[input]"){

  GIVEN( "An input file pointing at a potential table."){
    Langevin integrator;
    double EPSILON = 0.000005;//for double "comparisons"
    
    WHEN("We call the read_input() method."){
      std::string param_file = "test_params.txt";
      integrator.read_params(param_file);

      THEN( "The class variables should be set."){
	REQUIRE(abs(integrator.T - 10.0) < EPSILON);
	REQUIRE(abs(integrator.lambda - 0.55) < EPSILON);
	REQUIRE(abs(integrator.dt - 0.05) < EPSILON);
	REQUIRE(abs(integrator.tot_time - 4.0) < EPSILON);
	REQUIRE(integrator.potential_file_name == "test_pot.txt");
	REQUIRE(integrator.particle_file_name == "test_particles_50.txt");
      }

      AND_THEN("We should have read our potential file."){
	REQUIRE(integrator.nx == 100);
	REQUIRE(integrator.ny == 100);
	REQUIRE(integrator.ntot == 100*100);
	REQUIRE(abs(integrator.dx - 0.1) < EPSILON);
	REQUIRE(abs(integrator.dy - 0.1) < EPSILON);
	REQUIRE(integrator.x_axis[50] < EPSILON);//should be the 0
	REQUIRE(abs(integrator.x_axis[99] - 4.90) < EPSILON);
	REQUIRE(abs(integrator.f_x[0]) == 0);
	REQUIRE(abs(integrator.f_y[100]) == 0);
      }
      AND_THEN("We should have read our particle initial positions list."){
	REQUIRE(integrator.num_particles == 50);
	REQUIRE(abs(integrator.particles[0]->x + 2.5 ) < EPSILON);
	REQUIRE(abs(integrator.particles[49]->y + 2.4 ) < EPSILON);
	REQUIRE(abs(integrator.particles[24]->v_x - 0.5) < EPSILON);
	REQUIRE(abs(integrator.particles[24]->v_y - 0.25) < EPSILON);
      }
      AND_THEN("We should be able to calculate inter-particle distance."){
	REQUIRE(abs(integrator.particles[25]->x) < EPSILON);//should be 0
	REQUIRE(abs(integrator.particles[25]->y) < EPSILON);//should be 0
	double dist = integrator.calc_dist((integrator.particles[0]), (integrator.particles[25]));
	REQUIRE(abs(dist - 2.5*sqrt(2)) < EPSILON);
      }
    }
  }
}
 
