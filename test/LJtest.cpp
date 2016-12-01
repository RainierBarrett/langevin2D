#include "langevin2D.hpp"
#include "catch.hpp"

using namespace langevin2D;

SCENARIO( "Can tabulate LJ potentials, given our constants.", "[integrator]"){

  GIVEN( "An integrator and some constants..."){
    Langevin integrator;
    const double EPSILON = 0.0005;//for double "comparisons"
    double eps = 1.0;//the LJ epsilon value.
    double sig = 1.0;
    
    WHEN("We call the LJ tabulate function..."){
      integrator.set_epsilon(eps);
      integrator.set_sigma(sig);

      THEN( "The values should be set."){
	REQUIRE(integrator.forces.size() > 0);
      }
    }
  }
}
