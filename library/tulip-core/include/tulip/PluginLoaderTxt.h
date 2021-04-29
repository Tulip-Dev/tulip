/*
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

#ifndef TLP_PLUGINLOADERTXT
#define TLP_PLUGINLOADERTXT
#include <tulip/PluginLoader.h>

namespace tlp {

/**
 * @ingroup Plugins
 * @brief A simple implementation of tlp::PluginLoader to display load state into the console
 */
struct TLP_SCOPE PluginLoaderTxt : public PluginLoader {
  void start(const std::string &path) override;
  void loading(const std::string &filename) override;
  void loaded(const Plugin *info, const std::list<Dependency> &) override;
  void aborted(const std::string &filename, const std::string &erreurmsg) override;
  void finished(bool state, const std::string &msg) override;
};
} // namespace tlp
#endif
