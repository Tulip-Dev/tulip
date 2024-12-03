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

#ifndef MOUSEMAGNIFYINGGLASS_H_
#define MOUSEMAGNIFYINGGLASS_H_

#include <tulip/GLInteractor.h>
#include <tulip/GlMainWidget.h>
#include <tulip/StandardInteractorPriority.h>

class QOpenGLFramebufferObject;
class QLabel;

namespace tlp {

class MouseMagnifyingGlassInteractorComponent : public GLInteractorComponent {

public:
  MouseMagnifyingGlassInteractorComponent();
  MouseMagnifyingGlassInteractorComponent(
      const MouseMagnifyingGlassInteractorComponent &mouseMagnifyingGlassInteractorComponent);
  ~MouseMagnifyingGlassInteractorComponent() override;

  bool eventFilter(QObject *widget, QEvent *e) override;

  bool compute(GlMainWidget *) override {
    return false;
  }

  bool draw(GlMainWidget *glWidget) override;

  void viewChanged(View *view) override;

private:
  void generateMagnifyingGlassTexture(const Coord &magnifyingGlassCenterScr);

  QOpenGLFramebufferObject *fbo;
  QOpenGLFramebufferObject *fbo2;
  GlMainWidget *glWidget;
  Camera *camera;
  Coord boxCenter;
  bool drawInteractor;
  std::string textureName;
  float radius;
  float magnifyPower;
};

/*@{*/
/** \file
 *  \brief  Tulip Magnifying Glass Interactor

 * This interactor plugin provides a magnifying glass tool for all Tulip views.
 * It allows to zoom on a particular area of a visualization with the help of a lens
 * without having to modify the global zoom level.
 *
 *
 */
class MouseMagnifyingGlassInteractor : public GLInteractorComposite {

public:
  QLabel *doc;

  PLUGININFORMATION("MouseMagnifyingGlassInteractor", "Antoine Lambert", "19/06/2009",
                    "Mouse Magnifying Glass Interactor", "1.1", "Visualization")

  MouseMagnifyingGlassInteractor(const tlp::PluginContext *);

  ~MouseMagnifyingGlassInteractor();

  void construct() override;

  unsigned int priority() const override {
    return StandardInteractorPriority::MagnifyingGlass;
  }

  QLabel *configurationDocWidget() const override {
    return doc;
  }

  bool isCompatible(const std::string &viewName) const override;
};
} // namespace tlp
#endif /* MOUSEMAGNIFYINGGLASS_H_ */
