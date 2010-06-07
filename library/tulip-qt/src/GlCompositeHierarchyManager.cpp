#include "tulip/GlCompositeHierarchyManager.h"

#include <tulip/Graph.h>
#include "tulip/GlLayer.h"
#include "tulip/GlConvexHull.h"

#include <sstream>
#include <tulip/GlConvexGraphHull.h>

using namespace std;

namespace tlp {

	const std::string GlCompositeHierarchyManager::temporaryPropertyValue = "temporaryPropertyFromGlCompositeHierarchyManager";
	
	GlCompositeHierarchyManager::GlCompositeHierarchyManager(Graph* graph, GlLayer* layer, std::string layerName, LayoutProperty *layout, 
																													 SizeProperty *size, DoubleProperty *rotation, std::string namingProperty, std::string subCompositeSuffix) 
		:_currentColor(0), _graph(graph), _layer(layer), _layerName(layerName), _layout(layout), _size(size), _rotation(rotation), _subCompositesSuffix(subCompositeSuffix), _property(namingProperty) {
		this->_composite = new GlComposite();
		this->_composite->setVisible(false);
		this->_layer->addGlEntity(this->_composite, this->_layerName);
		
		_fillColors.push_back(Color(255, 148, 169, 100));
    _fillColors.push_back(Color(153, 250, 255, 100));
    _fillColors.push_back(Color(255, 152, 248, 100));
    _fillColors.push_back(Color(157, 152, 255, 100));
    _fillColors.push_back(Color(255, 220, 0, 100));
    _fillColors.push_back(Color(252, 255, 158, 100));
		
		this->buildComposite(_graph, _composite);
	}
	
	GlCompositeHierarchyManager::~GlCompositeHierarchyManager() {
		
	}

	const tlp::Color GlCompositeHierarchyManager::getColor() {
		tlp::Color current = this->_fillColors.at(_currentColor++);
		_currentColor = _currentColor > _fillColors.size() ? 0 : _currentColor;
		return current;
	}

  void GlCompositeHierarchyManager::buildComposite(Graph* current, GlComposite* composite) {
    
		current->addGraphObserver(this);
		GlComposite* newComposite = new GlComposite();
		GlConvexGraphHull* hull = new GlConvexGraphHull(getColor(), current, _layout, _size, _rotation);
		
		string propertyValue;
		current->getAttribute<string>(_property, propertyValue);
		
		composite->addGlEntity(hull, propertyValue);
		_hulls.insert(std::pair<Graph*, GlComposite*>(current, composite));
		
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
		GlComposite* composite = _hulls.at(parent);
		GlEntity* parentEntity = composite->findGlEntity(parentPropertyValue);
		GlComposite* parentComposite = dynamic_cast<GlComposite*>(composite->findGlEntity(parentPropertyValue + _subCompositesSuffix));
		if(parentComposite) {
			parentComposite->addGlEntity(hull, propertyValue);
		}
		else {
			parentComposite = new GlComposite();
			composite->addGlEntity(parentComposite, parentPropertyValue + _subCompositesSuffix);
			parentComposite->addGlEntity(hull, propertyValue);
		}
		_hulls.insert(std::pair<Graph*, GlComposite*>(subgraph, parentComposite));
		subgraph->addGraphObserver(this);
	}
	
	void GlCompositeHierarchyManager::delSubGraph(Graph *parent, Graph *subgraph) {
		GlComposite* composite = _hulls.at(subgraph);
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
			graph->getAttribute<string>(property, propertyValue);
			graph->setAttribute<string>(GlCompositeHierarchyManager::temporaryPropertyValue, propertyValue);
		}
	}
	
	void GlCompositeHierarchyManager::afterSetAttribute(Graph* graph, const std::string& property) {
		if(property == _property) {
			string propertyValue;
			graph->getAttribute<string>(property, propertyValue);
			string oldPropertyValue;
			graph->getAttribute<string>(GlCompositeHierarchyManager::temporaryPropertyValue, oldPropertyValue);
			GlComposite* composite = _hulls.at(graph);
			GlSimpleEntity* temporaryEntity = composite->findGlEntity(oldPropertyValue);
			if(temporaryEntity) {
				composite->deleteGlEntity(temporaryEntity);
				composite->addGlEntity(temporaryEntity, propertyValue);
			}
		}
	}
}
