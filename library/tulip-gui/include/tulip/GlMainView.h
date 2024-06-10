/*
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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

#ifndef Tulip_GLMAINVIEW_H
#define Tulip_GLMAINVIEW_H

#include <tulip/ViewWidget.h>
#include <tulip/GlMainWidget.h>

class QGraphicsProxyWidget;
class QAction;
class QPushButton;
class QRectF;

namespace tlp {
class GlOverviewGraphicsItem;
class SceneConfigWidget;
class SceneLayersConfigWidget;
class QuickAccessBar;
class ViewActionsManager;

/**
 * @ingroup Plugins
 *
 * @brief An abstract view that displays a GlMainWidget as its central widget.
 *
 * The GlMainView subclasses ViewWidget and always uses a GlMainWidget as the central widget of the
 panel. It also adds the following features:
 * @list
 * @li An overview of the scene that can be toggled on or off.
 * @li Some configuration widgets that modify the rendering parameters.
 * @li A quick access bar widget that allows the user to quickly modify some of the most used
 rendering parameters and graph properties (nodes color, edges display, etc)
 * @li The possibility to make snapshots of the current scene
 * @endlist
 *
 * Subclassing GlMainView means you will only want to display graphs in a single GlMainWidget.
 Switching the central widget can only be achieved from the ViewWidget class.
 *
 * @warning It is strongly unadvised to re-implement methods already implemented into tlp::View or
 tlp::ViewWidget. If you have to add custom behavior to those method, make sure to call the
 upper-class methods first:
 @code
 void MyView::setupWidget() { // Where MyView is a subclass of tlp::GlMainView
   GlMainView::setupWidget(); // call this first
   // insert custom behavior here
 }
 @endcode

 * @see tlp::ViewWidget
 */
class TLP_QT_SCOPE GlMainView : public tlp::ViewWidget {

  Q_OBJECT

  tlp::GlMainWidget *_glMainWidget;
  tlp::GlOverviewGraphicsItem *_overviewItem;
  tlp::ViewActionsManager *_viewActionsManager;

  QPushButton *_showOvButton, *_showQabButton;

protected:
  bool needQuickAccessBar, _needTooltipAndUrlManager;
  QGraphicsProxyWidget *_quickAccessBarItem;
  tlp::QuickAccessBar *_quickAccessBar;
  tlp::SceneConfigWidget *_sceneConfigurationWidget;
  tlp::SceneLayersConfigWidget *_sceneLayersConfigurationWidget;

public:
  enum OverviewPosition {
    OVERVIEW_TOP_LEFT,
    OVERVIEW_TOP_RIGHT,
    OVERVIEW_BOTTOM_LEFT,
    OVERVIEW_BOTTOM_RIGHT
  };

  GlMainView(bool needTooltipAndUrlManager = false);
  ~GlMainView() override;
  tlp::GlMainWidget *getGlMainWidget() const;
  std::list<QWidget *> configurationWidgets() const override;
  bool overviewVisible() const;
  bool quickAccessBarVisible() const;
  QPixmap snapshot(const QSize &outputSize = QSize()) const override;

  void setOverviewPosition(const OverviewPosition &position);
  OverviewPosition overviewPosition() const;

  void setUpdateOverview(bool updateOverview);
  bool updateOverview() const;

  void setState(const tlp::DataSet &) override;
  tlp::DataSet state() const override;

  /**
   * @brief Rotate the view camera by (x,y,z)
   * @param x rotation around the X axis in degree
   * @param y rotation around the Y axis in degree
   * @param z rotation around Z axis in degree
   */
  inline void rotateCamera(int x, int y, int z) {
    getGlMainWidget()->getScene()->rotateCamera(x, y, z);
  }

  /**
   * @brief Translate the view camera by (x,y,z)
   * @param x offset along the X axis
   * @param y offset along the Y axis
   * @param z offset along the Z axis
   */
  inline void translateCamera(int x, int y, int z) {
    getGlMainWidget()->getScene()->translateCamera(x, y, z);
  }

