/*
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
///@cond DOXYGEN_HIDDEN


#ifndef _GRAPHTEST_H
#define _GRAPHTEST_H

#include <tulip/Algorithm.h>

namespace tlp {
class GraphTest : public tlp::Algorithm {
public:
  GraphTest(const tlp::PluginContext* context) : Algorithm(context) {
    addOutParameter<bool>("result",
                          HTML_HELP_OPEN()        \
                          HTML_HELP_DEF( "type", "bool" )  \
                          HTML_HELP_BODY()        \
                          "Whether the graph passed the test or not." \
                          HTML_HELP_CLOSE());
  }

  virtual bool run() {
    bool result = test();

    if (dataSet) {
      dataSet->set("result",result);
    }

    return true;
  }

  virtual bool test()=0;
};

}
#endif //_GRAPHTEST_H
///@endcond
