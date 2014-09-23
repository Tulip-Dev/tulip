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

#ifndef PIXELORIENTEDALGORITHMS_H_
#define PIXELORIENTEDALGORITHMS_H_

#include "UniformDeformationScreen.h"
#include "FishEyesScreen.h"
#include "SpiralLayout.h"
#include "HSIColorMapping.h"
#include "potypes.h"
#include "ColorFunction.h"
#include "DimensionBase.h"

#include <map>

namespace pocore {

class PixelOrientedMediator {

public :

  PixelOrientedMediator(LayoutFunction *layout = new SpiralLayout(),
                        ColorFunction *color = new HSIColorMapping());

  ~PixelOrientedMediator();

  void setLayoutFunction(LayoutFunction *layout) {
    this->layout = layout;
  }
  LayoutFunction *getLayoutFunction() {
    return layout;
  }

  void setColorFunction(ColorFunction *color) {
    this->color = color;
  }
  ColorFunction *getColorFunction() {
    return color;
  }

  void setImageSize(unsigned int width, unsigned int height) {
    imageSize[0] = width ;
    imageSize[1] = height;
  }
  unsigned int getImageWidth() const {
    return imageSize[0];
  }
  unsigned int getImageHeight() const {
    return imageSize[1];
  }

  void changeZoom(int i);

  void backupScreenFunctionsParameters();
  void setScreenFunctionsParameters(double zoom, double translationX, double translationY, double fishEyeRadius);
  void restoreScreenFunctionsParameters();

  unsigned int getCenterItem() const {
    return centerItem;
  }
  void setCenterItem(const unsigned int centerItem) {
    this->centerItem = centerItem;
  }

  void setLastMousePosition(const int x, const int y);
  void updateFishEyePosition(const int x, const int y, DimensionBase *data);
  void translateFishEye(const int x, const int y);
  Vec2i getFishEyeTranslationVector() const {
    return fishTranslation;
  }
  Vec2f getFishEyeCenter() const {
    return fishCenter;
  }

  RGBA getColorForPixelAtPos(Vec2i pos, DimensionBase *data, bool withFishEye = false);
  unsigned int getRankForPixelPos(Vec2i pos);
  Vec2i getPixelPosForRank(const unsigned int rank);

private :

  Vec2f screenToScene(const pocore::Vec2i &p);
  Vec2i sceneToScreen(const pocore::Vec2i &p);

  Vec2i imageSize;

  LayoutFunction *layout;
  ColorFunction * color;
  FishEyesScreen *trans1;
  UniformDeformationScreen *trans2;

  double zoomBak, translationXBak, translationYBak, fishEyeRadiusBak;

  unsigned int centerItem;

  double zoom;
  Vec2i lastMousePosition;
  Vec2i totalMove;
  Vec2i totalFishMove;
  Vec2i fishTranslation;
  Vec2f fishCenter;
};

}

#endif /* PIXELORIENTEDALGORITHMS_H_ */
