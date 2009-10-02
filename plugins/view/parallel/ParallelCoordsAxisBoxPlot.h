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

#ifndef PARALLELCOORDSAXISBOXPLOT_H_
#define PARALLELCOORDSAXISBOXPLOT_H_

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#ifdef  _WIN32
// compilation pb workaround
#include <windows.h>
#endif
#include <QtGui/qcursor.h>
#include <QtGui/qevent.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string>
#include <map>

#include <tulip/GlPolygon.h>
#include <tulip/GlQuad.h>
#include <tulip/GlLines.h>
#include <tulip/GlLabel.h>
#include <tulip/InteractorComponent.h>

namespace tlp {

class GlAxisBoxPlot : public GlSimpleEntity {

public :

	GlAxisBoxPlot(QuantitativeParallelAxis *axis, const Color& fillColor, const Color& outlineColor);
	~GlAxisBoxPlot() {}

	void draw(float lod,Camera* camera);

	void getXML(xmlNodePtr rootNode) {}

	void setWithXML(xmlNodePtr rootNode) {}

	void setHighlightRangeIfAny(Coord sceneCoords);


private :

	void drawLabel(Coord position, std::string labelName);

	QuantitativeParallelAxis *axis;
	Coord bottomOutlierCoord;
	Coord firstQuartileCoord;
	Coord medianCoord;
	Coord thirdQuartileCoord;
	Coord topOutlierCoord;
	float boxWidth;
	Color fillColor, outlineColor;
	Coord *highlightRangeLowBound;
	Coord *highlightRangeHighBound;

};

class ParallelCoordsAxisBoxPlot : public InteractorComponent {

public :

	ParallelCoordsAxisBoxPlot();
	~ParallelCoordsAxisBoxPlot();
	bool eventFilter(QObject *, QEvent *);
	bool draw(GlMainWidget *glMainWidget);
	bool compute(GlMainWidget *glMainWidget);
	InteractorComponent *clone() { return new ParallelCoordsAxisBoxPlot(); }
	void setView(View *view);

private :

	void buildGlAxisPlot(std::vector<ParallelAxis *> currentAxis);
	void deleteGlAxisPlot();

	void initOrUpdateBoxPlots();

	ParallelCoordinatesView *parallelView;
	Graph *currentGraph;
	std::map<QuantitativeParallelAxis *, GlAxisBoxPlot *> axisBoxPlotMap;
	ParallelAxis *selectedAxis;
	unsigned int lastNbAxis;

};

}

#endif /* PARALLELCOORDSBOXPLOT_H_ */
