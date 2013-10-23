#include <tulip/GlBox.h>
#include <tulip/GlLine.h>
#include <tulip/TlpQtTools.h>
#include <tulip/GlMainWidget.h>
#include <tulip/MouseInteractors.h>
#include <QApplication>
#include <vector>

/**
 * Tutorial 102
 *
 * Display the Tutorial101 scene and an additional cube with a colored transparency effect.
 * export CPLUS_INCLUDE_PATH=/usr/include/qt4:/usr/include/qt4/QtCore:/usr/include/qt4/Qt:/usr/include/qt4/QtGui:/usr/include/qt4/QtOpenGL

 * g++ tutorial102.cpp `/home/tulip/Documents/tulip/tulip-install/bin/tulip-config --libs --cxxflags` -o tutorial102
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
	tlp::initTulipSoftware(NULL);

	// Creates the main widget that will display our graph
	GlMainWidget* mainWidget = new GlMainWidget(NULL);
	// Adds a layer to the scene
	GlLayer* mainLayer = mainWidget->getScene()->createLayer("Main");

	Coord center1(-1,-1,-1);
	Coord center2(1,1,1);
	Size size(1,1,1);
	Color whiteOpaque(255, 255, 255, 255);
	Color blackOpaque(0, 0, 0, 255);

	GlBox *node1 = new GlBox(center1, size, whiteOpaque, blackOpaque);
	GlBox *node2 = new GlBox(center2, size, whiteOpaque, blackOpaque);

  mainLayer->addGlEntity(node1, "Gl Tutorial 1: Node 1");
  mainLayer->addGlEntity(node2, "Gl Tutorial 1: Node 2");

  Coord centerBox(0, 0, 0);
  Size sizeBox(3.001, 3.001, 3.001);
	Color purpleTrans(155, 0, 155, 50);
	GlBox *box = new GlBox(centerBox, sizeBox, purpleTrans, blackOpaque);
  mainLayer->addGlEntity(box, "Gl Tutorial 2: Box");

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



