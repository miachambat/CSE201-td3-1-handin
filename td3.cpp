#include <iostream>
#include "td3.hpp"
#include "support.hpp"
#include <stdlib.h>
#include <math.h>       // sin, cos
#include <assert.h>

using namespace std;

using namespace support;

double* extend_array(double* array, int length, int new_size) {
  // the information is stored at array, we access it through this pointer
  // ALLOCATE a new array of size new_size
  double* array2 = new double[new_size];

  // copy all the elements of the array array, up to size length
  for (int i = 0; i < length; i++) {
    array2[i] = array[i];
          }

  // initialize to 0-s all the remaining array elements (i.e., from index length to index new_size-1), and
  for (int i = length; i < new_size; i++) {
    array2[i] = 0;
          }

  // DE-ALLOCATE (delete operator) the memory used by the “old” array (array variable).
  delete array;

  // return the newly allocated array as output of the function
  return array2;
}

double* shrink_array(double* array, int length, int new_size) {
    double* array2 = new double[new_size];

    // copy all the elements of the array array, up to size length
    for (int i = 0; i < new_size; i++) {
      array2[i] = array[i];
            }

    // DE-ALLOCATE (delete operator) the memory used by the “old” array (array variable).
    delete array;

    // return the newly allocated array as output of the function
    return array2;
}

double* append_to_array(double element,
                        double* array,
                        int &current_size,
                        int &max_size) {
   // appends the number contained in element to the array array, and returns the array pointer as output

  if ( current_size == max_size ) {
  array = extend_array(array, max_size, max_size+5);
  max_size = max_size + 5;
  }
  array[current_size] = element;
  current_size = current_size + 1;
  return array;

}

double* remove_from_array(double* array,
                          int &current_size,
                          int &max_size) {

    array[current_size-1] = 0;
    current_size = current_size - 1;

    if (max_size - current_size >= 5 ) {
        array= shrink_array(array, max_size, max_size-5);
        max_size = max_size - 5;
    }

    return array;
}

bool simulate_projectile(const double magnitude, const double angle,
                         const double simulation_interval,
                         double *targets, int &tot_targets,
                         int *obstacles, int tot_obstacles,
                         double* &telemetry,
                         int &telemetry_current_size,
                         int &telemetry_max_size) {
  // YOU CAN MODIFY THIS FUNCTION TO RECORD THE TELEMETRY

  bool hit_target, hit_obstacle;
  double v0_x, v0_y, x, y, t;
  double PI = 3.14159265;
  double g = 9.8;

  v0_x = magnitude * cos(angle * PI / 180);
  v0_y = magnitude * sin(angle * PI / 180);

  t = 0;
  x = 0;
  y = 0;

  hit_target = false;
  hit_obstacle = false;
  while (y >= 0 && (! hit_target) && (! hit_obstacle)) {

      // work with telemetry, telemetry_current_size and telemetry_max_size

      telemetry = append_to_array(t,telemetry,telemetry_current_size, telemetry_max_size);
      telemetry = append_to_array(x,telemetry,telemetry_current_size, telemetry_max_size);
      telemetry = append_to_array(y,telemetry,telemetry_current_size, telemetry_max_size);


    double * target_coordinates = find_collision(x, y, targets, tot_targets);
    if (target_coordinates != NULL) {
      remove_target(targets, tot_targets, target_coordinates);
      hit_target = true;
    } else if (find_collision(x, y, obstacles, tot_obstacles) != NULL) {
      hit_obstacle = true;
    } else {
      t = t + simulation_interval;
      y = v0_y * t  - 0.5 * g * t * t;
      x = v0_x * t;
    }
  }

  return hit_target;
}

void merge_telemetry(double **telemetries,
                     int tot_telemetries,
                     int *telemetries_sizes,
                     double* &global_telemetry,
                     int &global_telemetry_current_size,
                     int &global_telemetry_max_size) {


    // telemetries points to an allocated area of memory containing tot_telemetries pointers to doubles
    // the i-th pointer to doubles pointed by telemetries points to an array of doubles of size telemetries_sizes[i]
    for (int i = 0; i < tot_telemetries; i++) {

        for (int j = 0; j < telemetries_sizes[i]; j+=3) {
            global_telemetry = append_to_array(telemetries[i][j], global_telemetry,
                                    global_telemetry_current_size,
                                    global_telemetry_max_size);
            global_telemetry = append_to_array(telemetries[i][j+1], global_telemetry,
                                    global_telemetry_current_size,
                                    global_telemetry_max_size);
            global_telemetry = append_to_array(telemetries[i][j+2], global_telemetry,
                                    global_telemetry_current_size,
                                    global_telemetry_max_size);
    }
    }
    // now sort them by time

    for ( int x = 0; x < global_telemetry_current_size; x+=3 ) {
        for ( int y = x+3;  y < global_telemetry_current_size; y+=3 ) {

        if (global_telemetry[x] > global_telemetry[y])
            {
                double first , second ,third;
                first = global_telemetry[x];
                second = global_telemetry[x+1];
                third = global_telemetry[x+2];
                global_telemetry[x] = global_telemetry[y];
                global_telemetry[x+1] = global_telemetry[y+1];
                global_telemetry[x+2]= global_telemetry[y+2];
                global_telemetry[y] = first;
                global_telemetry[y+1] = second;
                global_telemetry[y+2] = third;

                               }
    }

}

}

