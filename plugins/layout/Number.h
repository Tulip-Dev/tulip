#ifndef NUMBER_H
#define NUMBER_H

#include <string.h>

/**
 *This class enables the comparison of floats or the initialization of float 
 *to infinity.
 */
class Number{
 public:
 
  float value;/**< Contain the value of the number. */
 
  static float infini;/**< contain our value of infinity. */

 public:
  /**
   *The constructor initializes the field value to the float f.
   */
  Number(float f) {value=f;};

  /**
   *Overload of the operator >.
   */
  bool operator > (float b);
};


  

#endif
