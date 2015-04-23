#include <tulip/TlpQtTools.h>
#include <tulip/TlpTools.h>
#include <tulip/GlMainWidget.h>
#include <tulip/MouseInteractors.h>
#include <QApplication>

/**
 * Tutorial 100
 *
 * Create a QApplication window and draw an empty scene
 * export CPLUS_INCLUDE_PATH=/usr/include/qt4:/usr/include/qt4/QtCore:/usr/include/qt4/Qt:/usr/include/qt4/QtGui:/usr/include/qt4/QtOpenGL

 * g++ tutorial100.cpp `/home/tulip/Documents/tulip/tulip-install/bin/tulip-config --libs --cxxflags` -o tutorial100
 * ./run-from-tulip tutorial100
 */

using namespace std;
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
	tlp::initTulipLib("/home/tulip/Documents/tulip/install-release/");

	// Create the main widget that will display our graph
	GlMainWidget* mainWidget = new GlMainWidget(NULL);

	// Add a layer to the scene
	GlLayer* mainLayer = mainWidget->getScene()->createLayer("Main");

	// Display the widget
	mainWidget->show();

	// Flush event loop to let paint events pass through, allowing for the scene to be initialized.
	QApplication::processEvents();

	// Center the camera and draw the graph
	mainWidget->centerScene();
	mainWidget->draw();

	// Add Zoom and pan navigation to the widget
	mainWidget->installEventFilter(new MouseNKeysNavigator);

	return app.exec();
}


