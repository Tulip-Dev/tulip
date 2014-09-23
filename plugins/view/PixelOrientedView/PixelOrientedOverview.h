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

#ifndef PIXELORIENTEDDRAWING_H_
#define PIXELORIENTEDDRAWING_H_

#include "POLIB/TulipGraphDimension.h"
#include "POLIB/PixelOrientedMediator.h"

#include <tulip/GlSimpleEntity.h>
#include <tulip/GlBoundingBoxSceneVisitor.h>
#include <tulip/Graph.h>
#include <tulip/GlRect.h>
#include <tulip/GlMainWidget.h>
#include <tulip/GlLabel.h>

#include <string>

namespace tlp {

class SizeProperty;
class LayoutProperty;

class PixelOrientedOverview : public GlComposite {

public :

  PixelOrientedOverview(pocore::TulipGraphDimension *data,
                        pocore::PixelOrientedMediator *pixelOrientedMediator,
                        Coord blCornerPos,
                        const std::string &dimName,
                        const Color &backgroundColor, const Color &textColor);

  ~PixelOrientedOverview();

  pocore::TulipGraphDimension *getData() const {
    return data;
  }
  std::string getDimensionName() const {
    return dimName;
  }
  LayoutProperty *getPixelViewLayout() const {
    return pixelLayout;
  }
  SizeProperty *getPixelViewSize() const {
    return pixelSize;
  }

  void setBLCorner(const Coord &blCorner);

  void computePixelView(GlMainWidget *glWidget = NULL);

  bool overviewGenerated() const  {
    return overviewGen;
  }

  void setBackgroundColor(const Color &color) {
    backgroundColor = color;
  }
  void setTextColor(const Color &color) {
    textColor = color;
  }

private :

  void computeBoundingBox() {
    GlBoundingBoxSceneVisitor glBBSV(NULL);
    acceptVisitor(&glBBSV);
    boundingBox = glBBSV.getBoundingBox();
  }

  pocore::TulipGraphDimension *data;
  pocore::PixelOrientedMediator *pixelOrientedMediator;

  GlGraphComposite *graphComposite;
  LayoutProperty *pixelLayout;
  SizeProperty *pixelSize;

  Coord blCornerPos;
  std::string textureName;
  std::string dimName;

  GlLabel *clickLabel;
  GlRect *backgroundRect;
  GlLabel *overviewLabel;
  GlRect *frame;
  GlRect *frame2;

  int overviewId;
  static int overviewCpt;

  bool overviewGen;

  Color backgroundColor, textColor;

};

}
#endif /* PIXELORIENTEDDRAWING_H_ */
