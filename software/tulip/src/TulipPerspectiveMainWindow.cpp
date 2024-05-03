/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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

#include <tulip/Graph.h>
#include <tulip/Perspective.h>
#include <tulip/TulipProject.h>

#if defined(__APPLE__)
// on MacOS we use a system tray icon
// in projectFileChanged method
#include <tulip/TlpTools.h>
#include <QSystemTrayIcon>
static QSystemTrayIcon *_sti = nullptr;

TulipPerspectiveProcessMainWindow::~TulipPerspectiveProcessMainWindow() {
  delete _sti;
  _sti = nullptr;
}

#endif

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

  auto perspective = _project->perspective();
  // add perspective name if is not a GraphPerspective
  if (perspective != "Tulip")
    wTitle += QString(" [") + _project->perspective() + "]";

  if (!_project->name().isEmpty())
    wTitle += QString(" - ") + _project->name();
  else if (!projectFile.isEmpty())
    wTitle += QString(" - ") + projectFile;
  else {
    wTitle += QString(" - ");
    tlp::Graph *cg =
        tlp::Perspective::instance() ? tlp::Perspective::instance()->currentGraph() : nullptr;
    if (cg)
      wTitle += QString(cg->getName().c_str()) + ' ';
    wTitle += QString("(unsaved project)");
  }

#if defined(__APPLE__)
  // on MacOS the app title displayed in the dock
  // is not the perspective window title
  // but the name of the app executable file.
  // We therefore use a system tray icon to display
  // the perspective window title and make it easier
  // to manage the visibility of multiple running perspectives
  delete _sti;
  _sti = new QSystemTrayIcon(this);
  _sti->setIcon(QIcon(":/tulip/gui/icons/tulip-file-icon.ico"));

  QMenu *menu = new QMenu(this);
  menu->addAction(QString(wTitle).replace('_', ' '));
  menu->addSeparator();
  menu->addAction("Show", this, SLOT(showAndActivate()));
  menu->addAction("Hide", this, SLOT(hide()));
  menu->addAction("Exit", this, SLOT(close()));
  _sti->setContextMenu(menu);
  _sti->setToolTip(wTitle);
  _sti->show();
#endif

  wTitle += "[*]"; // placeholder for window modification
#ifndef NDEBUG
  wTitle += " - [ Debug mode ]";
#endif
  setWindowTitle(wTitle);
}

void TulipPerspectiveProcessMainWindow::showAndActivate() {
  show();
  raise();
  activateWindow();
}
