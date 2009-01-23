#include "ParallelCoordinatesView.h"
#include "AxisConfigDialogs.h"
#include "NominalParallelAxis.h"
#include "GlProgressBar.h"

#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QToolTip>
#include <QtGui/QImageWriter>
#include <QtCore/QThread>
#include <QtCore/QDir>

#include <tulip/GWInteractor.h>
#include <tulip/InteractorManager.h>
#include <tulip/GWOverviewWidget.h>
#include <tulip/GlTools.h>
#include <tulip/GlLabel.h>


using namespace std;

namespace tlp {

class ParallelDrawingUpdateThread : public QThread {

public :

	ParallelDrawingUpdateThread(ParallelCoordinatesDrawing *parallelDrawing) : parallelDrawing(parallelDrawing) {
		parallelDrawing->resetNbDataProcessed();
		parallelDrawing->deleteAxisGlEntities();
	}

	void run() {
		parallelDrawing->update();
	}

private :

	ParallelCoordinatesDrawing *parallelDrawing;
};

VIEWPLUGIN(ParallelCoordinatesView, "Parallel Coordinates view", "Tulip Team", "16/04/2008", "Parallel Coordinates View", "1.0");


ParallelCoordinatesView::ParallelCoordinatesView() :
	GlMainView(), configDialog(NULL), graphProxy(NULL), parallelCoordsDrawing(NULL)  {
	addDependency<Interactor>("ParallelCoordsElementShowInfos", "1.0");
	addDependency<Interactor>("ParallelCoordsElementsSelector", "1.0");
	addDependency<Interactor>("ParallelCoordsElementDeleter", "1.0");
	addDependency<Interactor>("ParallelCoordsElementHighlighter", "1.0");
	addDependency<Interactor>("ParallelCoordsAxisSwapper", "1.0");
	addDependency<Interactor>("ParallelCoordsAxisSliders", "1.0");
}

ParallelCoordinatesView::~ParallelCoordinatesView() {
	cleanup();
}

void ParallelCoordinatesView::cleanup() {

	resetInteractors();

	if (graphProxy != NULL) {
		delete graphProxy;
		graphProxy = NULL;
	}

	if (parallelCoordsDrawing != NULL) {
		delete parallelCoordsDrawing;
		parallelCoordsDrawing = NULL;
	}

	if (configDialog != NULL) {
		delete configDialog;
		configDialog = NULL;
	}
}

QWidget *ParallelCoordinatesView::construct(QWidget *parent) {
	QWidget *widget=GlMainView::construct(parent);
	initGlWidget();
	buildMenuEntries();
	constructInteractorsMap();

	//Export Menu
	exportImageMenu=new QMenu("&Save Picture as ");
	// Tulip known formats (see GlGraph)
	// formats are sorted, "~" is just an end marker
	char *tlpFormats[] = {"EPS", "SVG", "~"};
	unsigned int i = 0;
	//Image PopuMenu
	// int Qt 4, output formats are not yet sorted and uppercased
	list<QString> formats;
	// first add Tulip known formats
	while (strcmp(tlpFormats[i], "~") != 0)
		formats.push_back(tlpFormats[i++]);
	// uppercase and insert all Qt formats
	foreach (QByteArray format, QImageWriter::supportedImageFormats()) {
		char* tmp = format.data();
		for (int i = strlen(tmp) - 1; i >= 0; --i)
	 	tmp[i] = toupper(tmp[i]);
		formats.push_back(QString(tmp));
	 }
	 // sort before inserting in exportImageMenu
	 formats.sort();
	 foreach(QString str, formats)
	 exportImageMenu->addAction(str);

	 connect(exportImageMenu, SIGNAL(triggered(QAction*)), SLOT(exportImage(QAction*)));

	 return widget;
}

void ParallelCoordinatesView::initGlWidget() {
	mainLayer = new GlLayer("Main");

	glGraphComposite = new GlGraphComposite(tlp::newGraph());
	GlGraphRenderingParameters param = glGraphComposite->getRenderingParameters();
	DataSet glGraphData;
	dataSet = new DataSet();
	if (dataSet->get<DataSet>("displaying", glGraphData)) {
		param.setParameters(glGraphData);
		glGraphComposite->setRenderingParameters(param);
	}

	mainLayer->addGlEntity(glGraphComposite, "graph");
	mainWidget->getScene()->addLayer(mainLayer);
	mainWidget->getScene()->addGlGraphCompositeInfo(mainLayer, glGraphComposite);
	mainWidget->setMouseTracking(true);

}

void ParallelCoordinatesView::buildMenuEntries() {
	dialogMenu=new QMenu(tr("Dialog"));
	dialogMenu->addAction(tr("Configuration"));
	viewSetupMenu = new QMenu(tr("View Setup"));
	viewSetupMenu->addAction(tr("Center View"));
	viewSetupMenu->addSeparator();
	view2d = viewSetupMenu->addAction(tr("Classic View"));
	view2d->setCheckable(true);
	view2d->setChecked(true);
	view2dSpline = viewSetupMenu->addAction(tr("Spline View"));
	view2dSpline->setCheckable(true);
	view2dSpline->setChecked(false);
	optionsMenu = new QMenu(tr("Options"));
	showToolTips = optionsMenu->addAction(tr("Tooltips"));
	showToolTips->setCheckable(true);
	showToolTips->setChecked(false);
}

void ParallelCoordinatesView::setData(Graph *graph, DataSet dataSet) {
  overviewWidget->setObservedView(NULL,NULL);

	center = true;
	vector<string> selectedPropertiesBak;

	// test if the graph to set belongs to the same hierarchy as the current graph
	bool sameGraphRoot = false;
	if (graphProxy != NULL && (graph->getRoot() == graphProxy->getRoot())) {
		sameGraphRoot = true;
		selectedPropertiesBak = graphProxy->getSelectedProperties();
		center = false;
	}

	if (graphProxy != NULL) {
		graphProxy->removeGraphObserver(parallelCoordsDrawing);
		delete graphProxy;
		graphProxy = NULL;
	}

	graphProxy = new ParallelCoordinatesGraphProxy(graph);
	if (sameGraphRoot) {
		graphProxy->setSelectedProperties(selectedPropertiesBak);
	}

	// if the graph to set don't belong to the same hierarchy as the previous one
	// delete the config dialog
	if (configDialog != NULL && !sameGraphRoot) {
		delete configDialog;
		configDialog = NULL;
	}

	if (parallelCoordsDrawing != NULL) {
		mainLayer->deleteGlEntity(parallelCoordsDrawing);
		delete parallelCoordsDrawing;
		parallelCoordsDrawing = NULL;
	}

	parallelCoordsDrawing = new ParallelCoordinatesDrawing(graphProxy);
	graphProxy->addGraphObserver(parallelCoordsDrawing);
	mainLayer->addGlEntity(parallelCoordsDrawing, "Parallel Coordinates");
	overviewWidget->setObservedView(mainWidget, parallelCoordsDrawing);



	if (configDialog == NULL) {

		configDialog = new ParallelCoordinatesConfigDialog(graphProxy, mainWidget);
		configDialog->setModal(true);

		unsigned int axisHeight = DEFAULT_AXIS_HEIGHT;
		unsigned int spaceBetweenAxis = DEFAULT_AXIS_HEIGHT / 2;
		unsigned int linesColorAlphaValue = DEFAULT_LINES_COLOR_ALPHA_VALUE;

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

		if (dataSet.exist("dataLocation")) {
			string dataLocation;
			dataSet.get("dataLocation", dataLocation);
			if (dataLocation == "node") {
				configDialog->setDataLocation(NODE);
			} else {
				configDialog->setDataLocation(EDGE);
			}
		}

		if (dataSet.exist("backgroundColor")) {
			Color backgroundColor;
			dataSet.get("backgroundColor", backgroundColor);
			configDialog->setBackgroundColor(backgroundColor);
		}

		if (dataSet.exist("axisPointMinSize")) {
			unsigned int axisPointMinSize;
			dataSet.get("axisPointMinSize", axisPointMinSize);
			configDialog->setAxisPointMinSize(axisPointMinSize);
		}

		if (dataSet.exist("axisPointMaxSize")) {
			unsigned int axisPointMaxSize;
			dataSet.get("axisPointMaxSize", axisPointMaxSize);
			configDialog->setAxisPointMaxSize(axisPointMaxSize);
		}

		if (dataSet.exist("drawPointsOnAxis")) {
			bool drawPointsOnAxis;
			dataSet.get("drawPointsOnAxis", drawPointsOnAxis);
			configDialog->setDrawPointOnAxis(drawPointsOnAxis);
		}

		if (dataSet.exist("linesTextureFileName")) {
			string linesTextureFileName;
			dataSet.get("linesTextureFileName", linesTextureFileName);
			configDialog->setLinesTextureFilename(linesTextureFileName);
		}

		if (dataSet.exist("viewType")) {
			string vType;
			dataSet.get("viewType", vType);
			if (vType == "view2d") {
				setViewType(VIEW_2D);
			} else {
				setViewType(VIEW_2D_SPLINE);
			}
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

		configDialog->setAxisHeight(axisHeight);
		configDialog->setSpaceBetweenAxis(spaceBetweenAxis);
		configDialog->setLinesColorAlphaValue(linesColorAlphaValue);

	}

	if (graphProxy->getNumberOfSelectedProperties() > 0) {
		setUpAndDrawView();
	} else {
		showConfigDialog();
	}

	list<QAction *> *actions=getInteractorsActionList();
	for(list<QAction *>::iterator it=actions->begin();it!=actions->end();++it){
	  if((*it)->isChecked())
	    (*it)->activate(QAction::Trigger);
	}
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

	if (graphProxy->getDataLocation() == NODE) {
		dataSet->set<string>("dataLocation", "node");
	} else {
		dataSet->set<string>("dataLocation", "edge");
	}

	dataSet->set("backgroundColor", configDialog->getBackgroundColor());
	dataSet->set("axisHeight", configDialog->getAxisHeight());
	dataSet->set("spaceBetweenAxis", configDialog->getSpaceBetweenAxis());
	unsigned int axisPointMinSize = (unsigned int) configDialog->getAxisPointMinSize().getW();
	unsigned int axisPointMaxSize = (unsigned int) configDialog->getAxisPointMaxSize().getW();
	dataSet->set("axisPointMinSize", axisPointMinSize);
	dataSet->set("axisPointMaxSize", axisPointMaxSize);
	dataSet->set("drawPointsOnAxis", configDialog->drawPointOnAxis());
	dataSet->set("linesTextureFileName", configDialog->getLinesTextureFilename());
	dataSet->set("linesColorAlphaValue", configDialog->getLinesColorAlphaValue());

	if (getViewType() == VIEW_2D) {
		dataSet->set<string>("viewType", "view2d");
	} else {
		dataSet->set<string>("viewType", "view2dSpline");
	}


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
	parallelCoordsDrawing->deleteAxisGlEntities();
	parallelCoordsDrawing->update();
	if (center) {
		centerView();
		center = false;
	}
	GlMainView::draw();
}

void ParallelCoordinatesView::updateWithProgressBar() {

	// removal of the parallel coordinates drawing from the main layer
	if (mainLayer->findGlEntity("Parallel Coordinates") != NULL) {
		mainLayer->deleteGlEntity(parallelCoordsDrawing);
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
	progressBar->setProgress(0, nbData);
	// add the progress bar to main layer, center scene on it and draw
	mainLayer->addGlEntity(progressBar, "progress bar");
	centerView();
	GlMainView::draw();

	// start the drawing update thread
	drawingUpdateThread.start();

	// get the drawing update progress running in background and update progress bar
	unsigned int nbDataProcessed = parallelCoordsDrawing->getNbDataProcessed();
	while (nbDataProcessed < nbData) {
		progressBar->setProgress(nbDataProcessed, nbData);
		GlMainView::draw();
		nbDataProcessed = parallelCoordsDrawing->getNbDataProcessed();
	}

	progressBar->setProgress(nbData, nbData);
	GlMainView::draw();

	// join the drawing update thread to main process
	drawingUpdateThread.wait();

	// remove progress bar from main layer and delete it
	mainLayer->deleteGlEntity(progressBar);
	delete progressBar;

	// add updated drawing to main layer
	mainLayer->addGlEntity(parallelCoordsDrawing, "Parallel Coordinates");
	overviewWidget->setObservedView(mainWidget, parallelCoordsDrawing);

	if (center) {
		centerView();
		center = false;
	} else {
		// restore original camera parameters
		mainWidget->getScene()->getCamera()->setSceneRadius(sceneRadiusBak);
		mainWidget->getScene()->getCamera()->setZoomFactor(zoomFactorBak);
		mainWidget->getScene()->getCamera()->setEyes(eyesBak);
		mainWidget->getScene()->getCamera()->setCenter(centerBak);
		mainWidget->getScene()->getCamera()->setUp(upBak);
	}
	GlMainView::draw();
}

void ParallelCoordinatesView::draw() {

	if (graphProxy->getDataCount() > PROGRESS_BAR_DISPLAY_NB_DATA_THRESHOLD) {
		updateWithProgressBar();
	} else {
		updateWithoutProgressBar();
	}
}

void ParallelCoordinatesView::centerView() {
	mainWidget->getScene()->centerScene();
}

void ParallelCoordinatesView::refresh() {
	mainWidget->redraw();
}

void ParallelCoordinatesView::init() {
	centerView();
}

//==================================================
// Interactor functions
//==================================================
void ParallelCoordinatesView::constructInteractorsMap() {
	MutableContainer<Interactor *> interactors;
	InteractorManager::getInst().initInteractorList(interactors);
	interactorsMap["Navigate in graph"].push_back(InteractorManager::getInst().getInteractor("MouseNKeysNavigator"));
	interactorsMap["Zoom on rectangle"].push_back(InteractorManager::getInst().getInteractor("MousePanNZoomNavigator"));
	interactorsMap["Zoom on rectangle"].push_back(InteractorManager::getInst().getInteractor("MouseBoxZoomer"));
	interactorsMap["Get information on nodes/edges"].push_back(InteractorManager::getInst().getInteractor("ParallelCoordsElementShowInfos"));
	interactorsMap["Get information on nodes/edges"].push_back(InteractorManager::getInst().getInteractor("MousePanNZoomNavigator"));
	interactorsMap["Select nodes/edges in a rectangle"].push_back(InteractorManager::getInst().getInteractor("ParallelCoordsElementsSelector"));
	interactorsMap["Select nodes/edges in a rectangle"].push_back(InteractorManager::getInst().getInteractor("MousePanNZoomNavigator"));
	interactorsMap["Delete nodes or edges"].push_back(InteractorManager::getInst().getInteractor("ParallelCoordsElementDeleter"));
	interactorsMap["Delete nodes or edges"].push_back(InteractorManager::getInst().getInteractor("MousePanNZoomNavigator"));
	interactorsMap["Highlight elements"].push_back(InteractorManager::getInst().getInteractor("ParallelCoordsElementHighlighter"));
	interactorsMap["Highlight elements"].push_back(InteractorManager::getInst().getInteractor("MousePanNZoomNavigator"));
	interactorsMap["Axis Swapper"].push_back(InteractorManager::getInst().getInteractor("MousePanNZoomNavigator"));
	interactorsMap["Axis Swapper"].push_back(InteractorManager::getInst().getInteractor("ParallelCoordsAxisSwapper"));
	interactorsMap["Axis Sliders"].push_back(InteractorManager::getInst().getInteractor("MousePanNZoomNavigator"));
	interactorsMap["Axis Sliders"].push_back(InteractorManager::getInst().getInteractor("ParallelCoordsAxisSliders"));
	interactorsMap["Axis Box Plot"].push_back(InteractorManager::getInst().getInteractor("MousePanNZoomNavigator"));
	interactorsMap["Axis Box Plot"].push_back(InteractorManager::getInst().getInteractor("ParallelCoordsAxisBoxPlot"));
}
//==================================================
void ParallelCoordinatesView::constructInteractorsActionList() {
	interactorsActionList.push_back(new QAction(QIcon(":/i_navigation.png"), "Navigate in graph", this));
	interactorsActionList.push_back(new QAction(QIcon(":/i_zoom.png"), "Zoom on rectangle", this));
	interactorsActionList.push_back(new QAction(QIcon(":/i_select.png"), "Get information on nodes/edges", this));
	interactorsActionList.push_back(new QAction(QIcon(":/i_selection.png"), "Select nodes/edges in a rectangle", this));
	interactorsActionList.push_back(new QAction(QIcon(":/i_del.png"), "Delete nodes or edges", this));
	interactorsActionList.push_back(new QAction(QIcon(":/i_element_highlighter.png"), "Highlight elements",this));
	interactorsActionList.push_back(new QAction(QIcon(":/i_axis_swapper.png"),"Axis Swapper",this));
	interactorsActionList.push_back(new QAction(QIcon(":/i_axis_sliders.png"),"Axis Sliders",this));
	interactorsActionList.push_back(new QAction(QIcon(":/i_axis_boxplot.png"),"Axis Box Plot",this));
}

void ParallelCoordinatesView::installInteractor(QAction *action) {
	resetInteractors(interactorsMap[action->text().toStdString()]);
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

	if (event->type() == QEvent::Close) {
		cleanup();
	}
}

void ParallelCoordinatesView::buildContextMenu(QObject *object, QMouseEvent *event, QMenu *contextMenu) {
	contextMenu->addMenu(dialogMenu);
	contextMenu->addMenu(viewSetupMenu);
	contextMenu->addMenu(optionsMenu);
	contextMenu->addMenu(exportImageMenu);

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
	if (action->text() == "Configuration") {
		showConfigDialog();
	} else if (action->text() == "Center View") {
		centerView();
		GlMainView::draw();
	} else if (action->text() == "Classic View") {
		view2d->setChecked(true);
		view2dSpline->setChecked(false);
		setUpAndDrawView();
	} else if (action->text() == "Spline View") {
		view2dSpline->setChecked(true);
		view2d->setChecked(false);
		setUpAndDrawView();
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

void ParallelCoordinatesView::showConfigDialog() {

	if (configDialog != NULL && configDialog->exec() == QDialog::Accepted) {
		configDialog->hide();
		setUpAndDrawView();
	}
}

void ParallelCoordinatesView::setUpAndDrawView() {
	mainWidget->getScene()->setBackgroundColor(configDialog->getBackgroundColor());
	parallelCoordsDrawing->setAxisHeight(configDialog->getAxisHeight());
	parallelCoordsDrawing->setSpaceBetweenAxis(configDialog->getSpaceBetweenAxis());
	parallelCoordsDrawing->setAxisPointMinSize(configDialog->getAxisPointMinSize());
	parallelCoordsDrawing->setAxisPointMaxSize(configDialog->getAxisPointMaxSize());
	parallelCoordsDrawing->setBackgroundColor(configDialog->getBackgroundColor());
	parallelCoordsDrawing->setDrawPointsOnAxis(configDialog->drawPointOnAxis());
	parallelCoordsDrawing->setLineTextureFilename(configDialog->getLinesTextureFilename());
	parallelCoordsDrawing->setLinesColorAlphaValue(configDialog->getLinesColorAlphaValue());
	parallelCoordsDrawing->setViewType(getViewType());
	graphProxy->setDataLocation(configDialog->getDataLocation());
	draw();
}

viewType ParallelCoordinatesView::getViewType() const {
	if (view2d->isChecked()) {
		return VIEW_2D;
	} else {
		return VIEW_2D_SPLINE;
	}
}

void ParallelCoordinatesView::setViewType(const viewType vType) {
	if (vType == VIEW_2D) {
		view2d->setChecked(true);
		view2dSpline->setChecked(false);
	} else  {
		view2d->setChecked(false);
		view2dSpline->setChecked(true);
	}
}

set<unsigned int> ParallelCoordinatesView::mapGlEntitiesInRegionToData(const int x, const int y, const unsigned int width, const unsigned int height) {

	vector<GlEntity *> selectedEntities;
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

	return mappedData;
}

void ParallelCoordinatesView::setDataUnderPointerSelectFlag(const int x, const int y, const bool selectFlag) {
	set<unsigned int> dataUnderPointer = mapGlEntitiesInRegionToData(x, y, 1, 1);
	set<unsigned int>::iterator it;
	for (it = dataUnderPointer.begin() ; it != dataUnderPointer.end() ; ++it) {
		graphProxy->setDataSelected(*it, selectFlag);
	}
}

void ParallelCoordinatesView::setDataInRegionSelectFlag(const int x, const int y, const unsigned int width, const unsigned int height, const bool selectFlag) {
	set<unsigned int> dataUnderPointer = mapGlEntitiesInRegionToData(x, y, width, height);
	set<unsigned int>::iterator it;
	for (it = dataUnderPointer.begin() ; it != dataUnderPointer.end() ; ++it) {
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
		graphProxy->deleteData(*it);
	}
}

void ParallelCoordinatesView::showDataUnderPointerProperties(const int x, const int y) {
	set<unsigned int> dataUnderPointer = mapGlEntitiesInRegionToData(x, y, 1, 1);
	if (dataUnderPointer.size() > 0) {
		if (graphProxy->getDataLocation() == NODE) {
			elementSelectedSlot(*(dataUnderPointer.begin()), true);
		} else {
			elementSelectedSlot(*(dataUnderPointer.begin()), false);
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

	Coord screenCoords((double) mainWidget->width() - (double) x, (double) y, 0);
	Coord sceneCoords = mainWidget->getScene()->getCamera()->screenTo3DWorld(screenCoords);

	return parallelCoordsDrawing->getAxisUnderPoint(sceneCoords);

}

void ParallelCoordinatesView::swapAxis(ParallelAxis *firstAxis, ParallelAxis *secondAxis) {
	parallelCoordsDrawing->swapAxis(firstAxis, secondAxis);
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

void ParallelCoordinatesView::updateWithAxisSlidersRange(ParallelAxis *axis) {
	parallelCoordsDrawing->updateWithAxisSlidersRange(axis);
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
