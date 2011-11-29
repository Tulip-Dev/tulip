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
#include "tulip/NodeLinkDiagramComponent.h"

#include <tulip/GlMainWidget.h>
#include <tulip/GlGraphComposite.h>
#include <tulip/GlGraphInputData.h>

using namespace tlp;

NodeLinkDiagramComponent::NodeLinkDiagramComponent() {
}

NodeLinkDiagramComponent::~NodeLinkDiagramComponent() {
}

void NodeLinkDiagramComponent::setState(const tlp::DataSet& data) {
  getGlMainWidget()->setData(graph(), data);
  registerTriggers();
}

void NodeLinkDiagramComponent::graphChanged(tlp::Graph* graph) {
  getGlMainWidget()->setGraph(graph);
  registerTriggers();
}

tlp::DataSet NodeLinkDiagramComponent::state() const {
  return getGlMainWidget()->getData();
}

void NodeLinkDiagramComponent::registerTriggers() {
  foreach (tlp::Observable* obs, triggers()) {
    removeRedrawTrigger(obs);
  }

  GlGraphInputData* inputData = getGlMainWidget()->getScene()->getGlGraphComposite()->getInputData();
  addRedrawTrigger(inputData->getElementBorderColor());
  addRedrawTrigger(inputData->getElementBorderWidth());
  addRedrawTrigger(inputData->getElementColor());
  addRedrawTrigger(inputData->getElementFont());
  addRedrawTrigger(inputData->getElementFontSize());
  addRedrawTrigger(inputData->getElementLabel());
  addRedrawTrigger(inputData->getElementLabelColor());
  addRedrawTrigger(inputData->getElementLabelPosition());
  addRedrawTrigger(inputData->getElementLayout());
  addRedrawTrigger(inputData->getElementRotation());
  addRedrawTrigger(inputData->getElementSelected());
  addRedrawTrigger(inputData->getElementShape());
  addRedrawTrigger(inputData->getElementSize());
  addRedrawTrigger(inputData->getElementTexture());
  addRedrawTrigger(inputData->getElementSrcAnchorShape());
  addRedrawTrigger(inputData->getElementSrcAnchorSize());
  addRedrawTrigger(inputData->getElementTgtAnchorShape());
  addRedrawTrigger(inputData->getElementTgtAnchorSize());
}

VIEWPLUGIN(NodeLinkDiagramComponent, "Node Link Diagram view", "Tulip Team", "16/04/2008", "Node link diagram", "1.0")
