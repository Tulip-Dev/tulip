#include "tulip/GlCompositeHierarchyManager.h"

#include <tulip/Graph.h>
#include "tulip/GlLayer.h"
#include "tulip/GlConvexHull.h"

#include <sstream>
#include <tulip/GlConvexGraphHull.h>
#include <tulip/LayoutProperty.h>

using namespace std;

namespace tlp {

	const std::string GlCompositeHierarchyManager::temporaryPropertyValue = "temporaryPropertyFromGlCompositeHierarchyManager";
	
	GlCompositeHierarchyManager::GlCompositeHierarchyManager(Graph* graph, GlLayer* layer, std::string layerName, LayoutProperty *layout, 
																													 SizeProperty *size, DoubleProperty *rotation, std::string namingProperty, std::string subCompositeSuffix) 
		:_currentColor(0), _graph(graph), _layer(layer), _composite(new GlHierarchyMainComposite(this)), _layout(layout), _size(size), _rotation(rotation), _layerName(layerName), 
		 _subCompositesSuffix(subCompositeSuffix), _property(namingProperty) {
		this->_composite->setVisible(false);
		this->_layer->addGlEntity(this->_composite, this->_layerName);
		
		_fillColors.push_back(Color(255, 148, 169, 100));
    _fillColors.push_back(Color(153, 250, 255, 100));
    _fillColors.push_back(Color(255, 152, 248, 100));
    _fillColors.push_back(Color(157, 152, 255, 100));
    _fillColors.push_back(Color(255, 220, 0, 100));
    _fillColors.push_back(Color(252, 255, 158, 100));
	}
	
	GlCompositeHierarchyManager::~GlCompositeHierarchyManager() {
		
	}

	const tlp::Color GlCompositeHierarchyManager::getColor() {
		tlp::Color current = this->_fillColors.at(_currentColor++);
		_currentColor = _currentColor % _fillColors.size();
		return current;
	}

  void GlCompositeHierarchyManager::buildComposite(Graph* current, GlComposite* composite) {
		current->addGraphObserver(this);
		GlComposite* newComposite = new GlComposite();
		GlConvexGraphHull* hull = new GlConvexGraphHull(getColor(), current, _layout, _size, _rotation);
// 		hull->setVisible(false);
		
		string propertyValue;
		current->getAttribute<string>(_property, propertyValue);
		
		composite->addGlEntity(hull, propertyValue);
		_graphsComposites.insert(std::pair<Graph*, GlComposite*>(current, composite));
		
		tlp::Iterator<Graph*>* it = current->getSubGraphs();
		if(it->hasNext()) {
			composite->addGlEntity(newComposite, propertyValue + _subCompositesSuffix);
		}
		while(it->hasNext()) {
			this->buildComposite(it->next(), newComposite);
		}
		delete it;
  }
  
  void GlCompositeHierarchyManager::addSubGraph(Graph *parent, Graph *subgraph) {
		GlConvexGraphHull* hull = new GlConvexGraphHull(getColor(), subgraph, _layout, _size, _rotation);
		string propertyValue;
		subgraph->getAttribute<string>(_property, propertyValue);
		
		string parentPropertyValue;
		parent->getAttribute<string>(_property, parentPropertyValue);
		GlComposite* composite = _graphsComposites.at(parent);
		GlComposite* parentComposite = dynamic_cast<GlComposite*>(composite->findGlEntity(parentPropertyValue + _subCompositesSuffix));
		if(parentComposite) {
			parentComposite->addGlEntity(hull, propertyValue);
		}
		else {
			parentComposite = new GlComposite();
			composite->addGlEntity(parentComposite, parentPropertyValue + _subCompositesSuffix);
			parentComposite->addGlEntity(hull, propertyValue);
		}
		_graphsComposites.insert(std::pair<Graph*, GlComposite*>(subgraph, parentComposite));
		subgraph->addGraphObserver(this);
	}
	
