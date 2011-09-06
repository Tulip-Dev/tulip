#include "tulip/Animation.h"

using namespace tlp;

Animation::Animation(int frameCount, QObject* parent): QPropertyAnimation(parent), _currentFrame(0), _frameCount(frameCount) {
  setPropertyName("frame");
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
}
