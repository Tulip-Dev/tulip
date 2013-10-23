#include <tulip/TlpQtTools.h>
#include <tulip/GlMainWidget.h>
#include <tulip/TulipProject.h>
#include <tulip/MouseInteractors.h>
#include <QApplication>
#include <qstring.h>
#include <iostream>
using namespace tlp;

int main(int argc, char** argv) {
	/*
	A QApplication must always be declared at the beginning of the main function in order for Tulip to work.
	This must be done before calling tlp::initTulipSoftware()
	*/
	QApplication app(argc,argv);
	/*
	Initialize the library, load plugins and set application runtime pathes accordingly to the host operating system
	This method should always be called if you intend to use plugins in your application.
	*/
	tlp::initTulipSoftware(NULL);
	/*
	Load the file passed as first argument into a graph.
	This method will select the default Tulip algorithm plugin (TLP)
	*/
	//Graph* g = tlp::loadGraph(argv[1]);
	Graph* g = newGraph();


	TulipProject* _project = TulipProject::openProject(QString::fromLatin1(argv[1]));

	//std::cout << QString::fromLatin1(argv[1]) << std::endl;

  if (_project->exists("/data/graphs/0/graph.tlp")) {
std::cout << "pouet pouet" << std::endl;
    DataSet data;

    data.set<std::string>("file::filename",_project->toAbsolutePath("/data/graphs/0/graph.tlp").toStdString());

    g = tlp::importGraph("TLP Import",data);
std::cout << g << std::endl;

  }
	
// Creates the main widget that will display our graph
	GlMainWidget* mainWidget = new GlMainWidget(NULL);
	// Adds a layer to the scene
	GlLayer* mainLayer = mainWidget->getScene()->createLayer("Main");
	// Adds the graph to this layer
	mainLayer->addGraph(g,"graph");

	// Display the widget
	mainWidget->show();
	// Flush event loop in order to let paint events pass through in order for the scene to be initialized.
	QApplication::processEvents();
	// Center the camera and draw the graph
	mainWidget->centerScene();
	mainWidget->draw();
	// Adds Zoom and pan navigation to the widget
	mainWidget->installEventFilter(new MouseNKeysNavigator);
	return app.exec();
}

