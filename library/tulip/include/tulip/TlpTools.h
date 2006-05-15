//-*-c++-*-
#ifndef _TLPTOOLS_H
#define _TLPTOOLS_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include "tulipconf.h"
#include "PluginLoader.h"

namespace tlp {
  extern TLP_SCOPE const char PATH_DELIMITER;
  extern TLP_SCOPE std::string TulipLibDir;
  extern TLP_SCOPE std::string TulipPluginsPath;
  extern TLP_SCOPE std::string TulipDocProfile;
  extern TLP_SCOPE std::string TulipUserHandBookIndex;
  
  extern TLP_SCOPE void initTulipLib(char* appDirPath = 0);
    
  TLP_SCOPE void loadPlugins(PluginLoader *plug=0);
  TLP_SCOPE bool loadPlugin(const std::string &filename, PluginLoader *plug=0);

  TLP_SCOPE std::istream *getIgzstream(const char *name, int open_mode = std::ios::in);
  /**
   * Return an ostream to write to a gzipped file (uses gzstream lib)
   * the stream has to be deleted after use.
   * \warning Don't forget to check the stream with ios::bad()!
   */
  TLP_SCOPE std::ostream *getOgzstream(const char *name, int open_mode = std::ios::out);
  
};

#endif
