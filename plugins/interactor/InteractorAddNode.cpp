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

#include <tulip/MouseInteractors.h>
#include <tulip/MouseNodeBuilder.h>

#include "NodeLinkDiagramComponentInteractor.h"

using namespace tlp;

/** \brief Tulip interactor to add a node
 *
 */
class InteractorAddNode  : public NodeLinkDiagramComponentInteractor {

public:

  /**
   * Default constructor
   */
  InteractorAddNode():NodeLinkDiagramComponentInteractor(":/i_addnode.png","Add nodes"){
    setPriority(1);
    setConfigurationWidgetText(QString("<h3>Add node interactor</h3>")+
                   "<b>Mouse left</b> click to add a node in the graph");
  }

  /**
   * Construct chain of responsibility
   */
  void construct(){
    pushInteractorComponent(new MousePanNZoomNavigator);
    pushInteractorComponent(new MouseNodeBuilder);
  }

  QCursor getCursor(){return QCursor(Qt::PointingHandCursor);}
};

INTERACTORPLUGIN(InteractorAddNode, "InteractorAddNode", "Tulip Team", "01/04/2009", "Add Node Interactor", "1.0");
