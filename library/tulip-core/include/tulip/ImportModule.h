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
 * @brief Base class for import plugins.
 **/
class TLP_SCOPE ImportModule : public tlp::Plugin {
public:
  ImportModule(const tlp::PluginContext *context);
  /**
   * @brief Gets the extensions of the file formats the plugin can import.
   * e.g. a TLP import would return 'tlp'.
   *
   * @return the list of file extensions the plugin can import.
   **/
  virtual std::list<std::string> fileExtensions() const {
    return std::list<std::string>();
  }

  std::string category() const override {
    return IMPORT_CATEGORY;
  }

  std::string icon() const override {
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

/**
 * @addtogroup Plugins
 * @brief Base class for plugins importing from a file.
 **/
class TLP_SCOPE ImportFileModule : public ImportModule {
protected:
  /**
   * @brief The pathname of the file to import
   **/
  std::string filename;

  /**
   * @brief The supported file extensions
   **/
  std::list<std::string> extensions;

public:
  ImportFileModule(const tlp::PluginContext *context, std::list<std::string> exts = {});

  /**
   * @brief override the inherited method
   **/
  std::list<std::string> fileExtensions() const override {
    return extensions;
  }

  /**
   * @brief check plugin parameters
   **/
  virtual bool check();

  /**
   * @brief create the graph contents from the file data
   **/
  virtual bool importFile() = 0;

  /**
   * @brief override the inherited method
   **/
  bool importGraph() override;
};

} // namespace tlp
#endif
///@endcond
