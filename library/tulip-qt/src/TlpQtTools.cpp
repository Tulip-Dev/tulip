#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <QtOpenGL/QGLPixelBuffer>

#include "tulip/TlpQtTools.h"

#include <tulip/Reflect.h>

#include "tulip/Interactor.h"
#include "tulip/View.h"
#include "tulip/Controller.h"


/**
 * For openDataSetDialog function : see OpenDataSet.cpp
 */

using namespace std;


namespace tlp {

  void openGraphOnGlMainWidget(Graph *graph,DataSet *dataSet,GlMainWidget *glMainWidget){
    glMainWidget->setData(graph,dataSet);
  }

  void loadInteractorPluginsFromDir(std::string dir,PluginLoader *loader) {
    InteractorFactory::initFactory();
    loadPluginsFromDir(dir, "Interactor", loader);
  }
  void loadViewPluginsFromDir(std::string dir,PluginLoader *loader) {
    ViewFactory::initFactory();
    loadPluginsFromDir(dir, "View", loader);
  }
  void loadControllerPluginsFromDir(std::string dir,PluginLoader *loader) {
    ControllerFactory::initFactory();
    loadPluginsFromDir(dir, "Controller", loader);
  }
  bool canUseQGLPixelBuffer() {
    QGLPixelBuffer glFrameBuf(2,2);
    return glFrameBuf.isValid();
  }

}
