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

#include <tulip/MouseInteractors.h>
#include <tulip/MouseEdgeSelector.h>
#include <tulip/MouseEdgeBendEditor.h>

#include "NodeLinkDiagramComponentInteractor.h"

using namespace tlp;

/** \brief Tulip interactor to edit edge bends
 *
 */
class InteractorEditEdgeBends  : public NodeLinkDiagramComponentInteractor {

public:

  /**
   * Default constructor
   */
  InteractorEditEdgeBends():NodeLinkDiagramComponentInteractor(":/i_bends.png","Edit edge bends"){
    setPriority(1);
    setConfigurationWidgetText(QString("<h3>Edit edge bends interactor</h3>")+
                   "Modify edge bends<br/><br/>"+
                   "Select edge: <ul><li>use rectangle selection</li></ul>" +
                   "Translate bend: <ul><li><b>Mouse left</b> down on a selected bend + moves</li></ul>" +
                   "Add bend: <ul><li><b>Shift + Mouse left</b> click on the selected edge</li></ul>"+
  #if !defined(__APPLE__)
                   "Delete bend: <ul><li><b>Ctrl + Mouse left</b> click on a selected bend</li></ul>"
  #else
                   "Delete bend: <ul><li><b>Alt + Mouse left</b> click on a selected bend</li></ul>"
  #endif
                   );
  }

  /**
   * Construct chain of responsibility
   */
  void construct(){
    pushInteractorComponent(new MousePanNZoomNavigator);
    pushInteractorComponent(new MouseEdgeSelector);
    pushInteractorComponent(new MouseEdgeBendEditor);
  }
};

INTERACTORPLUGIN(InteractorEditEdgeBends, "InteractorEditEdgeBends", "Tulip Team", "01/04/2009", "Edit Edge Bends Interactor", "1.0");
