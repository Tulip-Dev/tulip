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


using namespace std;

namespace tlp {

class TLP_QT_SCOPE ParallelCoordsElementDeleter : public Interactor {
public :
	ParallelCoordsElementDeleter() {}
	~ParallelCoordsElementDeleter() {}
	bool eventFilter(QObject *, QEvent *);
	Interactor *clone() { return new ParallelCoordsElementDeleter(); }
};

INTERACTORPLUGIN(ParallelCoordsElementDeleter, "ParallelCoordsElementDeleter", "Tulip Team", "16/10/2008", "Parallel Coordinates Element Deleter", "1.0", 26);

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
