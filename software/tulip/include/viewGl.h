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
#ifdef STATS_UI
class TulipStats;
#endif
class GlScene;
class GlLayer;  
}

class SGHierarchyWidget;
class ElementPropertiesWidget;
class GlMainWidget;
class TulipStats;
class PropertyDialog;
class QWorkspace;
class QProgressDialog;
class QEvent;
class View3DSetup;
class Cluster;
class GWOverviewWidget;
class LayerManagerWidget;

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
  SGHierarchyWidget *clusterTreeWidget;
#ifdef STATS_UI
  tlp::TulipStats *statsWidget;
#endif
  GWOverviewWidget *overviewWidget;
  LayerManagerWidget *layerWidget;
  QWidget *aboutWidget;
  QDockWidget *overviewDock;
  QDockWidget *tabWidgetDock;
  GlMainWidget *glWidget;
  PropertyDialog *propertiesWidget;
  ElementPropertiesWidget *eltProperties;
  tlp::Graph * copyCutPasteGraph;
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
  QAction *renderingParametersDialogAction;

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
  void addNode (tlp::Graph *, const tlp::node);
  void addEdge (tlp::Graph *, const tlp::edge);
  void delNode (tlp::Graph *, const tlp::node);
  void delEdge (tlp::Graph *, const tlp::edge);
  void reverseEdge (tlp::Graph *, const tlp::edge);
  void destroy (tlp::Graph *);
  // GlSceneObserver interface
  void addLayer(tlp::GlScene*, const std::string&, tlp::GlLayer*);
  void modifyLayer(tlp::GlScene*, const std::string&, tlp::GlLayer*);

public slots:
  void startTulip();
  void fileOpen(std::string *,QString &);
  void closeEvent(QCloseEvent *e); 
  void setSelect();
  void setAddEdge();
  void setEditEdgeBend();
  void setAddNode();
  void setDelete();
  void setZoomBox();
  void setMoveSelection();
  void setSelection();
  void setMagicSelection();
  void setGraphNavigate();
  void showElementProperties(unsigned int eltId, bool isNode);
  
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
  void showDialog(QAction*);
  void redrawView();
  void centerView();
  void updateCurrentGraphInfos();
  void selectAll();
  void deselectAll();
  void reverseSelection();
  void delSelection();
  void newSubgraph();
  void reverseSelectedEdgeDirection();
  void windowsMenuAboutToShow();
  void windowsMenuActivated(QAction*);
  void new3DView();
  void changeGraph(tlp::Graph *);
  void graphAboutToBeRemoved(tlp::Graph *);
  void glMainWidgetClosing(GlMainWidget *, QCloseEvent *);
  void group();  
  void gridOptions();
  void isAcyclic();
  void isSimple();
  void isConnected();
  void isBiconnected();
  void isTriconnected();
  void isTree();
  void isFreeTree();
  void isPlanar();
  void makeAcyclic();
  void makeBiconnected();
  void makeSimple();
  void makeConnected();
  void makeDirected();
  void deletePluginsUpdateChecker();

private:
  void deleteElement(unsigned int , unsigned int , GlMainWidget *);
  void selectElement(unsigned int , unsigned int , GlMainWidget *, bool);
  template<typename PROPERTY> bool changeProperty(std::string, std::string, bool = true, bool = false );
  GlMainWidget *newOpenGlView(tlp::Graph *graph,const QString &);
  void constructDefaultScene(GlMainWidget *glWidget);
  std::string newName();
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
  std::vector<tlp::GWInteractor *> *currentInteractors;
  void setCurrentInteractors(std::vector<tlp::GWInteractor *> *interactors);
  void deleteInteractors(std::vector<tlp::GWInteractor *> &interactors);

  QAssistantClient* assistant;
  unsigned int currentGraphNbNodes, currentGraphNbEdges;
  tlp::Graph* importedGraph;
};

#endif // viewGl_included

