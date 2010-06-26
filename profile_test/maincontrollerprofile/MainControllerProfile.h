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
#ifndef _Tulip_MAINCONTROLLERPROFILE_H
#define _Tulip_MAINCONTROLLERPROFILE_H

#include <iostream>

#include <QtCore/QObject>
#include <QtCore/QTimer>
#include <QtCore/QCoreApplication>

#include <tulip/Graph.h>
#include <tulip/MainController.h>
#include <tulip/ForEach.h>
#include <tulip/BooleanProperty.h>


class MainControllerProfile : public QObject {
  Q_OBJECT;

public :

  MainControllerProfile(tlp::Graph *graph,tlp::MainController *mainController):number(0),graph(graph),mainController(mainController){
    tlp::BooleanProperty* selection=graph->getProperty<tlp::BooleanProperty>("viewSelection");

    selection->setAllNodeValue(true);
    selection->setAllEdgeValue(true);

    mainController->editCopy();
  }

  public slots :

    void delNodes() {
      std::cout << "turn : " << number << std::endl;

      //mainController->editDelSelection();

      //QTimer::singleShot(0, this, SLOT(addNodes()));
    }

    void addNodes() {
      mainController->editPaste();

      if(number==10)
        QCoreApplication::exit(0);
      else
        QTimer::singleShot(0, this, SLOT(delNodes()));

      ++number;

    }

  protected :

    int number;
    tlp::Graph *graph;
    tlp::MainController *mainController;
};

#endif
