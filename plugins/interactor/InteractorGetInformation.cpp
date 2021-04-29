/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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

#include <vector>

#include <tulip/Interactor.h>
#include <tulip/MouseInteractors.h>
#include <tulip/MouseShowElementInfo.h>
#include <tulip/NodeLinkDiagramComponentInteractor.h>
#include <tulip/NodeLinkDiagramComponent.h>
#include <tulip/StringsListSelectionWidget.h>
#include "../utils/StandardInteractorPriority.h"
#include "../utils/PluginNames.h"

#include <QLabel>
#include <QVBoxLayout>

using namespace tlp;

/** \brief Tulip interactor to get information about an element of the graph
 *
 */
class InteractorGetInformation : public NodeLinkDiagramComponentInteractor {

  class ConfigWidget : public QWidget {
    InteractorGetInformation *_interactor;

  public:
    ConfigWidget(InteractorGetInformation *interactor) : _interactor(interactor) {}

    void hideEvent(QHideEvent *) override {
      _interactor->setVisibleProperties();
    }
  };

  MouseShowElementInfo *_elementInfo;
  ConfigWidget *_configWidget;
  StringsListSelectionWidget *_propsList;

public:
  PLUGININFORMATION("InteractorGetInformation", "Tulip Team", "01/04/2009",
                    "Get Information Interactor", "1.0", "Information")
  /**
   * Default constructor
   */
  InteractorGetInformation(const tlp::PluginContext *)
      : NodeLinkDiagramComponentInteractor(":/tulip/gui/icons/i_select.png",
                                           "Display node or edge properties",
                                           StandardInteractorPriority::GetInformation) {}

  /**
   * Construct chain of responsibility
   */
  void construct() override {
    setConfigurationWidgetText(
        QString("<h3>Display node or edge properties</h3>") +
        "<b>Mouse left click</b> on an element (the mouse cursor must be as <img "
        "src=\":/tulip/gui/icons/i_select.png\">),<br/>"
        "to display a panel showing its properties.<br/>"
        "As the panel is displayed, <b>Mouse left click</b> in a property row to edit the "
        "corresponding value.<br/>"
        "The visible properties can be filtered using the list of properties displayed in the "
        "<b>Options</b> tab.<br/>"
        "If none is filtered, when the element properties panel is displayed, the display of the "
        "visual rendering properties can be then toggled using a dedicated check box.<br/><br/>" +
        "<u>3D Navigation in the graph</u><br/><br/>" +
        "Translation: <ul><li><b>Mouse left</b> down + moves</li><li>or <b>Arrow</b> keys "
        "</li></ul>" +
        "X or Y rotation: <ul><li><b>Shift + Mouse left</b> down + up/down or left/right "
        "moves</li></ul>" +
#if !defined(__APPLE__)
        "Z rotation: <ul><li><b>Ctrl + Mouse left</b> down + left/right moves</li><li> or "
        "<b>Insert</b> key</li></ul>" +
        "Zoom/Unzoom: <ul><li><b>Mouse wheel</b> up/down</li><li> or <b>Ctrl + Mouse left</b> down "
        "+ up/down moves</li><li> or <b>Pg "
        "up/Pg down</b> keys</li></ul>"
#else
        "Z rotation: <ul><li><b>Alt + Mouse left</b> down + left/right moves</li><li> or "
        "<b>Insert</b> key</li></ul>" +
        "Translation: <ul><li><b>Arrow</b> keys</li></ul>" +
        "Zoom/Unzoom: <ul><li><b>Mouse wheel</b> down/up</li><li> or <b>Alt + Mouse left</b> down "
        "+ up/down moves</li><li> or <b>Pg up/Pg "
        "down</b> keys</li></ul>"
#endif
        +
        "Meta node navigation: <ul><li><b>double Mouse left click</b> go inside the metanode</li>" +
        "<li><b>Ctrl + double Mouse left click</b> go outside the metanode</li></ul>");
    push_back(new MouseNKeysNavigator);
    push_back(_elementInfo = new MouseShowElementInfo);
    // build configuration widget
    _configWidget = new ConfigWidget(this);
    Interactor::setupConfigWidget(_configWidget);
    QVBoxLayout *verticalLayout = new QVBoxLayout(_configWidget);
    QLabel *label = new QLabel("Visible properties");
    label->setObjectName("label");
    verticalLayout->addWidget(label);
    _propsList = new StringsListSelectionWidget(_configWidget,
                                                StringsListSelectionWidget::NON_ORDERABLE_LIST, 0);
    verticalLayout->addWidget(_propsList);

    auto graph = view()->graph();
    std::vector<std::string> stringsList;
    for (auto propName : graph->getProperties()) {
#ifdef NDEBUG
      if (propName == "viewMetaGraph")
        continue;
#endif
      stringsList.push_back(propName);
    }
    _propsList->setSelectedStringsList(stringsList);
  }

  QWidget *configurationOptionsWidget() const override {
    return _configWidget;
  }

  bool isCompatible(const std::string &viewName) const override {
    return ((viewName == NodeLinkDiagramComponent::viewName) ||
            (viewName == ViewName::PixelOrientedViewName));
  }

  void setVisibleProperties() {
    auto stringList = _propsList->getUnselectedStringsList();
    // an empty vector indicates that all the properties are visible
    if (!stringList.empty())
      stringList = _propsList->getSelectedStringsList();
    _elementInfo->setVisibleProperties(stringList);
  }
};

PLUGIN(InteractorGetInformation)
