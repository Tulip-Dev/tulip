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
#include "ScatterPlotQuickAccessBar.h"
#include "ScatterPlot2DOptionsWidget.h"

#include <tulip/TlpQtTools.h>
#include <tulip/ColorButton.h>

#include <QPushButton>

namespace tlp {

ScatterPlotQuickAccessBar::ScatterPlotQuickAccessBar(ScatterPlot2DOptionsWidget *opt,
                                                     QWidget *parent)
    : QuickAccessBarImpl(
          nullptr, QuickAccessBarImpl::QuickAccessButtons(
                       QuickAccessBarImpl::SCREENSHOT | QuickAccessBarImpl::BACKGROUNDCOLOR |
                       QuickAccessBarImpl::SHOWLABELS | QuickAccessBarImpl::COLORINTERPOLATION |
                       QuickAccessBarImpl::SHOWEDGES | QuickAccessBarImpl::LABELSSCALED |
                       QuickAccessBarImpl::NODECOLOR | QuickAccessBarImpl::EDGECOLOR |
                       QuickAccessBarImpl::NODEBORDERCOLOR | QuickAccessBarImpl::LABELCOLOR),
          parent),
      _optionsWidget(opt) {}

void ScatterPlotQuickAccessBar::setEdgesVisible(bool visible) {
  _optionsWidget->setDisplayGraphEdges(visible);
  showEdgesButton()->setIcon((visible ? QIcon(":/tulip/gui/icons/20/edges_enabled.png")
                                      : QIcon(":/tulip/gui/icons/20/edges_disabled.png")));
  emit settingsChanged();
}

void ScatterPlotQuickAccessBar::reset() {
  QuickAccessBarImpl::reset();
  showEdgesButton()->setChecked(_optionsWidget->displayGraphEdges());
  showEdgesButton()->setIcon((_optionsWidget->displayGraphEdges()
                                  ? QIcon(":/tulip/gui/icons/20/edges_enabled.png")
                                  : QIcon(":/tulip/gui/icons/20/edges_disabled.png")));
  showLabelsButton()->setChecked(_optionsWidget->displayNodeLabels());
  showLabelsButton()->setIcon((_optionsWidget->displayNodeLabels()
                                   ? QIcon(":/tulip/gui/icons/20/labels_enabled.png")
                                   : QIcon(":/tulip/gui/icons/20/labels_disabled.png")));
  backgroundColorButton()->setTulipColor(_optionsWidget->getBackgroundColor());
  showLabelScaled()->setChecked(_optionsWidget->displayScaleLabels());
  showLabelScaled()->setIcon((_optionsWidget->displayScaleLabels()
                                  ? QIcon(":/tulip/gui/icons/20/labels_scaled_enabled.png")
                                  : QIcon(":/tulip/gui/icons/20/labels_scaled_disabled.png")));
}

void ScatterPlotQuickAccessBar::setBackgroundColor(const QColor &col) {
  _optionsWidget->setBackgroundColor(tlp::QColorToColor(col));
  emit settingsChanged();
}

void ScatterPlotQuickAccessBar::setLabelsVisible(bool visible) {
  _optionsWidget->setDisplayNodeLabels(visible);
  showLabelsButton()->setIcon((visible ? QIcon(":/tulip/gui/icons/20/labels_enabled.png")
                                       : QIcon(":/tulip/gui/icons/20/labels_disabled.png")));
  emit settingsChanged();
}

void ScatterPlotQuickAccessBar::setLabelsScaled(bool scaled) {
  _optionsWidget->setDisplayScaleLabels(scaled);
  showLabelScaled()->setIcon((scaled ? QIcon(":/tulip/gui/icons/20/labels_scaled_enabled.png")
                                     : QIcon(":/tulip/gui/icons/20/labels_scaled_disabled.png")));
  emit settingsChanged();
}
}
