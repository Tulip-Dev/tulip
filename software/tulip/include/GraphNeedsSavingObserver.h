#ifndef GRAPHNEEDSSAVINGOBSERVER_H
#define GRAPHNEEDSSAVINGOBSERVER_H

#include <tulip/ObservableGraph.h>
#include <tulip/ObservableProperty.h>
#include <tulip/Observable.h>
#include <tulip/Graph.h>

#include <QtGui/qtabwidget.h>
#include <tulip/PropertyInterface.h>

class GraphNeedsSavingObserver : public tlp::GraphObserver, public tlp::PropertyObserver, public tlp::Observer {
   public : 
      GraphNeedsSavingObserver(QTabWidget* tabWidget, int graphIndex, tlp::Graph* graph, bool = true) :_needsSaving(false), _tabIndex(graphIndex), _tabWidget(tabWidget), _graph(graph) {
        addObserver();
      }
      virtual ~GraphNeedsSavingObserver() {
        
      }
      
      virtual void addEdge(tlp::Graph* , const tlp::edge ) { doNeedSaving(); }
      virtual void addNode(tlp::Graph* , const tlp::node ) { doNeedSaving(); }
      virtual void afterSetAllEdgeValue(tlp::PropertyInterface* ) { doNeedSaving(); }
      virtual void afterSetAllNodeValue(tlp::PropertyInterface* ) { doNeedSaving(); }
      virtual void afterSetAttribute(tlp::Graph* , const std::string& ) { doNeedSaving(); }
      virtual void afterSetEdgeValue(tlp::PropertyInterface* , const tlp::edge ) { doNeedSaving(); }
      virtual void afterSetNodeValue(tlp::PropertyInterface* , const tlp::node ) { doNeedSaving(); }
      virtual void beforeSetAllEdgeValue(tlp::PropertyInterface* ) { doNeedSaving(); }
      virtual void beforeSetAllNodeValue(tlp::PropertyInterface* ) { doNeedSaving(); }
      virtual void beforeSetAttribute(tlp::Graph* , const std::string& ) { doNeedSaving(); }
      virtual void beforeSetEdgeValue(tlp::PropertyInterface* , const tlp::edge ) { doNeedSaving(); }
      virtual void beforeSetNodeValue(tlp::PropertyInterface* , const tlp::node ) { doNeedSaving(); }
      virtual void delEdge(tlp::Graph* , const tlp::edge ) { doNeedSaving(); }
      virtual void delNode(tlp::Graph* , const tlp::node ) { doNeedSaving(); }
      virtual void removeAttribute(tlp::Graph* , const std::string& ) { doNeedSaving(); }
      virtual void reverseEdge(tlp::Graph* , const tlp::edge ) { doNeedSaving(); }
      
      virtual void addLocalProperty(tlp::Graph* graph, const std::string& propertyName) {
        graph->getProperty(propertyName)->addPropertyObserver(this);
        doNeedSaving();
      }
      virtual void delLocalProperty(tlp::Graph* graph, const std::string& propertyName) {
        graph->getProperty(propertyName)->removePropertyObserver(this);
        doNeedSaving();
      }
      
      virtual void addSubGraph(tlp::Graph* , tlp::Graph* newSubGraph) {
        newSubGraph->addGraphObserver(this);
        tlp::Iterator<std::string>* it = newSubGraph->getProperties();
        while(it->hasNext()) {
          std::string propertyName = it->next();
          tlp::PropertyInterface* property = newSubGraph->getProperty(propertyName);
          property->addPropertyObserver(this);
        }
        delete it;
      }
      virtual void delSubGraph(tlp::Graph* , tlp::Graph* newSubGraph) {
        tlp::Iterator<std::string>* it = newSubGraph->getProperties();
        while(it->hasNext()) {
          std::string propertyName = it->next();
          tlp::PropertyInterface* property = newSubGraph->getProperty(propertyName);
          property->removePropertyObserver(this);
        }
        delete it;
        newSubGraph->removeGraphObserver(this);
      }
      
      virtual void destroy(tlp::Graph* graph) {
        graph->removeGraphObserver(this);
      }
      virtual void destroy(tlp::PropertyInterface* property) {
        property->removePropertyObserver(this);
      }
    
      virtual void observableDestroyed(tlp::Observable* o) {
        o->removeObserver(this);
      }
      virtual void update(std::set< tlp::Observable* >::iterator, std::set< tlp::Observable* >::iterator) { doNeedSaving(); }
      
      void saved() {
        _needsSaving = false;
        addObserver();
      }
      
      bool needSaving() const { 
        return _needsSaving; 
      }
      
      void doNeedSaving() {
        if(!_needsSaving) {
          _tabWidget->setTabText(_tabIndex, _tabWidget->tabText(_tabIndex) + " *");
          tlp::Iterator<tlp::Graph*>* it = _graph->getSubGraphs();
          while(it->hasNext()) {
            tlp::Graph* sub = it->next();
            delSubGraph(_graph, sub);
          }
          delSubGraph(_graph, _graph);
          delete it;
          _needsSaving = true;
        }
      }
  private:
    void addObserver() {
      tlp::Iterator<std::string>* it = _graph->getProperties();
      while(it->hasNext()) {
        std::string propertyName = it->next();
        tlp::PropertyInterface* property = _graph->getProperty(propertyName);
        property->addPropertyObserver(this);
      }
      delete it;
    }
    
    bool _needsSaving; 
    int _tabIndex;
    QTabWidget* _tabWidget;
    tlp::Graph* _graph;
 };

 #endif //GRAPHNEEDSSAVINGOBSERVER_H
