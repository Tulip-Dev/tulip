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
#include "Orientation.h"
#include "EdgeTools.h"

using namespace tlp;

//====================================================================
void setOrthogonalEdge(OrientableLayout* oriLayout,
		       const Graph* tree,
		       float interNodeDistance) {
    Iterator<node>* itNode = tree->getNodes();
    while (itNode->hasNext()) {
        node            currentNode      = itNode->next();
	OrientableCoord currentNodeCoord = oriLayout->getNodeValue(currentNode);
        Iterator<edge>* itEdge           = tree->getOutEdges(currentNode);
        while (itEdge->hasNext())
            addControlPoints(oriLayout, tree, currentNodeCoord, 
                             itEdge->next(), interNodeDistance);
        delete itEdge;
    }
    delete itNode;
}

//====================================================================
void addControlPoints(OrientableLayout* oriLayout,
                      const Graph* tree, OrientableCoord fatherCoord,
                      edge e, float interNodeDistance) {
    node child                  = tree->target(e);
    node father                 = tree->source(e);
    OrientableCoord childCoord  = oriLayout->getNodeValue(child); 

    if (fatherCoord.getX() != childCoord.getX()) { 
        OrientableLayout::LineType  newControlPoints;     
        
        float coordModifier  = interNodeDistance / 2.f;
        
        OrientableCoord coord  = oriLayout->createCoord();
        float           coordY = fatherCoord.getY() + coordModifier;
        coord.set(fatherCoord.getX(), coordY, 0);    
        newControlPoints.push_back(coord);
    
    
        coord.set(childCoord.getX(), coordY, 0);
        newControlPoints.push_back(coord);
        
        oriLayout->setEdgeValue(e, newControlPoints);
    }
}
