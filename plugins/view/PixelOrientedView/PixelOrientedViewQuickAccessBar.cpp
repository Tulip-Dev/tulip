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
#include "PixelOrientedViewQuickAccessBar.h"
#include "PixelOrientedOptionsWidget.h"

#include <tulip/TlpQtTools.h>
#include <tulip/ColorButton.h>

#include <QPushButton>

namespace tlp {

PixelOrientedViewQuickAccessBar::PixelOrientedViewQuickAccessBar(PixelOrientedOptionsWidget *opt,
                                                                 QWidget *parent)
    : QuickAccessBarImpl(nullptr,
                         QuickAccessBarImpl::QuickAccessButtons(
                             QuickAccessBarImpl::SCREENSHOT | QuickAccessBarImpl::BACKGROUNDCOLOR |
                             QuickAccessBarImpl::SHOWLABELS | QuickAccessBarImpl::NODEBORDERCOLOR |
                             QuickAccessBarImpl::NODECOLOR | QuickAccessBarImpl::LABELSSCALED |
                             QuickAccessBarImpl::LABELCOLOR),
                         parent),
      _optionsWidget(opt) {}

void PixelOrientedViewQuickAccessBar::reset() {
  QuickAccessBarImpl::reset();
  backgroundColorButton()->setTulipColor(_optionsWidget->getBackgroundColor());
}

void PixelOrientedViewQuickAccessBar::setBackgroundColor(const QColor &col) {
  _optionsWidget->setBackgroundColor(tlp::QColorToColor(col));
  emit settingsChanged();
}
}
