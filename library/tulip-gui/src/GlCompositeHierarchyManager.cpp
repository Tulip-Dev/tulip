/**
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
#include "tulip/GlCompositeHierarchyManager.h"

#include <tulip/Graph.h>
#include <tulip/GlLayer.h>
#include <tulip/GlConvexGraphHull.h>
#include <tulip/GlTextureManager.h>
#include <tulip/DoubleProperty.h>
#include <tulip/LayoutProperty.h>
#include <tulip/SizeProperty.h>

#include <sstream>

using namespace std;

namespace tlp {

class GlHierarchyMainComposite : public GlConvexGraphHullsComposite {
public:
  GlHierarchyMainComposite(GlCompositeHierarchyManager *manager);
  void setVisible(bool visible) override;

private:
  GlCompositeHierarchyManager *_manager;
};

GlHierarchyMainComposite::GlHierarchyMainComposite(GlCompositeHierarchyManager *manager)
    : _manager(manager) {}

void GlHierarchyMainComposite::setVisible(bool visible) {
  _manager->setVisible(visible);
  GlSimpleEntity::setVisible(visible);
}

const std::string GlCompositeHierarchyManager::temporaryPropertyValue =
    "temporaryPropertyFromGlCompositeHierarchyManager";

GlCompositeHierarchyManager::GlCompositeHierarchyManager(Graph *graph, GlLayer *layer,
                                                         const std::string &layerName,
                                                         LayoutProperty *layout, SizeProperty *size,
                                                         DoubleProperty *rotation, bool visible,
                                                         const std::string &namingProperty,
                                                         const std::string &subCompositeSuffix)
    : _currentColor(0), _currentTexture(0), _graph(graph), _layer(layer),
      _composite(new GlHierarchyMainComposite(this)), _layout(layout), _size(size),
      _rotation(rotation), _layerName(layerName), _isVisible(visible),
      _subCompositesSuffix(subCompositeSuffix), _nameAttribute(namingProperty) {
  _layer->addGlEntity(_composite, _layerName);
  _composite->setVisible(_isVisible);
  _layout->addObserver(this);
  _size->addObserver(this);
  _rotation->addObserver(this);

  _fillColors.reserve(6);
  _fillColors.emplace_back(255, 148, 169, 100);
  _fillColors.emplace_back(153, 250, 255, 100);
  _fillColors.emplace_back(255, 152, 248, 100);
  _fillColors.emplace_back(157, 152, 255, 100);
  _fillColors.emplace_back(255, 220, 0, 100);
  _fillColors.emplace_back(252, 255, 158, 100);

  _fillTextures.reserve(5);
  _fillTextures.emplace_back(TulipBitmapDir + "verticalStripesTexture.png");
  _fillTextures.emplace_back(TulipBitmapDir + "horizontalStripesTexture.png");
  _fillTextures.emplace_back(TulipBitmapDir + "slashStripesTexture.png");
  _fillTextures.emplace_back(TulipBitmapDir + "backSlashStripesTexture.png");
  _fillTextures.emplace_back(TulipBitmapDir + "circleTexture.png");

  if (_isVisible) {
    createComposite();
  }
}

const tlp::Color &GlCompositeHierarchyManager::getColor() {
  const tlp::Color &current = _fillColors[_currentColor++];
  _currentColor = _currentColor % _fillColors.size();
  return current;
}

const std::string &GlCompositeHierarchyManager::getTexture() {
  const std::string &current = _fillTextures[_currentTexture++];
  _currentTexture = _currentTexture % _fillTextures.size();
  return current;
}

void GlCompositeHierarchyManager::buildComposite(Graph *current,
                                                 GlConvexGraphHullsComposite *composite) {
  static bool loadTextures = true;
  if (loadTextures) {
    for (const string &texture : _fillTextures)
      GlTextureManager::loadTexture(texture);
    loadTextures = false;
  }
  current->addListener(this);

  stringstream naming;
  naming << current->getName() << " [#" << current->getId() << ']';
  GlConvexGraphHull *hull =
      new GlConvexGraphHull(composite, naming.str(), getColor(), _fillTextures[_currentColor - 1],
                            current, _layout, _size, _rotation);
  hull->setTextureZoom(0.02);

  _graphsComposites.emplace(
      current, std::pair<GlConvexGraphHullsComposite *, GlConvexGraphHull *>(composite, hull));

  if (!current->subGraphs().empty()) {
    GlConvexGraphHullsComposite *newComposite = new GlConvexGraphHullsComposite();
    naming << " - " << _subCompositesSuffix;
    composite->addGlEntity(newComposite, naming.str());

    for (Graph *sg : current->subGraphs()) {
      buildComposite(sg, newComposite);
    }
  }
}

void GlCompositeHierarchyManager::treatEvent(const Event &evt) {
  const GraphEvent *gEvt = dynamic_cast<const GraphEvent *>(&evt);

  if (gEvt) {
    Graph *graph = gEvt->getGraph();

    switch (gEvt->getType()) {
    case GraphEvent::TLP_ADD_NODE:

      if (_graphsComposites[graph].second) {
        _graphsComposites[graph].second->updateHull();
      }

      break;

    case GraphEvent::TLP_AFTER_ADD_SUBGRAPH:
    case GraphEvent::TLP_AFTER_DEL_SUBGRAPH: {
      createComposite();
      break;
    }

    case GraphEvent::TLP_BEFORE_SET_ATTRIBUTE: {
      auto attributeName = gEvt->getAttributeName();

      // we save the old property value in a temporary attribute, so we can find the GlEntity and
      // change its name once the attribute has been set
      if (attributeName == _nameAttribute) {
        graph->setAttribute<string>(GlCompositeHierarchyManager::temporaryPropertyValue,
                                    graph->getName());
      }

      break;
    }

    case GraphEvent::TLP_AFTER_SET_ATTRIBUTE: {
      auto attributeName = gEvt->getAttributeName();

      if (attributeName == _nameAttribute) {
        string oldPropertyValue;
        graph->getAttribute<string>(GlCompositeHierarchyManager::temporaryPropertyValue,
                                    oldPropertyValue);
        graph->removeAttribute(GlCompositeHierarchyManager::temporaryPropertyValue);
        GlComposite *composite = _graphsComposites[graph].first;
        GlSimpleEntity *temporaryEntity = composite->findGlEntity(oldPropertyValue);

        if (temporaryEntity) {
          composite->deleteGlEntity(temporaryEntity);
          composite->addGlEntity(temporaryEntity, graph->getName());
        }
      }

      break;
    }

    case GraphEvent::TLP_BEFORE_DEL_LOCAL_PROPERTY: {
      auto propertyName = gEvt->getPropertyName();
      if (propertyName == _layout->getName())
        _layout = nullptr;

      break;
    }

    case GraphEvent::TLP_AFTER_DEL_LOCAL_PROPERTY: {
      if (!_layout) {
        auto propertyName = gEvt->getPropertyName();
        _layout = graph->getProperty<LayoutProperty>(propertyName);
        _layout->addObserver(this);
        treatEvents(vector<Event>());
      }
      break;
    }

    case GraphEvent::TLP_ADD_LOCAL_PROPERTY: {
      auto propertyName = gEvt->getPropertyName();

      if (propertyName == _layout->getName()) {
        _layout->removeObserver(this);
        _layout = graph->getProperty<LayoutProperty>(propertyName);
        _layout->addObserver(this);
        if (_layout->hasNonDefaultValuatedNodes(graph)) {
          treatEvents(vector<Event>());
        }
      } else if (propertyName == _size->getName()) {
        _size->removeObserver(this);
        _size = graph->getProperty<SizeProperty>(propertyName);
        _size->addObserver(this);
        if (_size->hasNonDefaultValuatedNodes(graph)) {
          treatEvents(vector<Event>());
        }
      } else if (propertyName == _rotation->getName()) {
        _rotation->removeObserver(this);
        _rotation = graph->getProperty<DoubleProperty>(propertyName);
        _rotation->addObserver(this);
        if (_rotation->hasNonDefaultValuatedNodes(graph)) {
          treatEvents(vector<Event>());
        }
      }
    }

    default:
      // we don't care about other events
      break;
    }
  }

  Graph *graph = dynamic_cast<Graph *>(evt.sender());
  if (graph && evt.type() == Event::TLP_DELETE) {
    if (graph == _graph) {
      _graph = nullptr;
      _graphsComposites.clear();
    } else
      _graphsComposites.erase(graph);
  }
}

void GlCompositeHierarchyManager::treatEvents(const std::vector<Event> &) {
  for (auto &it : _graphsComposites) {
    if (!it.first->isEmpty()) {
      it.second.second->updateHull(_layout, _size, _rotation);
    } else {
      it.second.second->setVisible(false);
    }
  }
}

void GlCompositeHierarchyManager::setGraph(tlp::Graph *graph) {
  // TODO here we could rebuild only if the graph is not in the composites map
  _graph = graph;
  //    deleteComposite();
  if (_isVisible)
    createComposite();
}

void GlCompositeHierarchyManager::createComposite() {
  _composite->reset(true);
  _graphsComposites.clear();
  LayoutProperty *layout = _graph->getProperty<LayoutProperty>(_layout->getName());
  if (layout != _layout) {
    _layout->removeObserver(this);
    _layout = layout;
    _layout->addObserver(this);
  }
  SizeProperty *size = _graph->getProperty<SizeProperty>(_size->getName());
  if (size != _size) {
    _size->removeObserver(this);
    _size = size;
    _size->addObserver(this);
  }
  DoubleProperty *rotation = _graph->getProperty<DoubleProperty>(_rotation->getName());
  if (rotation != _rotation) {
    _rotation->removeObserver(this);
    _rotation = rotation;
    _rotation->addObserver(this);
  }

  buildComposite(_graph, _composite);
}

void GlCompositeHierarchyManager::setVisible(bool visible) {
  if (_isVisible == visible)
    return;

  _isVisible = visible;
  _composite->setVisible(visible);

  if (_isVisible) {
    createComposite();
  }
}

bool GlCompositeHierarchyManager::isVisible() const {
  return _isVisible;
}

DataSet GlCompositeHierarchyManager::getData() {
  DataSet set;

  for (const auto &it : _graphsComposites) {
    unsigned int graphId = it.first->getId();
    unsigned int visibility =
        uint(it.second.first->isVisible()) * 2 + uint(it.second.second->isVisible());
    stringstream graph;
    graph << graphId;
    set.set(graph.str(), visibility);
  }

  return set;
}

void GlCompositeHierarchyManager::setData(const DataSet &dataSet) {
  for (auto &it : _graphsComposites) {
    stringstream graph;
    graph << it.first->getId();

    if (dataSet.exists(graph.str())) {
      unsigned int visibility = 0;
      dataSet.get(graph.str(), visibility);
      bool firstVisibility = visibility - 1 > 0;
      it.second.first->setVisible(firstVisibility);
      bool secondVisibility = visibility % 2 > 0;
      it.second.second->setVisible(secondVisibility);
    }
  }
}
} // namespace tlp
