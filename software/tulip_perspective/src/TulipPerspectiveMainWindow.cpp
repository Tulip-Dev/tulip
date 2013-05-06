/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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

TulipPerspectiveProcessMainWindow::TulipPerspectiveProcessMainWindow(QWidget *parent): QMainWindow(parent) {
#ifdef MEMORYCHECKER_ON
  QAction* a1 = new QAction(this);
  a1->setShortcut(QKeySequence(Qt::CTRL + Qt::ALT + Qt::Key_C));
  a1->setShortcutContext(Qt::ApplicationShortcut);
  connect(a1,SIGNAL(triggered()),this,SLOT(clearMemoryChecker()));
  addAction(a1);

  QAction* a2 = new QAction(this);
  a2->setShortcut(QKeySequence(Qt::CTRL + Qt::ALT + Qt::Key_P));
  a2->setShortcutContext(Qt::ApplicationShortcut);
  connect(a2,SIGNAL(triggered()),this,SLOT(printMemoryChecker()));
  addAction(a2);
#endif // MEMORYCHECKER_ON
}

void TulipPerspectiveProcessMainWindow::printMemoryChecker() {
#ifdef MEMORYCHECKER_ON
  memory_checker_print_report();
#endif // MEMORYCHECKER_ON
}

void TulipPerspectiveProcessMainWindow::clearMemoryChecker() {
#ifdef MEMORYCHECKER_ON
  memory_checker_clear_report();
#endif // MEMORYCHECKER_ON
}

void TulipPerspectiveProcessMainWindow::closeEvent(QCloseEvent* event) {
  if (_perspective->terminated()) {
    QMainWindow::closeEvent(event);
  }
  else
    event->ignore();
}
