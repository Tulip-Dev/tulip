/*
 * QtGlSceneZoomAndPanAnimator.h
 *
 *  Created on: 20 oct. 2009
 *      Author: antoine
 */

#ifndef QTGLSCENEZOOMANDPANANIMATOR_H_
#define QTGLSCENEZOOMANDPANANIMATOR_H_

#include <QtCore/QObject>

#include <tulip/GlMainWidget.h>

#include "tulip/GlSceneZoomAndPan.h"

namespace tlp {

/**
 * \brief A convenient class to perform a Zoom and Pan animation on a GlMainWidget
 *
 * This class allows to perform a Zoom and Pan animation on the Tulip OpenGL scene associated with a GlMainWidget
 *  The algorithm used to perform this task is the one published in : Jarke J. van Wijk and Wim A.A. Nuij, "Smooth and efficient zooming and panning"
 *  For more details, the paper can be downloaded at the following url : www.win.tue.nl/~vanwijk/zoompan.pdf
 */
class TLP_QT_SCOPE QtGlSceneZoomAndPanAnimator : public QObject, public GlSceneZoomAndPan {

	Q_OBJECT

public :

	/**
	 * QtGlSceneZoomAndPanAnimator constructor
	 *
	 * \param glWidget The Tulip OpenGL widget on which to perform the animation
	 * * \param boundingBox the bounding box in scene coordinates on which the Tulip OpenGL scene has to be zoomed and panned.
	 * 					    At the end of the animation, the viewport will be zoomed and centered on the content of that bounding box.
	 * \param optimalPath if true zooming and panning will be combined at each step of the animation, if false the scene will be zoomed out/in, panned then zoomed in/out
	 * \param velocity animation speed parameter, adjust it according to your needs
	 * \param p zoom/pan trade-off parameter, adjust it according to your needs
	 */
	QtGlSceneZoomAndPanAnimator(GlMainWidget *glWidget, const BoundingBox &boundingBox, const bool optimalPath = true, const double velocity = 1.1, const double p = sqrt(1.6));

	/**
	 * Method to call to start the Zoom and Pan animation
	 */
	void animateZoomAndPan();

protected slots :

	virtual void zoomAndPanAnimStepSlot(int animationStep);

protected :

	GlMainWidget *glWidget;
	double animationDurationMsec;

};

}


#endif /* QTGLSCENEZOOMANDPANANIMATOR_H_ */
