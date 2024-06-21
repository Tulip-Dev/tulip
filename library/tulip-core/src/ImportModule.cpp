/*
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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
#include <tulip/Graph.h>
#include <tulip/PluginProgress.h>
#include <tulip/ImportModule.h>

using namespace tlp;

ImportModule::ImportModule(const tlp::PluginContext *context) {
  if (context != nullptr) {
    const tlp::AlgorithmContext *algoritmContext =
        static_cast<const tlp::AlgorithmContext *>(context);
    graph = algoritmContext->graph;
    pluginProgress = algoritmContext->pluginProgress;
    dataSet = algoritmContext->dataSet;
  }
}

ImportFileModule::ImportFileModule(const tlp::PluginContext *context) : ImportModule(context) {
  // declare the parameter to set the filename data member
  addInParameter<std::string>("file::filename", "The pathname of the file to import", "", true);
}

bool ImportFileModule::check() {
  if (!dataSet->get("file::filename", filename) || filename.empty()) {
    pluginProgress->setError("\"filename\" parameter is empty.");
    return false;
  }
  return true;
}

bool ImportFileModule::importGraph() {
  if (check() && importFile()) {
    graph->setAttribute("file", filename);
    return true;
  }
  return false;
}
