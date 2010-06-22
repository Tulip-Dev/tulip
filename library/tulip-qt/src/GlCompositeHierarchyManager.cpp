/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
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
																													 SizeProperty *size, DoubleProperty *rotation, bool visible, std::string namingProperty, std::string subCompositeSuffix) 
		:_currentColor(0), _graph(graph), _layer(layer), _composite(new GlHierarchyMainComposite(this)), _layout(layout), _size(size), _rotation(rotation), _layerName(layerName), 
		 _subCompositesSuffix(subCompositeSuffix), _property(namingProperty), _isVisible(visible) {
		this->_layer->addGlEntity(this->_composite, this->_layerName);
		this->_composite->setVisible(_isVisible);
		
		_fillColors.push_back(Color(255, 148, 169, 100));
    _fillColors.push_back(Color(153, 250, 255, 100));
    _fillColors.push_back(Color(255, 152, 248, 100));
    _fillColors.push_back(Color(157, 152, 255, 100));
    _fillColors.push_back(Color(255, 220, 0, 100));
    _fillColors.push_back(Color(252, 255, 158, 100));
		
		if(_isVisible) {
			createComposite();
		}
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
		
		string propertyValue;
		current->getAttribute<string>(_property, propertyValue);
		GlConvexGraphHull* hull = new GlConvexGraphHull(composite, propertyValue, getColor(), current, _layout, _size, _rotation);
		
		_graphsComposites.insert(std::pair<Graph*, std::pair<GlComposite*, GlConvexGraphHull*> >(current, std::pair<GlComposite*, GlConvexGraphHull*>(composite, hull)));
		
		GlComposite* newComposite = new GlComposite();
		tlp::Iterator<Graph*>* it = current->getSubGraphs();
		while(it->hasNext()) {
			composite->addGlEntity(newComposite, propertyValue + _subCompositesSuffix);
			this->buildComposite(it->next(), newComposite);
		}
		delete it;
  }
  
  void GlCompositeHierarchyManager::addSubGraph(Graph *parent, Graph *subgraph) {
		string propertyValue;
		subgraph->getAttribute<string>(_property, propertyValue);
				
		string parentPropertyValue;
		parent->getAttribute<string>(_property, parentPropertyValue);
		GlComposite* composite = _graphsComposites[parent].first;
		GlComposite* parentComposite = dynamic_cast<GlComposite*>(composite->findGlEntity(parentPropertyValue + _subCompositesSuffix));
		if(!parentComposite) {
			parentComposite = new GlComposite();
			composite->addGlEntity(parentComposite, parentPropertyValue + _subCompositesSuffix);
		}
		GlConvexGraphHull* hull = new GlConvexGraphHull(parentComposite, propertyValue, getColor(), subgraph, _layout, _size, _rotation);
		_graphsComposites.insert(std::pair<Graph*, std::pair<GlComposite*, GlConvexGraphHull*> >(subgraph, std::pair<GlComposite*, GlConvexGraphHull*>(composite, hull)));
		subgraph->addGraphObserver(this);
	}
	
	void GlCompositeHierarchyManager::delSubGraph(Graph*, Graph *subgraph) {
		GlComposite* composite = _graphsComposites[subgraph].first;
		string propertyValue;
		subgraph->getAttribute<string>(_property, propertyValue);
		
		GlConvexGraphHull* oldHull = _graphsComposites[subgraph].second;
		oldHull->setVisible(_isVisible);
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
			GlComposite* composite = _graphsComposites[graph].first;
			GlSimpleEntity* temporaryEntity = composite->findGlEntity(oldPropertyValue);
			if(temporaryEntity) {
				composite->deleteGlEntity(temporaryEntity);
				composite->addGlEntity(temporaryEntity, propertyValue);
			}
		}
	}
	
	void GlCompositeHierarchyManager::setGraph(tlp::Graph* graph) {
		//TODO here we could rebuild only if the graph is not in the composites map
		this->_graph = graph;
		deleteComposite();
		
		if(_composite->isVisible())
			this->createComposite();
	}
	
	void GlCompositeHierarchyManager::createComposite() {
		this->_composite->reset(true);
// 		this->_composite->setVisible(_isVisible);
		this->buildComposite(_graph, _composite);
	}
	
	void GlCompositeHierarchyManager::deleteComposite() {
		for(std::map<tlp::Graph*, std::pair<tlp::GlComposite*, tlp::GlConvexGraphHull*> >::const_iterator it = _graphsComposites.begin(); it != _graphsComposites.end(); ++it) {
			it->first->removeGraphObserver(this);
			delete it->second.second;
		}
		_graphsComposites.clear();
	}

void GlCompositeHierarchyManager::setVisible(bool visible) {
		if(_isVisible == visible)
			return;
		
		this->_isVisible = visible;
		if(_isVisible) {
			createComposite();
		}
		else {
			deleteComposite();
		}
	}
	
	bool GlCompositeHierarchyManager::isVisible() const {
		return this->_isVisible;
	}
	
	GlHierarchyMainComposite::GlHierarchyMainComposite(GlCompositeHierarchyManager* manager) :_manager(manager) {
	}
	
	void GlHierarchyMainComposite::setVisible(bool visible) {
		_manager->setVisible(visible);
		GlSimpleEntity::setVisible(visible);
	}
}

