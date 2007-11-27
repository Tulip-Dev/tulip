//-*-c++-*-
/**
 Author: David Auber
 Email : auber@labri.fr
 Last modification : 06/09/2005
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>

#if (QT_REL == 3)
#include <qevent.h>
#else
#ifdef  _WIN32
// compilation pb workaround
#include <windows.h>
#endif
#include <QtGui/qevent.h>
#include "tulip/Qt3ForTulip.h"
#endif

#include <tulip/Graph.h>
#include <tulip/BooleanProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/GlGraphWidget.h>

#include <tulip/MouseMagicSelector.h>

using namespace std;
using namespace tlp;

MouseMagicSelector::MouseMagicSelector():
  x(0),y(0),w(0),h(0),started(false),graph(0)
{}

bool MouseMagicSelector::eventFilter(QObject *widget, QEvent *e) {
  if (e->type() == QEvent::MouseButtonPress) {
    QMouseEvent * qMouseEv = (QMouseEvent *) e;
    if (qMouseEv->button()==Qt::LeftButton) {
      QMouseEvent * qMouseEv = (QMouseEvent *) e;
      GlGraphWidget *glGraphWidget = (GlGraphWidget *) widget;
      x=qMouseEv->x();
      y=qMouseEv->y();
      ElementType type;
      node tmpNode;
      edge tmpEdge;
      glGraphWidget->doSelect(x, y, type, tmpNode, tmpEdge);
      if (type != NODE) {
	//cerr << __PRETTY_FUNCTION__ << ": type != NODE" << endl;
	return true;
      }
      Observable::holdObservers();
      graph=glGraphWidget->getScene()->getGlGraphComposite()->getInputData()->getGraph();
      BooleanProperty* selection=graph->getProperty<BooleanProperty>("viewSelection");
      BooleanProperty* visited=graph->getProperty<BooleanProperty>("tmpVisited");
      DoubleProperty* metric=graph->getProperty<DoubleProperty>("viewMetric");
      visited->setAllNodeValue(false);
      visited->setAllEdgeValue(false);
      selection->setAllNodeValue(false);
      selection->setAllEdgeValue(false);
      double initValue = metric->getNodeValue(tmpNode);
      list <node> bfsFifo;
      bfsFifo.push_back(tmpNode);
      while(!bfsFifo.empty()) {
      node itn = bfsFifo.front();
      bfsFifo.pop_front();
      selection->setNodeValue(itn,true);
      visited->setNodeValue(itn,true);
      Iterator<node> *itN = graph->getInOutNodes(itn);
	while (itN->hasNext()) {
	  node itv = itN->next();
	  double curValue = metric->getNodeValue(itv);
	  if ( curValue == initValue && (!visited->getNodeValue(itv)) ) {
	    bfsFifo.push_back(itv);
	  }
	} delete itN;
      }
      Observable::unholdObservers();
      return true;
    }
  }
  return false;
}
