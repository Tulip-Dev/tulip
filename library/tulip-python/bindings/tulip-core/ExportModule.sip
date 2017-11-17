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

// +-------------------------------------------------------------------------+
// | Tulip Python Bindings                                                   |
// | inspired from bindings by the Booggie project development team          |
// | (https://sourceforge.net/projects/booggie/)                             |
// +-------------------------------------------------------------------------+

%ModuleHeaderCode
#include <tulip/ExportModule.h>
%End

namespace tlp {

class ExportModule: tlp::Plugin {
%TypeHeaderCode
#include <tulip/ExportModule.h>
%End

public:
  
%Docstring
Bases: :class:`tlp.WithParameter`

This abstract class describes an export plugin.

.. rubric:: class attributes

.. py:attribute:: graph

  The :class:`tlp.Graph` to export

.. py:attribute:: dataSet

  A dictionnary containing parameters for this export plugin, if any

.. py:attribute:: pluginProgress

  A :class:`tlp.PluginProgress` to give feedback to the user. It can be :const:`None`, so use with caution 

.. rubric:: class methods

%End  
  
  ExportModule(const tlp::PluginContext* context);
  
  virtual ~ExportModule();

  virtual std::string fileExtension() const = 0;
%Docstring
tlp.ExportModule.fileExtension()

.. versionadded:: 5.0

This method returns the extension of the file format this export plugin saves to.
Derived class can implement it in order for the plugin to be automatically called
by the :func:`tlp.saveGraph` function.

:rtype: string
%End

  virtual std::list<std::string> gzipFileExtensions() const;
%Docstring
tlp.ExportModule.gzipFileExtensions()

.. versionadded:: 5.0

This method returns a list of the extensions file format when compressed with gzip
this export plugin saves to.
Derived class can implement it in order for the plugin to be automatically called
by the :func:`tlp.saveGraph` function.

:rtype: list of string
%End

  std::list<std::string> allFileExtensions() const;
%Docstring
tlp.ExportModule.allFileExtensions()

.. versionadded:: 5.0

This method returns a list of all extensions file format (normal and gzipped) this export plugin saves to.

:rtype: list of string
%End

  virtual std::string category() const;

  virtual bool exportGraph(std::ostream &os) = 0;
%Docstring
tlp.ExportModule.exportGraph(os)

This method is called to export the graph to a file.
Derived classes must implement it. 

The os parameter is an output file stream (initialized by the Tulip GUI
or by the :func:`tlp.exportGraph` function.). To write data to the file,
you have to use the following syntax::

  # write the number of nodes and edges to the file
  os << self.graph.numberOfNodes() << "\n"
  os << self.graph.numberOfEdges() << "\n"

A boolean must also be returned
to indicate if the export was successful.

:param os: an output file stream
:type os: std.ostream 
:rtype: boolean
%End 

  tlp::Graph *graph;
  tlp::PluginProgress *pluginProgress;  
  tlp::DataSet *dataSet
  {
%GetCode
  // add file parameters to the dataset without the special prefix used internally by Tulip
  std::vector<std::string> filesPrefixes;
  filesPrefixes.push_back("file::");
  filesPrefixes.push_back("anyfile::");
  filesPrefixes.push_back("dir::");
  std::pair<std::string, tlp::DataType*> entry;
  stableForEach(entry, sipCpp->dataSet->getValues()) {
    for (size_t i = 0 ; i < filesPrefixes.size() ; ++i) {
      if (entry.first.length() > filesPrefixes[i].length() && entry.first.substr(0, filesPrefixes[i].length()) == filesPrefixes[i]) {
        std::string paramName = entry.first.substr(filesPrefixes[i].length());
        if (!sipCpp->dataSet->exist(paramName)) {
          sipCpp->dataSet->set(paramName, *(static_cast<std::string*>(entry.second->value)));
        }
      }
    }
  }
  sipPy = sipConvertFromType(sipCpp->dataSet, sipFindType("tlp::DataSet"), NULL);
%End
  };
  
};

};
