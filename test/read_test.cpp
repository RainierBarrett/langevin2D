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
      }
    }
  }
}
