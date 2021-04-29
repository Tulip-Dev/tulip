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

#include "PixelOrientedMediator.h"

namespace pocore {

PixelOrientedMediator::PixelOrientedMediator(LayoutFunction *layout, ColorFunction *color)
    : layout(layout), color(color), trans1(new FishEyesScreen()),
      trans2(new UniformDeformationScreen()), centerItem(uint(-1)) {
  zoom = 1.0;
  totalMove[0] = 0;
  totalMove[1] = 0;
  totalFishMove[0] = 0;
  totalFishMove[1] = 0;
  fishTranslation[0] = 0;
  fishTranslation[1] = 0;
}

PixelOrientedMediator::~PixelOrientedMediator() {}

Vec2f PixelOrientedMediator::screenToScene(const pocore::Vec2i &p) {
  Vec2f q;
  q[0] = double(p[0]) - double(imageSize[0] / 2.);
  q[1] = double(p[1]) - double(imageSize[1] / 2.);
  return trans2->unproject(trans1->unproject(q));
}

Vec2i PixelOrientedMediator::sceneToScreen(const pocore::Vec2i &p) {
  Vec2f q;
  q[0] = double(p[0]);
  q[1] = double(p[1]);
  Vec2f res1 = trans1->project(trans2->project(q));
  res1[0] += imageSize[0] / 2.0;
  res1[1] += imageSize[1] / 2.0;
  Vec2i result;
  result[0] = int(rint(res1[0]));
  result[1] = int(rint(res1[1]));
  return result;
}

void PixelOrientedMediator::setScreenFunctionsParameters(double zoom, double translationX,
                                                         double translationY,
                                                         double fishEyeRadius) {
  trans2->setZoom(zoom);
  trans2->setTranslation(translationX, translationY);
  trans1->setRadius(fishEyeRadius);
}

unsigned int PixelOrientedMediator::getRankForPixelPos(Vec2i pos) {
  Vec2f fPos = screenToScene(pos);
  Vec2i fPosI;
  fPosI[0] = int(rint(fPos[0]));
  fPosI[1] = int(rint(fPos[1]));
  return layout->unproject(fPosI);
}

RGBA PixelOrientedMediator::getColorForPixelAtPos(Vec2i pos, DimensionBase *data,
                                                  bool withFishEye) {
  RGBA backgroundColor;
  backgroundColor.fill(255);

  Vec2f fPos = screenToScene(pos);
  Vec2i fPosI;
  fPosI[0] = int(rint(fPos[0]));
  fPosI[1] = int(rint(fPos[1]));

  unsigned int rank = layout->unproject(fPosI);

  if (rank < data->numberOfItems()) {
    RGBA curColor = color->getColor(data->getItemValueAtRank(rank), data->getItemIdAtRank(rank));

    if (withFishEye) {
      Vec2f fPosIf;
      fPosIf[0] = fPosI[0];
      fPosIf[1] = fPosI[1];
      Vec2f delta = fPos - fPosIf;
      double dist = delta.norm();
      Vec2f tmp;
      tmp[0] = pos[0] - double(imageSize[0]) / 2. - fishTranslation[0];
      tmp[1] = pos[1] - double(imageSize[0]) / 2. - fishTranslation[1];

      if ((fishCenter - tmp).norm() < 70.) {
        for (unsigned int i = 0; i < 3; ++i) {
          double a = -double(curColor[i]);
          double b = curColor[i];
          curColor[i] = uchar(a * dist * dist + b);
        }
      }
    }

    return curColor;

  } else {
    return backgroundColor;
  }
}

Vec2i PixelOrientedMediator::getPixelPosForRank(const unsigned int rank) {
  return sceneToScreen(layout->project(rank));
}
} // namespace pocore
