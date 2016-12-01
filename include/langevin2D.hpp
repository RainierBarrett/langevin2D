#ifndef _LANGEVIN_HPP_
#define _LANGEVIN_HPP_

namespace langevin2D {

    class Langevin {

    public:
      double** axes;//the array to hold the two axes we'll use
      double* x_axis;//the array of x-axis values
      double* y_axis;//the array of y-axis values

      double dx, dy;//dx and dy can differ, but I only allow 'regular' intervals
      void set_dx();//sets dx. Called during initial setup only
      void set_dy();//sets dy. Called during initial setup only
    };

}
#endif // _LANGEVIN_HPP_
