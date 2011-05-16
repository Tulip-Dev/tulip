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
#ifndef TLP_PLUGINLOADERTXT
#define TLP_PLUGINLOADERTXT
#include <tulip/PluginLoader.h>

namespace tlp {

struct TLP_SCOPE PluginLoaderTxt:public PluginLoader {
  virtual void start(const std::string &path,const std::string &type);
  virtual void loading(const std::string &filename);
  virtual void loaded(const AbstractPluginInfo* infos, const std::list <Dependency>&);
  virtual void aborted(const std::string &filename,const  std::string &erreurmsg);
  virtual void finished(bool state,const std::string &msg);
};

}
#endif
