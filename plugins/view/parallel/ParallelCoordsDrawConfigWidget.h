/*
	  ParallelCoordsDrawConfigWidget.h

   Created on: 6 avr. 2009
       Author: Antoine Lambert
       E-mail: antoine.lambert@labri.fr

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

 */

#ifndef PARALLELCOORDSDRAWCONFIGWIDGET_H_
#define PARALLELCOORDSDRAWCONFIGWIDGET_H_

#include <QtGui/QWidget>

#include <tulip/Size.h>
#include <tulip/Color.h>

#include "ParallelCoordsDrawConfigWidgetData.h"

namespace tlp {

class ParallelCoordsDrawConfigWidget : public QWidget, public Ui::ParallelCoordsDrawConfigWidgetData {

	Q_OBJECT

public :

	ParallelCoordsDrawConfigWidget(QWidget *parent = 0);

	unsigned int getAxisHeight() const;
	unsigned int getSpaceBetweenAxis() const;
	void setSpaceBetweenAxis(const unsigned int spaceBetweenAxis);
	void setAxisHeight(const unsigned int axisHeight);
	bool drawPointOnAxis() const;
	void setDrawPointOnAxis(const bool drawPointOnAxis);
	Size getAxisPointMinSize() const;
	void setAxisPointMinSize(const unsigned int axisPointMinSize);
	Size getAxisPointMaxSize() const;
	void setAxisPointMaxSize(const unsigned int axisPointMaxSize);
	void setLinesColorAlphaValue(unsigned int value);
	unsigned int getLinesColorAlphaValue() const;
	Color getBackgroundColor() const;
	void setBackgroundColor(const Color &color);
	unsigned int getUnhighlightedEltsColorsAlphaValue() const;
	void setUnhighlightedEltsColorsAlphaValue(const unsigned int alphaValue);
	std::string getLinesTextureFilename() const;
	void setLinesTextureFilename(const std::string &linesTextureFileName);

private slots :

	void pressButtonBrowse();
	void userTextureRbToggled(bool checked);
	void minAxisPointSizeValueChanged(int newValue);
	void maxAxisPointSizeValueChanged(int newValue);
	void pressColorButton();

};

}

#endif /* PARALLELCOORDSDRAWCONFIGWIDGET_H_ */
