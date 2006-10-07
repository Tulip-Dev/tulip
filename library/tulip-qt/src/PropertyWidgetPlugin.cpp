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
  return new PropertyWidget(parent, name());
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
  return "Element Table Widget";
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
