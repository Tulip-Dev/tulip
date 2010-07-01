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
#ifndef _TLPTOOLS_H
#define _TLPTOOLS_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include "tulip/tulipconf.h"
#include "tulip/PluginLoader.h"

namespace tlp {
  extern TLP_SCOPE const char PATH_DELIMITER;
  extern TLP_SCOPE std::string TulipLibDir;
  extern TLP_SCOPE std::string TulipPluginsPath;
  extern TLP_SCOPE std::string TulipDocProfile;
  extern TLP_SCOPE std::string TulipUserHandBookIndex;
  extern TLP_SCOPE std::string TulipBitmapDir;
  
  extern TLP_SCOPE void initTulipLib(char* appDirPath = 0);
    
  TLP_SCOPE void loadPlugins(PluginLoader *plug=0);
  TLP_SCOPE bool loadPlugin(const std::string &filename, PluginLoader *plug=0);
  TLP_SCOPE void loadPluginsFromDir(std::string dir, std::string type, PluginLoader *loader=0);
  TLP_SCOPE void loadPluginsCheckDependencies(PluginLoader* loader=0);

  /**
   * returns the demangled name of a C++ class defines in the tlp namespace.
   * The tlp:: prefix is omitted and the returned pointer
   * do not have to be deallocated.
   */
  TLP_SCOPE std::string demangleTlpClassName(const char *className);

  TLP_SCOPE std::istream *getIgzstream(const char *name, int open_mode = std::ios::in);
  /**
   * Returns an ostream to write to a gzipped file (uses gzstream lib)
   * the stream has to be deleted after use.
   * \warning Don't forget to check the stream with ios::bad()!
   */
  TLP_SCOPE std::ostream *getOgzstream(const char *name, int open_mode = std::ios::out);
  /**
   * Extracts Major number from a release number
   */
  TLP_SCOPE std::string getMajor(const std::string &release);
  /**
   * Extracts Minor number from a release number
   */
  TLP_SCOPE std::string getMinor(const std::string &release);
}

#endif
