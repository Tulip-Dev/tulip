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
        "<b>Mouse left click</b> on an element to display its "
        "properties.<br/>then <b>Mouse left click</b> on a row to edit the "
        "corresponding value.<br/>The visible properties can be filtered using the list of "
        "properties displayed in the <b>Options</b> tab.<br/>If none are filtered, the display of "
        "the visual rendering properties can be dynamically toggled when the element properties "
        "panel is displayed.");
    push_back(new MousePanNZoomNavigator);
    push_back(_elementInfo = new MouseShowElementInfo);
    // build configuration widget
    _configWidget = new ConfigWidget(this);
    QVBoxLayout *verticalLayout = new QVBoxLayout(_configWidget);
    QLabel *label = new QLabel("Visible properties");
    verticalLayout->addWidget(label);
    _propsList =
        new StringsListSelectionWidget(_configWidget, StringsListSelectionWidget::SIMPLE_LIST, 0);
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
