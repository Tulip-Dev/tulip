#include "tulip/Controller.h"

using namespace std;

namespace tlp {

  TemplateFactory<ControllerFactory,Controller, ControllerContext *> *ControllerFactory::factory;

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
