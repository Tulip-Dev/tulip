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

#ifndef MATRIXVIEW_H
#define MATRIXVIEW_H

#include <tulip/NodeLinkDiagramComponent.h>
#include <set>
#include "MatrixViewConfigurationWidget.h"

#include "../../utils/ViewNames.h"

namespace tlp {
class IntegerVectorProperty;
class IntegerProperty;
class BooleanProperty;
class DoubleProperty;
}

class PropertyValuesDispatcher;

/*@{*/
/** \file
 *  \brief Adjacency matrix view
 * In mathematics and computer science, an adjacency matrix is a means of representing which vertices of a graph are adjacent to which other vertices. Another matrix representation for a graph is the incidence matrix.
 *
 * Specifically, the adjacency matrix of a finite graph G on n vertices is the n  n matrix where the non-diagonal entry aij is the number of edges from vertex i to vertex j, and the diagonal entry aii, depending on the convention, is either once or twice the number of edges (loops) from vertex i to itself.
 * Undirected graphs often use the former convention of counting loops twice, whereas directed graphs typically use the latter convention.
 * There exists a unique adjacency matrix for each isomorphism class of graphs (up to permuting rows and columns), and it is not the adjacency matrix of any other isomorphism class of graphs. In the special case of a finite simple graph, the adjacency matrix is a (0,1)-matrix with zeros on its diagonal.
 * If the graph is undirected, the adjacency matrix is symmetric.
 */
class MatrixView: public tlp::NodeLinkDiagramComponent {
  Q_OBJECT
public:

  PLUGININFORMATION(ViewName::MatrixViewName, "Ludwig Fiolka", "07/01/2011",
                    "<p>In mathematics and computer science, an adjacency matrix is a means of representing which vertices of a graph are adjacent to which other vertices. Another matrix representation for a graph is the incidence matrix.</p>"
                    "<p>Specifically, the adjacency matrix of a finite graph G on n vertices is the n x n matrix where the non-diagonal entry aij is the number of edges from vertex i to vertex j, and the diagonal entry aii, depending on the convention, is either once or twice the number of edges (loops) from vertex i to itself."
                    "Undirected graphs often use the former convention of counting loops twice, whereas directed graphs typically use the latter convention.</p>"
                    "<p>There exists a unique adjacency matrix for each isomorphism class of graphs (up to permuting rows and columns), and it is not the adjacency matrix of any other isomorphism class of graphs. In the special case of a finite simple graph, the adjacency matrix is a (0,1)-matrix with zeros on its diagonal.</p>"
                    "If the graph is undirected, the adjacency matrix is symmetric.</p>", "2.0","View")

  MatrixView(const tlp::PluginContext *);
  virtual ~MatrixView();
  std::string icon() const {
    return ":/adjacency_matrix_view.png";
  }
  virtual void setState(const tlp::DataSet &dataSet);
  virtual void graphChanged(tlp::Graph *graph);

  virtual tlp::DataSet state() const;
  virtual QList<QWidget *> configurationWidgets() const;

  virtual void draw();
  virtual void refresh();

  GridDisplayMode gridDisplayMode() const {
    return _configurationWidget->gridDisplayMode();
  }

  virtual void addNode(tlp::Graph *, const tlp::node);
  virtual void addEdge(tlp::Graph *, const tlp::edge );
  virtual void delNode(tlp::Graph *,const tlp::node );
  virtual void delEdge(tlp::Graph *,const tlp::edge );

  void fillContextMenu(QMenu *menu,const QPointF &point);

private slots:
  void setBackgroundColor(QColor);
  void setOrderingMetric(const std::string&);
  void setGridDisplayMode();
  void applySettings();
  void showEdges(bool);

private:

  void registerTriggers();

  tlp::Graph *_matrixGraph;

  // Correspondance maps
  tlp::IntegerVectorProperty *_graphEntitiesToDisplayedNodes;
  tlp::IntegerProperty *_displayedNodesToGraphEntities;
  tlp::IntegerProperty *_displayedEdgesToGraphEdges;
  tlp::BooleanProperty *_displayedNodesAreNodes;
  PropertyValuesDispatcher *_dispatcher;
  QHash<tlp::edge,tlp::edge> _edgesMap;

  MatrixViewConfigurationWidget *_configurationWidget;

  bool _mustUpdateSizes;
  bool _mustUpdateLayout;

  std::set<std::string> _sourceToTargetProperties;
  std::string _orderingMetricName;
  std::vector<tlp::node> _orderedNodes;


  void deleteDisplayedGraph();
  void initDisplayedGraph();
  void addGridBackground();
  void removeGridBackground();
  void normalizeSizes(double max=1);
  void updateNodesOrder();
  void updateLayout();
};

#endif // MATRIXVIEW_H
