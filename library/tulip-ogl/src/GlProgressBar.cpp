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
#include <tulip/GlProgressBar.h>
#include <tulip/GlPolygon.h>
#include <tulip/GlLabel.h>
#include <tulip/GlQuad.h>
#include <tulip/TlpTools.h>

#include <vector>
#include <sstream>

using namespace std;

static const string PROGRESS_BAR_ID = "progress bar quad";
static const string COMMENT_ID = "comment label";
static const string PERCENT_ID = "percent label";

namespace tlp {

GlProgressBar::GlProgressBar(const Coord &centerPosition,
                             const unsigned int width,
                             const unsigned int height,
                             const Color &pbColor, const Color& commColor) :
  progressBarColor(pbColor), commentColor(commColor) {
  Coord globalFrameVertice1 = Coord(centerPosition.getX() - (width / 2.), centerPosition.getY() + (height/2.));
  Coord globalFrameVertice2 = Coord(centerPosition.getX() + (width / 2.), centerPosition.getY() + (height/2.));
  Coord globalFrameVertice3 = Coord(centerPosition.getX() + (width / 2.), centerPosition.getY() - (height/2.));
  Coord globalFrameVertice4 = Coord(centerPosition.getX() - (width / 2.), centerPosition.getY() - (height/2.));

  vector<Coord> globalFrameCoords;
  globalFrameCoords.push_back(globalFrameVertice1);
  globalFrameCoords.push_back(globalFrameVertice2);
  globalFrameCoords.push_back(globalFrameVertice3);
  globalFrameCoords.push_back(globalFrameVertice4);

  vector<Color> globalFrameColors;
  globalFrameColors.push_back(commentColor);
  globalFrameColors.push_back(commentColor);
  globalFrameColors.push_back(commentColor);
  globalFrameColors.push_back(commentColor);

  addGlEntity(new GlPolygon(globalFrameCoords, globalFrameColors, globalFrameColors, false, true), "global frame");

  progressBarMaxWidth = (4./5.) * width;
  progressBarHeight = (4./5.) * (height / 2.);
  commentWidth = (4./5.) * width;
  commentHeight = (4./5.) * (height / 2.);

  progressBarTLCorner = Coord(centerPosition.getX() - ((2./5.) * width), centerPosition.getY() - ((1./10.) * (height / 2.)), 0);
  commentLabelCenter = Coord(centerPosition.getX(), centerPosition.getY() + (height / 4.), 0);

  Coord progressBarFrameVertice1 = progressBarTLCorner + Coord(-2,2,0);
  Coord progressBarFrameVertice2 = progressBarFrameVertice1 + Coord(progressBarMaxWidth + 2, 0, 0);
  Coord progressBarFrameVertice3 = progressBarFrameVertice2 + Coord(0, -(progressBarHeight + 2), 0);
  Coord progressBarFrameVertice4 = progressBarFrameVertice3 + Coord(-(progressBarMaxWidth + 2), 0,0);

  vector<Coord> progressBarFrameCoords;
  progressBarFrameCoords.push_back(progressBarFrameVertice1);
  progressBarFrameCoords.push_back(progressBarFrameVertice2);
  progressBarFrameCoords.push_back(progressBarFrameVertice3);
  progressBarFrameCoords.push_back(progressBarFrameVertice4);

  vector<Color> progressBarFrameColors;
  progressBarFrameColors.push_back(commentColor);
  progressBarFrameColors.push_back(commentColor);
  progressBarFrameColors.push_back(commentColor);
  progressBarFrameColors.push_back(commentColor);

  addGlEntity(new GlPolygon(progressBarFrameCoords, progressBarFrameColors, progressBarFrameColors, false, true), "progress bar frame");
}

GlProgressBar::~GlProgressBar() {
  reset(true);
}

void GlProgressBar::progress_handler(int step, int max_step) {
  currentPercent = (unsigned int) (((double) ((double) step / (double) max_step)) * 100.);

  GlSimpleEntity *previousBar = findGlEntity(PROGRESS_BAR_ID);
  GlSimpleEntity *previousComment = findGlEntity(COMMENT_ID);
  GlSimpleEntity *previousPercent = findGlEntity(PERCENT_ID);

  if (previousBar != NULL) {
    deleteGlEntity(previousBar);
    delete previousBar;
  }

  if (previousComment != NULL) {
    deleteGlEntity(previousComment);
    delete previousComment;
  }

  if (previousPercent != NULL) {
    deleteGlEntity(previousPercent);
    delete previousPercent;
  }

  float progressBarWidth =  (currentPercent * progressBarMaxWidth) / 100.;

  if (progressBarWidth == 0)
    progressBarWidth = 1; // Avoid assert in GlAbstractPolygon::draw

  Coord progressBarCoords[4];
  progressBarCoords[0] = progressBarTLCorner;
  progressBarCoords[1] = progressBarCoords[0] + Coord(progressBarWidth, 0, 0);
  progressBarCoords[2] = progressBarCoords[1] + Coord(0, -progressBarHeight, 0);
  progressBarCoords[3] = progressBarCoords[2] + Coord(-progressBarWidth, 0, 0);
  GlQuad *progressBarQuad = new GlQuad(progressBarCoords[0], progressBarCoords[1], progressBarCoords[2], progressBarCoords[3], progressBarColor);
  progressBarQuad->setTextureName(TulipBitmapDir + SLIDER_TEXTURE_NAME);

  GlLabel *commentLabel = new GlLabel(commentLabelCenter, Size(commentWidth, commentHeight, 0), commentColor);
  commentLabel->setText(comment);

  GlLabel *percentLabel = new GlLabel(Coord(progressBarTLCorner.getX() + (progressBarMaxWidth / 2.), progressBarTLCorner.getY() - (progressBarHeight / 2.), 0),
                                      Size(((1./10.) * progressBarMaxWidth), ((8./10.) * progressBarHeight), 0), commentColor);
  stringstream str;
  str << currentPercent << " %";
  percentLabel->setText(str.str());

  addGlEntity(progressBarQuad, PROGRESS_BAR_ID);
  addGlEntity(commentLabel, COMMENT_ID);
  addGlEntity(percentLabel, PERCENT_ID);
}

}
