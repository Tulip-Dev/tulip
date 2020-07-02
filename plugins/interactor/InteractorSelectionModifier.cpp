/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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
#include "../utils/PluginNames.h"

using namespace tlp;

/** \brief Tulip interactor to move/reshape
 *
 */
class InteractorSelectionModifier : public NodeLinkDiagramComponentInteractor {

public:
  PLUGININFORMATION("InteractorSelectionModifier", "Tulip Team", "01/04/2009",
                    "Selection Modifier Interactor", "1.0", "Modification")
  /**
   * Default constructor
   */
  InteractorSelectionModifier(const tlp::PluginContext *)
      : NodeLinkDiagramComponentInteractor(":/tulip/gui/icons/i_move.png",
                                           "Move/Reshape rectangle selection",
                                           StandardInteractorPriority::RectangleSelectionModifier) {
  }

  /**
   * Construct chain of responsibility
   */
  void construct() override {
    setConfigurationWidgetText(
        QString("<h3>Move/Reshape rectangle selection</h3>") + "<u>Modify selection</u><br/><br/>" +
        "Resize: <ul><li><b>Mouse left</b> down on triangle + moves</li></ul>" +
        "<ul><li><b>Mouse left</b> down on square + moves</li></ul>" +
        "Only change node size: <ul><li><b>Ctrl + Mouse left</b> down on triangle + "
        "moves</li></ul>" +
        "Only change selection size: <ul><li><b>Shift + Mouse left</b> down on triangle + "
        "moves</li></ul>" +
        "Rotate: <ul><li><b>Mouse left</b> down on circle + moves</li></ul>" +
        "Only rotate nodes: <ul><li><b>Ctrl + Mouse left</b> down on circle + moves</li></ul>" +
        "Only rotate selection: <ul><li><b>Shift + Mouse left</b> down on circle + "
        "moves</li></ul>" +
        "Translate: <ul><li><b>Mouse left</b> down inside rectangle + moves</li>"
        "<li> or <b>Arrow</b> keys</li></ul>" +
        "Align vertically/horizontally: <ul><li><b>Mouse left</b> click on two arrows icon in top "
        "right zone</li></ul>" +
        "Align left/right/top/bottom: <ul><li><b>Mouse left</b> click on simple arrow icon in top "
        "right zone</li></ul>" +
        "<u>Navigation in the graph</u><br/><br/>" +
        "Translation (if no selection): <ul><li><b>Arrow</b> keys</li></ul>" +
#if !defined(__APPLE__)
        "Zoom/Unzoom: <ul><li><b>Mouse wheel</b> up/down</li><li> or <b>Pg up/Pg "
        "down</b> keys</li></ul>"
#else
        "Zoom/Unzoom: <ul><li><b>Mouse wheel</b> down/up</li><li> or <b>Pg up/Pg "
        "down</b> keys</li></ul>"
#endif
    );
    push_back(new MouseNKeysNavigator(false));
    push_back(new MouseSelector);
    push_back(new MouseSelectionEditor);
  }

  QCursor cursor() const override {
    return QCursor(Qt::CrossCursor);
  }

  bool isCompatible(const std::string &viewName) const override {
    return (viewName == NodeLinkDiagramComponent::viewName);
  }
};

PLUGIN(InteractorSelectionModifier)
