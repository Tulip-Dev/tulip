/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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

#ifndef SCATTERPLOT2DOPTIONSWIDGET_H_
#define SCATTERPLOT2DOPTIONSWIDGET_H_

#include <QtGui/QWidget>

#include <tulip/Color.h>
#include <tulip/Size.h>

#include "ui_ScatterPlot2DOptionsWidget.h"

namespace tlp {

class ScatterPlot2DOptionsWidget : public QWidget, public Ui::ScatterPlot2DOptionsWidgetData {

  Q_OBJECT

public :

  ScatterPlot2DOptionsWidget(QWidget *parent = 0);

  bool uniformBackground() const;
  Color getUniformBackgroundColor() const;
  Color getMinusOneColor() const;
  Color getZeroColor() const;
  Color getOneColor() const;
  void setBackgroundColor(const Color &color);
  Size getMinSizeMapping() const;
  Size getMaxSizeMapping() const;
  void setMinSizeMapping(const float minSize);
  void setMaxSizeMapping(const float maxSize);
  bool displayGraphEdges() const;
  void setDisplayGraphEdges(const bool showEdges);

  bool configurationChanged();

protected :

  void showEvent(QShowEvent * event);

private slots :

  void pressBackgroundColorButton();
  void pressMinusOneColorButton();
  void pressZeroColorButton();
  void pressOneColorButton();
  void minSizeSpinBoxValueChanged(int);
  void maxSizeSpinBoxValueChanged(int);

private :

  void setButtonBackgroundColor(QPushButton *button, const Color &color);
  void changeButtonBackgroundColor(QPushButton *button);
  void updateColorScale();
  Color getButtonColor(QPushButton *button) const;

signals :

  void applySettingsSignal();

private :

  bool oldValuesInitialized;
  bool oldUniformBackground;
  Color oldUniformBackgroundColor;
  Color oldMinusOneColor;
  Color oldZeroColor;
  Color oldOneColor;
  Size oldMinSizeMapping;
  Size oldMaxSizeMapping;
  bool oldDisplayGraphEdges;

};

}

#endif /* SCATTERPLOT2DOPTIONSWIDGET_H_ */
