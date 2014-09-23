/*
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
///@cond DOXYGEN_HIDDEN


#ifndef GLPROGRESSBAR_H_
#define GLPROGRESSBAR_H_

#include <tulip/GlComposite.h>
#include <tulip/SimplePluginProgress.h>
#include <tulip/Color.h>
#include <tulip/Coord.h>

const std::string SLIDER_TEXTURE_NAME = "cylinderTexture.png";

namespace tlp {
/**
 *  This class allows to display a progress bar in OpenGL
 */
class TLP_GL_SCOPE GlProgressBar : public GlComposite, public SimplePluginProgress {

public :

  /**
   * GlProgressBar constructor
   * \param centerPosition the position of the center of the frame containing the progress bar
   * \param width the width of the frame containing the progress bar
   * \param height the height of the frame containing the progress bar
   * \param progressBarColor the color of the progress bar
   * \param commentColor the color of the comment and the frame
   */
  GlProgressBar(const Coord &centerPosition, const unsigned int width, const unsigned int height, const Color& progressBarcolor, const Color& commentColor = Color(0, 0, 0));

  ~GlProgressBar();

  /**
   * This method allow to set a comment message above the progress bar
   * \param msg the text of the comment to display
   */
  void setComment(const std::string& msg) {
    comment = msg;
  }
protected:
  void progress_handler(int step, int max_step);

private :

  Color progressBarColor, commentColor;
  float progressBarMaxWidth, progressBarHeight;
  float commentWidth, commentHeight;
  Coord progressBarTLCorner;
  Coord commentLabelCenter;

  unsigned int currentPercent;
  std::string comment;
};

}
#endif /* GLPROGRESSBAR_H_ */
///@endcond
