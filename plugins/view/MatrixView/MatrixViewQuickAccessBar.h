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
#ifndef MATRIXVIEWQUICKACCESSBAR_H
#define MATRIXVIEWQUICKACCESSBAR_H

#include <tulip/QuickAccessBar.h>

namespace tlp {

class MatrixViewConfigurationWidget;

class MatrixViewQuickAccessBar : public tlp::QuickAccessBarImpl {
  Q_OBJECT

  MatrixViewConfigurationWidget *_optionsWidget;

public:
  MatrixViewQuickAccessBar(MatrixViewConfigurationWidget *opt, QWidget *parent = nullptr);

public slots:
  void reset() override;
  void setEdgesVisible(bool) override;
  void setLabelsVisible(bool) override;
  void setBackgroundColor(const QColor &) override;
  void setColorInterpolation(bool) override;
};
} // namespace tlp
#endif // SCATTERPLOTQUICKACCESSBAR_H
