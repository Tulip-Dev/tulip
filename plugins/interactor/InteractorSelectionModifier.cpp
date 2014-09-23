/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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
#include <tulip/NodeLinkDiagramComponentInteractor.h>
#include <tulip/MouseSelector.h>
#include <tulip/MouseSelectionEditor.h>
#include <tulip/NodeLinkDiagramComponent.h>

#include "../utils/StandardInteractorPriority.h"
#include "../utils/ViewNames.h"

using namespace tlp;

/** \brief Tulip interactor to move/reshape
 *
 */
class InteractorSelectionModifier  : public NodeLinkDiagramComponentInteractor {

public:
  PLUGININFORMATION("InteractorSelectionModifier", "Tulip Team", "01/04/2009", "Selection Modifier Interactor", "1.0", "Modification")
  /**
   * Default constructor
   */
  InteractorSelectionModifier(const tlp::PluginContext*):NodeLinkDiagramComponentInteractor(":/tulip/gui/icons/i_move.png","Move/Reshape rectangle selection") {
    setPriority(StandardInteractorPriority::RectangleSelectionModifier);
    setConfigurationWidgetText(QString("<h3>Move/Reshape rectangle selection</h3>")+
                               "Modify selection<br/><br/>"+
                               "Resize : <ul><li><b>Mouse left</b> down on triangle + moves</li></ul>" +
                               "<ul><li><b>Mouse left</b> down on square + moves</li></ul>" +
                               "Only change node size : <ul><li><b>Ctrl + Mouse left</b> down on triangle + moves</li></ul>" +
                               "Only change selection size : <ul><li><b>Shift + Mouse left</b> down on triangle + moves</li></ul>" +
                               "Rotate : <ul><li><b>Mouse left</b> down on circle + moves</li></ul>" +
                               "Only rotate nodes : <ul><li><b>Ctrl + Mouse left</b> down on circle + moves</li></ul>" +
                               "Only rotate selection : <ul><li><b>Shift + Mouse left</b> down on circle + moves</li></ul>" +
                               "Translate : <ul><li><b>Mouse left</b> down inside rectangle + moves</li></ul>"+
                               "Align vertically/horizontally : <ul><li><b>Mouse left</b> click on two arrows icon in top right zone</li></ul>"+
                               "Align left/right/top/bottom : <ul><li><b>Mouse left</b> click on simple arrow icon in top right zone</li></ul>");
  }

  /**
   * Construct chain of responsibility
   */
  void construct() {
    push_back(new MousePanNZoomNavigator);
    push_back(new MouseSelector);
    push_back(new MouseSelectionEditor);
  }

  QCursor cursor() const {
    return QCursor(Qt::CrossCursor);
  }

  bool isCompatible(const std::string &viewName) const {
    return ((viewName==NodeLinkDiagramComponent::viewName)
            ||(viewName==ViewName::GoogleMapsViewName));
  }

};

PLUGIN(InteractorSelectionModifier)
