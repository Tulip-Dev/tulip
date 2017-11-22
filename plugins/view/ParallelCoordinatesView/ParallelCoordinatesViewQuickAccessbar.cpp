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
#include "ParallelCoordinatesViewQuickAccessbar.h"
#include "ParallelCoordsDrawConfigWidget.h"

#include <tulip/TlpQtTools.h>
#include <tulip/ColorButton.h>

#include <QPushButton>

namespace tlp {

ParallelCoordinatesViewQuickAccessBar::ParallelCoordinatesViewQuickAccessBar(
    ParallelCoordsDrawConfigWidget *opt, QWidget *parent)
    : QuickAccessBarImpl(nullptr,
                         QuickAccessBarImpl::QuickAccessButtons(
                             QuickAccessBarImpl::SCREENSHOT | QuickAccessBarImpl::BACKGROUNDCOLOR |
                             QuickAccessBarImpl::SHOWLABELS | QuickAccessBarImpl::SHOWNODES |
                             QuickAccessBarImpl::LABELSSCALED | QuickAccessBarImpl::NODECOLOR |
                             QuickAccessBarImpl::EDGECOLOR | QuickAccessBarImpl::NODEBORDERCOLOR |
                             QuickAccessBarImpl::LABELCOLOR),
                         parent),
      _optionsWidget(opt) {}

void ParallelCoordinatesViewQuickAccessBar::setNodesVisible(bool visible) {
  _optionsWidget->setDrawPointOnAxis(visible);
  showNodesButton()->setIcon((visible ? QIcon(":/tulip/gui/icons/20/nodes_enabled.png")
                                      : QIcon(":/tulip/gui/icons/20/nodes_disabled.png")));
  emit settingsChanged();
}

void ParallelCoordinatesViewQuickAccessBar::reset() {
  QuickAccessBarImpl::reset();
  showNodesButton()->setChecked(_optionsWidget->drawPointOnAxis());
  showNodesButton()->setIcon((_optionsWidget->drawPointOnAxis()
                                  ? QIcon(":/tulip/gui/icons/20/nodes_enabled.png")
                                  : QIcon(":/tulip/gui/icons/20/nodes_disabled.png")));
  showLabelsButton()->setChecked(_optionsWidget->displayNodeLabels());
  showLabelsButton()->setIcon((_optionsWidget->displayNodeLabels()
                                   ? QIcon(":/tulip/gui/icons/20/labels_enabled.png")
                                   : QIcon(":/tulip/gui/icons/20/labels_disabled.png")));
  backgroundColorButton()->setTulipColor(_optionsWidget->getBackgroundColor());
}

void ParallelCoordinatesViewQuickAccessBar::setBackgroundColor(const QColor &col) {
  _optionsWidget->setBackgroundColor(tlp::QColorToColor(col));
  emit settingsChanged();
}

void ParallelCoordinatesViewQuickAccessBar::setLabelsVisible(bool visible) {
  _optionsWidget->setDisplayNodeLabels(visible);
  showLabelsButton()->setIcon((visible ? QIcon(":/tulip/gui/icons/20/labels_enabled.png")
                                       : QIcon(":/tulip/gui/icons/20/labels_disabled.png")));
  emit settingsChanged();
}
}
