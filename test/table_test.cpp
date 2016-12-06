#define CATCH_CONFIG_MAIN
#include "langevin2D.hpp"
#include "catch.hpp"
#include <iomanip>

using namespace langevin2D;

SCENARIO( "Can tabulate LJ potentials, given our constants.", "[integrator]"){

  GIVEN( "An integrator and some constants."){
    Langevin integrator;
    double EPSILON = 0.000005;//for double "comparisons"
    double eps = 1.0;//the LJ epsilon value.
    double sig = 1.0;
    
    WHEN("We call the LJ tabulate function."){
      integrator.set_epsilon(eps);
      integrator.set_sigma(sig);
      integrator.tabulate_forces();//this will tabulate the LJ force table

      THEN( "The values should be set."){
	REQUIRE((abs((integrator.epsilon - eps))) < EPSILON);
	int size0 = sizeof(integrator.table_dists)/sizeof(integrator.table_dists[0]);
	int size1 = sizeof(integrator.table_forces)/sizeof(integrator.table_forces[0]);
	REQUIRE(size0 == 1024);
	REQUIRE(size1 == 1024);
	REQUIRE(pow((integrator.table_dists[100] - integrator.table_dists[10]), 2) > EPSILON);
	REQUIRE(pow((integrator.table_forces[100] - integrator.table_forces[10]),2) > EPSILON);
      }
    }
  }
}
