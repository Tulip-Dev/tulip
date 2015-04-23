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


#ifndef _IMPORTMODULE_H
#define _IMPORTMODULE_H

#include <tulip/Plugin.h>

#include <list>
#include <string>

namespace tlp {

static const std::string IMPORT_CATEGORY = "Import";

class PluginProgress;
class Graph;
class DataSet;


/**
 * @addtogroup Plugins
 * @brief Base class for import plug-ins.
**/
class ImportModule : public tlp::Plugin {
public:
  /**
  * @brief Initializes the DataSet to the one passed in the context.
  *
  * @param context The context this import plug-in runs into.
  **/
  ImportModule (const tlp::PluginContext* context) {
    if(context != NULL) {
      const tlp::AlgorithmContext* algoritmContext = dynamic_cast<const tlp::AlgorithmContext*>(context);
      assert(algoritmContext != NULL);
      graph = algoritmContext->graph;
      pluginProgress = algoritmContext->pluginProgress;
      dataSet = algoritmContext->dataSet;
    }
  }

  virtual std::list<std::string> fileExtensions() const {
    return std::list<std::string>();
  }

  virtual std::string getGroup() const {
    return "Import";
  }

  virtual std::string category() const {
    return IMPORT_CATEGORY;
  }
  std::string icon() const {
    return ":/tulip/gui/icons/32/plugin_import_export.png";
  }

  /**
  * @brief The import operations should take place here.
  *
  * @return bool Whether the import was successful or not.
  **/
  virtual bool importGraph()=0;

  /**
  * @brief The Graph in which to write the data to import.
  **/
  Graph *graph;

  /**
  * @brief A means to report progress to the user.
  **/
  PluginProgress *pluginProgress;

  /**
  * @brief A container for the parameters of this import plug-in.
  **/
  DataSet *dataSet;
};


}
#endif
///@endcond
