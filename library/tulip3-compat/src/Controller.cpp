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
#include "tulip3/Controller.h"

using namespace std;

//====================================================
tlp::Controller* tlp::Controller::currentController=0;

namespace tlp {
  MainWindowFacade::MainWindowFacade()
  :mainWindow(NULL),menuBar(NULL),toolBar(NULL),interactorsToolBar(NULL),workspace(NULL),statusBar(NULL) {
  }
  MainWindowFacade::MainWindowFacade(QMainWindow *mainWindow,QToolBar *toolBar,QToolBar *interactorsToolBar,QWorkspace *workspace)
		:mainWindow(mainWindow),menuBar(mainWindow->menuBar()),toolBar(toolBar),interactorsToolBar(interactorsToolBar),workspace(workspace),statusBar(mainWindow->statusBar()){
  }

  void Controller::attachMainWindow(MainWindowFacade facade) {
    mainWindowFacade=facade;
  }

}
