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
    void updateCurrentGraphInfos(unsigned int nbNodes, unsigned int nbEdges);
    
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
