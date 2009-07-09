#ifndef _Tulip_VIEWPROFILE_H
#define _Tulip_VIEWPROFILE_H

#include <iostream>

#include <QtCore/QObject>
#include <QtCore/QTimer>
#include <QtCore/QCoreApplication>

#include <tulip/Graph.h>
#include <tulip/View.h>
#include <tulip/ForEach.h>


class ViewProfile : public QObject {
  Q_OBJECT;

public :

  ViewProfile(tlp::Graph *graph,tlp::View *view):number(0),graph(graph),view(view){

  }

  public slots :

    void addNodes() {
      for(int i=0;i<1000;++i){
        graph->addNode();
      }
      view->init();
      QTimer::singleShot(0, this, SLOT(delNodes()));
    }

    void delNodes() {
      std::vector<tlp::node> nodes;
      tlp::node n;
      forEach(n, graph->getNodes()) {
        nodes.push_back(n);
      }
      for(std::vector<tlp::node>::iterator it=nodes.begin();it!=nodes.end();++it){
        graph->delNode(*it);
      }
      view->init();
      if(number==10)
        QCoreApplication::exit(0);
      else
        QTimer::singleShot(0, this, SLOT(addNodes()));

      ++number;
    }

  protected :

    int number;
    tlp::Graph *graph;
    tlp::View *view;
};

#endif
