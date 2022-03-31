/*
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
///@cond DOXYGEN_HIDDEN

#ifndef MOUSEOBJECT_H
#define MOUSEOBJECT_H

#include <tulip/InteractorComposite.h>
#include <tulip/Camera.h>
#include <tulip/GlScene.h>

namespace tlp {
class GlMainWidget;
class NodeLinkDiagramComponent;

/** An interactor class allowing to zoom and pan using the mouse wheel.
 */
class TLP_QT_SCOPE MousePanNZoomNavigator : public InteractorComponent {
public:
  MousePanNZoomNavigator() : cameraScaleFactor(1), isGesturing(false) {}
  ~MousePanNZoomNavigator() override {}
  bool eventFilter(QObject *, QEvent *) override;

protected:
  float cameraScaleFactor;
  bool isGesturing;
};

/** An interactor class used to delete a graph element on mouse left click
 */
class TLP_QT_SCOPE MouseElementDeleter : public InteractorComponent {
public:
  MouseElementDeleter() : glMainWidget(nullptr) {}
  ~MouseElementDeleter() override {}
  bool eventFilter(QObject *, QEvent *) override;
  void clear() override;
  virtual void delElement(tlp::Graph *graph, tlp::SelectedEntity &selectedEntity);

private:
  GlMainWidget *glMainWidget;
};

/** An interactor class to translate/rotate using keys or mouse movements,
    or to zoom and pan using the mouse wheel
 */
class TLP_QT_SCOPE MouseNKeysNavigator : public MousePanNZoomNavigator {
private:
  InteractorComponent *currentSpecInteractorComponent;

  QCursor oldCursor;

public:
  MouseNKeysNavigator(bool enableMouseNavigation = true)
      : currentSpecInteractorComponent(nullptr), nldc(nullptr),
        mouseNavigationEnabled(enableMouseNavigation) {}
  ~MouseNKeysNavigator() override {}
  bool eventFilter(QObject *, QEvent *) override;
  void clear() override;
  void viewChanged(tlp::View *view) override;

private:
  // member below are to manage meta node navigation
  tlp::NodeLinkDiagramComponent *nldc;
  std::vector<tlp::Graph *> graphHierarchy;
  std::vector<tlp::Camera> cameraHierarchy;
  std::vector<tlp::node> nodeHierarchy;
  std::vector<float> alphaHierarchy;
  bool mouseNavigationEnabled;
};
} // namespace tlp
#endif
///@endcond
