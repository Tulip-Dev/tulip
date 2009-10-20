//-*-c++-*-

/**
 Author: Antoine Lambert
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
*/

#ifndef Tulip_TABWIDGETHIDABLEMENUGRAPHICSPROXY_H_
#define Tulip_TABWIDGETHIDABLEMENUGRAPHICSPROXY_H_

#include <QtGui/QTabWidget>

#include "tulip/GraphicsWidgetBase.h"

namespace tlp {

class TLP_QT_SCOPE TabWidgetHidableMenuGraphicsProxy : public GraphicsWidgetBase {

	Q_OBJECT

public :

	TabWidgetHidableMenuGraphicsProxy(const unsigned int offsetWhenHiding);

	void addTab(QWidget *widget, const QString &tabName);

	void setOffsetWhenHiding(const unsigned int offsetWhenHiding) {this->offsetWhenHiding = offsetWhenHiding;}

	void hideTabWidget();

protected :

	void wheelEvent(QGraphicsSceneWheelEvent * event);
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event);

private slots :

	void animateTranslationStep(int animStep);

private :

	QTabWidget *tabWidget;
	bool hidden;
	double xStart, xEnd;
	unsigned int offsetWhenHiding;

};

}

#endif
