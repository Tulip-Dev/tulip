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

namespace tlp {
class Algorithm : tlp::Plugin {
%TypeHeaderCode
#include <tulip/Algorithm.h>
%End

public:

%Docstring
Bases: :class:`tlp.Plugin`

This abstract class describes a general algorithm plugin.
Basic functionality consists in checking the algorithm can run on the current Graph 
(e.g. is the graph simple ?) then running the algorithm.
The algorithm can and should report progress and which task it is performing 
if it is decomposed in multiple phases (e.g. layouting the graph, coloring it, ...).

.. rubric:: class attributes

.. py:attribute:: graph

   The :class:`tlp.Graph` this algorithm will be run on.

.. py:attribute:: dataSet

   A dictionnary containing input parameters for this algorithm, if any. It can also be used to write
   output parameters computed by the algorithm that will remain available after its execution.

.. py:attribute:: pluginProgress

   A :class:`tlp.PluginProgress` to give feedback to the user. It can be :const:`None`, so use with caution. 

.. rubric:: class methods

%End

  Algorithm(const tlp::PluginContext* context);

  virtual ~Algorithm();

  virtual bool run() = 0;
%Docstring
tlp.Algorithm.run()

This method is the entry point of the algorithm when it is called.
Derived classes must implement it. It is a good practice to report progress through the PluginProgress.
The PluginProgress should also be used to report errors, if any. A boolean must also be returned
to indicate if the algorithm was successful.

:rtype: boolean
%End

  virtual bool check(std::string & /Out/) /TypeHint="Tuple[bool, str]"/;
%Docstring
tlp.Algorithm.check()

This method is called before applying the algorithm on the input graph.
You can perform some precondition checks here. Derived classes can reimplement it.
Must return a tuple whose first member indicates if the algorithm can be applied
and the second one can be used to provide an error message

:rtype: (boolean, string)
%End

  virtual std::string category() const;

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

