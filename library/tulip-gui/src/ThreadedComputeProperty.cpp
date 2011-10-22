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
#include "tulip/ThreadedComputeProperty.h"

#include <QtCore/QCoreApplication>

AbstractComputeProperty::AbstractComputeProperty(tlp::Graph* graph, std::string name, std::string& errorMsg, tlp::PluginProgress* progress, tlp::DataSet* dataset):
  _graph(graph), _name(name), _errorMsg(errorMsg), _progress(progress), _dataset(dataset) {
}

//=================
ComputePropertyThread::ComputePropertyThread(AbstractComputeProperty* computeProperty) : _computeProperty(computeProperty), _finished(false) {
  connect(this, SIGNAL(finished()), this, SLOT(notifyFinished()));
}

void ComputePropertyThread::run() {
  result = _computeProperty->run();
}

bool ComputePropertyThread::computeProperty() {
  start();

  while(!_finished) {
    QCoreApplication::processEvents();
  }

  return result;
}

void ComputePropertyThread::notifyFinished() {
  _finished = true;
}
