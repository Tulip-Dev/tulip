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
#include "tulip/GraphicsWidgetBase.h"

namespace tlp {

GraphicsWidgetBase::GraphicsWidgetBase() {
	setFlag(QGraphicsItem::ItemIsMovable, true);
	setFlag(QGraphicsItem::ItemIsSelectable, true);
	setFlag(QGraphicsItem::ItemIsFocusable, true);
	setCacheMode(QGraphicsItem::ItemCoordinateCache);
	setAcceptHoverEvents(true);
#if(QT_VERSION >= QT_VERSION_CHECK(4,5,0))
	setOpacity(0.8);
#endif
}

void GraphicsWidgetBase::wheelEvent(QGraphicsSceneWheelEvent * event) {
	int numDegrees = event->delta() / 8;
	int numSteps = numDegrees / 15;
	if (event->modifiers() != Qt::ControlModifier) {
		double sx, sy;
		double scalefactor = 1.1;
		if (numSteps > 0) {
			sx = sy = scalefactor;
		} else {
			sx = sy = 1. / scalefactor;
		}
		scale(sx, sy);
	}
 #if(QT_VERSION >= QT_VERSION_CHECK(4,5,0))
	else {
		double cur_opacity = opacity();
		cur_opacity += numSteps * 0.01;
		if (cur_opacity < 0.2) {
			cur_opacity = 0.2;
		} else if (cur_opacity > 1.0) {
			cur_opacity = 1.0;
		}
		setOpacity(cur_opacity);
	}
#endif
 }

}
