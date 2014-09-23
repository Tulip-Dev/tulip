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

#include <tulip/ImportModule.h>

using namespace tlp;

class EmptyGraphImport: public ImportModule {
public:
  PLUGININFORMATION("Empty graph","Tulip team", "05/09/2012","A no-op plugin to import empty graphs","1.0","")

  EmptyGraphImport(const tlp::PluginContext* context): ImportModule(context) {
  }

  virtual bool importGraph() {
    return true;
  }
};
PLUGIN(EmptyGraphImport)
