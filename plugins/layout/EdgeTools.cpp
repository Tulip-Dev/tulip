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
