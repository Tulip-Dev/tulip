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
#include <tulip/MouseSelector.h>

using namespace std;

namespace tlp {

class TLP_QT_SCOPE ParallelCoordsElementShowInfos : public Interactor {
	public :
		ParallelCoordsElementShowInfos(){}
		~ParallelCoordsElementShowInfos(){}
		bool eventFilter(QObject *, QEvent *);
		Interactor *clone() { return new ParallelCoordsElementShowInfos(); }
};

INTERACTORPLUGIN(ParallelCoordsElementShowInfos, "ParallelCoordsElementShowInfos", "Tulip Team", "15/10/2008", "Parallel Coordinates Element Show Infos", "1.0", 24);

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

