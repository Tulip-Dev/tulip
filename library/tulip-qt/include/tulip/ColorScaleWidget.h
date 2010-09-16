/*
 * ColorScaleWidget.h
 *
 *  Created on: 2 avr. 2010
 *      Author: Jonathan Dubois
 *       Email: jonathan.dubois@labri.fr
 *
 *	This program is free software; you can redistribute it and/or modify  *
 *  it under the terms of the GNU Lesser General Public License as published by  
 *  the Free Software Foundation; either version 2 of the License, or     
 *  (at your option) any later version.
 */

#ifndef COLORSCALEWIDGET_H_
#define COLORSCALEWIDGET_H_

#include <QtGui/QWidget>
#include <QtGui/QPainter>

#include <tulip/tulipconf.h>

namespace tlp{
class ColorScale;
}
/**
 *  @brief Widget used to display and edit color scale.
 */
class TLP_QT_SCOPE ColorScaleWidget : public QWidget{
public:
  ColorScaleWidget(QWidget* parent=NULL);
  virtual ~ColorScaleWidget();

  /**
   * @brief Set the color scale to display.
   */
  void setColorScale(tlp::ColorScale *colorScale);
  tlp::ColorScale* getColorScale()const{
    return colorScale;
  }

  void setOrientation(Qt::Orientation orientation);
  QSize minimumSizeHint () const;
  QSize sizeHint () const;
protected:
  void paintEvent ( QPaintEvent * event );

private:

  void paintColorScale(QPainter& painter,const QRect& rect);
  tlp::ColorScale *colorScale;
  Qt::Orientation orientation;
};

#endif /* COLORSCALEWIDGET_H_ */
