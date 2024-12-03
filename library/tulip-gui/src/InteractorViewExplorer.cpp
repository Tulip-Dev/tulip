/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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

#include <tulip/InteractorViewExplorer.h>
#include <tulip/MouseInteractors.h>
#include <tulip/MouseShowElementInfo.h>

#include <QLabel>
#include <QVBoxLayout>

using namespace tlp;

void InteractorViewExplorer::construct() {
  setConfigurationWidgetText(QString("<h3>Explore %1</h3>").arg(view()->name().c_str()) +
                             _configText);
  if (_firstInteractor)
    push_back(_firstInteractor);
  push_back(new MouseNKeysNavigator);
  push_back(_elementInfo);
  // build configuration widget
  _configWidget = new ConfigWidget(this);
  Interactor::setupConfigWidget(_configWidget);
  QVBoxLayout *verticalLayout = new QVBoxLayout(_configWidget);
  QLabel *label = new QLabel("Graph element visible properties");
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

void InteractorViewExplorer::setVisibleProperties() {
  auto stringList = _propsList->getUnselectedStringsList();
  // an empty vector indicates that all the properties are visible
  if (!stringList.empty())
    stringList = _propsList->getSelectedStringsList();
  _elementInfo->setVisibleProperties(stringList);
}
