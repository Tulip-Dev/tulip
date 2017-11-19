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

#ifndef SCATTERPLOTCORRELCOEFFSELECTOROPTIONSWIDGET_H_
#define SCATTERPLOTCORRELCOEFFSELECTOROPTIONSWIDGET_H_

#include <QWidget>

#include <tulip/Color.h>

class QPushButton;

namespace Ui {
class ScatterPlotCorrelCoeffSelectorOptionsWidgetData;
}

namespace tlp {

class ColorButton;
class ScatterPlotCorrelCoeffSelectorOptionsWidget : public QWidget {

  Q_OBJECT

public:
  ScatterPlotCorrelCoeffSelectorOptionsWidget(QWidget *parent = nullptr);
  ~ScatterPlotCorrelCoeffSelectorOptionsWidget() override;

  Color getMinusOneColor() const;
  Color getZeroColor() const;
  Color getOneColor() const;

protected:
  void showEvent(QShowEvent *event) override;

private slots:
  void updateColorScale();

private:
  void setButtonColor(tlp::ColorButton *button, const Color &color);
  Ui::ScatterPlotCorrelCoeffSelectorOptionsWidgetData *_ui;
};
}

#endif /* SCATTERPLOTCORRELCOEFFSELECTOROPTIONSWIDGET_H_ */