	void GlCompositeHierarchyManager::delSubGraph(Graph*, Graph *subgraph) {
		GlComposite* composite = _graphsComposites.at(subgraph);
		string propertyValue;
		subgraph->getAttribute<string>(_property, propertyValue);
		
		GlSimpleEntity* oldHull = composite->findGlEntity(propertyValue);
		composite->deleteGlEntity(oldHull);
		oldHull->setVisible(false);
		GlSimpleEntity* oldComposite = composite->findGlEntity(propertyValue + _subCompositesSuffix);
		composite->deleteGlEntity(oldComposite);
		delete oldHull;
		delete oldComposite;
	}
	
	void GlCompositeHierarchyManager::beforeSetAttribute(Graph* graph, const std::string& property) {
		//we save the old property value in a temporary attribute, so we can find the GlEntity and change its name once the attribute has been set
		if(property == _property) {
			string propertyValue;
			graph->getAttribute<string>(_property, propertyValue);
			graph->setAttribute<string>(GlCompositeHierarchyManager::temporaryPropertyValue, propertyValue);
		}
	}
	
	void GlCompositeHierarchyManager::afterSetAttribute(Graph* graph, const std::string& property) {
		if(property == _property) {
			string propertyValue;
			graph->getAttribute<string>(_property, propertyValue);
			string oldPropertyValue;
			graph->getAttribute<string>(GlCompositeHierarchyManager::temporaryPropertyValue, oldPropertyValue);
			graph->removeAttribute(GlCompositeHierarchyManager::temporaryPropertyValue);
			GlComposite* composite = _graphsComposites.at(graph);
			GlSimpleEntity* temporaryEntity = composite->findGlEntity(oldPropertyValue);
			if(temporaryEntity) {
				composite->deleteGlEntity(temporaryEntity);
				composite->addGlEntity(temporaryEntity, propertyValue);
			}
		}
	}
	
	void GlCompositeHierarchyManager::setGraph(tlp::Graph* graph) {
		this->_graph = graph;
		
		//TODO here we could rebuild only if the graph is not in the composites map
		this->_layer->deleteGlEntity(this->_layerName);
		delete this->_composite;
		this->_graphsComposites.clear();;
		
		this->_composite = new GlComposite();
		this->_layer->addGlEntity(this->_composite, this->_layerName);
		
		if(_composite->isVisible())
			this->buildComposite(_graph, _composite);
	}
	
	void GlCompositeHierarchyManager::addNode(tlp::Graph* graph, tlp::node) {
		//whenever a node gets added to a graph we observe, we add this node's coordinates to it's hull
		string attributeValue;
		graph->getAttribute<string>(_property, attributeValue);
		GlComposite* parent = _graphsComposites[graph];
		GlSimpleEntity* simpleEntity = parent->findGlEntity(attributeValue);
		GlConvexGraphHull* hull = dynamic_cast<GlConvexGraphHull*>(simpleEntity);
		hull->updateHull();
	}
	
	void GlCompositeHierarchyManager::createComposite() {
		this->_composite->reset(true);
		this->buildComposite(_graph, _composite);
		_layout->addPropertyObserver(this);
	}
	
	void GlCompositeHierarchyManager::deleteComposite() {
		
		for(std::map<tlp::Graph*, tlp::GlComposite*>::const_iterator it = _graphsComposites.begin(); it != _graphsComposites.end(); ++it) {
			it->first->removeGraphObserver(this);
		}
		_graphsComposites.clear();
		_layout->removePropertyObserver(this);
	}
	
	void GlCompositeHierarchyManager::afterSetNodeValue(PropertyInterface*, const node) {
		string attributeValue;
		std::map<tlp::Graph*, tlp::GlComposite*>::const_iterator it = _graphsComposites.begin();
		while(it != _graphsComposites.end()) {
			it->first->getAttribute<string>(_property, attributeValue);
			GlSimpleEntity* simpleEntity = it->second->findGlEntity(attributeValue);
			GlConvexGraphHull* hull = dynamic_cast<GlConvexGraphHull*>(simpleEntity);
			hull->updateHull();
			++it;
		}
	}
	
	GlHierarchyMainComposite::GlHierarchyMainComposite(GlCompositeHierarchyManager* manager) :_manager(manager) {
	}
	
	void GlHierarchyMainComposite::setVisible(bool visible) {
		if(visible) {
			_manager->createComposite();
		}
		else {
			_manager->deleteComposite();
		}
		GlSimpleEntity::setVisible(visible);
	}
}

