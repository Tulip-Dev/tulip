//-*-c++-*-
/*
 Author: Antoine Lambert

 Email : antoine.lambert@labri.fr

 Last modification : 12/08

 This program is free software; you can redistribute it and/or modify  *
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
*/

#ifndef   	PARALLELTOOLS_H_
#define   	PARALLELTOOLS_H_

#include <tulip/Coord.h>
#include <tulip/Color.h>
#include <tulip/GlLine.h>
#include <string>
#include <sstream>
#include <iostream>


namespace tlp {

enum viewType {VIEW_2D, VIEW_2D_SPLINE};

const std::string DEFAULT_TEXTURE_FILE = "parallel_texture.png";

const std::string SLIDER_TEXTURE_NAME = "parallel_sliders_texture.png";

const Color COLOR_SELECT(255, 102, 255, 255);

const Color COLOR_NON_HIGHLIGHT(128, 128, 128, 10);

const Color PROGRESS_BAR_COLOR(109, 97, 158);

// data count threshold from which a progress bar is displayed on screen
// when creating or refreshing the view
const unsigned int PROGRESS_BAR_DISPLAY_NB_DATA_THRESHOLD = 5000;

const unsigned int DEFAULT_LINES_COLOR_ALPHA_VALUE = 20;

const float DEFAULT_AXIS_HEIGHT = 400;

const float DEFAULT_GRAD_WIDTH = 6;

const float DEFAULT_GRAD_HEIGHT = 2;

const float DEFAULT_LABELS_HEIGHT = 15;

const float DEFAULT_BITMAPS_ZOOM_FACTOR = 5;

const float DEFAULT_CAPTION_HEIGHT = 30;

const float DEFAULT_CAPTION_VERTICAl_OFFSET = 20;

const unsigned int NB_VIEW_PROPERTIES = 14;

const std::string viewPropertiesName[NB_VIEW_PROPERTIES] = {"viewBorderColor", "viewBorderWidth", "viewColor", "viewLabel", "viewLabelColor", "viewLabelPosition", "viewLayout", "viewMetaGraph", "viewMetric", "viewRotation", "viewSelection", "viewShape", "viewSize", "viewTexture"};

template <typename T>
std::string getStringFromNumber(T number) {
  std::ostringstream oss;
  oss.precision(5);
  oss << number;
  return oss.str();
}

}

#endif 	    /* !PARALLELTOOLS_H_ */
