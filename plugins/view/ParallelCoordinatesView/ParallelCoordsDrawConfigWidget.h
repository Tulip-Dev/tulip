/**
 *
 * This file is part of Tulip (www.tulip-software.org)
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

#ifndef PARALLELCOORDSDRAWCONFIGWIDGET_H_
#define PARALLELCOORDSDRAWCONFIGWIDGET_H_

#include <QWidget>

#include <tulip/Size.h>
#include <tulip/Color.h>

namespace Ui {
class ParallelCoordsDrawConfigWidgetData;
}

namespace tlp {

class ParallelCoordsDrawConfigWidget : public QWidget {

  Q_OBJECT

public :

  ParallelCoordsDrawConfigWidget(QWidget *parent = 0);
  ~ParallelCoordsDrawConfigWidget();

  unsigned int getAxisHeight() const;
  void setAxisHeight(const unsigned int axisHeight);
  bool drawPointOnAxis() const;
  void setDrawPointOnAxis(const bool drawPointOnAxis);
  Size getAxisPointMinSize() const;
  void setAxisPointMinSize(const unsigned int axisPointMinSize);
  Size getAxisPointMaxSize() const;
  void setAxisPointMaxSize(const unsigned int axisPointMaxSize);
  bool displayNodesLabels() const;
  void setLinesColorAlphaValue(unsigned int value);
  unsigned int getLinesColorAlphaValue() const;
  Color getBackgroundColor() const;
  void setBackgroundColor(const Color &color);
  unsigned int getUnhighlightedEltsColorsAlphaValue() const;
  void setUnhighlightedEltsColorsAlphaValue(const unsigned int alphaValue);
  std::string getLinesTextureFilename() const;
  void setLinesTextureFilename(const std::string &linesTextureFileName);

  bool configurationChanged();

private slots :

  void pressButtonBrowse();
  void userTextureRbToggled(bool checked);
  void minAxisPointSizeValueChanged(int newValue);
  void maxAxisPointSizeValueChanged(int newValue);

private:

  bool oldValuesInitialized;
  unsigned int oldAxisHeight;
  bool oldDrawPointOnAxis;
  Size oldAxisPointMinSize;
  Size oldAxisPointMaxSize;
  bool oldDisplayNodesLabels;
  unsigned int oldLinesColorAlphaValue;
  Color oldBackgroundColor;
  unsigned int oldUnhighlightedEltsColorsAlphaValue;
  std::string oldLinesTextureFilename;
  Ui::ParallelCoordsDrawConfigWidgetData* _ui;


};

}

#endif /* PARALLELCOORDSDRAWCONFIGWIDGET_H_ */
