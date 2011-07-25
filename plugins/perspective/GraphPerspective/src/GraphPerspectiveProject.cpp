#include "GraphPerspectiveProject.h"

#include <tulip/Graph.h>
#include <QtCore/QDebug>
#include <QtCore/QCoreApplication>
#include <assert.h>
#include <tulip/TulipProject.h>
#include <string>
using namespace tlp;

static const QString GRAPHS_FOLDER = "/graphs";

GraphPerspectiveProject::GraphPerspectiveProject(TulipProject *project, tlp::PluginProgress *progress): _project(project) {
  assert(project);
  assert(progress);

  progress->setComment(trUtf8("Checking file/folder structure.").toStdString());
  if (_project->exists(GRAPHS_FOLDER) && !_project->isDir(GRAPHS_FOLDER))
    assert(_project->removeFile(GRAPHS_FOLDER));

  if (!_project->exists(GRAPHS_FOLDER))
    assert(_project->mkpath(GRAPHS_FOLDER));

  QString f;
  foreach(f,_project->entryList(GRAPHS_FOLDER)) {
    if (!_project->exists(GRAPHS_FOLDER + "/" + f + "/graph.tlp"))
      continue;
    progress->setComment((trUtf8("Importing graph from folder: ") + f).toStdString());
    DataSet dataSet;
    Graph *g = tlp::importGraph("tlp",dataSet,progress);
    if (!g)
      continue;
    _graphFolder[g] = f;
  }


}

GraphPerspectiveProject::~GraphPerspectiveProject() {
}

void GraphPerspectiveProject::addGraph(tlp::Graph *g) {
  if (_graphFolder.contains(g))
    return;
  QString folder = uniqueFolderName(g);
  _project->mkpath(GRAPHS_FOLDER + "/" + folder);
  _graphFolder[g] = folder;
}

void GraphPerspectiveProject::removeGraph(tlp::Graph *g) {
  QString folder = _graphFolder[g];
  _project->removeAllDir(GRAPHS_FOLDER + "/" + folder);
}

QList<tlp::Graph *> GraphPerspectiveProject::graphs() const {
  return _graphFolder.keys();
}

QString GraphPerspectiveProject::uniqueFolderName(tlp::Graph *g) {
  QString folderName;

  std::string nameAttr;
  g->getAttribute<std::string>("name",nameAttr);
  if (nameAttr.compare("") != 0)
    folderName = nameAttr.c_str();
  else
    folderName = "graph_" + QString::number(QCoreApplication::applicationPid());

  int i=0;
  do {
    folderName += "-" + i++;
  } while (_project->exists(GRAPHS_FOLDER + "/" + folderName));
  return folderName;
}
