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
#include "tulip/GlMainWidgetPlugin.h"
#include <QtCore/QtPlugin>
#include "tulip/GlMainWidget.h"

#include "../designer/GlMainWidget.xpm"

namespace tlp {

GlMainWidgetPlugin::GlMainWidgetPlugin(QObject *parent) : QObject(parent) {
  initialized = false;
}

void GlMainWidgetPlugin::initialize(QDesignerFormEditorInterface * /*core*/) {
  if (initialized)
    return;

  initialized = true;
}

bool GlMainWidgetPlugin::isInitialized() const {
  return initialized;
}

QWidget* GlMainWidgetPlugin::createWidget(QWidget *parent) {
  return new GlMainWidget(parent);
}

QString GlMainWidgetPlugin::name() const {
  return "tlp::GlMainWidget";
}

QString GlMainWidgetPlugin::domXml() const {
  return "<widget class=\"tlp::GlMainWidget\" name=\"glMainWidget\">\n"
	 "</widget>\n";
}

QString GlMainWidgetPlugin::includeFile() const {
  return "tulip/GlMainWidget.h";
}

QString GlMainWidgetPlugin::group() const {
  return "Tulip Widgets";
}

QIcon GlMainWidgetPlugin::icon() const {
  return QIcon(QPixmap(GlMainWidget_xpm));
}

QString GlMainWidgetPlugin::toolTip() const {
  return "GlMain Widget";
}

QString GlMainWidgetPlugin::whatsThis() const {
  return "A widget displaying a Tulip GlMain";
}

bool GlMainWidgetPlugin::isContainer() const {
  return false;
}

QString GlMainWidgetPlugin::codeTemplate() const {
    return "";
}

Q_EXPORT_PLUGIN(GlMainWidgetPlugin)

}
