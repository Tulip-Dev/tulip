//-*-c++-*-

/**
 Author: Antoine Lambert
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
*/
#ifndef Tulip_GRAPHICSWIDGETBASE_H_
#define Tulip_GRAPHICSWIDGETBASE_H_

#include <QtGui/QGraphicsProxyWidget>
#include <QtGui/QGraphicsSceneWheelEvent>

#include <tulip/tulipconf.h>

namespace tlp {

class TLP_QT_SCOPE GraphicsWidgetBase : public QGraphicsProxyWidget {

public :

	GraphicsWidgetBase();

protected :

	void wheelEvent(QGraphicsSceneWheelEvent * event);
};

}

#endif /* GRAPHICSWIDGETBASE_H_ */
