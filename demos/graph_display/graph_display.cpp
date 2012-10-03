#include <tulip/TlpQtTools.h>
#include <tulip/GlMainWidget.h>
#include <tulip/MouseInteractors.h>

#include <QtGui/QApplication>

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
  Graph* g = tlp::loadGraph(argv[1]);

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
