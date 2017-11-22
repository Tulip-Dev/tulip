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
  ImportModule(const tlp::PluginContext *context) {
    if (context != nullptr) {
      const tlp::AlgorithmContext *algoritmContext =
          static_cast<const tlp::AlgorithmContext *>(context);
      graph = algoritmContext->graph;
      pluginProgress = algoritmContext->pluginProgress;
      dataSet = algoritmContext->dataSet;
    }
  }

  /**
   * @brief Gets the extensions of the file formats the plugin can import.
   * e.g. a TLP import would return 'tlp'.
   *
   * @return the list of file extensions the plugin can import.
   **/
  virtual std::list<std::string> fileExtensions() const {
    return std::list<std::string>();
  }

  /**
   * @brief Gets the extensions of the gzipped file formats this plugin can import.
   * e.g. a TLP import would return 'tlp.gz and tlpz'.
   *
   * @since Tulip 5.0
   *
   * @return the list of gzipped file extensions the plugin can import.
   **/
  virtual std::list<std::string> gzipFileExtensions() const {
    return std::list<std::string>();
  }

  /**
   * @brief Gets all the extensions (normal and gzipped) of the file formats this plugin can import.
   *
   * @since Tulip 5.0
   *
   * @return the list of file extensions the plugin can import.
   **/
  std::list<std::string> allFileExtensions() const {
    std::list<std::string> zext(gzipFileExtensions());
    std::list<std::string> ext(fileExtensions());
    ext.splice(ext.end(), zext);
    return ext;
  }

  virtual std::string category() const {
    return IMPORT_CATEGORY;
  }

  std::string icon() const {
    return ":/tulip/gui/icons/64/document-import.png";
  }

  /**
  * @brief The import operations should take place here.
  *
  * @return bool Whether the import was successful or not.
  **/
  virtual bool importGraph() = 0;

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
