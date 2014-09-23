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

#ifndef     PARALLELTOOLS_H_
#define     PARALLELTOOLS_H_

#include <tulip/Coord.h>
#include <tulip/Color.h>
#include <tulip/GlLine.h>
#include <tulip/TlpTools.h>

#include <string>
#include <sstream>
#include <iostream>


namespace tlp {

const std::string DEFAULT_TEXTURE_FILE = "parallel_texture";

const std::string SLIDER_TEXTURE_NAME = "parallel_sliders_texture";

// data count threshold from which a progress bar is displayed on screen
// when creating or refreshing the view
const unsigned int PROGRESS_BAR_DISPLAY_NB_DATA_THRESHOLD = 5000;

const unsigned int DEFAULT_LINES_COLOR_ALPHA_VALUE = 200;

const float DEFAULT_AXIS_HEIGHT = 400;

const float DEFAULT_GRAD_WIDTH = 6;

const float DEFAULT_GRAD_HEIGHT = 2;

const float DEFAULT_LABELS_HEIGHT = 15;

const float DEFAULT_BITMAPS_ZOOM_FACTOR = 5;

const float DEFAULT_CAPTION_HEIGHT = 30;

const float DEFAULT_CAPTION_VERTICAl_OFFSET = 20;

template <typename T>
std::string getStringFromNumber(T number) {
  std::ostringstream oss;
  oss.precision(5);
  oss << number;
  return oss.str();
}

float computeABACAngleWithAlKashi(const Coord &A, const Coord &B, const Coord &C);

const int X_ROT = 0;
const int Y_ROT = 1;
const int Z_ROT = 2;
void rotateVector(Coord &vec, float alpha, int rot);

Coord *computeStraightLineIntersection(const Coord line1[2], const Coord line2[2]);

}

#endif      /* !PARALLELTOOLS_H_ */
