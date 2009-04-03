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

#include "ParallelTools.h"
#include "ParallelCoordinatesView.h"

#ifdef  _WIN32
// compilation pb workaround
#include <windows.h>
#endif
#include <QtGui/qcursor.h>
#include <QtGui/qevent.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string>

#include <tulip/tulipconf.h>
#include <tulip/Interactor.h>
#include <tulip/MouseSelector.h>

using namespace std;

namespace tlp {

class ParallelCoordsElementShowInfos : public InteractorComponent {
	public :
		ParallelCoordsElementShowInfos(){}
		~ParallelCoordsElementShowInfos(){}
		bool eventFilter(QObject *, QEvent *);
		InteractorComponent *clone() { return new ParallelCoordsElementShowInfos(); }
};

bool ParallelCoordsElementShowInfos::eventFilter(QObject *widget, QEvent *e) {

	if (e->type() == QEvent::MouseButtonPress && ((QMouseEvent *) e)->buttons()==Qt::LeftButton) {

		QMouseEvent *me = (QMouseEvent *) e;
		ParallelCoordinatesView *parallelView = (ParallelCoordinatesView *) view;

		parallelView->showDataUnderPointerProperties(me->x(), me->y());

		return true;
	} else {
		return false;
	}
}

}

