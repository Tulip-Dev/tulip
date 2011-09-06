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

#ifndef ABSTRACTANIMATION_H_
#define ABSTRACTANIMATION_H_
#include "Animation.h"
/**
 * @brief Abstract class to handle animation frame number for Animation classes.
 */
class AbstractAnimation: public Animation {
public:
  AbstractAnimation(int frameCount = 0);
  virtual ~AbstractAnimation();

  virtual void setFrameCount(int i);

  virtual int getFrameCount() const {
    return frameCount;
  }

protected:
  unsigned int frameCount;
};

#endif /* ABSTRACTANIMATION_H_ */

