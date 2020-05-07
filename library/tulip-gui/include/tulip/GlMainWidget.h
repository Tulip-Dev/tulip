/*
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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

#include <QOpenGLWidget>

#include <tulip/tulipconf.h>
#include <tulip/GlScene.h>
#include <tulip/Graph.h>

class QOpenGLFramebufferObject;

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
 * - Selection with pickNodesEdges() and pickGlEntities()
 * - Image output with getImage(), createPicture()
 * - Texture output with createTexture()
 * - others operation on GlScene and QGlWidget
 */
class TLP_QT_SCOPE GlMainWidget : public QOpenGLWidget {
  Q_OBJECT

public:
  /**
   * @brief Configure the rendering process ( see render function)
   * @see render
   **/
  enum RenderingOption {
    RenderScene = 0x1, /** Force to render the graph even if there is a previous buffered render.
                          You need to call this option if the graph is updated to regenerate the
                          buffer. If not set try to use the last buffered graph render, if there is
                          no valid buffer this flag is forced. **/
    SwapBuffers = 0x2  /** Call the swapBuffer functions at the end of the rendering process. If the
                          disabled  it's up to you to call the swapBuffer function. **/
  };
  Q_DECLARE_FLAGS(RenderingOptions, RenderingOption)

  /**
   * @brief Constructor of GlMainWidget
   *
   * Create a GlMainWidget with the GlScene associated to it
   * @param parent Qt Widget parent system
   * @param view if you want to link this GlMainWidget to a view : use this parameter
   */
  GlMainWidget(QWidget *parent = nullptr, View *view = nullptr);
  ~GlMainWidget() override;

