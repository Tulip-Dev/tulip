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
#include "PluginLoaderDispatcher.h"

PluginLoaderDispatcher::PluginLoaderDispatcher() : tlp::PluginLoader() {}

void PluginLoaderDispatcher::start(const std::string &path) {
  for (auto ldr : _loaders)
    ldr->start(path);
}

void PluginLoaderDispatcher::numberOfFiles(int n) {
  for (auto ldr : _loaders)
    ldr->numberOfFiles(n);
}

void PluginLoaderDispatcher::loading(const std::string &filename) {
  for (auto ldr : _loaders)
    ldr->loading(filename);
}

void PluginLoaderDispatcher::loaded(const tlp::Plugin *info,
                                    const std::list<tlp::Dependency> &dependencies) {
  for (auto ldr : _loaders)
    ldr->loaded(info, dependencies);
}

void PluginLoaderDispatcher::aborted(const std::string &filename, const std::string &errormsg) {
  for (auto ldr : _loaders)
    ldr->aborted(filename, errormsg);
}

void PluginLoaderDispatcher::finished(bool state, const std::string &msg) {
  for (auto ldr : _loaders)
    ldr->finished(state, msg);
}

void PluginLoaderDispatcher::registerLoader(tlp::PluginLoader *loader) {
  _loaders.insert(loader);
}

void PluginLoaderDispatcher::removeLoader(tlp::PluginLoader *loader) {
  _loaders.remove(loader);
}
