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
#include "MatrixViewQuickAccessBar.h"
#include "MatrixViewConfigurationWidget.h"

#include <tulip/TlpQtTools.h>
#include <tulip/ColorButton.h>

#include <QPushButton>

namespace tlp {

MatrixViewQuickAccessBar::MatrixViewQuickAccessBar(MatrixViewConfigurationWidget *opt,
                                                   QWidget *parent)
    : QuickAccessBarImpl(
          NULL, QuickAccessBarImpl::QuickAccessButtons(
                    QuickAccessBarImpl::SCREENSHOT | QuickAccessBarImpl::BACKGROUNDCOLOR |
                    QuickAccessBarImpl::SHOWLABELS | QuickAccessBarImpl::COLORINTERPOLATION |
                    QuickAccessBarImpl::SHOWEDGES | QuickAccessBarImpl::LABELSSCALED |
                    QuickAccessBarImpl::LABELCOLOR),
          parent),
      _optionsWidget(opt) {}

void MatrixViewQuickAccessBar::setEdgesVisible(bool visible) {
  _optionsWidget->setDisplayEdges(visible);
  showEdgesButton()->setIcon((visible ? QIcon(":/tulip/gui/icons/20/edges_enabled.png")
                                      : QIcon(":/tulip/gui/icons/20/edges_disabled.png")));
  emit settingsChanged();
}

void MatrixViewQuickAccessBar::reset() {
  QuickAccessBarImpl::reset();
  showEdgesButton()->setChecked(_optionsWidget->displayGraphEdges());
  showEdgesButton()->setIcon((_optionsWidget->displayGraphEdges()
                                  ? QIcon(":/tulip/gui/icons/20/edges_enabled.png")
                                  : QIcon(":/tulip/gui/icons/20/edges_disabled.png")));
  backgroundColorButton()->setTulipColor(_optionsWidget->getBackgroundColor());
  showColorInterpolation()->setChecked(_optionsWidget->isEdgeColorInterpolation());
  showColorInterpolation()->setIcon(
      (_optionsWidget->isEdgeColorInterpolation()
           ? QIcon(":/tulip/gui/icons/20/color_interpolation_enabled.png")
           : QIcon(":/tulip/gui/icons/20/color_interpolation_disabled.png")));
}

void MatrixViewQuickAccessBar::setBackgroundColor(const QColor &col) {
  _optionsWidget->setBackgroundColor(col);
  emit settingsChanged();
}

void MatrixViewQuickAccessBar::setColorInterpolation(bool state) {
  _optionsWidget->setEdgeColorInterpolation(state);
  showColorInterpolation()->setIcon(
      (state ? QIcon(":/tulip/gui/icons/20/color_interpolation_enabled.png")
             : QIcon(":/tulip/gui/icons/20/color_interpolation_disabled.png")));
  emit settingsChanged();
}
}
