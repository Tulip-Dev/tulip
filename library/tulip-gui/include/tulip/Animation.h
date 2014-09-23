/*
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
///@cond DOXYGEN_HIDDEN

#ifndef ANIMATION_H_
#define ANIMATION_H_

#include <QPropertyAnimation>
#include <tulip/tulipconf.h>

namespace tlp {
class TLP_QT_SCOPE Animation: public QPropertyAnimation {
  Q_OBJECT
  Q_PROPERTY(int frame READ currentFrame WRITE frameChanged)
  Q_PROPERTY(int frameCount READ frameCount WRITE setFrameCount)

  int _currentFrame;
  int _frameCount;

public:
  Animation(int frameCount=0,QObject* parent=0);
  virtual ~Animation();

  int currentFrame() const;
  int frameCount() const;

public slots:
  virtual void frameChanged(int)=0;
  virtual void setFrameCount(int);
};
}

#endif /* ANIMATION_H_ */
///@endcond
