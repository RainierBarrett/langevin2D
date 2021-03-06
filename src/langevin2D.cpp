#include "langevin2D.hpp"
#include <math.h>
#include <limits>
#include <fstream>
#include <cstdlib>
#include <boost/random.hpp>
#include <boost/random/normal_distribution.hpp>
#include <vector>
#include <algorithm>
#include <omp.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>


namespace langevin2D {

  void Particle::set_x(double new_x, double min, double max){
    if (new_x > max){
      x = fmod(new_x,(max)) + min;
    }
    else if(new_x < min){
      x = max - fmod(fabs(new_x), fabs(min));
    }
    else{
      x = new_x;
    }
  } 

  void Particle::set_y(double new_y, double min, double max){
    if(new_y > max){
      y = fmod(new_y,(max)) + min;
    }
    else if(new_y < min){
      y = max - fmod(fabs(new_y), fabs(min));
    }
    else{
      y = new_y;
    }
  }

  void Particle::set_v_x(double new_v_x){
    v_x = new_v_x;
  } 

  void Particle::set_v_y(double new_v_y){
    v_y = new_v_y;
  }
  
  void Langevin::set_epsilon(double new_eps){
    epsilon = new_eps;
    return;
  }

  void Langevin::set_sigma(double new_sig){
    sigma = new_sig;
    return;
  }

  double Langevin::calc_force(double dist, double sig12, double sig6, double coeff){
    double dist3 = pow(dist, 3);
    double dist6 = dist3*dist3;
    double dist7 = dist6*dist;
    double dist12 = dist6*dist6;
    double dist13 = dist12*dist;
    return(coeff * (2 * sig12 / dist13 - sig6 / dist7));
  }
  
  void Langevin::tabulate_forces(){
    static double inf = std::numeric_limits<double>::infinity();
    double coeff = -24.0 * epsilon;
    double sig3 = pow(sigma, 3);
    double sig6 = sig3*sig3;
    double sig12 = sig6*sig6;
    double current_dist = 0;
    table_dists[0] = 0.0;
    table_forces[0] = inf;
    int i = 1;
    for(i; i < 1024; i++){
      current_dist = (double)(i/1024.0) * r_cut;
      table_dists[i] = current_dist;
      table_forces[i] = calc_force(current_dist, sig12, sig6, coeff);
    }
  }

  double Langevin::read_force(double dist){
    if(dist > r_cut){
      return((float)0.0);
    }
    int idx = floor(1024 * dist/r_cut);//the place to look in the table
    return(table_forces[idx]);
  }

  double Langevin::calc_dist(Particle* p1, Particle* p2){
    double dist = sqrt(pow(p1->x - p2->x, 2) + pow(p1->y - p2->y, 2));
    return(dist);
  }

  double Langevin::calc_x_force(Particle* p1, Particle* p2, double force_tot, double dist){
    double cosine_val = (p2->x - p1->x) / dist;
    return(force_tot * cosine_val);
  }

  double Langevin::calc_y_force(Particle* p1, Particle* p2, double force_tot, double dist){
    double sine_val = (p2->y - p1->y) / dist;
    return(force_tot * sine_val);
  }

  double Langevin::get_tot_force_x(int idx){
    int i;
    double force_sum = 0.0;
    double pair_force, pair_dist;
#pragma omp parallel for private(i, pair_force, pair_dist) reduction(+: force_sum)
    for(i = 0; i < neighbor_list[idx].size(); i++){//loop over all the neighbors
      pair_dist = calc_dist(particles[idx], particles[(neighbor_list[idx][i])]);
      pair_force = read_force(pair_dist);
      force_sum += calc_x_force(particles[idx], particles[(neighbor_list[idx][i])], pair_force, pair_dist);
    }
    return(force_sum);
  }

  double Langevin::get_tot_force_y(int idx){
    int i;
    double force_sum = 0.0;
    double pair_force, pair_dist;
#pragma omp parallel for private(i, pair_force, pair_dist) reduction(+: force_sum)
    for(i = 0; i < neighbor_list[idx].size(); i++){//loop over all the neighbors
      pair_dist = calc_dist(particles[idx], particles[(neighbor_list[idx][i])]);
      pair_force = read_force(pair_dist);
      force_sum += calc_y_force(particles[idx], particles[(neighbor_list[idx][i])], pair_force, pair_dist);
    }
    return(force_sum);
  }

  //these don't work right now, so I'm moving on and not dealing with input forces yet
/*
  int Langevin::get_x_idx(Particle* p){
    int i = 0;
    double pos = (p->x);
    for(i; i < nx; i++){
      if(fabs(x_axis[i] - pos ) < dx){
	return(i);
      }
    }
    return(-1);
  }

  int Langevin::get_y_idx(Particle* p){
    int i = 0;
    double pos = (p->y);
    for(i; i < ny ; i++){
      if(fabs(y_axis[i] - pos) < dy){
	return(i);
      }
    }
    return(-1);
  }
*/
  void Langevin::seed_rng(){
    rng.seed(static_cast<unsigned int>(std::time(0)));
    return;
  }

  double Langevin::eta(){
    boost::normal_distribution<> nd(0.0,sqrt(2 * T * lambda));
    boost::variate_generator<boost::mt19937&, boost::normal_distribution<> > var_nor(rng, nd);
    double number = var_nor();
    return(number);
  }

