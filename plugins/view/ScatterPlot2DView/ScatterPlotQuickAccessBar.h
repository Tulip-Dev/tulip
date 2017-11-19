/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
 *
 * Authors: Tulip development Team from LaBRI, University of Bordeaux
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
#ifndef SCATTERPLOTQUICKACCESSBAR_H
#define SCATTERPLOTQUICKACCESSBAR_H

#include <tulip/QuickAccessBar.h>

namespace tlp {

class ScatterPlot2DOptionsWidget;

class ScatterPlotQuickAccessBar : public tlp::QuickAccessBarImpl {
  Q_OBJECT

  ScatterPlot2DOptionsWidget *_optionsWidget;

public:
  ScatterPlotQuickAccessBar(ScatterPlot2DOptionsWidget *opt, QWidget *parent = nullptr);

public slots:
  void reset() override;
  void setEdgesVisible(bool) override;
  void setBackgroundColor(const QColor &) override;
  void setLabelsVisible(bool) override;
  void setLabelsScaled(bool) override;
};
}
#endif // SCATTERPLOTQUICKACCESSBAR_H
