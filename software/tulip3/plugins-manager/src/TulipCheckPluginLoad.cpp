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
#include <tulip/TlpTools.h>
#include "PluginInfo.h"

using namespace std;
using namespace tlp;

int main(int argc,char **argv) {
  if(argc != 2)  {
    cerr << "library plugin file missing." << endl;
  }

  initTulipLib(NULL);
  // only check that the dynamic loading
  // of the library is OK
  PluginLibraryLoader::loadPluginLibrary(argv[1], NULL);

  cout << "pluginLoaded" << endl;

}
