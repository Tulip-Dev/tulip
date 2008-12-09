/*
 * GlProgressBar.h
 *
 *  Created on: 16 nov. 2008
 *      Author: Antoine Lambert
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
