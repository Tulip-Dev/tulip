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

#ifndef _METRICALGO_H
#define _METRICALGO_H

#include <tulip/Algorithm.h>

namespace tlp {

class DoubleProperty;

/** \addtogroup plugins */
/*@{*/
/// Interface for metric plug-ins
class TLP_SCOPE DoubleAlgorithm : public tlp::Algorithm {
  ///
  friend class DoubleProperty;
public:
  /** This metric is the one associated to the algorithm,
      The data should be store in it*/
  DoubleProperty *doubleResult;
  ///
  virtual ~DoubleAlgorithm();

protected:
  ///
  DoubleAlgorithm (const AlgorithmContext &context);
};
/*@}*/

}
#endif