  void Langevin::read_params(std::string name){
    using namespace std;
    ifstream pfile;
    pfile.open(name.c_str());
    pfile >> T >> lambda >> sigma >> epsilon >> dt >> tot_time >> potential_file_name >> particle_file_name;
    if(!pfile.is_open()){printf("THAT IS NOT GOOD \n");}
    pfile.close();
    read_potential(potential_file_name);//now read in from the potential file
    read_particles(particle_file_name);//and also get the particle list
    seed_rng();//need to do this before the sim starts
    tabulate_forces();
  }

  void Langevin::set_ntot(int new_ntot){
    ntot = new_ntot;
  }

  void Langevin::set_dx(){
    dx = abs(x_axis[1] - x_axis[0]);
  }

  void Langevin::set_dy(){
    dy = abs(y_axis[1] - y_axis[0]);
  }

  void Langevin::read_particles(std::string name){
    using namespace std;
    ifstream partfile;
    partfile.open(name.c_str());
    partfile >> num_particles;
    int i = 0;
    double x_in, y_in, v_x_in, v_y_in;//holders
    for(i; i < num_particles; i++){
      partfile >> x_in >> y_in >> v_x_in >> v_y_in;
      particles[i] = new Particle(x_in, y_in, v_x_in, v_y_in);
    }
    partfile.close();
    make_neighbor_lists();//this makes the neighbor lists for all particles
    return;
  }

  void Langevin::make_neighbor_lists(){
    int i, j;
    double dist;
    #pragma omp parallel for private(i, j, dist) 
    for(i = 0; i < num_particles; i++){//for each particle
      for(j = 0; j < num_particles; j++){//look at all the (other) particles
	if(j != i){//skip if it's the same index as me
	  dist = calc_dist(particles[i], particles[j]);
	  if(dist < r_cut){
	    neighbor_list[i].push_back(j);//this index is a neighbor!
	  }
	}
      }
    }
    return;
  }

  void Langevin::update_neighbor_lists(){
    int i, j;
    double dist;
    //it's cheaper to just clear them all and re-make??
    //I'm sure that's not true, but I don't have time to find a more economical solution...
    //But I *know* this will work, and clear() is fast for int vectors.
    #pragma omp parallel for private(i)
    for(i = 0; i < num_particles; i++){
      neighbor_list[i].clear();//since ints are 'trivially destructive', this is fast
    }
    //now just re-make all neighbor lists... (copying so I don't remake i, j, dist)
    #pragma omp parallel for private(i, j, dist) 
    for(i = 0; i < num_particles; i++){//for each particle
      for(j = 0; j < num_particles; j++){//look at all the (other) particles
	if(j != i){//skip if it's the same index as me
	  dist = calc_dist(particles[i], particles[j]);
	  if(dist < r_cut){
	    neighbor_list[i].push_back(j);//this index is a neighbor!
	  }
	}
      }
    }
    return;
  }

  void Langevin::read_potential(std::string name){
    using namespace std;
    ifstream potfile;
    potfile.open(name.c_str());
    potfile >> nx >> ny;
    set_ntot(nx*ny);
    int i = 0;
    int j = 0;
    for(i; i < nx; i++){
      for(j = 0; j < ny; j++){
	potfile >> x_axis[i] >> y_axis[j] >> f_x[i*ny + j] >> f_y[i*ny +j];
      }
    }
    set_dx();
    set_dy();
    potfile.close();
  }

  void Langevin::integrate(Particle* p, int idx){
    /*
  double tot_force = -lambda * v + eta() + forces[idx]/m;
  double new_x = x + dt*v;
  set_x(new_x);
  idx = get_idx(x);
  double new_v = v + dt * tot_force;
  set_v(new_v);
*/
    double x = p->x;
    double y = p->y;
    double vx = p->v_x;
    double vy = p->v_y;

    //get interparticle forces
    double particle_f_x = get_tot_force_x(idx);
    double particle_f_y = get_tot_force_y(idx);
    //get the langevin total forces
    double langevin_force_x = -lambda * vx + eta() + particle_f_x/m;
    double langevin_force_y = -lambda * vy + eta() + particle_f_y/m;

    //set all the new values
    double new_x = x + dt * vx;
    p->set_x(new_x);
    
    double new_y = y + dt * vy;
    p->set_y(new_y);
    
    double new_v_x = vx + dt * langevin_force_x;
    p->set_v_x(new_v_x);
    
    double new_v_y = vy + dt * langevin_force_y;
    p->set_v_y(new_v_y);
  }

  void Langevin::step(){
    //can't be parallelized; have to do get_force calls, which rely on other particle positions
    using namespace std;
    int i = 0;
    for(i; i < num_particles; i++){
      integrate(particles[i], i);

    }
    return;
  }

  void Langevin::run(){
    using namespace std;
    int i;
    int count = 0;
    ofstream output;
    string outfile = "output.txt";
    output.open(outfile.c_str());
    for(time; time < tot_time; time+=dt){      
      step();
      output << "TIME: " << time << endl;
      for(i = 0; i < num_particles; i++){
    output << string("PARTICLE: ") << i << "\t x: " << particles[i]->x << "\t v_x: " << particles[i]->v_x << "\t y: " << particles[i]->y << string("\t v_y: ") << particles[i]->v_y << endl;
      }
      output << endl;
      count++;
      if(count == 50){
    update_neighbor_lists();
    count = 0;
  }
    }
    output.close();
  }
  
}
