// Warning !!! this file does not have to be compiled
// when building Tulip with Qt 3
#include "tulip/SGHierarchyWidgetPlugin.h"
#include <QtCore/QtPlugin>
#include "tulip/SGHierarchyWidgetQt3.h"

#include "../designer/clustertree.xpm"

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
  return new SGHierarchyWidget(graph, parent, name());
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
  return QIcon(QPixmap(clustertree_xpm));
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
