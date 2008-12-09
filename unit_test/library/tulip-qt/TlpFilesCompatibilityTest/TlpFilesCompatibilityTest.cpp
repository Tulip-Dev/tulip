#include <QtCore/QObject>
#include <QtTest/QtTest>
#include <QtGui/QMainWindow>
#include <QtGui/QWorkspace>
#include <QtGui/QMenuBar>
#include <QtGui/QToolBar>

#include <tulip/Graph.h>
#include <tulip/GlyphManager.h>
#include <tulip/InteractorManager.h>
#include <tulip/ViewPluginsManager.h>
#include <tulip/Controller.h>
#include <tulip/MainController.h>
#include <tulip/ControllerPluginsManager.h>
#include <tulip/NodeLinkDiagramComponent.h>

using namespace std;
using namespace tlp;

class TlpFilesCompatibilityTest : public QObject {

  Q_OBJECT
private slots:
  void initTestCase();
  void testTlp_3_0_0B6();

private :
  DataSet *dataSet;
  Graph *graph;
  QMainWindow *mainWindow;
  QToolBar *graphToolBar;
  QWorkspace *workspace;
};

void TlpFilesCompatibilityTest::initTestCase() {
  initTulipLib();
  loadPlugins();
  GlyphManager::getInst().loadPlugins();   // software side plugins, i.e. glyphs
  InteractorManager::getInst().loadPlugins(); // interactors plugins
  ViewPluginsManager::getInst().loadPlugins(); // view plugins
  ControllerPluginsManager::getInst().loadPlugins(); // controller plugins


  dataSet = new DataSet();
  graph = tlp::newGraph();

  mainWindow =new QMainWindow();
  mainWindow->menuBar()->addAction("&Windows");
  graphToolBar=mainWindow->addToolBar("graphToolBar");
  workspace=new QWorkspace();
  mainWindow->setCentralWidget(workspace);
}
//==========================================================
void TlpFilesCompatibilityTest::testTlp_3_0_0B6() {
  dataSet->set("file::filename",string("../DATA/tlp_3_0_0B6.tlp"));
  tlp::importGraph("tlp", *dataSet, NULL ,graph);

  MainController *controller=(MainController*)ControllerPluginsManager::getInst().createController("MainController");
  controller->attachMainWindow(MainWindowFacade(mainWindow,graphToolBar,workspace));
  controller->setData(graph,*dataSet);
  QWidgetList widgetList=workspace->windowList();
  NodeLinkDiagramComponent *nldc=dynamic_cast<NodeLinkDiagramComponent*>(controller->getView(widgetList[0]));

  QRect rect=widgetList[0]->contentsRect();
  GlMainWidget *mainWidget=nldc->getGlMainWidget();
  vector<node> node;
  vector<edge> edge;
  mainWidget->doSelect(0,0,rect.width(),rect.height(),node,edge);
  QVERIFY(node.size()==3);
  QVERIFY(edge.size()==3);
  QVERIFY(mainWidget->getScene()->getBackgroundColor()==Color(0,0,0,255));
}
//==========================================================
QTEST_MAIN(TlpFilesCompatibilityTest)
#include "TlpFilesCompatibilityTest.moc"
