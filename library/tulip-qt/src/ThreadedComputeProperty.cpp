#include "tulip/ThreadedComputeProperty.h"

#include <QtCore/QCoreApplication>

#include <tulip/Graph.h>

AbstractComputeProperty::AbstractComputeProperty(tlp::Graph* graph, std::string name, std::string errorMsg, tlp::PluginProgress* progress, tlp::DataSet* dataset):
  _graph(graph), _name(name), _errorMsg(errorMsg), _progress(progress), _dataset(dataset) {
}

//=================
ComputePropertyThread::ComputePropertyThread(AbstractComputeProperty* computeProperty) : _computeProperty(computeProperty), _finished(false) {
  connect(this, SIGNAL(finished()), this, SLOT(notifyFinished()));
}

void ComputePropertyThread::run() {
  _result = _computeProperty->run();
}

bool ComputePropertyThread::computeProperty() {
  start();
  while(!_finished) {
    QCoreApplication::processEvents();
  }
  return _result;
}

void ComputePropertyThread::notifyFinished() {
  _finished = true;
}
