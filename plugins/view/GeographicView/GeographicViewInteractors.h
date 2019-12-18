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

#ifndef GEOGRAPHIC_VIEW_INTERACTORS_H
#define GEOGRAPHIC_VIEW_INTERACTORS_H

#include <tulip/GLInteractor.h>
#include <tulip/MouseInteractors.h>
#include "GeographicView.h"
#include <tulip/NodeLinkDiagramComponentInteractor.h>
#include <QCursor>

class GeographicViewInteractor : public tlp::GLInteractorComposite {

public:
  GeographicViewInteractor(const QString &iconPath, const QString &text);

  bool isCompatible(const std::string &viewName) const override;
};

class GeographicViewNavigator : public tlp::MouseNKeysNavigator {

public:
  GeographicViewNavigator();
  ~GeographicViewNavigator() override;

  bool eventFilter(QObject *, QEvent *) override;
  bool draw(tlp::GlMainWidget *) {
    return false;
  }

  bool compute(tlp::GlMainWidget *) {
    return false;
  }
  void viewChanged(tlp::View *) override;

protected:
  int x, y;
  bool inRotation;
};

class GeographicViewInteractorNavigation : public GeographicViewInteractor {

public:
  PLUGININFORMATION("InteractorNavigationGeographicView", "Tulip Team", "01/04/2009",
                    "Geographic View Navigation Interactor", "1.0", "Navigation")

  explicit GeographicViewInteractorNavigation(const tlp::PluginContext *);

  void construct() override;

  QWidget *configurationWidget() const override;
  unsigned int priority() const override;
};

class GeographicViewInteractorSelection : public GeographicViewInteractor {

public:
  PLUGININFORMATION("InteractorSelectionGeographicView", "Tulip Team", "01/12/2018",
                    "Geographic View Selection Interactor", "1.0", "Selection")

  explicit GeographicViewInteractorSelection(const tlp::PluginContext *);

  void construct() override;

  QWidget *configurationWidget() const override;

  QCursor cursor() const override;

  unsigned int priority() const override;
};

class GeographicViewInteractorSelectionEditor : public GeographicViewInteractor {

public:
  PLUGININFORMATION("InteractorSelectionEditorGeographicView", "Tulip Team", "01/12/2018",
                    "Geographic View Selection Editor Interactor", "1.0", "Selection")

  explicit GeographicViewInteractorSelectionEditor(const tlp::PluginContext *);

  void construct() override;

  QWidget *configurationWidget() const override;

  QCursor cursor() const override;

  unsigned int priority() const override;
};

class GeographicViewInteractorAddEdges : public tlp::NodeLinkDiagramComponentInteractor {

public:
  PLUGININFORMATION("InteractorAddEdgesGeographicView", "Tulip Team", "02/06/2017",
                    "Geographic View Add Edges Interactor", "1.0", "Modification")

  explicit GeographicViewInteractorAddEdges(const tlp::PluginContext *);

  void construct() override;

  QCursor cursor() const override;

  bool isCompatible(const std::string &viewName) const override;
};

class GeographicViewInteractorEditEdgeBends : public tlp::NodeLinkDiagramComponentInteractor {

public:
  PLUGININFORMATION("InteractorEditEdgeBendsGeographicView", "Tulip Team", "02/06/2017",
                    "Geographic View Edit Edge Bends Interactor", "1.0", "Modification")

  explicit GeographicViewInteractorEditEdgeBends(const tlp::PluginContext *);

  void construct() override;

  bool isCompatible(const std::string &viewName) const override;
};

#endif // GEOGRAPHIC_VIEW_INTERACTORS_H
