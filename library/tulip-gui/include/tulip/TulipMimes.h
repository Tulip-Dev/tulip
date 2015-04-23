/*
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
///@cond DOXYGEN_HIDDEN


#ifndef _TLP_MIMES_H
#define _TLP_MIMES_H

#include <QMimeData>
#include <QStringList>

#include <tulip/tulipconf.h>
#include <tulip/DataSet.h>

namespace tlp {
class Graph;
class WorkspacePanel;
class Algorithm;
class DataSet;


const QString GRAPH_MIME_TYPE=QString("application/x-tulip-mime;value=\"graph\"");
const QString WORKSPACE_PANEL_MIME_TYPE=QString("application/x-tulip-mime;value=\"workspace-panel\"");
const QString ALGORITHM_NAME_MIME_TYPE=QString("application/x-tulip-mime;value=\"algorithm-name\"");
const QString DATASET_MIME_TYPE=QString("application/x-tulip-mime;value=\"dataset\"");

/**
 * @brief The GraphMimeType class allows to tranfer a graph pointer trought a QMimeData
 */
class TLP_QT_SCOPE GraphMimeType : public QMimeData {
public:
  GraphMimeType():QMimeData(),_graph(NULL) {}
  void setGraph(tlp::Graph* graph) {
    _graph = graph;
  }

  tlp::Graph* graph() const {
    return _graph;
  }

  QStringList formats()const;

private:
  tlp::Graph* _graph;
};

class TLP_QT_SCOPE AlgorithmMimeType : public QMimeData {
  Q_OBJECT

  QString _algorithm;
  tlp::DataSet _params;
public:



  AlgorithmMimeType(QString algorithmName, const tlp::DataSet& data);
  void run(tlp::Graph*) const;

  QString algorithm() const {
    return _algorithm;
  }
  tlp::DataSet params() const {
    return _params;
  }

  QStringList formats()const;

signals:
  void mimeRun(tlp::Graph*) const;
};

class TLP_QT_SCOPE PanelMimeType : public QMimeData {
public:



  void setPanel(tlp::WorkspacePanel* panel) {
    _panel = panel;
  }

  tlp::WorkspacePanel* panel() const {
    return _panel;
  }

  QStringList formats()const;

private:
  tlp::WorkspacePanel* _panel;
};
}
#endif //_TLP_MIMES_H
///@endcond
