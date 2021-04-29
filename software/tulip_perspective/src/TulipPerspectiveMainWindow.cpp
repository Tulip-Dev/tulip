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
#include "TulipPerspectiveMainWindow.h"

#include <QCloseEvent>
#include <QShortcut>
#include <QAction>
#include <tulip/Perspective.h>
#include <tulip/TulipProject.h>

TulipPerspectiveProcessMainWindow::TulipPerspectiveProcessMainWindow(QString title, QWidget *parent)
    : QMainWindow(parent), _project(nullptr), _title(title) {}

void TulipPerspectiveProcessMainWindow::closeEvent(QCloseEvent *event) {
  if (tlp::Perspective::instance()->terminated()) {
    QMainWindow::closeEvent(event);
  } else
    event->ignore();
}

void TulipPerspectiveProcessMainWindow::setProject(tlp::TulipProject *project) {
  _project = project;
  connect(project, SIGNAL(projectFileChanged(const QString &)), this,
          SLOT(projectFileChanged(const QString &)));
  connect(tlp::Perspective::instance(), SIGNAL(resetWindowTitle()), this,
          SLOT(projectFileChanged()));
}

void TulipPerspectiveProcessMainWindow::projectFileChanged(const QString &projectFile) {
  QString wTitle(_title);

  wTitle += QString(" [") + _project->perspective() + "]";

  if (!_project->name().isEmpty())
    wTitle += QString(" - ") + _project->name();
  else if (!projectFile.isEmpty())
    wTitle += QString(" - ") + projectFile;
  else { // all graphs are deleted. Clear project. Useful?
    _project->clearProject();
    wTitle += QString(" - unsaved project");
  }

  wTitle += "[*]"; // placeholder for window modification
#ifndef NDEBUG
  wTitle += " - [ Debug mode ]";
#endif
  setWindowTitle(wTitle);
}
