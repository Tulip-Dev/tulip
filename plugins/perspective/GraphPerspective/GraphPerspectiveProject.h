#ifndef GRAPHPERSPECTIVEPROJECT_H
#define GRAPHPERSPECTIVEPROJECT_H

namespace tlp {
class TulipProject;
class Graph;
class PluginProgress;
}
#include <QtCore/QObject>
#include <QtCore/QVector>
#include <QtCore/QMap>

class GraphPerspectiveProject: public QObject {
  Q_OBJECT

  Q_PROPERTY(tlp::TulipProject * project READ project)
  tlp::TulipProject *_project;

  Q_PROPERTY(QVector<tlp::Graph *> graphs READ graphs)
  QVector<tlp::Graph *> _graphs;

  QMap<tlp::Graph *,QString> _graphPath;

public:
  GraphPerspectiveProject(tlp::TulipProject *project, tlp::PluginProgress *);
  virtual ~GraphPerspectiveProject();

  tlp::TulipProject *project() const { return _project; }
  QVector<tlp::Graph *> graphs() const { return _graphs; }

public slots:
  void addGraph(tlp::Graph *);
  void removeGraph(tlp::Graph *);

};

#endif // GRAPHPERSPECTIVEPROJECT_H
