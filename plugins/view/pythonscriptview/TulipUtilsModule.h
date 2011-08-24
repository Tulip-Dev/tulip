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

#ifndef TULIPUTILSMODULE_H_
#define TULIPUTILSMODULE_H_

#include "PythonInterpreter.h"

#include <tulip/MainController.h>
#include <tulip/ControllerAlgorithmTools.h>
#include <tulip/GlMainView.h>
#include <tulip/GlMainWidget.h>
#include <tulip/TemplateFactory.h>
#include <tulip/ImportModule.h>
#include <tulip/ExportModule.h>

#include <QtGui/QMenu>

static PyObject *
tuliputils_updateVisualization(PyObject *, PyObject *args) {
  int i;

  if (!PyArg_ParseTuple(args, "|i", &i))
    Py_RETURN_NONE;

  bool centerViews = i > 0;

  tlp::MainController *mainController = dynamic_cast<tlp::MainController *>(tlp::Controller::getCurrentController());

  if (mainController) {
    std::vector<tlp::View*> tulipViews = mainController->getViewsOfGraph(mainController->getGraph());

    for (size_t i = 0 ; i < tulipViews.size() ; ++i) {
      tlp::GlMainView *glView = dynamic_cast<tlp::GlMainView *>(tulipViews[i]);

      if (centerViews && glView) {
        glView->getGlMainWidget()->getScene()->centerScene();
      }

      tulipViews[i]->draw();
    }
  }

  Py_RETURN_NONE;
}


// Ugly code duplication from TulipApp.cpp but don't really have the choice
//**********************************************************************
static std::vector<std::string> getItemGroupNames(std::string itemGroup) {
  std::string::size_type start = 0;
  std::string::size_type end = 0;
  std::vector<std::string> groupNames;
  const char * separator = "::";

  while(true) {
    start = itemGroup.find_first_not_of(separator, end);

    if (start == std::string::npos) {
      return groupNames;
    }

    end = itemGroup.find_first_of(separator, start);

    if (end == std::string::npos)
      end = itemGroup.length();

    groupNames.push_back(itemGroup.substr(start, end - start));
  }
}
//**********************************************************************
static void insertInMenu(QMenu &menu, string itemName, string itemGroup,
                         std::vector<QMenu*> &groupMenus, std::string::size_type &nGroups,QObject *receiver, const char *slot) {
  std::vector<std::string> itemGroupNames = getItemGroupNames(itemGroup);
  QMenu *subMenu = &menu;
  std::string::size_type nGroupNames = itemGroupNames.size();

  for (std::string::size_type i = 0; i < nGroupNames; i++) {
    QMenu *groupMenu = (QMenu *) 0;

    for (std::string::size_type j = 0; j < nGroups; j++) {
      if (itemGroupNames[i] == groupMenus[j]->objectName().toAscii().data()) {
        subMenu = groupMenu = groupMenus[j];
        break;
      }
    }

    if (!groupMenu) {
      groupMenu = new QMenu(itemGroupNames[i].c_str(), subMenu);
      groupMenu->setObjectName(QString(itemGroupNames[i].c_str()));
      subMenu->addMenu(groupMenu);
      groupMenus.push_back(groupMenu);
      nGroups++;
      subMenu = groupMenu;
    }
  }

  QAction *action = subMenu->addAction(itemName.c_str());
  QObject::connect(action,SIGNAL(triggered()),receiver,slot);
}
//**********************************************************************
template <typename TFACTORY, typename TMODULE>
static void buildMenuWithContext(QMenu &menu, QObject *receiver, const char *slot) {
  typename tlp::TemplateFactory<TFACTORY, TMODULE, tlp::AlgorithmContext>::ObjectCreator::const_iterator it;
  std::vector<QMenu*> groupMenus;
  std::string::size_type nGroups = 0;

  for (it=TFACTORY::factory->objMap.begin(); it != TFACTORY::factory->objMap.end(); ++it)
    insertInMenu(menu, it->first.c_str(), it->second->getGroup(), groupMenus, nGroups,receiver,slot);
}
//**********************************************************************

static PyObject *
tuliputils_updatePluginsMenus(PyObject *, PyObject *) {
  tlp::MainController *mainController = dynamic_cast<tlp::MainController *>(tlp::Controller::getCurrentController());

  if (mainController) {

    tlp::ControllerAlgorithmTools::cleanPluginParameters();

    mainController->buildMenu();

    // Big hack to get access to the Import and Export Menu to update their content
    QWidget *mainWindow = mainController->getMainWindowFacade()->getParentWidget();
    QMenu *importMenu = NULL;
    QMenu *exportMenu = NULL;
    QObjectList childWidgets = mainWindow->children();

    while (!childWidgets.empty()) {
      QObject *obj = childWidgets.front();
      QMenu *menu = dynamic_cast<QMenu *>(obj);

      if (menu) {
        if (menu->title() == "&Import") {
          importMenu = menu;
        }
        else if (menu->title() == "&Export") {
          exportMenu = menu;
        }
      }

      if (importMenu && exportMenu) {
        break;
      }
      else {
        childWidgets.pop_front();
        childWidgets += obj->children();
      }
    }

    if (importMenu) {
      importMenu->clear();
      buildMenuWithContext<tlp::ImportModuleFactory, tlp::ImportModule>(*importMenu, mainWindow, SLOT(importGraph()));
    }

    if (exportMenu) {
      exportMenu->clear();
      buildMenuWithContext<tlp::ExportModuleFactory, tlp::ExportModule>(*exportMenu, mainWindow, SLOT(exportGraph()));
    }

  }

  Py_RETURN_NONE;
}

static PyMethodDef TulipUtilsMethods[] = {
  {"updateVisualization",  tuliputils_updateVisualization, METH_VARARGS, "Update views on current graph."},
  {"updatePluginsMenus",  tuliputils_updatePluginsMenus, METH_VARARGS, "Update the plugins menus entries in the Tulip GUI."},
  {NULL, NULL, 0, NULL}        /* Sentinel */
};

void
inittuliputils(void) {
  PyObject *m = Py_InitModule("tuliputils", TulipUtilsMethods);

  if (m == NULL)
    return;
}

#endif /* TULIPUTILSMODULE_H_ */
