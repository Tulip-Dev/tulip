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

  Q_PROPERTY(QList<tlp::Graph *> graphs READ graphs)

  QMap<tlp::Graph *,QString> _graphFolder;

  QString uniqueFolderName(tlp::Graph *g);

public:
  GraphPerspectiveProject(tlp::TulipProject *project, tlp::PluginProgress *);
  virtual ~GraphPerspectiveProject();

  tlp::TulipProject *project() const {
    return _project;
  }
  QList<tlp::Graph *> graphs() const;

public slots:
  void addGraph(tlp::Graph *);
  void removeGraph(tlp::Graph *);

};

#endif // GRAPHPERSPECTIVEPROJECT_H
