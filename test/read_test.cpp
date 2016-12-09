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
	REQUIRE(integrator.particle_file_name == "test_particles_1.txt");
      }
    }
  }
}
