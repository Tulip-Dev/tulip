#ifndef CLUSTERTREE_H
#define CLUSTERTREE_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <QtCore/qobject.h>
#include <QtCore/qpoint.h>
#include <QtGui/qtreewidget.h>

#include <tulip/tulipconf.h>
#include <tulip/MutableContainer.h>

class QTreeWidgetItem;

namespace tlp {
class Graph;
}

/** \addtogroup Tulip_Widgets */ 
/*@{*/
class TLP_QT_SIMPLE_SCOPE SGHierarchyWidget : public QTreeWidget { 
  Q_OBJECT;
  
public:
  SGHierarchyWidget(QWidget* parent = 0, tlp::Graph *rootGraph = 0);
  tlp::Graph *getGraph() const;
  
public slots:
  void setGraph(tlp::Graph *);
  void update();
  void updateCurrentGraphInfos(unsigned int nbNodes, unsigned int nbEdges);

signals:
  void aboutToRemoveView(tlp::Graph *);
  void aboutToRemoveAllView(tlp::Graph *);
  void graphChanged(tlp::Graph *);

private slots:
  void contextRemoveCluster();
  void contextRemoveAllCluster();
  void contextCloneSubgraphCluster();
  void contextRenameCluster();
  void contextCloneCluster();
  void currentGraphChanged(const tlp::Graph *);
  void displayContextMenu(const QPoint &p);
  void changeGraph(QTreeWidgetItem *item, const QPoint &p, int i);
  void changeGraph(QTreeWidgetItem *current, QTreeWidgetItem *previous);
  void setItemInfos(QTreeWidgetItem *item, tlp::Graph *, unsigned int nbNodes, unsigned int nbEdges);

 protected:
  void buildTreeView(tlp::Graph *p, QTreeWidgetItem *item = NULL);
  void removeSubgraph(tlp::Graph *graph, bool recursive);

private:
  tlp::MutableContainer<QTreeWidgetItem *> graphItems;
  tlp::Graph *_currentGraph;
};
/*@}*/
#endif // CLUSTERTREE_H
