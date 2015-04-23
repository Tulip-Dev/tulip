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

#include "GradientManager.h"

using namespace std;
using namespace tlp;

GradientManager::GradientManager() :
  beginColorRange(0), endColorRange(359), minSVal(0) {
}

GradientManager::~GradientManager() {
  cleanAllGradients();
}

void GradientManager::cleanAllGradients() {
  for (map<string, ColorScale*>::iterator it = colorScaleMap.begin(); it
       != colorScaleMap.end(); ++it) {
    delete it->second;
  }

  colorScaleMap.clear();
}

void GradientManager::init(const std::vector<std::string>& properties) {
  //Save old map in order to preserve user definition
  cleanAllGradients();

  if (properties.empty())
    return;

  int shift =
    (int) floor(double((endColorRange - beginColorRange) / properties.size()));
  pair<Color, Color> newColors;
  newColors.first.setV(255);
  newColors.first.setS(255);
  newColors.second.setV(255);
  newColors.second.setS(255);

  for (unsigned int i = 0; i < properties.size(); ++i) {

    //newColors.second.setS(minSVal);
    newColors.first.setS(255);
    newColors.first.setV(minSVal);
    newColors.first.setH(beginColorRange + (i * shift));

    newColors.second.setS(255);
    newColors.second.setV(255);
    newColors.second.setH(beginColorRange + (i * shift));
    vector<Color> vc;
    vc.push_back(newColors.first);
    vc.push_back(newColors.second);
    colorScaleMap[properties[i]] = new ColorScale();
    colorScaleMap[properties[i]]->setColorScale(vc);
  }
}

ColorScale *GradientManager::getColorScale(const std::string& propertyName) {
  if (colorScaleMap.find(propertyName) != colorScaleMap.end()) {
    return colorScaleMap[propertyName];
  }
  else
    return NULL;
}

