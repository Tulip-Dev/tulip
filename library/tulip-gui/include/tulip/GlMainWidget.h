/*
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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

#ifndef Tulip_GLMAINWIDGET_H
#define Tulip_GLMAINWIDGET_H


#include <QGLWidget>

#include <tulip/tulipconf.h>
#include <tulip/GlScene.h>
#include <tulip/Graph.h>

class QGLFramebufferObject;

namespace tlp {

class View;
class GlCompositeHierarchyManager;

/** @ingroup OpenGL
 * @brief This widget provide a simple system to visualize data/graph with OpenGL 3D engine
 *
 * This widget is an interface between Qt Widget system and tulip OpenGL engine
 * The central object of GlMainWidget is the GlScene member
 * @see GlScene
 *
 * To use this class you have to :
 *   - create a GlMainWidget
 *   - get the GlScene with getScene() function
 *   - add GlLayer and GlEntity to this scene
 *   - call centerScene() to compute a good GlCamera
 *   - see the result
 *
 * @see GlLayer
 * @see GlSimpleEntity
 *
 *
 * If you only want to visualize a graph, you can call the setGraph function
 *
 *
 * After scene construction you can perform some operation on GlMainWidget :
 * - Selection with doSelect() and selectGlEntities()
 * - Image output with getImage(), createPicture(), outputSVG() and outputEPS()
 * - Texture output with createTexture()
 * - others operation on GlScene and QGlWidget
 */
class TLP_QT_SCOPE GlMainWidget : public QGLWidget {
  Q_OBJECT

public:
  /**
    * @brief Configure the rendering process ( see render function)
    * @see render
    **/
  enum RenderingOption {
    RenderScene=0x1, /** Force to render the graph even if there is a previous buffered render. You need to call this option if the graph is updated to regenerate the buffer. If not set try to use the last buffered graph render, if there is no valid buffer this flag is forced. **/
    SwapBuffers=0x2 /** Call the swapBuffer functions at the end of the rendering process. If the disabled it's up to you to call the swapBuffer function. **/
  };
  Q_DECLARE_FLAGS ( RenderingOptions, RenderingOption )

  /**
   * @brief Constructor of GlMainWidget
   *
   * Create a GlMainWidget with the GlScene associated to it
   * @param parent Qt Widget parent system
   * @param view if you want to link this GlMainWidget to a view : use this parameter
   */
  GlMainWidget(QWidget *parent=NULL, View *view=NULL);
  ~GlMainWidget();

  /**
   * @brief Get the GlScene of this GlMainWidget
   * You have to add yours GlLayer and GlEntity to this GlScene
   * At the construction this GlScene is empty
   * @see GlScene
   * @see GlScene::createLayer(const std::string &name)
   * @see GlLayer::addGlEntity(GlSimpleEntity *entity,const std::string& name)
   */
  tlp::GlScene* getScene() {
    return &scene;
  }

  /** @brief Select nodes and edges in a region of the screen
   *
   *  Select all nodes and edges lying in the area of the screen of given width and height,
   *  and with its upper-left corner at (x,y)
   *  @param selectedNodes filled by the method with the nodes found in the region
   *  @param selectedEdges filled by the method with the edges found in the region
   *  @param layer specify the layer in which to perform the picking
   *  @param pickNodes enable or disable the picking of nodes
   *  @param pickEdges enable or disable the picking of edges
   */
  void pickNodesEdges(const int x, const int y,
                      const int width, const int height,
                      std::vector<SelectedEntity> &selectedNodes, std::vector<SelectedEntity> &selectedEdges,
                      tlp::GlLayer* layer=NULL, bool pickNodes=true, bool pickEdges=true);

  /** @brief Select a node or edge at a point
   *  Try to select at point (x,y) a node in the first place then if no result try to select an edge
   *  @param type tells what has been found: NODE, EDGE
   *  @param layer specify the layer in which to perform the picking
   *  @param pickNodes enable or disable the picking of nodes
   *  @param pickEdges enable or disable the picking of edges
   *  @return true if something has been found, false otherwise
   */
  bool pickNodesEdges(const int x, const int y,
                      SelectedEntity &selectedEntity,
                      tlp::GlLayer* layer=NULL,
                      bool pickNodes=true, bool pickEdges=true);

