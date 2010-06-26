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
#include "tulip/PropertyWidgetPlugin.h"
#include <QtCore/QtPlugin>
#include "tulip/PropertyWidget.h"

#include "../designer/elementtable.xpm"

using namespace tlp;

PropertyWidgetPlugin::PropertyWidgetPlugin(QObject *parent) : QObject(parent) {
  initialized = false;
}

void PropertyWidgetPlugin::initialize(QDesignerFormEditorInterface * /*core*/) {
  if (initialized)
    return;

  initialized = true;
}

bool PropertyWidgetPlugin::isInitialized() const {
  return initialized;
}

QWidget* PropertyWidgetPlugin::createWidget(QWidget *parent) {
  return new PropertyWidget(parent);
}

QString PropertyWidgetPlugin::name() const {
  return "PropertyWidget";
}
  
QString PropertyWidgetPlugin::domXml() const {
  return "<widget class=\"PropertyWidget\" name=\"propertyWidget\">\n"
         "</widget>\n";
}

QString PropertyWidgetPlugin::includeFile() const {
  return "tulip/PropertyWidget.h";
}

QString PropertyWidgetPlugin::group() const {
  return "Tulip Widgets";
}

QIcon PropertyWidgetPlugin::icon() const {
  return QIcon(QPixmap(elementtable_xpm));
}

QString PropertyWidgetPlugin::toolTip() const {
  return "Graph Properties Widget";
}

QString PropertyWidgetPlugin::whatsThis() const {
  return "A widget to display the properties associated to a Tulip graph";
}

bool PropertyWidgetPlugin::isContainer() const {
  return false;
}

QString PropertyWidgetPlugin::codeTemplate() const {
    return "";
}

Q_EXPORT_PLUGIN(PropertyWidgetPlugin)
