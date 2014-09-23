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

#ifndef COLORSCALEPREVIEW_H_
#define COLORSCALEPREVIEW_H_

#include <QLabel>

class QResizeEvent;
class QMouseEvent;

/**
 * @brief Label used to preview ColorScale object in Qt widget.
 * Label used to preview ColorScale object in Qt widget. This object display the render of a ColorScale in a label.
 * When user click on this object he can edit the ColorScale.
 */

namespace tlp {

class ColorScale;

class ColorScalePreview: public QLabel {
  Q_OBJECT
public:
  ColorScalePreview(ColorScale* colorScale);
  virtual ~ColorScalePreview();
  void setColorScale(ColorScale* colorScale);
  ColorScale* getColorScale() {
    return currentColorScale;
  }

  void resizeEvent (QResizeEvent * event);
  void mousePressEvent ( QMouseEvent * event );

protected:

  void fillLabel();

  ColorScale* currentColorScale;

};
}
#endif /* COLORSCALEPREVIEW_H_ */
