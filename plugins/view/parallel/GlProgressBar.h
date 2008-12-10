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

#include <tulip/GlComposite.h>

namespace tlp {

class TLP_GL_SCOPE GlProgressBar : public GlComposite {

public :

	GlProgressBar(const Coord &centerPosition, const unsigned int width, const unsigned int height, const Color &color);
	~GlProgressBar();

	void setComment(const std::string& comment) {this->comment = comment;}
	void setProgress(const int current, const int max);

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
