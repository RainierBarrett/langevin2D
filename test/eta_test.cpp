#include "langevin2D.hpp"
#include "catch.hpp"
#include <string>
#include <cstdlib>
#include <unistd.h>

using namespace langevin2D; 

SCENARIO( "Make sure eta function is random enough", "[eta]"){

  GIVEN( "An input file"){
    const double EPSILON = 0.000005;//for double "comparisons"
    Langevin integrator;
    std::string filename = "test_params.txt";
    integrator.read_params(filename);
    WHEN("making random numbers"){
      double eta1 = integrator.eta();
//      sleep(1);
      double eta2 = integrator.eta();
//      sleep(1);
      double eta3 = integrator.eta();
      THEN("we should get lots of different results"){
	REQUIRE( eta1 != eta2);
	REQUIRE( eta1 != eta3);
	REQUIRE( eta2 != eta3);
      }
    }
  }
}
 
