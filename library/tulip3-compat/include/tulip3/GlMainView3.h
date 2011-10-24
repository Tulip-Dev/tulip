#ifndef Tulip_GlMainView3_H
#define Tulip_GlMainView3_H

#include "tulip/AbstractView.h"

#include <QtGui/QMainWindow>

#include <QtGui/QDockWidget>

class QFrame;
class QMenu;

namespace tlp {
class GWOverviewWidget;
class PropertyDialog;
class SGHierarchyWidget;
class ElementPropertiesWidget;
class RenderingParametersDialog;
class LayerManagerWidget;
class GridOptionsWidget;
class GlMainWidget;
}

using namespace tlp;

namespace tlp3 {


/** \brief Provide an abstract view with OpenGl rendering
 *
 * This class provide an abstract view with OpenGl rendering and overview
 * To do this rendering GlMainView3 class is use
 */
class TLP_QT_SCOPE GlMainView3 : public tlp::AbstractView {

  Q_OBJECT

public:
  /**
   * Basic contructor
   */
  GlMainView3();
  virtual ~GlMainView3();

  /**
   * Construct this view
   * \warning If your view inherit of GlMainView3 you must extend construct function and in this new function call GlMainView3::construct(QWidget *)
   */
  virtual QWidget *construct(QWidget *parent);

  /**
   * get the graph use in this view
   * \return the graph use in this view
   */
  Graph *getGraph();
  /**
   * You can call this function if you want to have the GlMainWidget of this view
   *   If this view doesn't use GlMainWidget : this function return NULL
   *   This function is use to export a view to a picture
   */
  virtual GlMainWidget *getGlMainWidget();
  /**
   * Take a snapshot of the view and put it in a picture
   * \deprecated In Tulip 4.x.y this function will be replace by void createPicture(const string &pictureName,int width, int height, bool center, int zoom, int xOffset, int yOffset)
   */
  virtual void createPicture(const std::string &pictureName,int width=0, int height=0);

  /**
   * Take a snapshot of the view like old createPicture function but we add some parameters
   * \param pictureName : name of the picture with extension, extension is used to encode the picture
   * \param width : width of the picture
   * \param height : height of the picture
   * \param center : if we do a center view before create picture or use the current zoom and pan
   * \param zoom : create a picture of a sub part of the view : with zoom=0 : create only one picture with entire view, with zoom=N : the view is cut into 2^N part in width and height
   * \param xOffset : this parameters is used to know the part of the view to render if zoom!=0 : xOffset must be : 0 <= xOffset < 2^zoom
   * \param yOffset : this parameters is used to know the part of the view to render if zoom!=0 : yOffset must be : 0 <= yOffset < 2^zoom
   */
  virtual bool createPicture(const std::string &pictureName,int width, int height,bool center, int zoom=1, int xOffset=0, int yOffset=0);

  /**
   * Take a snapshot of the view like old createPicture function and return a QImage
   * \param width : width of the picture
   * \param height : height of the picture
   * \param center : if we do a center view before create picture or use the current zoom and pan
   * \param zoom : create a picture of a sub part of the view : with zoom=0 : create only one picture with entire view, with zoom=N : the view is cut into 2^N part in width and height
   * \param xOffset : this parameters is used to know the part of the view to render if zoom!=0 : xOffset must be : 0 <= xOffset < 2^zoom
   * \param yOffset : this parameters is used to know the part of the view to render if zoom!=0 : yOffset must be : 0 <= yOffset < 2^zoom
   */
  virtual QImage createPicture(int width, int height,bool center, int zoom=1, int xOffset=0, int yOffset=0);

  /**
   * Build context menu for this view
   */
  virtual void buildContextMenu(QObject *object,QContextMenuEvent *event,QMenu *contextMenu);

  /**
   * Return the overview widget use by this view
   */
  GWOverviewWidget *getOverviewWidget() {
    return overviewWidget;
  }

  /**
   * Return QAction of overview use in the context menu
   */
  QAction *getOverviewAction() {
    return overviewAction;
  }

protected :

  GlMainWidget *mainWidget;
  QDockWidget *overviewDock;
  GWOverviewWidget *overviewWidget;
  QFrame *overviewFrame;
  QMenu *dialogMenu;

  QAction* overviewAction;

public slots:

  /**
   * Draw the OpenGl widget
   */
  virtual void draw();

  /**
   * Refresh the OpenGl widget
   */
  virtual void refresh();

  /**
   * Hide the overview
   */
  virtual void hideOverview(bool);

  virtual void displayOverview(bool display) {
    hideOverview(!display);
  }

  /**
   * This slot is call when a QAction in dialog sub menu of context menu is toggle
   */
  virtual void showDialog(QAction* action);

};

}

#endif
