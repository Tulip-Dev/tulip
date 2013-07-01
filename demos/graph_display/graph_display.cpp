#include <tulip/PluginLoaderTxt.h>
#include <tulip/PluginLibraryLoader.h>
#include <tulip/GlMainWidget.h>
#include <tulip/MouseInteractors.h>
#include <tulip/TlpQtTools.h>

#include <QApplication>
#include <QString>

#include <iostream>

using namespace tlp;
using namespace std;

int main(int argc, char** argv) {
  /*
   A QApplication must always be declared at the beginning of the main function if you intend to use the tulip-gui library
   This must be done before calling tlp::initTulipSoftware()
   */
  QApplication app(argc,argv);

  /*
   Initialize the library and load all plugins
   */
  tlp::initTulipSoftware();

  /*
   Load the file passed as first argument into a graph.
   This method will select the default Tulip algorithm plugin (TLP)
   */
  if(QApplication::arguments().size()!=2) {
      cout << "Usage: " <<  QApplication::arguments()[0].toStdString() <<" tlp_file (.tlp, .tlp.gz)" << endl;
      exit(EXIT_FAILURE);
  }
  QString filename = QApplication::arguments()[1];
  if(!((filename.endsWith(".tlp"))||(filename.endsWith(".tlp.gz")))) {
      cout << "File " << filename.toStdString() << " not compatible. Use a tlp file or a tlp.gz file" << endl;
      exit(EXIT_FAILURE);
  }
  Graph* g = tlp::loadGraph(filename.toStdString());

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
