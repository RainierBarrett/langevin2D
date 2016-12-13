# langevin2D

"Extension of my first c++ Langevin project, now in 2D!"

## Building From Source

First make sure that you have [CMake](http://www.cmake.org/) and an C++ compiler environment installed.

Then open a terminal, go to the source directory and type the following commands:

    $ mkdir build
    $ cd build
    $ cmake ..
    $ make

## Using the Program

Input format --  input parameter file should contain one line with all values space-separated as follows:
`temperature (float) damping_coefficient (float) time_step (float) total_time (float) potential_energy_filename (string) particle_file_name (string)`.

For an example parameter file, see `test_params.txt`. Likewise, you can examine the included `test_particles_50.txt` and `test_input.txt` files for examples of a particle file, and a potential/force file.

Potential file format:
`num_lines (int)`, followed by num_lines lines, each containing:
`x_pos (float) y_pos (float) force_x_component (float) force_y_component (float)`. Yes, you have to put in the *forces*, and not the potentials.

## Running unit tests

After building this project you may run its unit tests by using these commands:

    $ make test  # To run all tests via CTest
    $ make catch # Run all tests directly, showing more details to you

Unit tests are documented within their respective test files, inside the test directory. 
Right now the tests are set to run all and reports all at once, too. Use catch to see if an individual test is failing. *DO NOT EDIT* the `*test*.txt` files included, or **your tests will fail**.

## Checking Code Coverage

From within the build directory, and only AFTER you have fully built the project (cmake and make), you can invoke `make coverage` to run the coverage test script, which will print out the unit test coverage of the langevin2D.cpp file, and where to find the gcov files for it, if you're interested.

## TODO

* Implement neighbor list functionality for the Langevin class. -- Parallelism
* Use neighbor list to make a calc_total_f_x() and calc_total_f_y() method. -- Parallelism
* Implement an actual integrate function that takes in a particle and its neighbor list, and updates its position and velocities. -- Parallelism
* Write a method to print each timestep to an output file.
* Write a python file for generating a movie of the simulation.

## License

![GNU GPLv3 Image](https://www.gnu.org/graphics/gplv3-127x51.png)

This program is Free Software: You can use, study share and improve it at your
will. Specifically you can redistribute and/or modify it under the terms of the
[GNU General Public License](https://www.gnu.org/licenses/gpl.html) as
published by the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.