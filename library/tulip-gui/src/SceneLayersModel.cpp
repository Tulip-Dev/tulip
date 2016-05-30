/**
 *
 * This file is part of Tulip (www.tulip-software.org)
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

#include <tulip/SceneLayersModel.h>

#include <QFont>
#include <QVector>

#include <tulip/GlScene.h>
#include <tulip/GlLayer.h>
#include <tulip/GlGraph.h>


using namespace std;
using namespace tlp;

const quint32 NODES_ID = 1;
const quint32 EDGES_ID = 2;
const quint32 META_NODES_ID = 3;
const quint32 META_NODE_LABELS_ID = 4;
const quint32 NODE_LABELS_ID = 5;
const quint32 EDGE_LABELS_ID = 6;
const QVector<quint32> GRAPH_COMPOSITE_IDS = QVector<quint32>() << NODES_ID << EDGES_ID
    << META_NODES_ID
    << META_NODE_LABELS_ID << NODE_LABELS_ID << EDGE_LABELS_ID;
const int NO_STENCIL = 0xFF;
const int FULL_STENCIL = 0x02;

SceneLayersModel::SceneLayersModel(GlScene* scene, QObject *parent): TulipModel(parent), _scene(scene) {
  _scene->addListener(this);
}

QModelIndex SceneLayersModel::index(int row, int column,const QModelIndex &parent) const {
  if(!hasIndex(row,column,parent))
    return QModelIndex();

  if (!parent.isValid()) { // Top level: layers
    GlLayer* layer = _scene->getLayersList()[row];
    assert(layer != nullptr);
    return createIndex(row,column,layer);
  }

  GlComposite* composite = nullptr;

  if (!parent.parent().isValid())  {// 1st sublevel, parent is a layer
    GlLayer *layer = reinterpret_cast<GlLayer*>(parent.internalPointer());
    composite = layer->getComposite();
  }
  else {  // Deeper sublevel, the parent is a composite
    GlEntity *entity = reinterpret_cast<GlEntity*>(parent.internalPointer());
    GlGraph *glGraph = dynamic_cast<GlGraph*>(entity);
    if (glGraph) {
      return createIndex(row,column,GRAPH_COMPOSITE_IDS[row]);
    }
    composite = reinterpret_cast<GlComposite*>(parent.internalPointer());
  }

  int i=0;
  std::map<std::string, GlEntity*> entities = composite->getGlEntities();

  for (std::map<std::string,GlEntity*>::iterator it = entities.begin(); it != entities.end(); ++it) {
    if (i++ == row) {
      return createIndex(row,column,it->second);
    }
  }

  return QModelIndex();
}

QModelIndex SceneLayersModel::graphCompositeIndex() const {

  for(GlLayer *layer : _scene->getLayersList()) {
    GlComposite* composite = layer->getComposite();
    int row = 0;
    for (const pair<std::string, GlEntity*>& entity : composite->getGlEntities()) {
      if (entity.second == _scene->getMainGlGraph())
        return createIndex(row,0,_scene->getMainGlGraph());

      row++;
    }
  }

  return QModelIndex();
}

QModelIndex SceneLayersModel::parent(const QModelIndex &child) const {
  if (!child.isValid())
    return QModelIndex();

  if (GRAPH_COMPOSITE_IDS.contains(child.internalId()))
    return graphCompositeIndex();

  for(GlLayer *layer : _scene->getLayersList()) {
    if (layer == child.internalPointer())
      return QModelIndex(); // Item was a layer, aka. a top level item.
  }

  GlEntity* entity = reinterpret_cast<GlEntity*>(child.internalPointer());
  GlComposite* parent = entity->getParent();

  if (parent == nullptr)
    return QModelIndex();

  GlComposite* ancestor = parent->getParent();

  if (ancestor == nullptr) { // Parent is a layer composite
    int row = 0;

    for(GlLayer *layer : _scene->getLayersList()) {
      if (layer->getComposite() == parent)
        return createIndex(row,0,layer); // Item was a layer, aka. a top level item.

      row++;
    }
  } else {

    int row = 0;

    for (const pair<std::string, GlEntity*>& entity : ancestor->getGlEntities()) {
      if (entity.second == parent)
        return createIndex(row,0,parent);

      row++;
    }
  }

  return QModelIndex();
}

int SceneLayersModel::rowCount(const QModelIndex &parent) const {
  if (!parent.isValid()) { // Top level, layers count
    return _scene->getLayersList().size();
  }

  if (!parent.parent().isValid()) {// First sublevel: parent is a GlLayer
    GlLayer* layer = reinterpret_cast<GlLayer*>(parent.internalPointer());
    return layer->getComposite()->getGlEntities().size();
  }

  if (GRAPH_COMPOSITE_IDS.contains(parent.internalId()))
    return 0;

  GlEntity* entity = reinterpret_cast<GlEntity*>(parent.internalPointer());

  if (_scene->getMainGlGraph() == entity)
    return GRAPH_COMPOSITE_IDS.size();

  if (dynamic_cast<GlComposite*>(entity) != nullptr)
    return static_cast<GlComposite*>(entity)->getGlEntities().size();

  return 0;
}

int SceneLayersModel::columnCount(const QModelIndex &) const {
  return 3;
}

QVariant SceneLayersModel::data(const QModelIndex &index, int role) const {
  GlComposite* parent = nullptr;
  GlEntity* entity = nullptr;
  GlLayer* layer = nullptr;

  if (GRAPH_COMPOSITE_IDS.contains(index.internalId())) {
    quint32 id = index.internalId();
    GlGraphRenderingParameters& parameters = _scene->getMainGlGraph()->getRenderingParameters();
    QString display;
    int stencil = NO_STENCIL;
    bool visible = false;

    if (id == NODES_ID) {
      display = trUtf8("Nodes");
      stencil = parameters.nodesStencil();
      visible = parameters.displayNodes();
    }
    else if (id == EDGES_ID) {
      display = trUtf8("Edges");
      stencil = parameters.edgesStencil();
      visible = parameters.displayEdges();
    }
    else if (id == META_NODES_ID) {
      display = trUtf8("Meta nodes content");
      stencil = parameters.metaNodesStencil();
      visible = parameters.displayMetaNodes();
    }
    else if (id == META_NODE_LABELS_ID) {
      display = trUtf8("Meta node content labels");
      stencil = parameters.metaNodesLabelsStencil();
      visible = parameters.displayMetaNodesLabels();
    }
    else if (id == NODE_LABELS_ID) {
      display = trUtf8("Node labels");
      stencil = parameters.nodesLabelsStencil();
      visible = parameters.displayNodesLabels();
    }
    else if (id == EDGE_LABELS_ID) {
      display = trUtf8("Edge labels");
      stencil = parameters.edgesLabelsStencil();
      visible = parameters.displayEdgesLabels();
    }

    if (role == Qt::DisplayRole && index.column() == 0)
      return display;

    if (role == Qt::CheckStateRole) {
      if (index.column() == 1)
        return (visible ? Qt::Checked : Qt::Unchecked);

      if (index.column() == 2)
        return (stencil == NO_STENCIL ? Qt::Unchecked : Qt::Checked);
    }

    return QVariant();
  }

  if (!index.parent().isValid()) {
    layer = reinterpret_cast<GlLayer*>(index.internalPointer());
  }
  else {
    entity = reinterpret_cast<GlEntity*>(index.internalPointer());
    parent = entity->getParent();
  }

  if (role == Qt::DisplayRole && index.column() == 0) {
    if (layer != nullptr)
      return layer->getName().c_str();

    std::map<std::string, GlEntity*> siblings = parent->getGlEntities();

    for(std::map<std::string, GlEntity*>::iterator it = siblings.begin(); it != siblings.end(); ++it) {
      if (it->second == entity)
        return it->first.c_str();
    }
  }

  if (role == Qt::FontRole && layer != nullptr) {
    QFont f;
    f.setBold(true);
    return f;
  }

  if (role == Qt::CheckStateRole) {
    if (index.column() == 1) {
      if (layer) {
        return (layer->isVisible() ? Qt::Checked : Qt::Unchecked);
      } else {
        return (entity->isVisible() ? Qt::Checked : Qt::Unchecked);
      }
    }
    if (index.column() == 2) {
      if (layer) {
        return Qt::Unchecked;
      } else {
        return (entity->getStencil() == NO_STENCIL ? Qt::Unchecked : Qt::Checked);
      }
    }
  }

  if (role == Qt::TextAlignmentRole && index.column() != 0)
    return Qt::AlignCenter;

  return QVariant();
}

bool SceneLayersModel::setData(const QModelIndex &index, const QVariant &value, int role) {
  if (index.column() == 0 || role != Qt::CheckStateRole)
    return false;

  if (GRAPH_COMPOSITE_IDS.contains(index.internalId())) {
    quint32 id = index.internalId();
    GlGraphRenderingParameters& p = _scene->getMainGlGraph()->getRenderingParameters();

    if (index.column() == 1) {
      bool visible = value.value<int>() == (int)(Qt::Checked);

      if (id == NODES_ID)
        p.setDisplayNodes(visible);
      else if (id == EDGES_ID)
        p.setDisplayEdges(visible);
      else if (id == META_NODES_ID)
        p.setDisplayMetaNodes(visible);
      else if (id == NODE_LABELS_ID)
        p.setDisplayNodesLabels(visible);
      else if (id == EDGE_LABELS_ID)
        p.setDisplayEdgesLabels(visible);
      else if (id == META_NODE_LABELS_ID)
        p.setDisplayMetaNodesLabels(visible);
    }
    else if (index.column() == 2) {
      int stencil = (value.value<int>() == (int)(Qt::Checked) ? FULL_STENCIL : NO_STENCIL);

      if (id == NODES_ID)
        p.setNodesStencil(stencil);
      else if (id == EDGES_ID)
        p.setEdgesStencil(stencil);
      else if (id == META_NODES_ID)
        p.setMetaNodesStencil(stencil);
      else if (id == META_NODE_LABELS_ID)
        p.setMetaNodesLabelsStencil(stencil);
      else if (id == NODE_LABELS_ID)
        p.setNodesLabelsStencil(stencil);
      else if (id == EDGE_LABELS_ID)
        p.setEdgesLabelsStencil(stencil);
    }

    emit drawNeeded(_scene);
    return true;
  }

  GlEntity* entity = nullptr;
  GlLayer* layer = nullptr;

  if (!index.parent().isValid()) {
    layer = reinterpret_cast<GlLayer*>(index.internalPointer());
  }
  else
    entity = reinterpret_cast<GlEntity*>(index.internalPointer());

  bool val = value.value<int>() == (int)Qt::Checked;

  if (index.column() == 1) {
    if (layer)
      layer->setVisible(val);

    if (entity)
      entity->setVisible(val);
  }
  else if (index.column() == 2 && entity)
    entity->setStencil(val ? FULL_STENCIL : 0xFFFF);

  emit drawNeeded(_scene);
  return true;
}

QVariant SceneLayersModel::headerData(int section, Qt::Orientation orientation, int role) const {
  if (orientation == Qt::Horizontal) {
    if (role == Qt::DisplayRole) {
      if (section == 0)
        return trUtf8("Name");
      else if (section == 1)
        return trUtf8("Visible");
      else
        return trUtf8("Stencil");
    }

    else if (role == Qt::TextAlignmentRole)
      return Qt::AlignCenter;
  }

  return TulipModel::headerData(section,orientation,role);
}

Qt::ItemFlags SceneLayersModel::flags(const QModelIndex &index) const {
  Qt::ItemFlags result = QAbstractItemModel::flags(index);

  if (index.column() != 0)
    result |= Qt::ItemIsUserCheckable;

  return result;
}

void SceneLayersModel::treatEvent(const Event &e) {
//  if (e.type() == Event::TLP_MODIFICATION) {
//    const GlSceneEvent *glse = dynamic_cast<const GlSceneEvent *>(&e);

//    if (glse) {
//      emit layoutAboutToBeChanged();

//      // prevent dangling pointers to remain in the model persistent indexes
//      if (glse->getSceneEventType() == GlSceneEvent::TLP_DELENTITY) {
//        QModelIndexList persistentIndexes = persistentIndexList();

//        for (int i = 0 ; i < persistentIndexes.size() ; ++i) {
//          if (persistentIndexes.at(i).internalPointer() == glse->getGlEntity()) {
//            changePersistentIndex(persistentIndexes.at(i), QModelIndex());
//            break;
//          }
//        }
//      }

//      emit layoutChanged();

//    }
//  }
}


