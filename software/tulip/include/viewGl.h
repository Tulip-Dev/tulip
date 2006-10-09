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
#if (QT_REL == 3)
#include <qlistview.h>
#include <qtextview.h> 
#include <qsplitter.h>
#include <qpopupmenu.h>
#include <qstring.h>
#include <qmainwindow.h>
#include <qassistantclient.h>
#else
#include <Qt3Support/q3listview.h>
#include <Qt3Support/q3textview.h>
#include <Qt3Support/q3dockwindow.h>
#include <Qt3Support/q3popupmenu.h>
#include <QtGui/qsplitter.h>
#include <QtCore/qstring.h>
#include <QtGui/qmainwindow.h>
#include <QtAssistant/qassistantclient.h>
#include "tulip/Qt3ForTulip.h"
#endif
#include <string>
#include <tulip/Reflect.h>
#include <tulip/Graph.h>
#include <tulip/Observable.h>
#include <tulip/GWInteractor.h>
#include "TulipData.h"

namespace tlp {
class GlGraph;
class Morphing;
class GridOptionsWidget;
}

class SGHierarchyWidget;
class ElementPropertiesWidget;
class GlGraphWidget;
class TulipStats;
class PropertyDialog;
class QWorkspace;
class QProgressDialog;
class QEvent;
class View3DSetup;
class Cluster;
class ToolBar;
class GWOverviewWidget;

// minimal structure to keep open files infos
struct viewGlFile {
  std::string name;
  std::string author;
  std::string comments;
};

///Widget for manipulation and visualization of a graph
class viewGl : public TulipData, tlp::Observer {
  Q_OBJECT;

public:
  viewGl(QWidget *parent=NULL, const char *name=NULL);
  virtual ~viewGl();
  void setParameters(const tlp::DataSet);
  bool eventFilter(QObject *, QEvent *);

protected:
  tlp::GridOptionsWidget *gridOptionsWidget;
  SGHierarchyWidget *clusterTreeWidget;
#ifdef STATS_UI
  tlp::TulipStats *statsWidget;
#endif
  ToolBar *mouseToolBar;
  GWOverviewWidget *overviewWidget;
  QWidget *aboutWidget;
  QDockWindow *overviewDock;
  QDockWindow *tabWidgetDock;
  QDockWindow *mouseToolBarDock;
  GlGraphWidget *glWidget;
  PropertyDialog *propertiesWidget;
  ElementPropertiesWidget *nodeProperties;
  tlp::Graph * copyCutPasteGraph;
  bool elementsDisabled;

  QPopupMenu layoutMenu;
  QPopupMenu metricMenu;
  QPopupMenu colorsMenu;
  QPopupMenu clusteringMenu;
  QPopupMenu sizesMenu;
  QPopupMenu intMenu;
  QPopupMenu stringMenu;
  QPopupMenu importGraphMenu;
  QPopupMenu exportGraphMenu;
  QPopupMenu optionMenu;
  QPopupMenu selectMenu;
  QPopupMenu exportImageMenu;
  QPopupMenu dialogMenu;

  //QPopupMenu* windowsMenu;
  void focusInEvent ( QFocusEvent * );
  typedef std::set< tlp::Observable * >::iterator ObserverIterator;
  void update ( ObserverIterator begin, ObserverIterator end);
  void observableDestroyed(tlp::Observable *);
  void initObservers();
  void clearObservers();
  void enableQPopupMenu(QPopupMenu *, bool);
  void enableElements(bool);
  void setNavigateCaption(std::string);
  void initializeGraph(tlp::Graph *);
  void initializeGlGraph(tlp::GlGraph *);

public slots:
  void startTulip();
  void fileOpen(std::string *,QString &);
  void closeEvent(QCloseEvent *e); 
  
protected slots:
  void helpIndex();
  void helpContents();
  void helpAbout();
  void helpAssistantError(const QString &msg);
  void fileExit();
  void fileSave();
  void fileSaveAs();
  int  closeWin();
  void windowActivated(QWidget *);
  void hierarchyChangeGraph(tlp::Graph *);
  void fileNew();
  void fileOpen();  
  void filePrint();
  void editCut();
  void editCopy();
  void editPaste();
  void editFind();
  void changeMetric(int);
  void changeString(int);
  void changeSizes(int);
  void changeColors(int);
  void changeInt(int);
  void changeSelection(int);
  void changeLayout(int);
  void importGraph(int);
  void exportGraph(int);
  void exportImage(int);
  void makeClustering(int );
  void outputEPS();
  void showDialog(int);
  void redrawView();
  void centerView();
  void updateStatusBar();
  void deselectALL();
  void reverseSelection();
  void delSelection();
  void newSubgraph();
  void reverseSelectedEdgeDirection();
  void windowsMenuAboutToShow();
  void windowsMenuActivated( int id );
  void new3DView();
  void goInside();
  void changeGraph(tlp::Graph *);
  void graphAboutToBeRemoved(tlp::Graph *);
  void glGraphWidgetClosed(GlGraphWidget *);
  void ungroup();  
  void group();  
  void gridOptions();
  void deleteElement();
  void selectElement();
  void addRemoveElement();
  void isAcyclic();
  void isSimple();
  void isConnected();
  void isBiconnected();
  void isTriconnected();
  void isTree();
  void isPlanar();
  void makeAcyclic();
  void makeBiconnected();
  //void makeTriangulated();
  void makeSimple();
  void makeConnected();
  void showElementProperties();

private:
  void deleteElement(unsigned int , unsigned int , GlGraphWidget *);
  void selectElement(unsigned int , unsigned int , GlGraphWidget *, bool);
  template<typename PROPERTY> bool changeProperty(std::string, std::string, bool = true, bool = false );
  GlGraphWidget *newOpenGlView(tlp::Graph *graph,const QString &);
  std::string newName();
  stdext::hash_map<unsigned int, viewGlFile> openFiles;
  void buildMenus();
  bool fileSave(std::string plugin, std::string filename, std::string author, std::string comments);
  int alreadyTreated(std::set<unsigned int>, tlp::Graph *);
  unsigned int mouseClicX,mouseClicY;
  tlp::Morphing *morph;

  QAssistantClient* assistant;
};

#endif // viewGl_included

