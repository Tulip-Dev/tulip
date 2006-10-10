#if (QT_REL == 4)
#include <QtGui/qapplication.h>
#else
#include <qapplication.h>
#endif
#include <tulip/Graph.h>
#include <tulip/TlpTools.h>
#include <tulip/GlGraphWidget.h>
#include <tulip/MouseObject.h>

using namespace std;

/*******************************************************************/
void importGraph(const string &filename, GlGraph *render) {
  DataSet dataSet;
  dataSet.set("file::filename", filename);
  Graph *newGraph=tlp::importGraph("tlp", dataSet, NULL);
  if (newGraph!=0) {
    render->setGraph(newGraph);
    
    DataSet glGraphData;
    if (dataSet.get<DataSet>("displaying", glGraphData))
      render->setParameters(glGraphData);
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
  /* tlp::loadPlugins();   // library side plugins
     GlGraph::loadPlugins(); //Glyoh plugins */
  /****************************************************/
  GlGraphWidget MainWin;
  
  MainApp.setMainWidget(&MainWin);
  MainWin.resize(640,480);
  MainWin.show();

  if( argv[1][0] !='g' || argv[1][1] !='r' || argv[1][2] !='i' || argv[1][3] !='d'){
    importGraph(argv[1], &MainWin);
  }

  MainWin.centerScene();
  MouseGraphNavigate m;
  MainWin.pushInteractor(&m);
  return MainApp.exec();
}
