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
// Warning !!! this file does not have to be compiled
// when building Tulip with Qt 3
#include "tulip/SGHierarchyWidgetPlugin.h"
#include <QtCore/QtPlugin>
#include "tulip/SGHierarchyWidget.h"

#include "../designer/sghierarchywidget.xpm"

using namespace tlp;

SGHierarchyWidgetPlugin::SGHierarchyWidgetPlugin(QObject *parent) : QObject(parent) {
  initialized = false;
  graph = tlp::newGraph();
}

SGHierarchyWidgetPlugin::~SGHierarchyWidgetPlugin() {
  delete graph;
}

void SGHierarchyWidgetPlugin::initialize(QDesignerFormEditorInterface * /*core*/) {
  if (initialized)
    return;

  initialized = true;
}

bool SGHierarchyWidgetPlugin::isInitialized() const {
  return initialized;
}

QWidget* SGHierarchyWidgetPlugin::createWidget(QWidget *parent) {
  return new SGHierarchyWidget(parent, graph);
}

QString SGHierarchyWidgetPlugin::name() const {
  return "SGHierarchyWidget";
}
  
QString SGHierarchyWidgetPlugin::domXml() const {
  return "<widget class=\"SGHierarchyWidget\" name=\"hierarchyWidget\">\n"
         "</widget>\n";
}

QString SGHierarchyWidgetPlugin::includeFile() const {
  return "tulip/SGHierarchyWidget.h";
}

QString SGHierarchyWidgetPlugin::group() const {
  return "Tulip Widgets";
}

QIcon SGHierarchyWidgetPlugin::icon() const {
  return QIcon(QPixmap(sghierarchywidget_xpm));
}

QString SGHierarchyWidgetPlugin::toolTip() const {
  return "Cluster Tree";
}

QString SGHierarchyWidgetPlugin::whatsThis() const {
  return "A widget displaying the subgraphs tree hierarchy of a graph";
}

bool SGHierarchyWidgetPlugin::isContainer() const {
  return false;
}

QString SGHierarchyWidgetPlugin::codeTemplate() const {
    return "";
}

Q_EXPORT_PLUGIN(SGHierarchyWidgetPlugin)
