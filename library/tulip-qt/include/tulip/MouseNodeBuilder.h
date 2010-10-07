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
#ifndef MOUSEADDNODE_H
#define MOUSEADDNODE_H

#include <tulip/InteractorComponent.h>
#include <QtGui/qevent.h>

namespace tlp {

/** \addtogroup Mouse_interactor */
/*@{*/
class TLP_QT_SCOPE MouseNodeBuilder:public InteractorComponent
{

public:
  MouseNodeBuilder(QEvent::Type eventType = QEvent::MouseButtonPress):_eventType(eventType) {}
  ~MouseNodeBuilder() {}
  bool eventFilter(QObject *, QEvent *);
  InteractorComponent *clone() { return new MouseNodeBuilder(_eventType); }
private:
  QEvent::Type _eventType;
};
/*@}*/

}
#endif
