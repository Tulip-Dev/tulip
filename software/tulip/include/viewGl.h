//-*-c++-*-

#ifndef viewGl_included
#define viewGl_included
#include <map>

#if (__GNUC__ < 3)
#include <hash_map>
#else
#include <ext/hash_map>
#endif

#include <vector>
#include <QtGui/qdockwidget.h>
#include <QtGui/qsplitter.h>
#include <QtCore/qstring.h>
#include <QtGui/qmainwindow.h>
#include <QtGui/qmenu.h>
#include <QtAssistant/qassistantclient.h>
#include <string>
#include <tulip/Reflect.h>
#include <tulip/Graph.h>
#include <tulip/GlScene.h>
#include <tulip/Observable.h>
#include <tulip/GlSceneObserver.h>
#include <tulip/GWInteractor.h>
#include <PluginsUpdateChecker.h>
#include "TulipPluginLoader.h"
#include "TulipData.h"

namespace tlp {
  class GlGraph;
  class Morphing;
  class GridOptionsWidget;
  /*class GlMainWidget;
  class GlScene;
  class GlLayer;*/  
  class LayerManagerWidget;
  class View;
  class GlMainView;
  class SGHierarchyWidget;
  class PropertyDialog;
  class ElementPropertiesWidget;
#ifdef STATS_UI
  class TulipStat;
#endif
}

//class QWorkspace;
class QProgressDialog;
class QEvent;
class View3DSetup;
class Cluster;

// minimal structure to keep open files infos
struct viewGlFile {
  std::string name;
  std::string author;
  std::string comments;
};

///Widget for manipulation and visualization of a graph
class viewGl : public QMainWindow, public Ui::TulipData, tlp::Observer, tlp::GraphObserver, tlp::GlSceneObserver {
  Q_OBJECT;

public:
  viewGl(QWidget *parent=NULL);
  virtual ~viewGl();
  void setParameters(const tlp::DataSet &);
  bool eventFilter(QObject *, QEvent *);

protected:
  tlp::GridOptionsWidget *gridOptionsWidget;
  tlp::LayerManagerWidget *layerWidget;
  QWidget *aboutWidget;
  //tlp::GlMainWidget *glWidget;
  tlp::TulipPluginLoader pluginLoader;
  tlp::PluginsUpdateChecker *pluginsUpdateChecker;
  tlp::MultiServerManager *multiServerManager;
  bool elementsDisabled;

  QMenu layoutMenu;
  QMenu metricMenu;
  QMenu colorsMenu;
  QMenu generalMenu;
  QMenu sizesMenu;
  QMenu intMenu;
  QMenu stringMenu;
  QMenu importGraphMenu;
  QMenu exportGraphMenu;
  QMenu optionMenu;
  QMenu selectMenu;
  QMenu exportImageMenu;

  QDockWidget *tabWidgetDock;
  tlp::SGHierarchyWidget *clusterTreeWidget;
  tlp::PropertyDialog *propertiesWidget;
  tlp::ElementPropertiesWidget *eltProperties;
#ifdef STATS_UI
  tlp::TulipStat *statsWidget;
#endif

