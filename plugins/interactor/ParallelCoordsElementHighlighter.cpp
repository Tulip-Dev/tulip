/*
 * ParallelCoordsElementsSelector.cpp
 *
 *  Created on: 4 nov. 2008
 *      Author: antoine
 */

#include "ParallelCoordsGlEntitiesSelector.h"

using namespace std;

namespace tlp {

class TLP_QT_SCOPE ParallelCoordsElementHighlighter : public ParallelCoordsGlEntitiesSelector {
	public :
		ParallelCoordsElementHighlighter(Qt::MouseButton button = Qt::LeftButton,
									   Qt::KeyboardModifier modifier = Qt::NoModifier) : ParallelCoordsGlEntitiesSelector(button, modifier) {}
		~ParallelCoordsElementHighlighter(){}
		Interactor *clone() { return new ParallelCoordsElementHighlighter(mButton, kModifier); }
		void unselectAllEntitiesHandler(ParallelCoordinatesView *parallelView);
		void selectedEntitiesHandler(ParallelCoordinatesView *parallelView, int x, int y, bool addToSelection);
		void selectedEntitiesHandler(ParallelCoordinatesView *parallelView, int x, int y, int w, int h, bool addToSelection);
};

INTERACTORPLUGIN(ParallelCoordsElementHighlighter, "ParallelCoordsElementHighlighter", "Tulip Team", "17/10/2008", "Parallel Coordinates Element Highlighter", "1.0", 27);

void ParallelCoordsElementHighlighter::unselectAllEntitiesHandler(ParallelCoordinatesView *parallelView) {
	parallelView->resetHighlightedElements();
}

void ParallelCoordsElementHighlighter::selectedEntitiesHandler(ParallelCoordinatesView *parallelView, int x, int y, bool addToSelection) {
	parallelView->highlightDataUnderPointer(x, y, addToSelection);
}

void ParallelCoordsElementHighlighter::selectedEntitiesHandler(ParallelCoordinatesView *parallelView, int x, int y, int w, int h, bool addToSelection) {
	parallelView->highlightDataInRegion(x, y, w, h, addToSelection);
}

}