  /**
   * @brief Return the 3D world position for the given view position
   */
  inline Coord viewToWorld(const Coord &vpos) const {
    return getGlMainWidget()->getScene()->getGraphCamera().viewportTo3DWorld(vpos);
  }

  /**
   * @brief Return the view position for the given 3D position
   */
  inline Coord worldToView(const Coord &wpos) const {
    return getGlMainWidget()->getScene()->getGraphCamera().worldTo2DViewport(wpos);
  }

  /**
   * @brief Zoom by step to the given (x,y) view position
   * @param step of zoom
   */
  inline void zoomXY(int step, const int x, const int y) {
    getGlMainWidget()->getScene()->zoomXY(step, x, y);
  }

  /**
   * @brief Zoom by step
   * @param step of zoom
   */
  inline void zoom(int step) {
    getGlMainWidget()->getScene()->zoom(step);
  }

  /**
   * @brief Zoom by factor
   * @param factor of zoom
   */
  inline void zoomFactor(float factor) {
    getGlMainWidget()->getScene()->zoomFactor(factor);
  }

  /**
   * @brief Do a zoom and pan animation
   * @param boundingBox the bounding box in scene coordinates on which the view
   * has to be zoomed and panned. If it is not valid, the scene bounding box will be used.
   * At the end of the animation, the view will be zoomed and centered on the content of that
   * bounding box.
   * @param duration the animation duration in msecs
   */
  void zoomAndPanAnimation(const tlp::BoundingBox &boundingBox, const double duration = 1000.);

public slots:
  /**
   * @brief Calls GlMainWidget::draw();
   */
  void draw() override;

  /**
   * @brief Calls GlMainWidget::redraw();
   */
  void redraw();

  /**
   * @brief Calls GlMainWidget::redraw();
   */
  void refresh() override;

  /**
   * @brief Force the overview to be redrawn. Since GlMainView already detects graph's
   * modifications, this method should not be called manually to avoid extra rendering.
   */
  virtual void drawOverview(bool generatePixmap = true);

  /**
   * @brief Centers the scene's camera
   */
  void centerView(bool graphChanged = false) override;

  /**
   * @brief Toggles the overview on or off
   */
  void setOverviewVisible(bool);

  /**
   * @brief allows to control the quick access bar visibility
   */
  void setQuickAccessBarVisible(bool);

  /**
   * @brief Toggles the orthogonal projection on or off, then draws
   */
  void setViewOrtho(bool);

  /**
   * @brief Force the settings set in the configuration widgets to be re-applied.
   */
  void applySettings() override;

  void undoCallback() override;

  static bool getNodeOrEdgeAtViewportPos(GlMainWidget *glw, int x, int y, node &n, edge &e);

  bool getNodeOrEdgeAtViewportPos(int x, int y, node &n, edge &e) const override {
    return getNodeOrEdgeAtViewportPos(_glMainWidget, x, y, n, e);
  }

  /**
   * @brief Return the glMainWidget current display in the RGB888 format.
   */
  QImage getRGBImage() const;

protected slots:
  virtual void glMainViewDrawn(bool graphChanged);
  virtual void sceneRectChanged(const QRectF &);
  void fillContextMenu(QMenu *menu, const QPointF &) override;

protected:
  void setupWidget() override;
  void assignNewGlMainWidget(GlMainWidget *glMainWidget, bool deleteOldGlMainWidget = true);
  bool eventFilter(QObject *obj, QEvent *event) override;

  tlp::GlOverviewGraphicsItem *overviewItem() const;
  void updateShowOverviewButton();
  void updateShowQuickAccessBarButton();
  virtual QuickAccessBar *getQuickAccessBarImpl();

  OverviewPosition _overviewPosition;

  bool _updateOverview;
};
} // namespace tlp

#endif /* GLMAINVIEW_H_ */
