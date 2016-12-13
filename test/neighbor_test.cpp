#include "langevin2D.hpp"
#include "catch.hpp"
#include <iomanip>
#include <string>
#include <vector>

using namespace langevin2D;

SCENARIO( "Can generate correct neighbor lists, and update them.", "[neighbor]"){

  GIVEN( "A simple input file pointing at a 3-particle file."){
    Langevin integrator;
    double EPSILON = 0.000005;//for double "comparisons"
    
    WHEN("We call the read_input() method."){
      std::string param_file = "test_params_3_part.txt";
      integrator.read_params(param_file);//now we should have 3 particles

      THEN( "The number of particles should be 3."){
	REQUIRE(integrator.num_particles == 3);
      }
      AND_THEN("We should automatically generate neighbor lists for each one."){
	REQUIRE(integrator.neighbor_list[0].size() == 1);//0, 0-- sees only 2,0
	REQUIRE(integrator.neighbor_list[1].size() == 2);//2, 0-- sees both
	REQUIRE(integrator.neighbor_list[2].size() == 1);//3, 0-- sees only 2,0
      }
      AND_THEN("The list entries should be correct."){
	REQUIRE(integrator.neighbor_list[0][0] == 1);
	REQUIRE(integrator.neighbor_list[1][0] == 0);
	REQUIRE(integrator.neighbor_list[1][1] == 2);
	REQUIRE(integrator.neighbor_list[2][0] == 1);
      }
      AND_THEN("If we change the positions of the particles and update the neighbor list, it should update correctly."){
	integrator.particles[0]->set_x(5.0);//now should see particles[2] only...
	integrator.update_neighbor_lists();//this should cover all branches
	REQUIRE(integrator.neighbor_list[0].size() == 1);//should still see only 1
	REQUIRE(integrator.neighbor_list[1].size() == 1);//should now see only 1
	REQUIRE(integrator.neighbor_list[2].size() == 2);//should now see 2
	REQUIRE(integrator.neighbor_list[0][0] == 2);
	REQUIRE(integrator.neighbor_list[1][0] == 2);
	//order of the neighbor lists doesn't matter, so just make sure both are in there:
	int item1 = integrator.neighbor_list[2][0];
	int item2 = integrator.neighbor_list[2][1];
	bool b1 = (item1 == 0 || item1 == 1);
	bool b2 = (item2 == 0 || item2 == 1);
	REQUIRE(b1);
	REQUIRE(b2);
      }
    }
  }
}
 
