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

#ifndef GOOGLEMAPSVIEWINTERACTORS_H_
#define GOOGLEMAPSVIEWINTERACTORS_H_

#include <tulip/GLInteractor.h>
#include <tulip/MouseInteractors.h>
#include "GeographicView.h"
#include <tulip/NodeLinkDiagramComponentInteractor.h>

using namespace tlp;

class GeographicViewInteractor : public GLInteractorComposite {

public:
  GeographicViewInteractor(const QString &iconPath, const QString &text);

  bool isCompatible(const std::string &viewName) const override;
};

class GeographicViewNavigator : public MouseNKeysNavigator {

public:
  GeographicViewNavigator();
  ~GeographicViewNavigator() override;

  bool eventFilter(QObject *, QEvent *) override;
  bool draw(GlMainWidget *) {
    return false;
  }

  bool compute(GlMainWidget *) {
    return false;
  }
  void viewChanged(View *) override;

protected:
  int x, y;
  bool inRotation;
};

class GeographicViewInteractorNavigation : public GeographicViewInteractor {

public:
  PLUGININFORMATION("InteractorNavigationGeographicView", "Tulip Team", "01/04/2009",
                    "Geographic View Navigation Interactor", "1.0", "Navigation")

  GeographicViewInteractorNavigation(const PluginContext *);

  void construct() override;

  QWidget *configurationWidget() const override;
  unsigned int priority() const override;
};

class GeographicViewInteractorAddEdges : public NodeLinkDiagramComponentInteractor {

public:
  PLUGININFORMATION("InteractorAddEdgesGeographicView", "Tulip Team", "02/06/2017",
                    "Geographic View Add Edges Interactor", "1.0", "Modification")

  GeographicViewInteractorAddEdges(const PluginContext *);

  void construct() override;

  QCursor cursor() const override;

  bool isCompatible(const std::string &viewName) const override;
};

class GeographicViewInteractorEditEdgeBends : public NodeLinkDiagramComponentInteractor {

public:
  PLUGININFORMATION("InteractorEditEdgeBendsGeographicView", "Tulip Team", "02/06/2017",
                    "Geographic View Edit Edge Bends Interactor", "1.0", "Modification")

  GeographicViewInteractorEditEdgeBends(const PluginContext *);

  void construct() override;

  bool isCompatible(const std::string &viewName) const override;
};

#endif /* GOOGLEMAPSVIEWINTERACTORS_H_ */