  //QMenu* windowsMenu;
  void focusInEvent ( QFocusEvent * );
  typedef std::set< tlp::Observable * >::iterator ObserverIterator;
  void update ( ObserverIterator begin, ObserverIterator end);
  void observableDestroyed(tlp::Observable *);
  void initObservers();
  void clearObservers();
  void enableQMenu(QMenu *, bool);
  void enableElements(bool);
  void setNavigateCaption(std::string);
  void initializeGraph(tlp::Graph *);
  void initializeGlScene(tlp::GlScene *);
  // GraphObserver interface
  /*void addNode (tlp::Graph *, const tlp::node);
  void addEdge (tlp::Graph *, const tlp::edge);
  void delNode (tlp::Graph *, const tlp::node);
  void delEdge (tlp::Graph *, const tlp::edge);
  void reverseEdge (tlp::Graph *, const tlp::edge);
  void destroy (tlp::Graph *);*/
  // GlSceneObserver interface
  /*void addLayer(tlp::GlScene*, const std::string&, tlp::GlLayer*);
    void modifyLayer(tlp::GlScene*, const std::string&, tlp::GlLayer*);*/

public slots:
  void startTulip();
  void fileOpen(std::string *,QString &);
  void closeEvent(QCloseEvent *e); 
  
protected slots:
  void helpIndex();
  void helpContents();
  void helpAbout();
  void plugins();
  void helpAssistantError(const QString &msg);
  void fileExit();
  void fileSave();
  void fileSaveAs();
  void windowActivated(QWidget *);
  void hierarchyChangeGraph(tlp::Graph *);
  void fileNew();
  void fileOpen();  
  void filePrint();
  void editCut();
  void editCopy();
  void editPaste();
  void editFind();
  void editCreateGroup();
  void editCreateSubGraph();
  void editDelSelection();
  void editReverseSelection();
  void editSelectAll();
  void editDeselectAll();
  void changeMetric(QAction*);
  void changeString(QAction*);
  void changeSizes(QAction*);
  void changeColors(QAction*);
  void changeInt(QAction*);
  void changeSelection(QAction*);
  void changeLayout(QAction*);
  void importGraph(QAction*);
  void exportGraph(QAction*);
  void exportImage(QAction*);
  void applyAlgorithm(QAction*);
  void outputEPS();
  void outputSVG();
  tlp::View* createView(const std::string &name,tlp::Graph *graph,std::string *xmlData=NULL);
  void addView(QAction *action);
  void displayView(tlp::View *view,const std::string &name);
  void changeInteractor(QAction*);
  void updateCurrentGraphInfos();
  void windowsMenuAboutToShow();
  void windowsMenuActivated(QAction*);
  void new3DView();
  void changeGraph(tlp::Graph *);
  void graphAboutToBeRemoved(tlp::Graph *);
  //void glMainWidgetClosing(GlMainWidget *, QCloseEvent *);
  void gridOptions();
  void isAcyclic();
  void isSimple();
  void isConnected();
  void isBiconnected();
  void isTriconnected();
  void isTree();
  void isFreeTree();
  void isPlanar();
  void isOuterPlanar();
  void makeAcyclic();
  void makeBiconnected();
  void makeSimple();
  void makeConnected();
  void makeDirected();
  void deletePluginsUpdateChecker();
  void showElementProperties(unsigned int eltId, bool isNode);

private:
  //void deleteElement(unsigned int , unsigned int , GlMainWidget *);
  //void selectElement(unsigned int , unsigned int , GlMainWidget *, bool);
  template<typename PROPERTY> bool changeProperty(std::string, std::string, bool = true, bool = false );
  //GlMainWidget *newOpenGlView(tlp::Graph *graph,const QString &);
  //void constructDefaultScene(GlMainWidget *glWidget);
  stdext::hash_map<unsigned int, viewGlFile> openFiles;
  void buildMenus();
  bool doFileSave();
  bool doFileSaveAs();
  bool doFileSave(std::string plugin, std::string filename, std::string author, std::string comments);
  bool askSaveGraph(const std::string name);
  bool closeWin();
  int alreadyTreated(std::set<unsigned int>, tlp::Graph *);
  unsigned int mouseClicX,mouseClicY;
  tlp::Morphing *morph;
  std::vector<tlp::Interactor *> *currentInteractors;
  tlp::GlMainView* initMainView(std::string *in=NULL);
  void installInteractors(tlp::View *view);
  void installEditMenu(tlp::View *view);

  QAssistantClient* assistant;
  unsigned int currentGraphNbNodes, currentGraphNbEdges;
  tlp::Graph* importedGraph;
  tlp::Graph* currentGraph;
  tlp::View *currentView;
};

#endif // viewGl_included

