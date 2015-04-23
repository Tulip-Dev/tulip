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
#include "tulip/Animation.h"

using namespace tlp;

Animation::Animation(int frameCount, QObject* parent): QPropertyAnimation(parent), _currentFrame(0), _frameCount(0) {
  setPropertyName("frame");
  setTargetObject(this);
  setStartValue(0);
  setFrameCount(frameCount);
}

Animation::~Animation() {
}

int Animation::currentFrame() const {
  return _currentFrame;
}

int Animation::frameCount() const {
  return _frameCount;
}

void Animation::setFrameCount(int fc) {
  _frameCount = fc;
  setEndValue(frameCount()-1);
}
