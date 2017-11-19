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
#ifndef PLUGINLOADERDISPATCHER_H
#define PLUGINLOADERDISPATCHER_H

#include <tulip/PluginLoader.h>
#include <QSet>

class PluginLoaderDispatcher : public tlp::PluginLoader {
public:
  PluginLoaderDispatcher();

  void start(const std::string &path) override;
  void finished(bool state, const std::string &msg) override;

  void numberOfFiles(int n) override;

  void loading(const std::string &filename) override;
  void loaded(const tlp::Plugin *info, const std::list<tlp::Dependency> &dependencies) override;
  void aborted(const std::string &filename, const std::string &errormsg) override;

  void registerLoader(tlp::PluginLoader *loader);
  void removeLoader(tlp::PluginLoader *loader);

private:
  QSet<tlp::PluginLoader *> _loaders;
};

#endif // PLUGINLOADERDISPATCHER_H
