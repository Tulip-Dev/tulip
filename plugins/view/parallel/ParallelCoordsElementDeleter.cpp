//-*-c++-*-
/*
 Author: Antoine Lambert

 Email : antoine.lambert@labri.fr

 Last modification : 12/08

 This program is free software; you can redistribute it and/or modify  *
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
*/

#include "ParallelCoordinatesView.h"
#include "ParallelCoordsElementDeleter.h"

using namespace std;

namespace tlp {

bool ParallelCoordsElementDeleter::eventFilter(QObject *widget, QEvent *e) {

	if (e->type() == QEvent::MouseButtonPress && ((QMouseEvent *) e)->buttons()==Qt::LeftButton) {

		QMouseEvent *me = (QMouseEvent *) e;
		ParallelCoordinatesView *parallelView = (ParallelCoordinatesView *) view;

		Observable::holdObservers();
		parallelView->deleteDataUnderPointer(me->x(), me->y());
		Observable::unholdObservers();
		return true;

	} else {
		return false;
	}
}

}
