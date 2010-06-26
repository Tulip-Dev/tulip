/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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

#ifndef DOXYGEN_NOTFOR_DEVEL
#ifndef TLP_POLYNOME_H
#define TLP_POLYNOME_H

namespace tlp {
/** \brief Simple class used to represent a Polynome up to the 3rd degree
 *
 * This class represents a Polynome up to the 3rd degree.
 * It follows the equation : \f$ax^{3} + bx^{2} + cx + d = 0\f$
 *
 * \author Maxime Delorme maxime.delorme@gmail.com
 *
*/
class Polynome
{
 public:
  float a, b, c, d;

  /**
   * Constructs the Polynome.
   * \attention a = b = c = d = 0
   */
  Polynome();

  /**
   * Constructs the Polynome given the coefficients
   */
  Polynome(float a, float b, float c, float d);

  /**
   * Destructs the Polynome
   */
  ~Polynome();

  /**
   * Find the roots of the polynome so that \f$ax^{3} + bx^{2} + cx + d = 0\f$
   */
  void resolv(float result[3], int &nResult);
};
}
#endif
#endif //DOXYGEN_NOTFOR_DEVEL
