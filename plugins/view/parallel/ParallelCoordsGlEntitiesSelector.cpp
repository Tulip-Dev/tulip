#include "ParallelCoordsGlEntitiesSelector.h"

#include <tulip/GlMainWidget.h>

using namespace std;

namespace tlp {

bool ParallelCoordsGlEntitiesSelector::eventFilter(QObject *widget, QEvent *e) {

	ParallelCoordinatesView *parallelView = (ParallelCoordinatesView *) view;
	GlMainWidget *glMainWidget = (GlMainWidget *) widget;


	if (e->type() == QEvent::MouseButtonPress) {

		QMouseEvent * qMouseEv = (QMouseEvent *) e;

		if (qMouseEv->buttons()== Qt::LeftButton) {

			if (!started) {
				x = qMouseEv->x();
				y = qMouseEv->y();
				w = 0;
				h = 0;
				started = true;
				graph = glMainWidget->getScene()->getGlGraphComposite()->getInputData()->getGraph();
			}
			return true;
		}

	}

	if (e->type() == QEvent::MouseMove && ((((QMouseEvent *) e)->buttons() & Qt::LeftButton))) {

		QMouseEvent * qMouseEv = (QMouseEvent *) e;

		if (started) {
			if ((qMouseEv->x() > 0) && (qMouseEv->x() < glMainWidget->width()))
			w = qMouseEv->x() - x;
			if ((qMouseEv->y() > 0) && (qMouseEv->y() < glMainWidget->height()))
			h = qMouseEv->y() - y;
			parallelView->refresh();
			return true;
		}
	}

	if (e->type() == QEvent::MouseButtonRelease) {

		QMouseEvent * qMouseEv = (QMouseEvent *) e;
		if (started) {
			Observable::holdObservers();
			bool boolVal = true; // add to selection

			if (qMouseEv->modifiers() != Qt::ShiftModifier) {
				if (qMouseEv->modifiers() !=
#if defined(__APPLE__)
						Qt::AltModifier
#else
						Qt::ControlModifier
#endif
				) {

					unselectAllEntitiesHandler(parallelView);

				} else
					boolVal = false; // remove from selection
				}

			if ((w == 0) && (h == 0)) {

				selectedEntitiesHandler(parallelView, x, y, boolVal);

			} else {

				if (w < 0) {
					w *= -1;
					x -= w;
				}

				if (h < 0) {
					h *= -1;
					y -= h;
				}

				selectedEntitiesHandler(parallelView, x, y, w, h, boolVal);
			}

			started = false;
			Observable::unholdObservers();
			return true;
		}
	}
	return false;
}

}
