/*
 * ParallelCoordsGlEntitiesSelector.h
 *
 *  Created on: 4 nov. 2008
 *      Author: antoine
 */

#ifndef PARALLELCOORDSGLENTITIESSELECTOR_H_
#define PARALLELCOORDSGLENTITIESSELECTOR_H_

#include "../view/parallel/ParallelTools.h"
#include "../view/parallel/ParallelCoordinatesView.h"

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
#include <tulip/GWInteractor.h>
#include <tulip/BooleanProperty.h>
#include <tulip/MouseSelector.h>

namespace tlp {

class TLP_QT_SCOPE ParallelCoordsGlEntitiesSelector : public MouseSelector {
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
