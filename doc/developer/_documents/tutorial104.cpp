#include <tulip/GlCircle.h>
#include <tulip/GlRect.h>
#include <tulip/GlBox.h>
#include <tulip/TlpQtTools.h>
#include <tulip/GlMainWidget.h>
#include <tulip/MouseInteractors.h>
#include <QApplication>
#include <vector>
#include <iostream>

/**
 * Tutorial 104
 *
 * Create 2 boxes and add a static 2D layer to display a circle and 4 squares in the foreground
 * export CPLUS_INCLUDE_PATH=/usr/include/qt4:/usr/include/qt4/QtCore:/usr/include/qt4/Qt:/usr/include/qt4/QtGui:/usr/include/qt4/QtOpenGL

 * g++ tutorial104.cpp `/home/tulip/Documents/tulip/tulip-install/bin/tulip-config --libs --cxxflags` -o tutorial104
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

	//We update the scene and the viewport
  mainWidget->show();
	mainWidget->centerScene();

  //We firstly add a new GlLayer (with name "2D layer") in the scene and set layer to 2D mode
	GlLayer *layer2D=new GlLayer("2D layer");
	layer2D->set2DMode();
	mainWidget->getScene()->addExistingLayer(layer2D);

	//after, get the viewport to guess the center of the window
	Vector<int, 4> viewport;
	viewport = mainWidget->getScene()->getViewport();

	// This is the position of the center of the circle
	// (ScreenWidth / 2, ScreenHeight / 2, 1)
  Coord circleCenter(viewport[2] / 2, viewport[3] / 2, 1);
	Color whiteTrans(255, 255, 255, 0);
	Color circleColor(100, 100, 255, 255);

	// We create the circle giving it's center position, it's color, it's radius and the number of segments
	GlCircle* circle = new GlCircle(circleCenter, 200., circleColor, whiteTrans, false, true, 0, 50);

  // Create the 4 rectangles for the augmented display
  GlComposite *composite = new GlComposite();
  GlRect* rects;
  Coord center;

  for(int i=0; i < 4; i++)
  {
    // We calculate the position of the center of each square
    center[0] = cos((double)i * 3.14/2.0) * 200.;
    center[1] = sin((double)i * 3.14/2.0) * 200.;
    center[2] = 0;
    center = center + circleCenter;

    // We add the square to the composite entity
    rects = new GlRect(center, 32, 32, whiteTrans, circleColor);
    composite->addGlEntity(rects, "Gl Tutorial 4 : Rect" + i+1);
  }

  // Finally we add the circle to the GlLayer.
	composite->addGlEntity(circle, "Gl Tutorial 4 : Circle");
	layer2D->addGlEntity(composite, "Gl Tutorial 4 : composite");

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



