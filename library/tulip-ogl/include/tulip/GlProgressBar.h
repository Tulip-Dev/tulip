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

#ifndef GLPROGRESSBAR_H_
#define GLPROGRESSBAR_H_

#include "tulip/GlComposite.h"
#include <tulip/PluginProgress.h>

const std::string SLIDER_TEXTURE_NAME = "cylinderTexture.png";

namespace tlp {
/**
 * 	This class allows to display a progress bar in OpenGL
 */
class TLP_GL_SCOPE GlProgressBar : public GlComposite, public PluginProgress {

public :

	/**
	 * GlProgressBar constructor
	 * \param centerPosition the position of the center of the frame containing the progress bar
	 * \param width the width of the frame containing the progress bar
	 * \param height the height of the frame containing the progress bar
	 * \param color the color of the progress bar
	 */
	GlProgressBar(const Coord &centerPosition, const unsigned int width, const unsigned int height, const Color &color);
	~GlProgressBar();

	/**
	 * This method allow to set a comment message above the progress bar
	 * \param msg the text of the comment to display
	 */
	void setComment(const std::string& msg) {comment = msg;}
	void progress_handler(int step, int max_step);

private :

	float width, height;
	Color color, percentLabelAndFrameColor;
	float progressBarMaxWidth, progressBarHeight;
	float commentWidth, commentHeight;
	Coord progressBarTLCorner;
	Coord commentLabelCenter;

	unsigned int currentPercent;
	std::string comment;
};

}
#endif /* GLPROGRESSBAR_H_ */
