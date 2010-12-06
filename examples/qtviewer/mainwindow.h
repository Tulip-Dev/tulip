#include <QtGui/QMainWindow>
#include <QtGui/QToolBar>
#include <tulip/Graph.h>
#include <tulip/Observable.h>
#include <tulip/Interactor.h>
#include <tulip/TlpQtTools.h>
#include <tulip/NodeLinkDiagramComponent.h>
#include <tulip/PluginLoaderTxt.h>
#include <tulip/InteractorManager.h>

using namespace std;
using namespace tlp;

typedef std::set< tlp::Observable * >::iterator ObserverIterator;

class QtViewerMainWindow : public QMainWindow, public Observer {
  
  Q_OBJECT

public :

  QtViewerMainWindow(const string &filename);
  ~QtViewerMainWindow();	

public slots:
  
  void observableDestroyed(Observable *) {}
  void changeInteractor();
  void update ( ObserverIterator begin, ObserverIterator end);		


private :

  void initObservers();
  void clearObservers();

  Graph *graph;
  NodeLinkDiagramComponent *nodeLinkView;
  QToolBar *toolBar;

};
