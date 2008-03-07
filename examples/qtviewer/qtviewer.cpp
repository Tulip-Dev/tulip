#include <QtGui/qapplication.h>
#include <tulip/Graph.h>
#include <tulip/TlpTools.h>
#include <tulip/TlpQtTools.h>
#include <tulip/GlGraphWidget.h>
#include <tulip/MouseInteractors.h>
#include <tulip/PluginLoaderTxt.h>
#include <tulip/GlyphManager.h>
#include <tulip/GlDisplayListManager.h>
#include <tulip/GlTextureManager.h>
#include <tulip/GlScene.h>
#include <tulip/GlLayer.h>

using namespace std;
using namespace tlp;

/*******************************************************************/
void importGraph(const string &filename, GlMainWidget *glw) {
  DataSet dataSet;
  dataSet.set("file::filename", filename);
  Graph *newGraph = tlp::importGraph("tlp", dataSet, NULL);
  if (newGraph != 0) {
    openGraphOnGlMainWidget(newGraph,&dataSet,glw);
  }
}
/*******************************************************************/
int main(int argc,char ** argv ){
  
  QApplication MainApp(argc,argv);
  
  if (argc<2) {
    cerr << "usage :" << endl;
    cerr << "\t " << argv[0] << " <filename>" << endl;
    cerr << "file must be in tlp format" << endl;
    exit(1);
  }

  /****************************************************/
  tlp::initTulipLib();

  PluginLoaderTxt txtPlug;
  tlp::loadPlugins(&txtPlug);   // library side plugins
  GlyphManager::getInst().loadPlugins(&txtPlug);   // software side plugins, i.e. glyphs
  //  GlGraph::loadPlugins(); //Glyoh plugins */
  /****************************************************/
  GlMainWidget MainWin;
  
  MainApp.setMainWidget(&MainWin);
  MainWin.resize(640,480);
  MainWin.show();

  if( argv[1][0] !='g' || argv[1][1] !='r' || argv[1][2] !='i' || argv[1][3] !='d'){
    importGraph(argv[1], &MainWin);
  }

  MainWin.getScene()->centerScene();
  MouseNKeysNavigator m;
  MainWin.pushInteractor(&m);
  return MainApp.exec();
}
