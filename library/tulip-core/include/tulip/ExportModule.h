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
///@cond DOXYGEN_HIDDEN

#ifndef _EXPORTMODULE_H
#define _EXPORTMODULE_H

#include <iostream>
#include <tulip/Plugin.h>
#include <tulip/Algorithm.h>

namespace tlp {

static const std::string EXPORT_CATEGORY = "Export";

class Graph;
class DataSet;
class PluginProgress;

/**
 * @ingroup Plugins
 * @brief The ExportModule class
 */
class ExportModule : public tlp::Plugin {

  /**
   * @brief The supported file extensions
   **/
  std::list<std::string> extensions;

public:
  ExportModule(const tlp::PluginContext *context, std::list<std::string> exts = {})
      : extensions(exts) {
    if (context != nullptr) {
      const tlp::AlgorithmContext *algoritmContext =
          static_cast<const tlp::AlgorithmContext *>(context);
      graph = algoritmContext->graph;
      pluginProgress = algoritmContext->pluginProgress;
      dataSet = algoritmContext->dataSet;
    }
  }

  ~ExportModule() override {}

  std::string category() const override {
    return EXPORT_CATEGORY;
  }

  std::string icon() const override {
    return ":/tulip/gui/icons/64/document-export.png";
  }

  /**
   * @brief Gets the list of the extensions file formats supported.
   * The ones after the first are supposed to be gzip compressed
   *
   * @return the list of file extensions this export plugin saves to.
   */
  std::list<std::string> fileExtensions() const {
    return extensions;
  }

  /**
   * @brief The export operations should take place here.
   * @param the output stream
   * @return bool Whether the export was successful or not.
   **/
  virtual bool exportGraph(std::ostream &os) = 0;

  /** It is the root graph*/
  Graph *graph;

  PluginProgress *pluginProgress;
  DataSet *dataSet;
};
} // namespace tlp
#endif
///@endcond