  /**
   * @deprecated this function should not be used anymore, use pickNodesEdges()
   */
  _DEPRECATED void doSelect(const int x, const int y,
                            const int width, const int height,
                            std::vector<tlp::node> &sNode, std::vector<tlp::edge> &sEdge,
                            tlp::GlLayer* layer=NULL);

  /**
   * @deprecated this function should not be used anymore, use pickNodesEdges()
   */
  _DEPRECATED bool doSelect(const int x, const int y,
                            tlp::ElementType &type,
                            tlp::node &n,tlp::edge &e,
                            tlp::GlLayer* layer=NULL);

  /**
   * @brief EPS output of the GlMainWidget
   */
  bool outputEPS(int size, int doSort, const char *filename);
  /**
   * @brief SVG output of the GlMainWidget
   */
  bool outputSVG(int size, const char* filename);

  /**
   * @brief Compute texture size in power of two with given height and width
   * For example if you set width to 94 and height to 256, this function set textureRealWidth to 128 and textureRealHeight to 256
   */
  static void getTextureRealSize(int width, int height, int &textureRealWidth, int &textureRealHeight);

  /**
   * @brief Take a snapshot of the Widget and put it in an OpenGl texture
   * @param width power of two number (for example 256)
   * @param height power of two number (for example 256)
   * You can use this texture with Tulip texture system
   * @see GlTextureManager
   */
  QGLFramebufferObject *createTexture(const std::string &textureName,int width, int height);
  /**
   * @brief Take a snapshot of the Widget and put it in a picture
   * @param width size
   * @param height size
   * @param center if true this function call a centerScene() before picture output
   */
  void createPicture(const std::string &pictureName,int width, int height,bool center=true);

  /**
   * Take a snapshot of the Widget and return it
   * @param width size
   * @param height size
   * @param center if true this function call a centerScene() before picture output
   */
  QImage createPicture(int width, int height, bool center=true);

  /**
   * @brief Function to do picking on entities.
   * It just calls selectEntities on the GlScene instance.
   * @param x screen coordinates
   * @param y screen coordinates
   * @param width screen size
   * @param height screen size
   * @param pickedEntities filled with entity under the selection screen rectangle
   * @param layer if you want to do the selection only on one GlLayer
   */
  bool pickGlEntities(const int x, const int y,
                      const int width, const int height,
                      std::vector<SelectedEntity>
                      &pickedEntities,
                      tlp::GlLayer* layer=NULL);
  /**
   * @brief Function to do picking on entities.
   * It just calls selectEntities on the GlScene instance with a small window of twelve pixels.
   * @param x screen coordinates
   * @param y screen coordinates
   * @param pickedEntities filled with entity under the selection screen rectangle
   * @param layer if you want to do the selection only on one GlLayer
   */
  bool pickGlEntities(const int x, const int y,
                      std::vector<SelectedEntity>
                      &pickedEntities,
                      tlp::GlLayer* layer=NULL);

  /**
   * @deprecated this function should not be used anymore, please use pickGlEntities() instead.
   */
  _DEPRECATED bool selectGlEntities(const int x, const int y,
                                    const int width, const int height,
                                    std::vector<GlSimpleEntity*> &pickedEntities,
                                    tlp::GlLayer* layer=NULL) {
    std::vector<SelectedEntity> entities;
    pickGlEntities(x,y,width,height,entities,layer);
    bool foundEntity=false;

    for(std::vector<SelectedEntity>::iterator it=entities.begin(); it!=entities.end(); ++it) {
      if((*it).getEntityType()==SelectedEntity::SIMPLE_ENTITY_SELECTED) {
        pickedEntities.push_back((*it).getSimpleEntity());
        foundEntity=true;
      }
    }

    return foundEntity;
  }

  /**
   * @deprecated this function should not be used anymore, please use pickGlEntities() instead.
   */
  _DEPRECATED bool selectGlEntities(const int x, const int y,
                                    std::vector<GlSimpleEntity*> &pickedEntities,
                                    tlp::GlLayer* layer=NULL) {
    std::vector<SelectedEntity> entities;
    pickGlEntities(x,y,entities,layer);
    bool foundEntity=false;

    for(std::vector<SelectedEntity>::iterator it=entities.begin(); it!=entities.end(); ++it) {
      if((*it).getEntityType()==SelectedEntity::SIMPLE_ENTITY_SELECTED) {
        pickedEntities.push_back((*it).getSimpleEntity());
        foundEntity=true;
      }
    }

    return foundEntity;
  }

