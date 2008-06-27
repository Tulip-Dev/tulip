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
