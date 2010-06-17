/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */

/**
 Author: Morgan Mathiaut
 Email : mathiaut@labri.fr
 Last modification : 14/05/2008
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
*/

#ifndef Tulip_GLMAINVIEW_H
#define Tulip_GLMAINVIEW_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "tulip/GlSceneObserver.h"

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

  /** \brief Provide an abstract view with OpenGl rendering
   *
   * This class provide an abstract view with OpenGl rendering and overview
   * To do this rendering GlMainView class is use
   */
  class TLP_QT_SCOPE GlMainView : public AbstractView, public GlSceneObserver {

    Q_OBJECT;

  public:
    /**
     * Basic contructor
     */
    GlMainView();
    virtual ~GlMainView();

    /**
     * Construct this view
     * \warning If your view inherit of GlMainView you must extend construct function and in this new function call GlMainView::construct(QWidget *)
     */
    virtual QWidget *construct(QWidget *parent);

    /**
     * get the graph use in this view
     * \return the graph use in this view
     */
    Graph *getGraph();
    /**
     * get GlMainWidget use by this view
     * \return the GlMainWidget use in this view
     */
    GlMainWidget *getGlMainWidget();
    /**
     * Take a snapshot of the view and put it in a picture
     */
    virtual void createPicture(const std::string &pictureName,int width=0, int height=0);

    /**
     * Build context menu for this view
     */
    virtual void buildContextMenu(QObject *object,QMouseEvent *event,QMenu *contextMenu);

    /**
     * Return the overview widget use by this view
     */
    GWOverviewWidget *getOverviewWidget() {return overviewWidget;}

    /**
     * Return QAction of overview use in the context menu
     */
    QAction *getOverviewAction() {return overviewAction;}

  protected :

    /**
     * function call by GlSceneObserver
     */
    virtual void addLayer(tlp::GlScene*, const std::string&, tlp::GlLayer*) {}
    /**
     * function call by GlSceneObserver
     */
    virtual void modifyLayer(tlp::GlScene*, const std::string&, tlp::GlLayer*) {}

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

    /**
     * This slot is call when a QAction in dialog sub menu of context menu is toggle
     */
    virtual void showDialog(QAction* action);

  };

}

#endif
