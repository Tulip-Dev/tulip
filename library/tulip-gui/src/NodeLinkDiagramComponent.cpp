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

#include <QtCore/QDebug>

using namespace tlp;
using namespace std;

NodeLinkDiagramComponent::NodeLinkDiagramComponent() {
}

NodeLinkDiagramComponent::~NodeLinkDiagramComponent() {
}

void NodeLinkDiagramComponent::setState(const tlp::DataSet& data) {
  getGlMainWidget()->setData(graph(), data);
  registerTriggers();
  connect(getGlMainWidget(),SIGNAL(viewDrawn(GlMainWidget*,bool)),this,SLOT(glMainWidgetDrawn(GlMainWidget *,bool)));
  this->drawOverview(true);
}

void NodeLinkDiagramComponent::graphChanged(tlp::Graph* graph) {
  getGlMainWidget()->setGraph(graph);
  registerTriggers();
  emit drawNeeded();
}

tlp::DataSet NodeLinkDiagramComponent::state() const {
  return getGlMainWidget()->getData();
}

void NodeLinkDiagramComponent::registerTriggers() {
  foreach (tlp::Observable* obs, triggers()) {
    removeRedrawTrigger(obs);
  }
  addRedrawTrigger(getGlMainWidget()->getGraph());
  std::set<tlp::PropertyInterface*> properties = getGlMainWidget()->getScene()->getGlGraphComposite()->getInputData()->properties();

  for(std::set<tlp::PropertyInterface*>::iterator it = properties.begin(); it != properties.end(); ++it) {
    addRedrawTrigger(*it);
  }
}

bool NodeLinkDiagramComponent::isLayoutProperty(tlp::PropertyInterface* pi) const {
  return pi == getGlMainWidget()->getScene()->getGlGraphComposite()->getInputData()->getElementLayout();
}

QList<QWidget*> NodeLinkDiagramComponent::configurationWidgets() const {
  return QList<QWidget*>();
}

void NodeLinkDiagramComponent::glMainWidgetDrawn(GlMainWidget *,bool graphChanged) {
  drawOverview(graphChanged);
}

VIEWPLUGIN(NodeLinkDiagramComponent, "Node Link Diagram view", "Tulip Team", "16/04/2008", "Node link diagram", "1.0")
