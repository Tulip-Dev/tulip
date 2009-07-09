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

#ifndef PARALLELCOORDSGLENTITIESSELECTOR_H_
#define PARALLELCOORDSGLENTITIESSELECTOR_H_

#ifdef  _WIN32
// compilation pb workaround
#include <windows.h>
#endif
#include <QtGui/qcursor.h>
#include <QtGui/qevent.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tulip/MouseSelector.h>

#include "ParallelCoordinatesView.h"

namespace tlp {

class ParallelCoordsGlEntitiesSelector : public MouseSelector {
	public :
		ParallelCoordsGlEntitiesSelector(Qt::MouseButton button = Qt::LeftButton,
									     Qt::KeyboardModifier modifier = Qt::NoModifier) : MouseSelector(button, modifier){}
		~ParallelCoordsGlEntitiesSelector(){}
		bool eventFilter(QObject *, QEvent *);
		virtual void unselectAllEntitiesHandler(ParallelCoordinatesView *parallelView) = 0;
		virtual void selectedEntitiesHandler(ParallelCoordinatesView *parallelView, int x, int y, bool addToSelection) = 0;
		virtual void selectedEntitiesHandler(ParallelCoordinatesView *parallelView, int x, int y, int w, int h, bool addToSelection) = 0;
};

}

#endif /* PARALLELCOORDSGLENTITIESSELECTOR_H_ */
