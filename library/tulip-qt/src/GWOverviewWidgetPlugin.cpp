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
#include <QtCore/QtPlugin>
#include "tulip/GWOverviewWidgetPlugin.h"
#include "tulip/GWOverviewWidget.h"

#include "../designer/gwoverviewwidget.xpm"


using namespace tlp;

GWOverviewWidgetPlugin::GWOverviewWidgetPlugin(QObject *parent) : QObject(parent) {
  initialized = false;
}

GWOverviewWidgetPlugin::~GWOverviewWidgetPlugin() {
}

void GWOverviewWidgetPlugin::initialize(QDesignerFormEditorInterface * /*core*/) {
  if (initialized)
    return;

  initialized = true;
}

bool GWOverviewWidgetPlugin::isInitialized() const {
  return initialized;
}

QWidget* GWOverviewWidgetPlugin::createWidget(QWidget *parent) {
  return new GWOverviewWidget(parent);
}

QString GWOverviewWidgetPlugin::name() const {
  return "GWOverviewWidget";
}
  
QString GWOverviewWidgetPlugin::domXml() const {
  return "<widget class=\"GWOverviewWidget\" name=\"overviewWidget\">\n"
         "</widget>\n";
}

QString GWOverviewWidgetPlugin::includeFile() const {
  return "tulip/GWOverviewWidget.h";
}

QString GWOverviewWidgetPlugin::group() const {
  return "Tulip Widgets";
}

QIcon GWOverviewWidgetPlugin::icon() const {
  return QIcon(QPixmap(gwoverviewwidget_xpm));
}

QString GWOverviewWidgetPlugin::toolTip() const {
  return "Cluster Tree";
}

QString GWOverviewWidgetPlugin::whatsThis() const {
  return "A widget displaying the overview of the Graph associated to a GlMainWidget";
}

bool GWOverviewWidgetPlugin::isContainer() const {
  return false;
}

QString GWOverviewWidgetPlugin::codeTemplate() const {
    return "";
}

Q_EXPORT_PLUGIN(GWOverviewWidgetPlugin)
