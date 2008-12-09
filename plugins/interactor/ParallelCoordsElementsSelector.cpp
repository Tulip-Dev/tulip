/*
 * ParallelCoordsElementsSelector.cpp
 *
 *  Created on: 4 nov. 2008
 *      Author: antoine
 */

#include "ParallelCoordsGlEntitiesSelector.h"

using namespace std;

namespace tlp {

class TLP_QT_SCOPE ParallelCoordsElementsSelector : public ParallelCoordsGlEntitiesSelector {
	public :
		ParallelCoordsElementsSelector(Qt::MouseButton button = Qt::LeftButton,
									   Qt::KeyboardModifier modifier = Qt::NoModifier) : ParallelCoordsGlEntitiesSelector(button, modifier) {}
		~ParallelCoordsElementsSelector(){}
		Interactor *clone() { return new ParallelCoordsElementsSelector(mButton, kModifier); }
		void unselectAllEntitiesHandler(ParallelCoordinatesView *parallelView);
		void selectedEntitiesHandler(ParallelCoordinatesView *parallelView, int x, int y, bool addToSelection);
		void selectedEntitiesHandler(ParallelCoordinatesView *parallelView, int x, int y, int w, int h, bool addToSelection);
};

INTERACTORPLUGIN(ParallelCoordsElementsSelector, "ParallelCoordsElementsSelector", "Tulip Team", "16/10/2008", "Parallel Coordinates Elements Selector", "1.0", 25);

void ParallelCoordsElementsSelector::unselectAllEntitiesHandler(ParallelCoordinatesView *parallelView) {
	parallelView->resetSelection();
}

void ParallelCoordsElementsSelector::selectedEntitiesHandler(ParallelCoordinatesView *parallelView, int x, int y, bool addToSelection) {
	parallelView->setDataUnderPointerSelectFlag(x, y, addToSelection);
}

void ParallelCoordsElementsSelector::selectedEntitiesHandler(ParallelCoordinatesView *parallelView, int x, int y, int w, int h, bool addToSelection) {
	parallelView->setDataInRegionSelectFlag(x, y, w, h, addToSelection);
}

}
