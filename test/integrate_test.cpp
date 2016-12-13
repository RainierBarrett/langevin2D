#include "langevin2D.hpp"
#include "catch.hpp"
#include <iomanip>
#include <string>
#include <vector>

using namespace langevin2D;

SCENARIO( "Can call the integrate() method on a particle and change its stuff.", "[integrate]"){

  GIVEN( "Our integrator and a particle."){
    Langevin integrator;
    double EPSILON = 0.000005;//for double "comparisons"
    
    WHEN("We call the integrate() method."){
      std::string param_file = "test_params_1_part.txt";
      integrator.read_params(param_file);
      //to track changes:
      double x1 = integrator.particles[0]->x;
      double y1 = integrator.particles[0]->y;
      double vx1 = integrator.particles[0]->v_x;
      double vy1 = integrator.particles[0]->v_y;
      for(int i = 0; i < 20; i++){
	integrator.integrate(integrator.particles[0], 0);
      }


      THEN( "The particle should have moved."){
	REQUIRE((x1 != integrator.particles[0]->x));
	REQUIRE((y1 - integrator.particles[0]->y) > EPSILON);
	REQUIRE((vx1 - integrator.particles[0]->v_x) > EPSILON);
	REQUIRE((vy1 - integrator.particles[0]->v_y) > EPSILON);
      }
    }
  }
}




/*
  double tot_force = -lambda * v + eta() + forces[idx]/m;
  double new_x = x + dt*v;
  set_x(new_x);
  idx = get_idx(x);
  double new_v = v + dt * tot_force;
  set_v(new_v);
*/
