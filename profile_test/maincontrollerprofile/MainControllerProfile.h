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
