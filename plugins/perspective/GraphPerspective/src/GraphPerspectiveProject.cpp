#include "GraphPerspectiveProject.h"

#include <QtCore/QDebug>
#include <assert.h>
#include <tulip/TulipProject.h>
using namespace tlp;

#define GRAPHS_FOLDER "/graphs"

GraphPerspectiveProject::GraphPerspectiveProject(TulipProject *project, tlp::PluginProgress *progress): _project(project) {
  assert(project);
  assert(progress);

  progress->setComment(trUtf8("Checking file/folder structure.").toStdString());
  if (_project->exists(GRAPHS_FOLDER) && !_project->isDir(GRAPHS_FOLDER))
    assert(_project->removeFile(GRAPHS_FOLDER));

  if (!_project->exists(GRAPHS_FOLDER))
    assert(_project->mkpath(GRAPHS_FOLDER));

}

GraphPerspectiveProject::~GraphPerspectiveProject() {
}

void GraphPerspectiveProject::addGraph(tlp::Graph *g) {

}

void GraphPerspectiveProject::removeGraph(tlp::Graph *g) {

}
