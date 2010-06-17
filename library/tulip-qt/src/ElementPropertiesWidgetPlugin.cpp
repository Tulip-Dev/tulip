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
// Warning !!! this file does not have to be compiled
// when building Tulip with Qt 3
#include "tulip/ElementPropertiesWidgetPlugin.h"
#include <QtCore/QtPlugin>
#include "tulip/ElementPropertiesWidget.h"

#include "../designer/elementtable.xpm"

using namespace tlp;

ElementPropertiesWidgetPlugin::ElementPropertiesWidgetPlugin(QObject *parent) : QObject(parent) {
  initialized = false;
}

void ElementPropertiesWidgetPlugin::initialize(QDesignerFormEditorInterface * /*core*/) {
  if (initialized)
    return;

  initialized = true;
}

bool ElementPropertiesWidgetPlugin::isInitialized() const {
  return initialized;
}

QWidget* ElementPropertiesWidgetPlugin::createWidget(QWidget *parent) {
  return new ElementPropertiesWidget(NULL, QStringList(), QStringList(), parent);
}

QString ElementPropertiesWidgetPlugin::name() const {
  return "ElementPropertiesWidget";
}
  
QString ElementPropertiesWidgetPlugin::domXml() const {
  return "<widget class=\"ElementPropertiesWidget\" name=\"tulipElementProperties\">\n"
         "</widget>\n";
}

QString ElementPropertiesWidgetPlugin::includeFile() const {
  return "tulip/ElementPropertiesWidget.h";
}

QString ElementPropertiesWidgetPlugin::group() const {
  return "Tulip Widgets";
}

QIcon ElementPropertiesWidgetPlugin::icon() const {
  return QIcon(QPixmap(elementtable_xpm));
}

QString ElementPropertiesWidgetPlugin::toolTip() const {
  return "Node Properties Widget";
}

QString ElementPropertiesWidgetPlugin::whatsThis() const {
  return "A widget to display the properties of a Tulip graph element (node or edge)";
}

bool ElementPropertiesWidgetPlugin::isContainer() const {
  return false;
}

QString ElementPropertiesWidgetPlugin::codeTemplate() const {
    return "";
}

Q_EXPORT_PLUGIN(ElementPropertiesWidgetPlugin)
