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
#include <tulip/MouseSelector.h>

#include "NodeLinkDiagramComponentInteractor.h"

using namespace tlp;

/** \brief Tulip interactor to select par of the graph
 *
 */
class InteractorSelection  : public NodeLinkDiagramComponentInteractor {

public:

  /**
   * Default constructor
   */
  InteractorSelection():NodeLinkDiagramComponentInteractor(":/i_selection.png","Select nodes/edges in a rectangle"){
    setPriority(3);
    setConfigurationWidgetText(QString("<h3>Selection interactor</h3>")+
                   "Select on rectangle.<br/><b>Mouse left</b> down indicates the first corner, <b>Mouse left</b> up indicates the opposite corner.<br/><br/>"+
  #if !defined(__APPLE__)
                   "Add from selection: <ul><li><b>Ctrl + Mouse left</b> click on an element</li></ul>" +
  #else
                   "Add/Remove from selection: <ul><li><b>Alt + Mouse left</b> click</li></ul>"+
  #endif
                   "Remove from selection: <ul><li><b>Shift + Mouse</b> click</li></ul>");
  }

  /**
   * Construct chain of responsibility
   */
  void construct(){
    pushInteractorComponent(new MousePanNZoomNavigator);
    pushInteractorComponent(new MouseSelector);
  }
};

INTERACTORPLUGIN(InteractorSelection, "InteractorSelection", "Tulip Team", "01/04/2009", "Selection Interactor", "1.0");
