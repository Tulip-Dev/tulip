/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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
#include <cstdio>
#include <string>
#include <iostream>

#include <tulip/TulipPluginHeaders.h>

using namespace std;

class Test2 : public tlp::BooleanAlgorithm {
public:
  PLUGININFORMATION("Test2", "Jezequel", "03/11/2004", "0", "1.0", "")
  Test2(tlp::PluginContext *context) : tlp::BooleanAlgorithm(context) {}
  ~Test2() {}
  bool run() {
    return true;
  }
};
PLUGIN(Test2)
