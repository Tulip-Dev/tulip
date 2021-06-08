/**
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

#ifndef DATACUBEVIEW_H_
#define DATACUBEVIEW_H_

#include <tulip/GlMainView.h>

#include <QWidget>

#include "POLIB/potypes.h"
#include "POLIB/SpiralLayout.h"
#include "POLIB/SquareLayout.h"
#include "POLIB/ZOrderLayout.h"
#include "POLIB/HilbertLayout.h"
#include "POLIB/TulipNodeColorMapping.h"
#include "POLIB/HSIColorMapping.h"
#include "POLIB/PixelOrientedMediator.h"
#include "POLIB/LinearMappingColor.h"
#include "POLIB/TulipGraphDimension.h"

#include "../../utils/PluginNames.h"

#include "PixelOrientedOverview.h"
#include "PixelOrientedOptionsWidget.h"

class QGraphicsProxyWidget;

namespace tlp {

class ViewGraphPropertiesSelectionWidget;
class PixelOrientedViewQuickAccessBar;

/*@{*/
/** \file
 *  \brief  Tulip Pixel Oriented View

 * This view plugin allows to visualize graph properties using pixel oriented visualization
 techniques.
 * This type of visualization aims to use each pixel of the display to visualize one data value
 * and therefore allow the visualization of the largest amount of data possible.
 * Pixels are mapped so that neighbor pixels in the data are placed close to each other on the
 screen.
 * The technique uses a linear order on data elements (graph nodes), inferred from a selected
 property,
 * which can be seen as a map from the data space onto a line segment. The mapping onto a 2D portion
 * of the plane then uses a “space-filling curve”.
 *
 *
 */
class PixelOrientedView : public GlMainView {

  static const unsigned int MIN_IMAGE_WIDTH = 512;
  static const unsigned int MIN_IMAGE_HEIGHT = 512;

  Q_OBJECT

  void registerTriggers();

public:
  PLUGININFORMATION(ViewName::PixelOrientedViewName, "Antoine Lambert", "12/2008",
                    "<p>The Pixel Oriented view allows to visualize graph properties using pixel "
                    "oriented visualization techniques.</p>"
                    "<p>This type of visualization aims to use each pixel of the display to "
                    "visualize one data value and therefore allows the visualization of the "
                    "largest amount of data possible. "
                    "Pixels are mapped so that neighbor pixels in the data are placed close to "
                    "each other on the screen.</p>"
                    "<p>The technique uses a linear order on data elements (graph nodes), inferred "
                    "from a selected property, which can be seen as a map from the data space onto "
                    "a line segment. The mapping onto a 2D portion of the plane then uses a "
                    "\"space-filling curve\"</p>",
                    "1.0", "View")

  PixelOrientedView(const PluginContext *);
  ~PixelOrientedView() override;
  std::string icon() const override {
    return ":/pixel_oriented_view.png";
  }
  QuickAccessBar *getQuickAccessBarImpl() override;
  void setState(const DataSet &dataSet) override;
  DataSet state() const override;
  void graphicsViewResized(int w, int h) override;
  Graph *getPixelOrientedGraph() {
    return pixelOrientedGraph;
  }
  QList<QWidget *> configurationWidgets() const override;
  void interactorsInstalled(const QList<tlp::Interactor *> &) override;

  void toggleInteractors(const bool activate);

  std::vector<PixelOrientedOverview *> getOverviews();
  bool smallMultiplesViewSet() const {
    return smallMultiplesView;
  }
  void switchFromSmallMultiplesToDetailView(PixelOrientedOverview *scatterPlot);
  void switchFromDetailViewToSmallMultiples();
  BoundingBox getSmallMultiplesViewBoundingBox();
  void generatePixelOverview(PixelOrientedOverview *pixelOverview);

  void draw() override;
  void refresh() override;

public slots:

  // inherited from View
  void graphChanged(Graph *graph) override;
  void applySettings() override;
  // inherited from GlMainView
  void centerView(bool = false) override;

  void init();
  void showPropertiesSelectionWidget();

private:
  void initPixelView();
  void destroyData();
  void destroyOverviewsIfNeeded();
  void updateOverviews(const bool updateAll = false);
  void propertiesSelected(bool);
  Color getTextColor() const;

  Graph *pixelOrientedGraph;
  GlGraphComposite *graphComposite;
  GlLayer *mainLayer;
  GlComposite *overviewsComposite;

  PixelOrientedOptionsWidget *optionsWidget;
  ViewGraphPropertiesSelectionWidget *propertiesSelectionWidget;

  pocore::PixelOrientedMediator *pixelOrientedMediator;

  std::map<std::string, pocore::LayoutFunction *> layoutFunctionsMap;

  void initGlWidget();

  void initLayoutFunctions();
  void setColorFunction(pocore::ColorFunction *colorFunction);
  void setLayoutFunction(pocore::LayoutFunction *layoutFunction);

  unsigned int lastNbNodes;
  std::vector<std::string> selectedGraphProperties;
  std::map<std::string, PixelOrientedOverview *> overviewsMap;
  std::map<std::string, bool> overviewGenMap;
  std::map<std::string, pocore::TulipGraphDimension *> dataMap;

  QGraphicsProxyWidget *noPropertyMsgBox;

  unsigned int overviewWidth;
  unsigned int overviewHeight;
  unsigned int minWidth;
  float refSize;

  pocore::HilbertLayout *hilbertLayout;
  pocore::SquareLayout *squareLayout;
  pocore::SpiralLayout *spiralLayout;
  pocore::ZorderLayout *zorderLayout;

  pocore::TulipNodeColorMapping *tulipNodeColorMapping;

  bool smallMultiplesView;

  double sceneRadiusBak;
  double zoomFactorBak;
  Coord eyesBak;
  Coord centerBak;
  Coord upBak;

  GlLabel *detailViewLabel;
  PixelOrientedOverview *detailOverview;
  std::string detailOverviewPropertyName;

  bool newGraphSet;
  bool smallMultiplesNeedUpdate;

  int lastViewWindowWidth, lastViewWindowHeight;

  bool center;
  bool isConstruct;
};
} // namespace tlp
#endif /* DATACUBEVIEW_H_ */
