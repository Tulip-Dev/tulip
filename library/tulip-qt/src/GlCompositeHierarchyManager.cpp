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
		:_shouldRecreate(false), _currentColor(0), _graph(graph), _layer(layer), _composite(new GlHierarchyMainComposite(this)), _layout(layout), _size(size), _rotation(rotation), _layerName(layerName), 
		 _isVisible(visible), _subCompositesSuffix(subCompositeSuffix), _property(namingProperty) {
		this->_layer->addGlEntity(this->_composite, this->_layerName);
		this->_composite->setVisible(_isVisible);
		_layout->addObserver(this);
		
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
	
	const tlp::Color GlCompositeHierarchyManager::getColor() {
		tlp::Color current = this->_fillColors.at(_currentColor++);
		_currentColor = _currentColor % _fillColors.size();
		return current;
	}

  void GlCompositeHierarchyManager::buildComposite(Graph* current, GlComposite* composite) {
		current->addGraphObserver(this);
		
		string propertyValue;
		current->getAttribute<string>(_property, propertyValue);
		stringstream naming;
		naming << propertyValue << " (" << current->getId() << ")";
		GlConvexGraphHull* hull = new GlConvexGraphHull(composite, naming.str(), getColor(), current, _layout, _size, _rotation);
		
		_graphsComposites.insert(std::pair<Graph*, std::pair<GlComposite*, GlConvexGraphHull*> >(current, std::pair<GlComposite*, GlConvexGraphHull*>(composite, hull)));
		
		GlComposite* newComposite = new GlComposite();
		naming  << _subCompositesSuffix;
		composite->addGlEntity(newComposite, naming.str());
		
		tlp::Iterator<Graph*>* it = current->getSubGraphs();
		while(it->hasNext()) {
			this->buildComposite(it->next(), newComposite);
		}
		delete it;
  }
  
  void GlCompositeHierarchyManager::addSubGraph(Graph *, Graph*) {
		_shouldRecreate = true;
	}
	
	void GlCompositeHierarchyManager::delSubGraph(Graph*, Graph*subGraph) {
	      subGraph->removeGraphObserver(this);
		_shouldRecreate = true;
	}
	
	void GlCompositeHierarchyManager::beforeSetAttribute(Graph* graph, const std::string& property) {
		//we save the old property value in a temporary attribute, so we can find the GlEntity and change its name once the attribute has been set
		if(property == _property) {
			string propertyValue;
			graph->getAttribute<string>(_property, propertyValue);
			graph->setAttribute<string>(GlCompositeHierarchyManager::temporaryPropertyValue, propertyValue);
		}
	}

void GlCompositeHierarchyManager::addNode(tlp::Graph* source, tlp::node) {
  if(_graphsComposites[source].second) { 
    _graphsComposites[source].second->updateHull();
  }
}

// void GlCompositeHierarchyManager::afterSetNodeValue(PropertyInterface*, const node) {
//   updateHull();
// }

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

  void GlCompositeHierarchyManager::treatEvent(const Event& evt) {
    GraphObserver::treatEvent(evt);
  }
      
	
	void GlCompositeHierarchyManager::update(std::set< Observable* >::iterator, std::set< Observable* >::iterator) {
	  if(_shouldRecreate) {
	    createComposite();
	  }
	  else {
	    for(std::map<tlp::Graph*, std::pair<tlp::GlComposite*, tlp::GlConvexGraphHull*> >::const_iterator it = _graphsComposites.begin(); it != _graphsComposites.end(); ++it) {
	      if(it->first->numberOfNodes() != 0) {
		it->second.second->setVisible(true);
		it->second.second->updateHull();
	      }
	      else {
		it->second.second->setVisible(false);
	      }
	    }
	  }
	  _shouldRecreate = false;
	}
		
	void GlCompositeHierarchyManager::observableDestroyed(Observable*){
	}
	
	void GlCompositeHierarchyManager::setGraph(tlp::Graph* graph) {
		//TODO here we could rebuild only if the graph is not in the composites map
		this->_graph = graph;
// 		deleteComposite();
		
		if(_composite->isVisible())
			this->createComposite();
	}
	
	void GlCompositeHierarchyManager::createComposite() {
		this->_composite->reset(true);
		_graphsComposites.clear();
// 		this->_composite->setVisible(_isVisible);
		this->buildComposite(_graph, _composite);
	}

	void GlCompositeHierarchyManager::setVisible(bool visible) {
		if(_isVisible == visible)
			return;
		
		this->_isVisible = visible;
		_composite->setVisible(visible);
		
		if(_isVisible) {
			createComposite();
		}
	}
	
	bool GlCompositeHierarchyManager::isVisible() const{
		return _isVisible;
	}
	
	DataSet GlCompositeHierarchyManager::getData() {
		DataSet set;
		for(std::map<tlp::Graph*, std::pair<tlp::GlComposite*, tlp::GlConvexGraphHull*> >::const_iterator it = _graphsComposites.begin(); it != _graphsComposites.end(); ++it) {
			unsigned int graphId = it->first->getId();
			unsigned int visibility = (int)it->second.first->isVisible()*2 + (int)it->second.second->isVisible();
			stringstream graph;
			graph << graphId;
			set.set<unsigned int>(graph.str(), visibility);
// 			set.set<pair<unsigned int, unsigned int> >(graph.str(), pair<unsigned int, unsigned int>(graphId, visibility));
		}
		return set;
	}
	
	void GlCompositeHierarchyManager::setData(DataSet dataSet) {
		for(std::map<tlp::Graph*, std::pair<tlp::GlComposite*, tlp::GlConvexGraphHull*> >::const_iterator it = _graphsComposites.begin(); it != _graphsComposites.end(); ++it) {
			stringstream graph;
			graph << it->first->getId();
			if(dataSet.exist(graph.str())) {
				unsigned int visibility = 0;
				dataSet.get<unsigned int>(graph.str(), visibility);
				bool firstVisibility = visibility-1 > 0;
				it->second.first->setVisible(firstVisibility);
				bool secondVisibility = visibility%2 > 0;
				it->second.second->setVisible(secondVisibility);
			}
		}
	}
			
	GlHierarchyMainComposite::GlHierarchyMainComposite(GlCompositeHierarchyManager* manager) :_manager(manager) {
	}
		
	void GlHierarchyMainComposite::setVisible(bool visible) {
		_manager->setVisible(visible);
		GlSimpleEntity::setVisible(visible);
	}
}

