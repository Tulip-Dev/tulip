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

#include <tulip/TriconnectedTest.h>
#include <tulip/GraphTest.h>

class TriconnectedTest : public tlp::GraphTest {
public:
  PLUGININFORMATION("Triconnected", "Tulip team", "18/04/2012", "Tests whether a graph is triconnected or not.", "1.0", "Topological Test")
  TriconnectedTest(const tlp::PluginContext* context) : tlp::GraphTest(context) {
  }

  virtual bool test() {
    return tlp::TriconnectedTest::isTriconnected(graph);
  }
};
PLUGIN(TriconnectedTest)

