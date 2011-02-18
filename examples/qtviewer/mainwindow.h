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
#include <QtGui/QMainWindow>
#include <QtGui/QToolBar>
#include <tulip/Graph.h>
#include <tulip/Observable.h>
#include <tulip/Interactor.h>
#include <tulip/TlpQtTools.h>
#include <tulip/NodeLinkDiagramComponent.h>
#include <tulip/PluginLoaderTxt.h>
#include <tulip/InteractorManager.h>

using namespace std;
using namespace tlp;

typedef std::set< tlp::Observable * >::iterator ObserverIterator;

class QtViewerMainWindow : public QMainWindow, public Observer {
  
  Q_OBJECT

public :

  QtViewerMainWindow(const string &filename);
  ~QtViewerMainWindow();	

public slots:
  
  void observableDestroyed(Observable *) {}
  void changeInteractor();
  void update ( ObserverIterator begin, ObserverIterator end);		


private :

  void initObservers();
  void clearObservers();

  Graph *graph;
  NodeLinkDiagramComponent *nodeLinkView;
  QToolBar *toolBar;

};
