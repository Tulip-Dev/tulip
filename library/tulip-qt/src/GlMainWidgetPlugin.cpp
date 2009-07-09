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
