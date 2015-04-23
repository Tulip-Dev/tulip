/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
#ifndef NUMBER_H
#define NUMBER_H

/**
 *This class enables the comparison of floats or the initialization of float
 *to infinity.
 */
class Number {
public:

  float value;/**< Contain the value of the number. */

  static float infini;/**< contain our value of infinity. */

public:
  /**
   *The constructor initializes the field value to the float f.
   */
  Number(float f) {
    value=f;
  }

  /**
   *Overload of the operator >.
   */
  bool operator > (float b);
};




#endif
