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
#ifndef CLUSTERTREE_H
#define CLUSTERTREE_H

#include <QtCore/qobject.h>
#include <QtCore/qpoint.h>
#include <QtGui/qtreewidget.h>

#include <tulip/tulipconf.h>
#include <tulip/MutableContainer.h>

class QTreeWidgetItem;

namespace tlp {
 
 class Graph;

  /** \addtogroup Tulip_Widgets */ 
  /*@{*/
  class TLP_QT_SIMPLE_SCOPE SGHierarchyWidget : public QTreeWidget { 
    Q_OBJECT;
    
  public:
    SGHierarchyWidget(QWidget* parent = 0, Graph *rootGraph = 0);
    Graph *getGraph() const;
    
    public slots:
    void setGraph(Graph *);
    void update();
    void updateCurrentGraphInfos(Graph *graph);
    
  signals:
    void aboutToRemoveView(Graph *);
    void aboutToRemoveAllView(Graph *);
    void graphChanged(Graph *);
    
  private slots:
    void contextRemoveCluster();
    void contextRemoveAllCluster();
    void contextCloneSubgraphCluster();
    void contextRenameCluster();
    void contextCloneCluster();
    void currentGraphChanged(const Graph *);
    void displayContextMenu(const QPoint &p);
    void changeGraph(QTreeWidgetItem *item, const QPoint &p, int i);
    void changeGraph(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    void setItemInfos(QTreeWidgetItem *item, Graph *, unsigned int nbNodes, unsigned int nbEdges);
    
  protected:
    void buildTreeView(Graph *p, QTreeWidgetItem *item = NULL);
    void removeSubgraph(Graph *graph, bool recursive);
    
  private:
    MutableContainer<QTreeWidgetItem *> graphItems;
    Graph *_currentGraph;
  };
  /*@}*/
}

#endif // CLUSTERTREE_H
