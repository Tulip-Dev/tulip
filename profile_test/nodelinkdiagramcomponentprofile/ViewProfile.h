/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
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
