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