  /**
   * @brief Get the GlScene of this GlMainWidget
   * You have to add yours GlLayer and GlEntity to this GlScene
   * At the construction this GlScene is empty
   * @see GlScene
   * @see GlScene::createLayer(const std::string &name)
   * @see GlLayer::addGlEntity(GlSimpleEntity *entity,const std::string& name)
   */
  tlp::GlScene *getScene() {
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
  void pickNodesEdges(const int x, const int y, const int width, const int height,
                      std::vector<SelectedEntity> &selectedNodes,
                      std::vector<SelectedEntity> &selectedEdges, tlp::GlLayer *layer = nullptr,
                      bool pickNodes = true, bool pickEdges = true);

  /** @brief Select a node or edge at a screen point
   *  Try to select at point (x,y) a node in the first place then if no result try to select an edge
   *  @param type tells what has been found: NODE, EDGE
   *  @param layer specify the layer in which to perform the picking
   *  @param pickNodes enable or disable the picking of nodes
   *  @param pickEdges enable or disable the picking of edges
   *  @return true if something has been found, false otherwise
   */
  bool pickNodesEdges(const int x, const int y, SelectedEntity &selectedEntity,
                      tlp::GlLayer *layer = nullptr, bool pickNodes = true, bool pickEdges = true);

  /**
   * @brief convert a screen measure into a viewport measure
   * @param a measure in screen coordinates specified as an integer
   * @return the converted measure in viewport coordinates as an integer
   */
  inline int screenToViewport(int l) const {
    return l * devicePixelRatio();
  }

  /**
   * @brief convert a screen measure into a viewport measure
   * @param a measure in screen coordinates specified as a double
   * @return the converted measure in viewport coordinates as a double
   */
  inline double screenToViewport(double l) const {
    return l * devicePixelRatio();
  }

  /**
   * @brief convert a screen point into a viewport point
   * @param a point in screen coordinates
   * @return the converted point in viewport coordinates
   */
  inline Coord screenToViewport(const Coord &point) const {
    qreal dpr = devicePixelRatio();
    return Coord(point.x() * dpr, point.y() * dpr);
  }

  /**
   * @brief convert a viewport measure into a screen measure
   * @param a measure in viewport coordinates specified as a double
   * @return the converted measure in screen coordinates as a double
   */
  inline double viewportToScreen(double l) const {
    return l / devicePixelRatio();
  }

  /**
   * @brief convert a viewport point into a screen point
   * @param a point in viewport coordinates
   * @return the converted point in screen coordinates
   */
  inline Coord viewportToScreen(const Coord &point) const {
    qreal dpr = devicePixelRatio();
    return Coord(point.x() / dpr, point.y() / dpr);
  }

  /**
   * @brief Compute texture size in power of two with given height and width
   * For example if you set width to 94 and height to 256, this function set textureRealWidth to 128
   * and textureRealHeight to 256
   */
  static void getTextureRealSize(int width, int height, int &textureRealWidth,
                                 int &textureRealHeight);

  /**
   * @brief Take a snapshot of the Widget and put it in a picture
   * @param width size
   * @param height size
   * @param center if true this function calls a centerScene() before picture output
   */
  void createPicture(const std::string &pictureName, int width, int height, bool center = true);

  /**
   * Take a snapshot of the Widget and return it
   * @param width size
   * @param height size
   * @param center if true this function calls a centerScene() before picture output
   * @param format indicates the format of the created image
   */
  QImage createPicture(int width, int height, bool center = true,
                       QImage::Format format = QImage::Format_RGB32);

  /**
   * @brief Function to do picking on entities in a screen region
   * It just calls selectEntities on the GlScene instance.
   * @param x screen coordinates
   * @param y screen coordinates
   * @param width screen size
   * @param height screen size
   * @param pickedEntities filled with entity under the selection screen rectangle
   * @param layer if you want to do the selection only on one GlLayer
   */
  bool pickGlEntities(const int x, const int y, const int width, const int height,
                      std::vector<SelectedEntity> &pickedEntities, tlp::GlLayer *layer = nullptr);
  /**
   * @brief Function to do picking on entities.
   * It just calls selectEntities on the GlScene instance with a small window of twelve pixels.
   * @param x screen coordinates
   * @param y screen coordinates
   * @param pickedEntities filled with entity under the selection screen rectangle
   * @param layer if you want to do the selection only on one GlLayer
   */
  bool pickGlEntities(const int x, const int y, std::vector<SelectedEntity> &pickedEntities,
                      tlp::GlLayer *layer = nullptr);

  /**
   * Override default makeCurrent/doneCurrent behavior to activate deactivate
   * adequate OpenGL context based on the QOpenGLWidget visibility
   */
  void makeCurrent();
  void doneCurrent();

  /**
   * Indicates if this is associated to the current OpenGL context
   */
  bool isCurrent();

  /**
   * Resize openGL view
   */
  void resizeGL(int w, int h) override;

  /**
   * Compute interactors before drawing
   */
  void computeInteractors();

  /**
   * Draw interactors
   */
  void drawInteractors();

  /**
   * @brief This function performs all the rendering process of the graph.
   * Use this function only for advanced purpose, if you want to perform simple rendering use the
   *draw or redraw functions instead.
   * @param options Configure the rendering process, see the RenderingOption documentation for more
   *information on each rendering option effect.
   * @see RenderingOption
   * @param checkVisibility If check visibility is true : the engine check if GlMainWidget
   *QWidget is visible. If false: the engine renders the scene in all cases
   **/
  void render(RenderingOptions options = RenderingOptions(RenderScene | SwapBuffers),
              bool checkVisibility = true);

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

private:
  void createFramebuffers(int width, int height);
  void deleteFramebuffers();

  tlp::GlScene scene;
  QRegion _visibleArea;
  View *view;
  int widthStored;
  int heightStored;
  bool frameBufferStored;
  QOpenGLFramebufferObject *glFrameBuf, *glFrameBuf2;
  static bool inRendering;
  bool keepPointOfViewOnSubgraphChanging;
  std::string sceneTextureId;

public slots:
  /**
   * Draw the GlScene and the interactors
   */
  void draw(bool graphChanged = true);
  /**
   * This function is given for optimisation purpose only. If the hardware allows it,
   * it enables to redraw only the Augmented display and the interactors and not the graph
   * it is really useful for interactors such as zoom box etc..
   * Warning, if you change the graph or the properties of element (Colors, size, etc...)
   * applying that function will not display the change, in that case, use the draw function.
   */
  void redraw();

  void closeEvent(QCloseEvent *e) override;

  /**
   * @brief Convenience function that calls center function on the current scene, applies a zoom (if
   *needed) and draws the view.
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
  void paintEvent(QPaintEvent *) override;

signals:
  /**
   * This signal is emitted when the GlMainWidget will be deleted
   */
  void closing(GlMainWidget *, QCloseEvent *);

  /**
   * This signal is emitted when GlMainWidget::redraw() is call
   */
  void viewRedrawn(GlMainWidget *glWidget);
  /**
   * This signal is emitted when GlMainWidget::draw() is call
   */
  void viewDrawn(GlMainWidget *glWidget, bool graphChanged);

  void glResized(int w, int h);

  void graphChanged();
};
} // namespace tlp

#endif
