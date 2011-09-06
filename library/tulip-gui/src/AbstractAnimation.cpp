#include "tulip/AbstractAnimation.h"

AbstractAnimation::AbstractAnimation(int frameCount) :
  frameCount(frameCount) {
}

AbstractAnimation::~AbstractAnimation() {
}

void AbstractAnimation::setFrameCount(int i) {
  frameCount = i;
}
