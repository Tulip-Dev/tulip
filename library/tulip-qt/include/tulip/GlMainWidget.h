//-*-c++-*-

/**
 Author: David Auber
 Email : auber@labri.fr
 Last modification : 06/09/2005
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/

#ifndef Tulip_GLMAINWIDGET_H
#define Tulip_GLMAINWIDGET_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tulip/GlScene.h>

#include <QtOpenGL/qgl.h>
#include <QtCore/qpoint.h>
#include <QtGui/qaction.h>

#include "tulip/GWInteractor.h"
#include "tulip/View.h"
#include <tulip/GlHierarchyConvexHulls.h>

namespace tlp {

//class QTextView;

/** \addtogroup Tulip_Widgets */ 
/*@{*/
class TLP_QT_SIMPLE_SCOPE GlMainWidget : public QGLWidget {
  Q_OBJECT;
  
 public:
  GlMainWidget(QWidget *parent=NULL, const char *name=NULL);
  ~GlMainWidget();

  void setData(Graph *graph,std::string *in);
  Graph *getGraph();
  
  /**************************************
   * inherited methods overloading
   **************************************/
  /** \brief select nodes and edges in a region of the screen
   *
   *  select all nodes and edges lying in the area of the screen of given width and height,
   *  and with its upper-left corner at (x,y)
   *  \param sNode filled by the method with the nodes found in the region
   *  \param sEdge filled by the method with the edges found in the region
   */
  void doSelect(const int x, const int y, 
		const int width, const int height,
		std::vector<tlp::node> &sNode, std::vector<tlp::edge> &sEdge,		
		tlp::GlLayer* layer=NULL);
  /** \brief select a node or edge at a point
   *  select either a node or edge at point (x,y)
   *  \param type tells what has been found: NODE, EDGE
   *  \return true if something has been found, false otherwise
   */
  bool doSelect(const int x, const int y,
		tlp::ElementType &type, 
		tlp::node &,tlp::edge &,
		tlp::GlLayer* layer=NULL);
  /** 
   *  Take a snapshot of the Widget
   *  \return an array of dimension width*height*3 char (8bits per color RGB).
   *   The pointer has to be freed after (with free, not delete)
   **/
  unsigned char* getImage();
  // EPS output of a GlGraph
  bool outputEPS(int size, int doSort, const char *filename);
  // SVG output of a GlGraph
  bool outputSVG(int size, const char* filename);

  /**************************************
   * Management of threads
   **************************************/
  bool  isDrawing();
  void  stopDrawing();
  void  waitDrawing();

  /**
   * Function to do picking on entities.  It just calls
   * selectEntities on the GlScene instance.
   */
  bool selectGlEntities(const int x, const int y, 
			const int width, const int height,
			std::vector<tlp::GlEntity *>
			&pickedEntities,
			tlp::GlLayer* layer=NULL);
  /**
   * Function to do picking on entities.  It just calls
   * selectEntities on the GlScene instance with a small window of 
   * twelve pixels.
   */
  bool selectGlEntities(const int x, const int y, 
			std::vector<tlp::GlEntity *>
			&pickedEntities,
			tlp::GlLayer* layer=NULL);

  /**
   * return list of interactor installed on this widget
   */
  tlp::Iterator<tlp::Interactor *> *getInteractors() const;

  /**
   * Grab the image of this widget
   */
  virtual QImage grabFrameBuffer(bool withAlpha = false);

  /**
   * return the scene of this glMainWidget
   */
  tlp::GlScene* getScene(){return &scene;}

  /**
   * set if the label is rendered in this widget
   */
  void setViewLabel(bool viewLabel) {scene.setViewLabel(viewLabel);}

  /**
   * reimplement makeCuurent function of QGLWidget to inform TextureManager and DisplayListManager of context changement
   */
  virtual void makeCurrent();

public slots:
  /** 
   * Draw the graph, the augmented dispaly and the interactors
   */
  void draw();
  /**
   * That function is given for optimisation purpose. If the hardware enable it,
   * it enables to redraw only the Augmented display and the interactors and not the graph
   * it is really usefull for interactors such as zoom box etc..
   * Warning, if you change the graph or the porperties of element (Colors, size, etc...)
   * applying that fonction will not display the change, in that case, use the draw function.
   */
  void redraw();

  void closeEvent(QCloseEvent *e);
  /// install a clone of the interactor as event filter and assign the returned id
  tlp::Interactor::ID pushInteractor(tlp::Interactor *interactor);
  /// remove the last added interactor from the event filters list and delete it
  void popInteractor();
  /// remove the interactor with id from the event filters list and delete it
  void removeInteractor(tlp::Interactor::ID id);
  ///  remove all interactors and delete them, push a new one if any
  tlp::Interactor::ID resetInteractors(tlp::Interactor *interactor = NULL);
  /// remove all iteractors and delete them, then install clones of the interactors
  std::vector<tlp::Interactor::ID> resetInteractors(const std::vector<tlp::Interactor *>&interactors);

signals:
  void closing(GlMainWidget *, QCloseEvent *);
  void graphRedrawn(GlMainWidget *glWidget);

protected slots:
  void paintEvent( QPaintEvent* );

private:
  void  updateGL();
  void  glInit();
  void  glDraw();
  void  paintGL();
  void  resizeGL(int w, int h);
  void  setupOpenGlContext();
  void  initializeGL();
  void  computeInteractors();
  void  drawInteractors();

  tlp::GlScene scene;
  tlp::GlHierarchyConvexHulls hulls;
  bool _firstStepOfIncrementalRendering;
  QRegion _visibleArea;
  tlp::Interactor::ID _id;
  std::vector<tlp::Interactor *> _interactors;

};

}

/*@}*/
#endif
