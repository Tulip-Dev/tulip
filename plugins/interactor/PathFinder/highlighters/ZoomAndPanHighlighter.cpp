#include "ZoomAndPanHighlighter.h"

using namespace std;
using namespace tlp;

#include <tulip/Graph.h>
#include <tulip/GlMainView.h>
#include <tulip/GlGraphInputData.h>
#include <tulip/LayoutProperty.h>
#include <tulip/Coord.h>
#include <tulip/BoundingBox.h>
#include <tulip/DrawingTools.h>
#include <tulip/GlGraphComposite.h>
#include <tulip/GlMainWidget.h>
#include <tulip/QtGlSceneZoomAndPanAnimator.h>

void ZoomAndPanHighlighter::highlight(const PathFinder*, GlMainWidget *glMainWidget, BooleanProperty *selection, node, node) {
  GlGraphInputData *inputData(getInputData(glMainWidget));
  Graph *graph = glMainWidget->getScene()->getGlGraphComposite()->getGraph();
	LayoutProperty *layout = inputData->getElementLayout();
  BoundingBox bbox(computeBoundingBox(graph, layout, inputData->getElementSize(), inputData->getElementRotation(), selection));

  QtGlSceneZoomAndPanAnimator animator(glMainWidget, bbox);
	animator.animateZoomAndPan();
}

void ZoomAndPanHighlighter::draw(GlMainWidget*) {
}

PathHighlighter *ZoomAndPanHighlighter::clone() {
	return new ZoomAndPanHighlighter;
}

bool ZoomAndPanHighlighter::isConfigurable() {
	return false;
}

QWidget *ZoomAndPanHighlighter::getConfigurationWidget() {
	return 0;
}
