#include "ParallelCoordinatesView.h"
#include "AxisConfigDialogs.h"
#include "NominalParallelAxis.h"

#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QToolTip>
#include <QtGui/QImageWriter>
#include <QtCore/QThread>
#include <QtCore/QDir>

#include <tulip/InteractorManager.h>
#include <tulip/GWOverviewWidget.h>
#include <tulip/GlTools.h>
#include <tulip/GlLabel.h>
#include <tulip/GlProgressBar.h>
#include <tulip/GlMainWidget.h>
#include <tulip/Interactor.h>


using namespace std;

namespace tlp {

class ParallelDrawingUpdateThread : public QThread {

public :

	ParallelDrawingUpdateThread(ParallelCoordinatesDrawing *parallelDrawing) : parallelDrawing(parallelDrawing) {
		parallelDrawing->resetNbDataProcessed();
	}

	void run() {
		parallelDrawing->update();
	}

private :

	ParallelCoordinatesDrawing *parallelDrawing;
};

VIEWPLUGIN(ParallelCoordinatesView, "Parallel Coordinates view", "Tulip Team", "16/04/2008", "Parallel Coordinates View", "1.0");


ParallelCoordinatesView::ParallelCoordinatesView() :
	GlMainView(), graphProxy(NULL), parallelCoordsDrawing(NULL) , firstSet(true),
	lastNbSelectedProperties(0), center(false), lastViewWindowWidth(0), lastViewWindowHeight(0) {}

ParallelCoordinatesView::~ParallelCoordinatesView() {
	cleanup();
}

void ParallelCoordinatesView::cleanup() {
	axisPointsGraph->removeGraphObserver(glGraphComposite);
	mainLayer->getComposite()->reset(true);
	delete mainLayer;
	delete axisSelectionLayer;
	delete axisPointsGraph;
	if (graphProxy != NULL) {
		graphProxy->removeGraphObserver(parallelCoordsDrawing);
		delete graphProxy;
		graphProxy = NULL;
	}
}

QWidget *ParallelCoordinatesView::construct(QWidget *parent) {
	QWidget *widget=GlMainView::construct(parent);
	initGlWidget();
	buildMenuEntries();

	dataConfigWidget = new ParallelCoordsDataConfigWidget();
	connect(dataConfigWidget->applyButton, SIGNAL(clicked()), this, SLOT(setupAndDrawView()));
	drawConfigWidget = new ParallelCoordsDrawConfigWidget();
	connect(drawConfigWidget->applyButton, SIGNAL(clicked()), this, SLOT(setupAndDrawView()));

	return widget;
}

void ParallelCoordinatesView::initGlWidget() {
	mainLayer = new GlLayer("Main");

	axisPointsGraph = tlp::newGraph();
	glGraphComposite =new GlGraphComposite(axisPointsGraph);
	mainLayer->addGlEntity(glGraphComposite, "graph");
	mainWidget->getScene()->addLayer(mainLayer);
	axisSelectionLayer = new GlLayer("Axis selection layer");
	mainWidget->getScene()->addGlGraphCompositeInfo(mainLayer, glGraphComposite);
	GlGraphRenderingParameters param = mainWidget->getScene ()->getGlGraphComposite ()->getRenderingParameters ();
	param.setAntialiasing (true);
	param.setNodesStencil(2);
	param.setSelectedNodesStencil(1);
	param.setDisplayEdges(false);
	param.setDisplayNodes(true);
	param.setViewNodeLabel(false);
	mainWidget->getScene()->getGlGraphComposite ()->setRenderingParameters (param);
	mainWidget->setMouseTracking(true);
}

void ParallelCoordinatesView::toggleGraphView(const bool displayGraph) {
	GlGraphRenderingParameters param = mainWidget->getScene ()->getGlGraphComposite ()->getRenderingParameters ();
	param.setDisplayNodes(displayGraph);
	mainWidget->getScene()->getGlGraphComposite ()->setRenderingParameters (param);
}

void ParallelCoordinatesView::buildMenuEntries() {
	viewSetupMenu = new QMenu(tr("View Setup"));
	viewSetupMenu->addAction(tr("Center View"));
	viewSetupMenu->addSeparator();
	viewSetupMenu->addAction(tr("Layout Type"))->setEnabled(false);
	classicLayout = viewSetupMenu->addAction(tr("Classic Layout"));
	classicLayout->setCheckable(true);
	classicLayout->setChecked(true);
	circularLayout = viewSetupMenu->addAction(tr("Circular Layout"));
	circularLayout->setCheckable(true);
	circularLayout->setChecked(false);
	viewSetupMenu->addSeparator();
	viewSetupMenu->addAction(tr("Lines Type"))->setEnabled(false);
	straightLinesType = viewSetupMenu->addAction(tr("Straight"));
	straightLinesType->setCheckable(true);
	straightLinesType->setChecked(true);
	splineLinesType = viewSetupMenu->addAction(tr("Spline"));
	splineLinesType->setCheckable(true);
	splineLinesType->setChecked(false);
	optionsMenu = new QMenu(tr("Options"));
	showToolTips = optionsMenu->addAction(tr("Tooltips"));
	showToolTips->setCheckable(true);
	showToolTips->setChecked(false);
}

list<pair<QWidget *,string> > ParallelCoordinatesView::getConfigurationWidget() {
	list<pair<QWidget *,string> > widgetList;
	widgetList.push_back(pair<QWidget*,string>(dataConfigWidget,"Data Configuration"));
	widgetList.push_back(pair<QWidget*,string>(drawConfigWidget,"Draw Configuration"));
	return widgetList;
}

void ParallelCoordinatesView::setData(Graph *graph, DataSet dataSet) {
	overviewWidget->setObservedView(NULL,NULL);

	vector<string> selectedPropertiesBak;

	bool sameGraphRoot = false;
	if (graphProxy != NULL && (graph->getRoot() == graphProxy->getRoot())) {
		sameGraphRoot = true;
		selectedPropertiesBak = graphProxy->getSelectedProperties();
	}

	if (parallelCoordsDrawing != NULL && graphProxy->getGraph() != graph) {
		mainLayer->deleteGlEntity(parallelCoordsDrawing);
		delete parallelCoordsDrawing;
		parallelCoordsDrawing = NULL;
	}

	if (graphProxy != NULL && graphProxy->getGraph() != graph) {
		graphProxy->removeGraphObserver(parallelCoordsDrawing);
		delete graphProxy;
		graphProxy = NULL;
	}

	if (graphProxy == NULL) {
		graphProxy = new ParallelCoordinatesGraphProxy(graph);
	}

	if (sameGraphRoot) {
		graphProxy->setSelectedProperties(selectedPropertiesBak);
	}

	if (dataSet.exist("selectedProperties")) {
		vector<string> selectedProperties;
		DataSet items;
		dataSet.get("selectedProperties", items);
		int i=0;
		stringstream ss;
		ss<<i;
		while (items.exist(ss.str())) {
			string item;
			items.get(ss.str(), item);
			selectedProperties.push_back(item);
			ss.str("");
			++i;
			ss << i;
		}
		graphProxy->setSelectedProperties(selectedProperties);
	}

	dataConfigWidget->setGraphProxy(graphProxy);
	dataConfigWidget->updateSelectedProperties();

	if (parallelCoordsDrawing == NULL) {
		parallelCoordsDrawing = new ParallelCoordinatesDrawing(graphProxy, axisPointsGraph);
		graphProxy->addGraphObserver(parallelCoordsDrawing);
		mainLayer->addGlEntity(parallelCoordsDrawing, "Parallel Coordinates");
		overviewWidget->setObservedView(mainWidget, parallelCoordsDrawing);
	}


	unsigned int axisHeight = DEFAULT_AXIS_HEIGHT;
	unsigned int spaceBetweenAxis = DEFAULT_AXIS_HEIGHT / 2;
	unsigned int linesColorAlphaValue = DEFAULT_LINES_COLOR_ALPHA_VALUE;


	if (dataSet.exist("dataLocation")) {
		int dataLocation;
		dataSet.get("dataLocation", dataLocation);
		dataConfigWidget->setDataLocation((ElementType) dataLocation);
	}

	if (dataSet.exist("backgroundColor")) {
		Color backgroundColor;
		dataSet.get("backgroundColor", backgroundColor);
		drawConfigWidget->setBackgroundColor(backgroundColor);
	}

	if (dataSet.exist("axisPointMinSize")) {
		unsigned int axisPointMinSize;
		dataSet.get("axisPointMinSize", axisPointMinSize);
		drawConfigWidget->setAxisPointMinSize(axisPointMinSize);
	}

	if (dataSet.exist("axisPointMaxSize")) {
		unsigned int axisPointMaxSize;
		dataSet.get("axisPointMaxSize", axisPointMaxSize);
		drawConfigWidget->setAxisPointMaxSize(axisPointMaxSize);
	}

	if (dataSet.exist("drawPointsOnAxis")) {
		bool drawPointsOnAxis;
		dataSet.get("drawPointsOnAxis", drawPointsOnAxis);
		drawConfigWidget->setDrawPointOnAxis(drawPointsOnAxis);
	}

	if (dataSet.exist("linesTextureFileName")) {
		string linesTextureFileName;
		dataSet.get("linesTextureFileName", linesTextureFileName);
		drawConfigWidget->setLinesTextureFilename(linesTextureFileName);
	}

	if (dataSet.exist("axisHeight")) {
		dataSet.get("axisHeight", axisHeight);
	}

	if (dataSet.exist("spaceBetweenAxis")) {
		dataSet.get("spaceBetweenAxis", spaceBetweenAxis);
	}

	if (dataSet.exist("linesColorAlphaValue")) {
		dataSet.get("linesColorAlphaValue", linesColorAlphaValue);
	}

	if (dataSet.exist("non highlighted alpha value")) {
		unsigned int nonHighlightedAlphaValue;
		dataSet.get("non highlighted alpha value", nonHighlightedAlphaValue);
		drawConfigWidget->setUnhighlightedEltsColorsAlphaValue(nonHighlightedAlphaValue);
	}

	if (dataSet.exist("linesType")) {
		int linesType;
		dataSet.get("linesType", linesType);
		if (linesType == ParallelCoordinatesDrawing::STRAIGHT) {
			straightLinesType->setChecked(true);
			splineLinesType->setChecked(false);
		} else {
			straightLinesType->setChecked(false);
			splineLinesType->setChecked(true);
		}
	}

	if (dataSet.exist("layoutType")) {
		int layoutType;
		dataSet.get("layoutType", layoutType);
		if (layoutType == ParallelCoordinatesDrawing::PARALLEL) {
			classicLayout->setChecked(true);
			circularLayout->setChecked(false);
		} else {
			classicLayout->setChecked(false);
			circularLayout->setChecked(true);
		}
	}

	drawConfigWidget->setAxisHeight(axisHeight);
	drawConfigWidget->setSpaceBetweenAxis(spaceBetweenAxis);
	drawConfigWidget->setLinesColorAlphaValue(linesColorAlphaValue);

	dataSet.get("lastViewWindowWidth", lastViewWindowWidth);
	dataSet.get("lastViewWindowHeight", lastViewWindowHeight);

	setupAndDrawView();

}

void ParallelCoordinatesView::getData(Graph **graph, DataSet *dataSet) {

	vector<string> selectedProperties=graphProxy->getSelectedProperties();
	DataSet selectedPropertiesData;
	int i=0;
	for (vector<string>::iterator it=selectedProperties.begin(); it
	!=selectedProperties.end(); ++it) {
		std::stringstream s;
		s << i;
		selectedPropertiesData.set<string>(s.str(), *it);
		i++;
	}
	dataSet->set("selectedProperties", selectedPropertiesData);
	dataSet->set("dataLocation", (int) graphProxy->getDataLocation());
	dataSet->set("backgroundColor", drawConfigWidget->getBackgroundColor());
	dataSet->set("axisHeight", drawConfigWidget->getAxisHeight());
	dataSet->set("spaceBetweenAxis", drawConfigWidget->getSpaceBetweenAxis());
	unsigned int axisPointMinSize = (unsigned int) drawConfigWidget->getAxisPointMinSize().getW();
	unsigned int axisPointMaxSize = (unsigned int) drawConfigWidget->getAxisPointMaxSize().getW();
	dataSet->set("axisPointMinSize", axisPointMinSize);
	dataSet->set("axisPointMaxSize", axisPointMaxSize);
	dataSet->set("drawPointsOnAxis", drawConfigWidget->drawPointOnAxis());
	dataSet->set("linesTextureFileName", drawConfigWidget->getLinesTextureFilename());
	dataSet->set("linesColorAlphaValue", drawConfigWidget->getLinesColorAlphaValue());
	dataSet->set("non highlighted alpha value", drawConfigWidget->getUnhighlightedEltsColorsAlphaValue());
	dataSet->set("layoutType", (int) getLayoutType());
	dataSet->set("linesType", (int) getLinesType());
	dataSet->set("lastViewWindowWidth",  mainWidget->width());
	dataSet->set("lastViewWindowHeight",  mainWidget->height());

	*graph = graphProxy->getGraph();
}

Graph* ParallelCoordinatesView::getGraph() {

	return graphProxy->getGraph();

}

void ParallelCoordinatesView::setGraph(Graph *graph) {
	setData(graph, DataSet());
}

void ParallelCoordinatesView::updateWithoutProgressBar() {
	parallelCoordsDrawing->resetNbDataProcessed();
	parallelCoordsDrawing->update();
}

void ParallelCoordinatesView::updateWithProgressBar() {

	// removal of the parallel coordinates drawing from the main layer
	if (mainLayer->findGlEntity("Parallel Coordinates") != NULL) {
		mainLayer->deleteGlEntity(parallelCoordsDrawing);
		mainLayer->deleteGlEntity(glGraphComposite);
		mainWidget->getScene()->addGlGraphCompositeInfo(NULL, NULL);
		overviewWidget->setObservedView(NULL, NULL);
	}

	// creation of a thread to compute the drawing update
	ParallelDrawingUpdateThread drawingUpdateThread(parallelCoordsDrawing);

	// get the total umber of data to process
	unsigned int nbData = graphProxy->getDataCount();

	// backup camera parameters
	double sceneRadiusBak = mainWidget->getScene()->getCamera()->getSceneRadius();
	double zoomFactorBak = mainWidget->getScene()->getCamera()->getZoomFactor();
	Coord eyesBak = mainWidget->getScene()->getCamera()->getEyes();
	Coord centerBak = mainWidget->getScene()->getCamera()->getCenter();
	Coord upBak = mainWidget->getScene()->getCamera()->getUp();

	// GlProgressBar creation and setup
	GlProgressBar *progressBar = new GlProgressBar(Coord(0, 0, 0), 600, 100, PROGRESS_BAR_COLOR);
	progressBar->setComment("Updating parallel coordinates view, please wait ...");
	progressBar->progress(0, nbData);
	// add the progress bar to main layer, center scene on it and draw
	mainLayer->addGlEntity(progressBar, "progress bar");
	centerView();
	GlMainView::draw();

	// start the drawing update thread
	drawingUpdateThread.start();

	// get the drawing update progress running in background and update progress bar
	unsigned int nbDataProcessed = parallelCoordsDrawing->getNbDataProcessed();
	while (nbDataProcessed < nbData) {
		progressBar->progress(nbDataProcessed, nbData);
		GlMainView::draw();
		nbDataProcessed = parallelCoordsDrawing->getNbDataProcessed();
	}

	progressBar->progress(nbData, nbData);
	GlMainView::draw();

	// join the drawing update thread to main process
	drawingUpdateThread.wait();

	// remove progress bar from main layer and delete it
	mainLayer->deleteGlEntity(progressBar);
	delete progressBar;

	// add updated drawing to main layer
	mainLayer->addGlEntity(parallelCoordsDrawing, "Parallel Coordinates");
	mainLayer->addGlEntity(glGraphComposite, "graph");
	mainWidget->getScene()->addGlGraphCompositeInfo(mainLayer, glGraphComposite);
	overviewWidget->setObservedView(mainWidget, parallelCoordsDrawing);

	// restore original camera parameters
	mainWidget->getScene()->getCamera()->setSceneRadius(sceneRadiusBak);
	mainWidget->getScene()->getCamera()->setZoomFactor(zoomFactorBak);
	mainWidget->getScene()->getCamera()->setEyes(eyesBak);
	mainWidget->getScene()->getCamera()->setCenter(centerBak);
	mainWidget->getScene()->getCamera()->setUp(upBak);
}

void ParallelCoordinatesView::addEmptyViewLabel() {
	Color backgroundColor = drawConfigWidget->getBackgroundColor();
	mainWidget->getScene()->setBackgroundColor(backgroundColor);

	Color foregroundColor;
	int bgV = backgroundColor.getV();
	if (bgV < 128) {
		foregroundColor = Color(255,255,255);
	} else {
		foregroundColor = Color(0,0,0);
	}

	GlLabel *noDimsLabel = new GlLabel(Coord(0,0,0), Coord(400,200), foregroundColor);
	noDimsLabel->setText("No graph properties selected.");
	mainLayer->addGlEntity(noDimsLabel, "no dimensions label");
	GlLabel *noDimsLabel2 = new GlLabel(Coord(0,-100,0), Coord(700,200), foregroundColor);
	noDimsLabel2->setText("Go to the \"Data configuration\" tab in the \"View editor\" panel to init the view.");
	mainLayer->addGlEntity(noDimsLabel2, "no dimensions label 2");
	mainLayer->deleteGlEntity(parallelCoordsDrawing);
	mainLayer->deleteGlEntity(glGraphComposite);
}

void ParallelCoordinatesView::removeEmptyViewLabel() {
	GlSimpleEntity *noDimsLabel = mainLayer->findGlEntity("no dimensions label");
	GlSimpleEntity *noDimsLabel2 = mainLayer->findGlEntity("no dimensions label 2");
	if (noDimsLabel != NULL) {
		mainLayer->deleteGlEntity(noDimsLabel);
		delete noDimsLabel;
		mainLayer->deleteGlEntity(noDimsLabel2);
		delete noDimsLabel2;
		mainLayer->addGlEntity(parallelCoordsDrawing, "Parallel Coordinates");
		mainLayer->addGlEntity(glGraphComposite, "graph");
	}
}

void ParallelCoordinatesView::draw() {
	if (graphProxy->getNumberOfSelectedProperties() == 0) {
		removeEmptyViewLabel();
		addEmptyViewLabel();
		mainWidget->getScene()->centerScene();
		GlMainView::draw();
		return;
	} else {
		removeEmptyViewLabel();
		if (graphProxy->getDataCount() > PROGRESS_BAR_DISPLAY_NB_DATA_THRESHOLD) {
			updateWithProgressBar();
		} else {
			updateWithoutProgressBar();
		}
	}
	if (lastNbSelectedProperties != graphProxy->getNumberOfSelectedProperties() || center) {
		centerView();
		center = false;
	}
	lastNbSelectedProperties = graphProxy->getNumberOfSelectedProperties();
	GlMainView::draw();
}

void ParallelCoordinatesView::centerView() {
	if (!mainWidget->isVisible()) {
		if (lastViewWindowWidth != 0 && lastViewWindowHeight != 0) {
			mainWidget->getScene()->ajustSceneToSize(lastViewWindowWidth, lastViewWindowHeight);
		} else {
			mainWidget->getScene()->centerScene();
		}
	} else {
		mainWidget->getScene()->ajustSceneToSize(mainWidget->width(), mainWidget->height());
	}
}

void ParallelCoordinatesView::refresh() {
	mainWidget->redraw();
}

void ParallelCoordinatesView::init() {
	draw();
}

void ParallelCoordinatesView::specificEventFilter(QObject *object,QEvent *event) {
	if (event->type() == QEvent::ToolTip && showToolTips->isChecked()) {
		QHelpEvent *he = static_cast<QHelpEvent *>(event);
		if (parallelCoordsDrawing != NULL) {
			QString ttip;
			set<unsigned int> dataUnderPointer = mapGlEntitiesInRegionToData(he->x(), he->y(), 1, 1);
			if (dataUnderPointer.size() > 0) {
				ttip = QString(graphProxy->getToolTipTextforData(*(dataUnderPointer.begin())).c_str());
				QToolTip::showText(he->globalPos(), ttip);
			}
		}
	}

	if (graphProxy != NULL && graphProxy->graphColorsModified()) {
		Observable::holdObservers();
		graphProxy->colorDataAccordingToHighlightedElts();
		Observable::unholdObservers();
	}

	if (event->type() == QEvent::Close) {
		activeInteractor->remove();
	}
}

void ParallelCoordinatesView::buildContextMenu(QObject *object, QMouseEvent *event, QMenu *contextMenu) {
  contextMenu->addMenu(viewSetupMenu);
	contextMenu->addMenu(optionsMenu);

	SpreadView::buildContextMenu(object,event,contextMenu);

	axisUnderPointer = getAxisUnderPointer(event->x(), event->y());
	if (axisUnderPointer != NULL) {
		contextMenu->addSeparator();
		contextMenu->addAction(tr("Axis Configuration"));
		contextMenu->addAction(tr("Remove Axis"));
	}

	if (graphProxy->highlightedEltsSet()) {
		contextMenu->addSeparator();
		contextMenu->addAction(tr("Select Highlighted Elements"));
		contextMenu->addAction(tr("Reset Highlighted Elements"));
	}

	set<unsigned int> dataUnderPointer = mapGlEntitiesInRegionToData(event->x(), event->y(), 1, 1);
	if (dataUnderPointer.size() > 0) {
		dataUnderMousePointer = *(dataUnderPointer.begin());
		contextMenu->addSeparator();
		contextMenu->addAction(tr(graphProxy->getToolTipTextforData(dataUnderMousePointer).c_str()))->setEnabled(false);
		contextMenu->addSeparator();
		addRemoveDataFromSelection = contextMenu->addAction(tr("Add/Remove From Selection"));
		selectData = contextMenu->addAction(tr("Select"));
		deleteData = contextMenu->addAction(tr("Delete"));
		showDataProperties = contextMenu->addAction(tr("Properties"));
	}
}

void ParallelCoordinatesView::computeContextMenuAction(QAction *action) {
	Observable::holdObservers();
	if (action->text() == "Center View") {
		centerView();
		GlMainView::draw();
	} else if (action->text() == "Classic Layout") {
		classicLayout->setChecked(true);
		circularLayout->setChecked(false);
		center = true;
		setupAndDrawView();
	} else if (action->text() == "Circular Layout") {
		circularLayout->setChecked(true);
		classicLayout->setChecked(false);
		center = true;
		setupAndDrawView();
	} else if (action == straightLinesType) {
		straightLinesType->setChecked(true);
		splineLinesType->setChecked(false);
		setupAndDrawView();
	} else if (action == splineLinesType) {
		straightLinesType->setChecked(false);
		splineLinesType->setChecked(true);
		setupAndDrawView();
	} else if (action == addRemoveDataFromSelection) {
		graphProxy->setDataSelected(dataUnderMousePointer, !graphProxy->isDataSelected(dataUnderMousePointer));
	} else if (action == selectData) {
		graphProxy->resetSelection();
		graphProxy->setDataSelected(dataUnderMousePointer, true);
	} else if (action == deleteData) {
		graphProxy->deleteData(dataUnderMousePointer);
	} else if (action == showDataProperties) {
		bool isNode = graphProxy->getDataLocation() == NODE;
		elementSelectedSlot(dataUnderMousePointer, isNode);
	} else if (action->text() == "Axis Configuration") {
		axisUnderPointer->showConfigDialog();
		draw();
	} else if (action->text() == "Remove Axis") {
		graphProxy->removePropertyFromSelection(axisUnderPointer->getAxisName());
		dataConfigWidget->updateSelectedProperties();
		draw();
	} else if (action->text() == "Select Highlighted Elements") {
		graphProxy->selectHighlightedElements();
	} else if (action->text() == "Reset Highlighted Elements") {
		graphProxy->unsetHighlightedElts();
		parallelCoordsDrawing->resetAxisSlidersPosition();
		graphProxy->colorDataAccordingToHighlightedElts();
	}
	Observable::unholdObservers();
}

void ParallelCoordinatesView::setupAndDrawView() {
	graphProxy->setSelectedProperties(dataConfigWidget->getSelectedProperties());
	graphProxy->setDataLocation(dataConfigWidget->getDataLocation());
	mainWidget->getScene()->setBackgroundColor(drawConfigWidget->getBackgroundColor());
	parallelCoordsDrawing->setAxisHeight(drawConfigWidget->getAxisHeight());
	parallelCoordsDrawing->setSpaceBetweenAxis(drawConfigWidget->getSpaceBetweenAxis());
	parallelCoordsDrawing->setAxisPointMinSize(drawConfigWidget->getAxisPointMinSize());
	parallelCoordsDrawing->setAxisPointMaxSize(drawConfigWidget->getAxisPointMaxSize());
	parallelCoordsDrawing->setBackgroundColor(drawConfigWidget->getBackgroundColor());
	parallelCoordsDrawing->setDrawPointsOnAxis(drawConfigWidget->drawPointOnAxis());
	parallelCoordsDrawing->setLineTextureFilename(drawConfigWidget->getLinesTextureFilename());
	parallelCoordsDrawing->setLinesColorAlphaValue(drawConfigWidget->getLinesColorAlphaValue());
	parallelCoordsDrawing->setLayoutType(getLayoutType());
	parallelCoordsDrawing->setLinesType(getLinesType());
	if (graphProxy->getUnhighlightedEltsColorAlphaValue() != drawConfigWidget->getUnhighlightedEltsColorsAlphaValue()) {
		graphProxy->setUnhighlightedEltsColorAlphaValue(drawConfigWidget->getUnhighlightedEltsColorsAlphaValue());
		Observable::holdObservers();
		graphProxy->colorDataAccordingToHighlightedElts();
		Observable::unholdObservers();
	}

	draw();
}

ParallelCoordinatesDrawing::LayoutType ParallelCoordinatesView::getLayoutType() const {
	if (classicLayout->isChecked()) {
		return ParallelCoordinatesDrawing::PARALLEL;
	} else {
		return ParallelCoordinatesDrawing::CIRCULAR;
	}
}

ParallelCoordinatesDrawing::LinesType ParallelCoordinatesView::getLinesType() const {
	if (straightLinesType->isChecked()) {
		return ParallelCoordinatesDrawing::STRAIGHT;
	} else {
		return ParallelCoordinatesDrawing::SPLINE;
	}
}

set<unsigned int> ParallelCoordinatesView::mapGlEntitiesInRegionToData(const int x, const int y, const unsigned int width, const unsigned int height) {

	vector<GlEntity *> selectedEntities;
	vector<node> selectedAxisPoints;
	vector<edge> dummy;
	set<unsigned int> mappedData;

	bool result = mainWidget->selectGlEntities(x, y, width, height, selectedEntities, mainLayer);

	if (result) {
		vector<GlEntity *>::iterator it;
		for (it = selectedEntities.begin(); it != selectedEntities.end(); ++it) {
			GlEntity *entity = *it;
			unsigned int selectedEltId;
			if (parallelCoordsDrawing->getDataIdFromGlEntity(entity, selectedEltId)) {
				mappedData.insert(selectedEltId);
			}
		}
	}

	mainWidget->doSelect(x, y, width , height, selectedAxisPoints, dummy, mainLayer);
	vector<node>::iterator it;
	for (it = selectedAxisPoints.begin(); it != selectedAxisPoints.end(); ++it) {
		node n = *it;
		unsigned int selectedEltId;
		if (parallelCoordsDrawing->getDataIdFromAxisPoint(n, selectedEltId)) {
			mappedData.insert(selectedEltId);
		}
	}

	return mappedData;
}

void ParallelCoordinatesView::setDataUnderPointerSelectFlag(const int x, const int y, const bool selectFlag) {
	set<unsigned int> dataUnderPointer = mapGlEntitiesInRegionToData(x, y, 1, 1);
	set<unsigned int>::iterator it;
	for (it = dataUnderPointer.begin() ; it != dataUnderPointer.end() ; ++it) {
		if (!graphProxy->highlightedEltsSet() || (graphProxy->highlightedEltsSet() && graphProxy->isDataHighlighted(*it)))
			graphProxy->setDataSelected(*it, selectFlag);
	}
}

void ParallelCoordinatesView::setDataInRegionSelectFlag(const int x, const int y, const unsigned int width, const unsigned int height, const bool selectFlag) {
	set<unsigned int> dataUnderPointer = mapGlEntitiesInRegionToData(x, y, width, height);
	set<unsigned int>::iterator it;
	for (it = dataUnderPointer.begin() ; it != dataUnderPointer.end() ; ++it) {
		if (!graphProxy->highlightedEltsSet() || (graphProxy->highlightedEltsSet() && graphProxy->isDataHighlighted(*it)))
			graphProxy->setDataSelected(*it, selectFlag);
	}
}

void ParallelCoordinatesView::resetSelection() {
	graphProxy->resetSelection();
}

void ParallelCoordinatesView::deleteDataUnderPointer(const int x, const int y) {
	set<unsigned int> dataUnderPointer = mapGlEntitiesInRegionToData(x, y, 1, 1);
	set<unsigned int>::iterator it;
	for (it = dataUnderPointer.begin() ; it != dataUnderPointer.end() ; ++it) {
		if (!graphProxy->highlightedEltsSet() || (graphProxy->highlightedEltsSet() && graphProxy->isDataHighlighted(*it)))
			graphProxy->deleteData(*it);
	}
}

void ParallelCoordinatesView::showDataUnderPointerProperties(const int x, const int y) {
	set<unsigned int> dataUnderPointer = mapGlEntitiesInRegionToData(x, y, 1, 1);
	if (dataUnderPointer.size() > 0) {
		unsigned int dataId;
		if (!graphProxy->highlightedEltsSet()) {
			dataId = *(dataUnderPointer.begin());
		} else {
			set<unsigned int>::iterator it = dataUnderPointer.begin();
			while (it != dataUnderPointer.end() && !graphProxy->isDataHighlighted(*it)) {
				++it;
			}
			if (it == dataUnderPointer.end()) {
				return;
			} else {
				dataId = *it;
			}
		}
		if (graphProxy->getDataLocation() == NODE) {
			elementSelectedSlot(dataId, true);
		} else {
			elementSelectedSlot(dataId, false);
		}
	}
}

void ParallelCoordinatesView::highlightDataUnderPointer(const int x, const int y, const bool addEltToMagnifyFlag) {
	if (!addEltToMagnifyFlag) {
		graphProxy->unsetHighlightedElts();
	}
	set<unsigned int> dataUnderPointer = mapGlEntitiesInRegionToData(x, y, 1, 1);
	set<unsigned int>::iterator it;
	for (it = dataUnderPointer.begin() ; it != dataUnderPointer.end() ; ++it) {
		graphProxy->addOrRemoveEltToHighlight(*it);
	}
	graphProxy->colorDataAccordingToHighlightedElts();
}

void ParallelCoordinatesView::highlightDataInRegion(const int x, const int y, const int width, const int height, const bool addEltToMagnifyFlag) {
	if (!addEltToMagnifyFlag) {
		graphProxy->unsetHighlightedElts();
	}

	set<unsigned int> dataUnderPointer = mapGlEntitiesInRegionToData(x, y, width, height);
	set<unsigned int>::iterator it;
	for (it = dataUnderPointer.begin() ; it != dataUnderPointer.end() ; ++it) {
		graphProxy->addOrRemoveEltToHighlight(*it);
	}
	graphProxy->colorDataAccordingToHighlightedElts();
}

void ParallelCoordinatesView::resetHighlightedElements() {
	graphProxy->unsetHighlightedElts();
	graphProxy->colorDataAccordingToHighlightedElts();
}

ParallelAxis *ParallelCoordinatesView::getAxisUnderPointer(const int x, const int y) const {
	vector<ParallelAxis *> allAxis = parallelCoordsDrawing->getAllAxis();
	axisSelectionLayer->setCamera(mainWidget->getScene()->getLayer("Main")->getCamera());
	axisSelectionLayer->getComposite()->reset(false);
	for (unsigned int i = 0 ; i < allAxis.size() ; ++i) {
		axisSelectionLayer->addGlEntity(allAxis[i], getStringFromNumber(allAxis[i]));
	}
	vector<GlEntity *> pickedEntities;
	if (mainWidget->selectGlEntities(x, y, pickedEntities, axisSelectionLayer)) {
		return dynamic_cast<ParallelAxis *>(pickedEntities[0]);
	}
	return NULL;
}

void ParallelCoordinatesView::swapAxis(ParallelAxis *firstAxis, ParallelAxis *secondAxis) {
	parallelCoordsDrawing->swapAxis(firstAxis, secondAxis);
	dataConfigWidget->updateSelectedProperties();
}

void ParallelCoordinatesView::removeAxis(ParallelAxis *axis) {
	parallelCoordsDrawing->removeAxis(axis);
}

void ParallelCoordinatesView::addAxis(ParallelAxis *axis) {
	parallelCoordsDrawing->addAxis(axis);
}

vector<ParallelAxis *> ParallelCoordinatesView::getAllAxis() {
	return parallelCoordsDrawing->getAllAxis();
}

void ParallelCoordinatesView::updateAxisSlidersPosition() {
	if (!graphProxy->highlightedEltsSet()) {
		parallelCoordsDrawing->resetAxisSlidersPosition();
	} else {
		set<unsigned int> highlightedElts = graphProxy->getHighlightedElts();
		vector<ParallelAxis *> axis = getAllAxis();
		vector<ParallelAxis *>::iterator it;
		for (it = axis.begin() ; it != axis.end() ; ++it) {
			(*it)->updateSlidersWithDataSubset(highlightedElts);
		}
	}
}

void ParallelCoordinatesView::updateWithAxisSlidersRange(ParallelAxis *axis, bool multiFiltering) {
	parallelCoordsDrawing->updateWithAxisSlidersRange(axis, multiFiltering);
	graphProxy->colorDataAccordingToHighlightedElts();
}

bool ParallelCoordinatesView::highlightedElementsSet() const {
	return graphProxy->highlightedEltsSet();
}

void ParallelCoordinatesView::highlightDataInAxisBoxPlotRange(QuantitativeParallelAxis *axis) {
	set<unsigned int> eltToHighlight = axis->getDataBetweenBoxPlotBounds();
	if (eltToHighlight.size() > 0) {
		graphProxy->resetHighlightedElts(eltToHighlight);
		graphProxy->colorDataAccordingToHighlightedElts();
		updateAxisSlidersPosition();
	}
}

}
