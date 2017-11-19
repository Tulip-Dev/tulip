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

#ifndef FISHEYEINTERACTOR_H_
#define FISHEYEINTERACTOR_H_

#include <tulip/GLInteractor.h>
#include <tulip/Coord.h>

#include "../../utils/StandardInteractorPriority.h"

namespace tlp {

class FishEyeConfigWidget;
class GlShaderProgram;

class FishEyeInteractorComponent : public GLInteractorComponent {

public:
  FishEyeInteractorComponent(FishEyeConfigWidget *configWidget);
  FishEyeInteractorComponent(const FishEyeInteractorComponent &fisheyeInteractorComponent);

  bool eventFilter(QObject *widget, QEvent *e) override;

  bool compute(GlMainWidget *) override {
    return false;
  }

  void viewChanged(View *view) override;

  bool draw(GlMainWidget *glMainWidget) override;

private:
  FishEyeConfigWidget *configWidget;
  Coord fisheyeCenter;
  static GlShaderProgram *fisheyeShader;
  bool activateFishEye;
};

/*@{*/
/** \file
 *  \brief  Tulip Fisheye Interactor

 * This interactor plugin allow to perform a fisheye distortion effect on Tulip views.
 * It allows to gain focus on a certain area of a visualization by magnifying it without losing
 * the context.
 *
 * Your graphic card must support shader programs otherwise the plugin won't work.
 *
 *
 */
class FishEyeInteractor : public GLInteractorComposite {

public:
  PLUGININFORMATION("FishEyeInteractor", "Antoine Lambert", "29/05/2009", "FishEye Interactor",
                    "1.0", "Visualization")

  FishEyeInteractor(const PluginContext *);
  ~FishEyeInteractor() override;

  void construct() override;

  void uninstall() override;

  QWidget *configurationWidget() const override;

  unsigned int priority() const override {
    return StandardInteractorPriority::FishEye;
  }

  bool isCompatible(const std::string &viewName) const override;

private:
  FishEyeConfigWidget *fisheyeConfigWidget;
};
}

#endif /* FISHEYEINTERACTOR_H_ */
