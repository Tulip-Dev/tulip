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

#ifndef GRADIENTMANAGER_H_
#define GRADIENTMANAGER_H_

#include <map>
#include <string>
#include <vector>
#include <tulip/Color.h>
#include <tulip/ColorScale.h>

/**
 * @brief Object used to store ColorScale for different properties.
 */

namespace tlp {

class GradientManager {
public:
  GradientManager();
  virtual ~GradientManager();

  void init(const std::vector<std::string>& properties);

  tlp::ColorScale* getColorScale(const std::string& propertyName);

protected:

  void cleanAllGradients();

  std::map<std::string, tlp::ColorScale*> colorScaleMap;
  int beginColorRange;
  int endColorRange;
  int minSVal;
};
}
#endif /* GRADIENTMANAGER_H_ */