  /**
   * Grab the FrameBuffer of this GlMainWidget
   * @param withAlpha use alpha chanel
   */
  virtual QImage grabFrameBuffer(bool withAlpha = false);


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
    * @brief This function performs all the rendering process of the graph.
    * Use this function only for advanced purpose, if you want to perform simple rendering use the draw or redraw functions instead.
    * @param options Configure the rendering process, see the RenderingOption documentation for more information on each rendering option effect.
    * @see RenderingOption
    * @param checkVisibility If check visibility is set as true : the engine check if GlMainWidget QWidget is visible. If set at false : the engine render the scene in all cases
    **/
  void render(RenderingOptions options=RenderingOptions(RenderScene|SwapBuffers), bool checkVisibility=true);

  /**
   * @brief Specify if the scene point of view must be kept
   * when changing between graphs belonging to the same hierarchy
   */
  void setKeepScenePointOfViewOnSubgraphChanging(bool);
  /**
   * @brief Returns if the scene point of view must be kept
   * when changing between graphs belonging to the same hierarchy
   */
  bool keepScenePointOfViewOnSubgraphChanging() const;

  /**
   * @brief Specify if an advanced technique for better scene anti-aliasing has to be activated
   *
   * That option allows to obtain a better scene antialiasing through the use of offscreen rendering and sampling.
   * The result rendering will look better while the performance will be slightly altered.
   * That option is desactivated by default. Use it with caution as it could cause crashes with some buggy OpenGL drivers.
   */
  void setAdvancedAntiAliasing(bool advancedAntiAliasing) {
    this->advancedAntiAliasing = advancedAntiAliasing;
  }

  /**
   * Returns the advanced anti-aliasing technique state
   */
  bool advancedAntiAliasingActivated() const {
    return advancedAntiAliasing;
  }

private:

  void  setupOpenGlContext();
  void createRenderingStore(int width, int height);
  void deleteRenderingStore();

  tlp::GlScene scene;
  QRegion _visibleArea;
  View *view;
  int widthStored;
  int heightStored;
  unsigned char *renderingStore;
  bool frameBufferStored;
  bool useFramebufferObject;
  QGLFramebufferObject *glFrameBuf, *glFrameBuf2;
  static bool inRendering;
  bool keepPointOfViewOnSubgraphChanging;
  bool advancedAntiAliasing;

public slots:
  /**
   * Draw the GlScene and the interactors
   */
  void draw(bool graphChanged=true);
  /**
   * This function is given for optimisation purpose only. If the hardware enable it,
   * it enables to redraw only the Augmented display and the interactors and not the graph
   * it is really usefull for interactors such as zoom box etc..
   * Warning, if you change the graph or the porperties of element (Colors, size, etc...)
   * applying that fonction will not display the change, in that case, use the draw function.
   */
  void redraw();


  void closeEvent(QCloseEvent *e);

  /**
   * @brief Convenience function that call center function on the current scene, apply a zoom (if needed) and draw the view.
   * Same thing than
   * @code
   * getScene()->centerScene();
   * getScene()->zoomFactor();
   * draw();
   * @endcode
  **/
  void centerScene(bool graphChanged = false, float zoomFactor = 1.0);

  void emitGraphChanged();

protected slots:
  void paintEvent( QPaintEvent* );

signals:
  /**
   * This signal is emit when the GlMainWidget will be deleted
   */
  void closing(GlMainWidget *, QCloseEvent *);

  /**
   * This signal is emit when GlMainWidget::redraw() is call
   */
  void viewRedrawn(GlMainWidget *glWidget);
  /**
   * This signal is emit when GlMainWidget::draw() is call
   */
  void viewDrawn(GlMainWidget *glWidget,bool graphChanged);

  void glResized(int w,int h);

  void graphChanged();

public :

  /**
   * This function return the first QGLWidget created
   * This function is use to share OpenGL context
   */
  static QGLWidget* getFirstQGLWidget();

  static void clearFirstQGLWidget();

private :
  static QGLWidget *firstQGLWidget;

};

}


#endif
