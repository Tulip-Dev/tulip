#ifndef CLUSTERTREE_H
#define CLUSTERTREE_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <qobject.h>
#include <qpoint.h>

#include <tulip/tulipconf.h>
#include <tulip/MutableContainer.h>
#include "tulip/SGHierarchyWidgetUI.h"

class QListViewItem;
class QListView;
class QPopupMenu;

namespace tlp {
class Graph;
}

/** \addtogroup Tulip_Widgets */ 
/*@{*/
class TLP_QT_SIMPLE_SCOPE SGHierarchyWidget : public tlp::SGHierarchyWidgetUI { 
  Q_OBJECT;
  
public:
  SGHierarchyWidget(tlp::Graph *rootGraph, QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = 0);
  SGHierarchyWidget(QWidget* parent = 0, const char *name = 0, Qt::WFlags fl = 0);
  tlp::Graph *getGraph() const;
  
public slots:
  void setGraph(tlp::Graph *);
  void update();

signals:
  void aboutToRemoveView(tlp::Graph *);
  void aboutToRemoveAllView(tlp::Graph *);
  void graphChanged(tlp::Graph *);

private slots:
  void contextRemoveCluster();
  void contextRemoveAllCluster();
  void contextCloneSubgraphCluster();
  void contextMoveUpCluster();
  void contextRenameCluster();
  void contextCloneCluster();
  void currentGraphChanged(const tlp::Graph *);
  void rightButtonSGHierarchyWidget(QListViewItem *item, const QPoint &p, int c);
  void changeGraph(QListViewItem *item,const QPoint &p, int i);
  void changeGraph(QListViewItem *item);

 protected:
  void buildTreeView(QListView *item, tlp::Graph *p);
  void buildTreeView(QListViewItem *item, tlp::Graph *p);
  void removeSubgraph(tlp::Graph *graph, bool recursive);

private:
  tlp::MutableContainer<QListViewItem *> graphItems;
  tlp::Graph *_currentGraph;
};
/*@}*/
#endif // CLUSTERTREE_H
