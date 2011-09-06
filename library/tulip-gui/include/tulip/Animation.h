/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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
#ifndef ANIMATION_H_
#define ANIMATION_H_

#include <QtCore/QTimeLine>

/**
 * @brief Interface for animation.
 *
 * An animation is defined by it's frame number it define the number of picture to create for this animation.
 * To use an animation follow these steps :
 * <code>
 * Animation *animation;
 * animation->setFrameCount(50);
 * for(unsigned int i = 0 ; i < 50 ; ++i){
 *  animation->frameChanged(i);
 * }
 * </code>
 */
class Animation {
public:
  Animation() {
  }
  virtual ~Animation() {
  }

  virtual int getFrameCount() const=0;
  virtual void setFrameCount(int frameCount)=0;

  virtual void frameChanged(int frame)=0;
};

#endif /* ANIMATION_H_ */
