#include <QtGui/qapplication.h>
#include <tulip/Graph.h>
#include <tulip/TlpTools.h>
#include <tulip/TlpQtTools.h>
#include <tulip/NodeLinkDiagramComponent.h>
#include <tulip/GlMainViewInteractor.h>
#include <tulip/PluginLoaderTxt.h>

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
  NodeLinkDiagramComponent MainWin;
  QWidget *widget=MainWin.construct(NULL);
  //MainApp.setMainWidget(&MainWin);
  widget->resize(640,480);
  widget->show();

  if( argv[1][0] !='g' || argv[1][1] !='r' || argv[1][2] !='i' || argv[1][3] !='d'){
    importGraph(argv[1], MainWin.getGlMainWidget());
  }

  MainWin.init();
  InteractorNavigation interactor;
  MainWin.setActiveInteractor(&interactor);
  return MainApp.exec();
}
