/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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
#ifndef THREADEDCOMPUTEPROPERTY_H
#define THREADEDCOMPUTEPROPERTY_H

#include <QThread>

#include <tulip/Graph.h>
#include <tulip/ForEach.h>

#include <iostream>

namespace tlp {
class DataSet;
class PluginProgress;
}

struct TLP_QT_SCOPE AbstractComputeProperty {
public:
  explicit AbstractComputeProperty(tlp::Graph* graph, std::string name, std::string& errorMsg, tlp::PluginProgress* progress, tlp::DataSet* dataset);
  virtual bool run() = 0;

protected:
  tlp::Graph* _graph;
  std::string _name;
  std::string& _errorMsg;
  tlp::PluginProgress* _progress;
  tlp::DataSet* _dataset;
};

template<typename PropertyType>
struct ComputePropertyTemplate : public AbstractComputeProperty {
public:
  ComputePropertyTemplate(tlp::Graph* graph, std::string name, PropertyType* result, std::string& errorMsg, tlp::PluginProgress* progress, tlp::DataSet* dataset):
    AbstractComputeProperty(graph, name, errorMsg, progress, dataset), _property(result) {
  }
  virtual bool run() {
    bool res = _graph->computeProperty<PropertyType>(_name, _property, _errorMsg, _progress, _dataset);
    return res;
  }

private:
  PropertyType* _property;
};

class TLP_QT_SCOPE ComputePropertyThread : public QThread {
  Q_OBJECT
public:
  explicit ComputePropertyThread(AbstractComputeProperty* computeProperty);
  virtual void run();
  bool computeProperty();

private:
  bool result;
  AbstractComputeProperty* _computeProperty;
  bool _finished;
public slots:
  void notifyFinished();
};

#endif //THREADEDCOMPUTEPROPERTY_H
