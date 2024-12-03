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

#ifndef INTERACTOREXPLORER_H
#define INTERACTOREXPLORER_H

#include <vector>

#include <tulip/Interactor.h>
#include <tulip/MouseInteractors.h>
#include <tulip/MouseShowElementInfo.h>
#include <tulip/NodeLinkDiagramComponentInteractor.h>
#include <tulip/NodeLinkDiagramComponent.h>
#include <tulip/StandardInteractorPriority.h>
#include <tulip/StringsListSelectionWidget.h>

namespace tlp {

/** @class Tulip interactor to navigate
 *  and edit graph elements information
 *
 */
class TLP_QT_SCOPE InteractorViewExplorer : public NodeLinkDiagramComponentInteractor {

  class ConfigWidget : public QWidget {
    InteractorViewExplorer *_interactor;

  public:
    ConfigWidget(InteractorViewExplorer *interactor) : _interactor(interactor) {}

    void hideEvent(QHideEvent *) override {
      _interactor->setVisibleProperties();
    }
  };

  ConfigWidget *_configWidget;
  StringsListSelectionWidget *_propsList;
  QString _configText;
  InteractorComponent *_firstInteractor;
  MouseShowElementInfo *_elementInfo;

public:
  PLUGININFORMATION("InteractorViewExplorer", "Tulip Team", "01/12/2024", "Explore current view",
                    "1.0", "Information")
  /**
   * Default constructor
   */
  InteractorViewExplorer(QString configText = QString(), InteractorComponent *interactor = nullptr,
                         MouseShowElementInfo *elementInfo = new MouseShowElementInfo)
      : NodeLinkDiagramComponentInteractor(":/tulip/gui/icons/i_explore.png",
                                           "Explore current view",
                                           StandardInteractorPriority::Navigation),
        _configText(configText), _firstInteractor(interactor), _elementInfo(elementInfo) {}

  /**
   * Construct chain of responsibility
   */
  void construct() override;

  QWidget *configurationOptionsWidget() const override {
    return _configWidget;
  }

  void setVisibleProperties();
};
} // namespace tlp
#endif // INTERACTOREXPLORER_H
