/*
 *
 * This file is part of Tulip (www.tulip-software.org)
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
class ExportModule: public tlp::Plugin {
public:
  ///
  ExportModule(const tlp::PluginContext* context) {
    if(context != NULL) {
      const tlp::AlgorithmContext* algoritmContext = dynamic_cast<const tlp::AlgorithmContext*>(context);
      assert(algoritmContext != NULL);
      graph = algoritmContext->graph;
      pluginProgress = algoritmContext->pluginProgress;
      dataSet = algoritmContext->dataSet;
    }
  }
  ///
  virtual ~ExportModule() {}

  virtual std::string category() const {
    return EXPORT_CATEGORY;
  }
  std::string icon() const {
    return ":/tulip/gui/icons/32/plugin_import_export.png";
  }

  /**
   * @brief Gets the extension of the file format this export plugin saves to.
   * e.g. a GraphML export would return 'xml'.
   *
   * @return :string the extension that this export module saves to.
   **/
  virtual std::string fileExtension() const = 0;

  /**
   * @brief The export operations should take place here.
   * @param the output stream
   * @return bool Whether the export was successful or not.
   **/
  virtual bool exportGraph(std::ostream &os)=0;
  /** It is the root graph*/
  Graph *graph;
  ///
  PluginProgress *pluginProgress;
  DataSet *dataSet;
};


}
#endif
///@endcond
