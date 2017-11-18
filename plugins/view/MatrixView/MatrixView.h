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

#ifndef MATRIXVIEW_H
#define MATRIXVIEW_H

#include <tulip/NodeLinkDiagramComponent.h>
#include <set>
#include "MatrixViewConfigurationWidget.h"

#include "../../utils/PluginNames.h"

namespace tlp {
class IntegerVectorProperty;
class IntegerProperty;
class BooleanProperty;
class DoubleProperty;

class MatrixViewQuickAccessBar;

class PropertyValuesDispatcher;

/*@{*/
/** \file
 *  \brief Adjacency matrix view
 * In mathematics and computer science, an adjacency matrix is a means of representing which
 * vertices of a graph are adjacent to which other vertices. Another matrix representation for a
 * graph is the incidence matrix.
 *
 * Specifically, the adjacency matrix of a finite graph G on n vertices is the n  n matrix where the
 * non-diagonal entry aij is the number of edges from vertex i to vertex j, and the diagonal entry
 * aii, depending on the convention, is either once or twice the number of edges (loops) from vertex
 * i to itself.
 * Undirected graphs often use the former convention of counting loops twice, whereas directed
 * graphs typically use the latter convention.
 * There exists a unique adjacency matrix for each isomorphism class of graphs (up to permuting rows
 * and columns), and it is not the adjacency matrix of any other isomorphism class of graphs. In the
 * special case of a finite simple graph, the adjacency matrix is a (0,1)-matrix with zeros on its
 * diagonal.
 * If the graph is undirected, the adjacency matrix is symmetric.
 */
class MatrixView : public NodeLinkDiagramComponent {
  Q_OBJECT

  MatrixViewQuickAccessBar *_bar;

public:
  PLUGININFORMATION(tlp::ViewName::MatrixViewName, "Ludwig Fiolka", "07/01/2011",
                    "<p>In Mathematics and Computer Science, an adjacency matrix is used to "
                    "represent which vertices of a graph are adjacents to each other. Another "
                    "matrix representation for a graph is the incidence matrix.</p>"
                    "<p>Specifically, the adjacency matrix of a finite graph G on n vertices is "
                    "the n x n matrix where the non-diagonal entry a<sub>ij</sub> is the number of "
                    "edges from vertex <i>i</i> to vertex <i>j</i>, and the diagonal entry "
                    "a<sub>ii</sub>, depending on the convention, is either once or twice the "
                    "number of edges (loops) from vertex <i>i</i> to itself. "
                    "Undirected graphs often use the former convention of counting loops twice, "
                    "whereas directed graphs typically use the latter convention.</p>"
                    "<p>There exists a unique adjacency matrix for each isomorphism class of "
                    "graphs (up to permuting rows and columns), and it is not the adjacency matrix "
                    "of any other isomorphism class of graphs. In the special case of a finite "
                    "simple graph, the adjacency matrix is a (0,1)-matrix with zeros on its "
                    "diagonal.</p>"
                    "If the graph is undirected, the adjacency matrix is symmetric.</p>",
                    "2.0", "View")

  MatrixView(const tlp::PluginContext *);
  ~MatrixView();
  std::string icon() const {
    return ":/adjacency_matrix_view.png";
  }
  QuickAccessBar *getQuickAccessBarImpl();
  void setState(const tlp::DataSet &dataSet);
  void graphChanged(tlp::Graph *graph);

  tlp::DataSet state() const;
  QList<QWidget *> configurationWidgets() const;

  void draw();
  void refresh();

  GridDisplayMode gridDisplayMode() const {
    return _configurationWidget->gridDisplayMode();
  }

  void addNode(tlp::Graph *, const tlp::node);
  void addEdge(tlp::Graph *, const tlp::edge);
  void delNode(tlp::Graph *, const tlp::node);
  void delEdge(tlp::Graph *, const tlp::edge);
  void treatEvent(const Event &message);

  void fillContextMenu(QMenu *menu, const QPointF &point);

private slots:
  void setBackgroundColor(QColor);
  void setOrderingMetric(const std::string &);
  void setGridDisplayMode();
  void applySettings();
  void showEdges(bool);
  void enableEdgeColorInterpolation(bool);
  void setOriented(bool);

private:
  void registerTriggers();

  tlp::Graph *_matrixGraph;

  // Correspondance maps
  tlp::IntegerVectorProperty *_graphEntitiesToDisplayedNodes;
  tlp::IntegerProperty *_displayedNodesToGraphEntities;
  tlp::IntegerProperty *_displayedEdgesToGraphEdges;
  tlp::BooleanProperty *_displayedNodesAreNodes;
  PropertyValuesDispatcher *_dispatcher;
  QHash<tlp::edge, tlp::edge> _edgesMap;

  MatrixViewConfigurationWidget *_configurationWidget;

  bool _mustUpdateSizes;
  bool _mustUpdateLayout;
  bool _isOriented;

  std::set<std::string> _sourceToTargetProperties;
  std::string _orderingMetricName;
  std::vector<node> _orderedNodes;

  void deleteDisplayedGraph();
  void initDisplayedGraph();
  void addGridBackground();
  void removeGridBackground();
  void normalizeSizes(double max = 1);
  void updateNodesOrder();
  void updateLayout();
};
}
#endif // MATRIXVIEW_H
