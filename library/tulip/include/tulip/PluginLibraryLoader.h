#ifndef PLUGINLIBLOADER_H
#define PLUGINLIBLOADER_H

#ifndef DOXYGEN_NOTFOR_USER

#include "PluginLoader.h"
#include "tulipconf.h"

namespace tlp {

class TLP_SCOPE PluginLibraryLoader {
 public:
  int n;
  std::string msg;
  std::string pluginPath;
  PluginLoader *loader;

  PluginLibraryLoader(std::string pluginPath, PluginLoader *loader);
  bool hasPluginLibraryToLoad() { return n >= 0; }
  bool loadNextPluginLibrary(PluginLoader *loader);
  static TLP_SCOPE bool loadPluginLibrary(const std::string & filename,
					  PluginLoader *loader = 0);

 private:
  void *infos;
};

}
#endif //DOXYGEN_NOTFOR_USER
#endif //PLUGINLIBLOADER_H
