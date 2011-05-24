/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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

#include <tulip/GlScene.h>

#include <QtOpenGL/qgl.h>
#include <QtCore/qpoint.h>
#include <QtGui/qaction.h>
#include <QtOpenGL/QGLFramebufferObject>

#include "tulip/AbstractView.h"
#include "tulip/ForegroundEntity.h"

class QGLFramebufferObject;

namespace tlp {

class GlCompositeHierarchyManager;


//class QTextView;

/** \addtogroup Tulip_Widgets */
/*@{*/
/** \brief Widget of visualisation
 *
 * This widget provide a simple system to display visualisation
 * If you only want to visualise a graph, you can call the setGraph function
 * But in other cases you can directly use the GlScene of the GlMainWidget
 * \see GlScene
 *
 * After scene construction you can perform some operation on GlMainWidgte :
 * - Selection with doSelect() and selectGlEntities()
 * - Image output with getImage(), createPicture(), outputSVG() and outputEPS()
 * - Texture output with createTexture()
 * - others operation on GlScene and QGlWidget
 *
 */

class TLP_QT_SIMPLE_SCOPE GlMainWidget : public QGLWidget {
	Q_OBJECT;

public:
	/**
	 * Construct the GlMainWidget
	 */
  GlMainWidget(QWidget *parent,View *view=NULL);
	~GlMainWidget();

	/**
	 * Set graph and data of GlMainWidget
	 */
	void setData(Graph *graph,DataSet dataSet);
	/**
	 * Get data of GlMainWidget
	 */
	DataSet getData();
	/**
	 * Set the graph
	 */
	void setGraph(Graph *graph);
	/**
	 * GetGraph of GlMainWidget
	 */
	Graph *getGraph();

	/**
	 * Get the view who use this GlMainWidget
	 */
	View *getView() {return view;}

	/**
	 * Get the scene of this glMainWidget
	 */
	tlp::GlScene* getScene(){return &scene;}

	/**
	 * add a foregroundEntity to this GlMainWidget
	 */
	void addForegroundEntity(ForegroundEntity *entity) {foregroundEntity.push_back(entity);}

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

	/**
	 * Compute real texture size with given height and width
	 */
	static void getTextureRealSize(int width, int height, int &textureRealWidth, int &textureRealHeight);

	/**
	 *
	 */
	void getTextureShift(int width, int height,float &xTextureShift, float &yTextureShift);

	/**
	 * Take a snapshot of the Widget and put it in an OpenGl texture
	 */
	QGLFramebufferObject *createTexture(const std::string &textureName,int width, int height);
	/**
	 * Take a snapshot of the Widget and put it in a picture
	 */
	void createPicture(const std::string &pictureName,int width, int height,bool center=true, int zoom=1, int xDec=0, int yDec=0);

	/**
	 * Take a snapshot of the Widget and return it
	 */
	QImage createPicture(int width, int height,bool center=true, int zoom=1, int xDec=0, int yDec=0);

	/**
	 * Function to do picking on entities.  It just calls
	 * selectEntities on the GlScene instance.
	 */
	bool selectGlEntities(const int x, const int y,
			const int width, const int height,
			std::vector<tlp::GlSimpleEntity *>
	&pickedEntities,
	tlp::GlLayer* layer=NULL);
	/**
	 * Function to do picking on entities.  It just calls
	 * selectEntities on the GlScene instance with a small window of
	 * twelve pixels.
	 */
	bool selectGlEntities(const int x, const int y,
			std::vector<tlp::GlSimpleEntity *>
	&pickedEntities,
	tlp::GlLayer* layer=NULL);

	/**
	 * Grab the image of this widget
	 */
	virtual QImage grabFrameBuffer(bool withAlpha = false);

	/**
	 * set if the label is rendered in this widget
	 */
	void setViewLabel(bool viewLabel) {scene.setViewLabel(viewLabel);}

	/**
	 * Extend makeCurrent function of QGLWidget to inform TextureManager and DisplayListManager of context changement
	 */
	virtual void makeCurrent();

	/**
	 * Resize openGL view
	 */
	void  resizeGL(int w, int h);

	/**
	 * Compute interactors before drawing
	 */
	void  computeInteractors();

	/**
	 * Draw interactors
	 */
	void  drawInteractors();

	/**
	 * Draw foreground entities
	 */
	void drawForegroundEntities();

	void useHulls(bool hasHulls);
	bool hasHulls() const;

private:

	void  updateGL();
	void  glInit();
	void  glDraw();
	void  paintGL();
	void  setupOpenGlContext();
	void  initializeGL();
	void createRenderingStore(int width, int height);

	tlp::GlScene scene;
	QRegion _visibleArea;
  View *view;
	int widthStored;
	int heightStored;
	char *renderingStore;
	bool frameBufferStored;
	std::vector<ForegroundEntity *> foregroundEntity;
	GlCompositeHierarchyManager* manager;
	bool _hasHulls;
	bool useFramebufferObject;
	QGLFramebufferObject *glFrameBuf;
  static bool inRendering;
  int renderingNumber;

public slots:
/**
 * Draw the graph, the augmented dispaly and the interactors
 */
void draw(bool graphChanged=true);
/**
 * That function is given for optimisation purpose. If the hardware enable it,
 * it enables to redraw only the Augmented display and the interactors and not the graph
 * it is really usefull for interactors such as zoom box etc..
 * Warning, if you change the graph or the porperties of element (Colors, size, etc...)
 * applying that fonction will not display the change, in that case, use the draw function.
 */
void redraw();

void closeEvent(QCloseEvent *e);

protected slots:
void paintEvent( QPaintEvent* );
void viewDrawnSlot(GlMainWidget *glWidget,bool graphChanged) {emit graphRedrawn(glWidget,graphChanged);}

signals:
void closing(GlMainWidget *, QCloseEvent *);
void graphRedrawn(GlMainWidget *glWidget,bool graphChanged);
void viewRedrawn(GlMainWidget *glWidget);
void viewDrawn(GlMainWidget *glWidget,bool graphChanged);

public :
static QGLWidget* getFirstQGLWidget();

private :
static QGLWidget *firstQGLWidget;

};

}

/*@}*/
#endif
