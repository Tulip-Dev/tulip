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
#ifndef TULIPPERSPECTIVEPROCESSMAINWINDOW_H
#define TULIPPERSPECTIVEPROCESSMAINWINDOW_H

#include <QtGui/QMainWindow>

namespace tlp {
class Perspective;
}

/*
  This class can be used to add pre(-defined behavior to the main window before passing it to the perspective.
  Note that no GUI modification should be done since the Perspective must be free to do anything on it
  */
class TulipPerspectiveProcessMainWindow : public QMainWindow {
  Q_OBJECT
  tlp::Perspective* _perspective;

public:
  explicit TulipPerspectiveProcessMainWindow(QWidget *parent = NULL);
  void setPerspective(tlp::Perspective* perspective) {
    _perspective = perspective;
  }

signals:

public slots:
  void clearMemoryChecker();
  void printMemoryChecker();

protected:
  void closeEvent(QCloseEvent *);
};

#endif // TULIPPERSPECTIVEPROCESSMAINWINDOW_H
