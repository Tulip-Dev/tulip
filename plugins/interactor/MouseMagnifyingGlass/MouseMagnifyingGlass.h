/**
 *
 * This file is part of Tulip (www.tulip-software.org)
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

#include "../../utils/StandardInteractorPriority.h"

class QGLFramebufferObject;

namespace tlp {

class MouseMagnifyingGlassInteractorComponent : public GLInteractorComponent {

public :

  MouseMagnifyingGlassInteractorComponent();
  MouseMagnifyingGlassInteractorComponent(const MouseMagnifyingGlassInteractorComponent &mouseMagnifyingGlassInteractorComponent);
  ~MouseMagnifyingGlassInteractorComponent();

  bool eventFilter(QObject *widget, QEvent *e);

  bool compute(GlMainWidget *) {
    return false;
  }

  bool draw(GlMainWidget *glWidget);

  void viewChanged(View *view);

private :

  void generateMagnifyingGlassTexture(const Coord &magnifyingGlassCenterScr);

  QGLFramebufferObject *fbo;
  QGLFramebufferObject *fbo2;
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
 *  <b>LICENCE</b>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 */
class MouseMagnifyingGlassInteractor : public GLInteractorComposite {

public :

  PLUGININFORMATION("MouseMagnifyingGlassInteractor", "Antoine Lambert", "19/06/2009", "Mouse Magnifying Glass Interactor Interactor", "1.0", "Visualization")

  MouseMagnifyingGlassInteractor(const tlp::PluginContext *);

  void construct();

  unsigned int priority() const {
    return StandardInteractorPriority::MagnifyingGlass;
  }
  QWidget *configurationWidget() const {
    return NULL;
  }

  bool isCompatible(const std::string &viewName) const;
};
}
#endif /* MOUSEMAGNIFYINGGLASS_H_ */
