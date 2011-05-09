/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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
#ifndef Tulip_GLVERTEXARRAYMANAGER_H
#define Tulip_GLVERTEXARRAYMANAGER_H

#include <cassert>
#include <iostream>
#include <tulip/OpenGlConfigManager.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tulip/tulipconf.h>
#include <tulip/Coord.h>
#include <tulip/Color.h>
#include <tulip/Size.h>
#include <tulip/tuliphash.h>
#include <tulip/Observable.h>
#include <tulip/ObservableGraph.h>
#include <tulip/ObservableProperty.h>

#include <vector>

namespace tlp {

class Graph;
class GlEdge;
class GlNode;
class GlGraphInputData;

/** \brief Class used to render edges/nodes with vertex array
 *
 * Class used to render edges/nodes with vertex array
 */
class TLP_GL_SCOPE GlVertexArrayManager :
 private GraphObserver, private PropertyObserver, private Observable {

public:

	GlVertexArrayManager(GlGraphInputData *inputData);

	~GlVertexArrayManager();

	void setInputData(GlGraphInputData *inputData);

	inline bool renderingIsBegin(){
		return isBegin;
	}

	bool haveToCompute();

	void setHaveToComputeAll(bool compute);
	void setHaveToComputeLayout(bool compute);
	void setHaveToComputeColor(bool compute);

	void beginRendering();
	void endRendering();

	void addEdge(GlEdge *edge);
	void addNode(GlNode *node);

	void pauseRendering(bool pause);
	void activate(bool act);

	void activateLineEdgeDisplay(GlEdge *edge, bool selected);
	void activatePointEdgeDisplay(GlEdge *edge, bool selected);
	void activatePointNodeDisplay(GlNode *node, bool onePixel, bool selected);

protected:

	void propertyValueChanged(PropertyInterface *property);

	void addEdge(Graph *,const edge);
	void addNode(Graph *,const node);
	void beforeSetAllNodeValue(PropertyInterface*);
	void beforeSetAllEdgeValue(PropertyInterface*);
	void beforeSetNodeValue(PropertyInterface*, const node);
	void beforeSetEdgeValue(PropertyInterface*, const edge);
	void addLocalProperty(Graph*, const std::string&);
	void delLocalProperty(Graph*, const std::string&);

	void destroy(Graph *);
	void destroy(PropertyInterface*);

	void clearLayoutData();
	void clearColorData();
	void clearData();
	void initObservers();
	void clearObservers();

	GlGraphInputData *inputData;
	Graph *graph;
	bool graphObserverActivated;
	bool layoutObserverActivated;
	bool colorObserverActivated;

	bool activated;
	bool isBegin;
	bool toComputeAll;
	bool toComputeLayout;
	bool toComputeColor;

	bool vectorLayoutSizeInit;
	bool vectorColorSizeInit;
	bool vectorIndexSizeInit;

	bool colorInterpolate;
	bool edgesModified;

	std::vector<Coord> linesCoordsArray;
	std::vector<Color> linesColorsArray;
	std::vector<GLint> linesIndexArray;
	std::vector<GLsizei> linesIndexCountArray;
	std::vector<GLuint> linesRenderingIndexArray;
	std::vector<GLuint> linesSelectedRenderingIndexArray;

	std::vector<Coord> pointsCoordsArray;
	std::vector<Color> pointsColorsArray;

	std::vector<GLuint> points1PNodesRenderingIndexArray;
	std::vector<GLuint> points1PNodesSelectedRenderingIndexArray;
	std::vector<GLuint> points2PNodesRenderingIndexArray;
	std::vector<GLuint> points2PNodesSelectedRenderingIndexArray;

	std::vector<GLuint> points1PEdgesRenderingIndexArray;
	std::vector<GLuint> points1PEdgesSelectedRenderingIndexArray;


	std::vector<std::pair<unsigned int,unsigned int> > edgeToLineIndexVector;
	std::vector<unsigned int> edgeToPointIndexVector;
	std::vector<unsigned int> nodeToPointIndexVector;
};

}

#endif
