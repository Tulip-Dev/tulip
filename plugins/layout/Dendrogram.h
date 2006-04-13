#ifndef DENDROGRAM_H
#define DENDROGRAM_H

#include <map>
#include <vector>
#include <tulip/TulipPlugin.h>

class OrientableLayout;
class OrientableCoord;
class OrientableSizeProxy;

/** \addtogroup layout */
/*@{*/
/** This plugin is an implementation of a dendrogram, an extended implementation
 *  of a "Bio representation" which includes variable orientation 
 *  and variable node sizelayout. 
 * 
 *  \note This works on tree.
 *  Let n be the number of nodes, the algorithm complexity is in O(n).
 *
 *  \author Julien Testut, Antony Durand, Pascal Ollier, Yashvin Nababsing, \n 
 *  Sebastien Leclerc, Thibault Ruchon, Eric Dauchier \n
 *  University Bordeaux I France
 **/
class Dendrogram: public LayoutAlgorithm {
public:
    Dendrogram(const PropertyContext&);
    ~Dendrogram();
  
    bool run();
    bool check(std::string&);
    void reset();

private: 
    static const float      INTER_NODE_DISTANCE_X;  
    static const float      INTER_NODE_DISTANCE_Y;  
    
    std::map<node, float>   leftshift;
    node                    root;
    OrientableLayout*       oriLayout;
    OrientableSizeProxy*    oriSize;

    float   setAllNodesCoordX(node n, float rightMargin);
    void    setAllNodesCoordY();
    float   computeFatherXPosition(node father);
    void    shiftAllNodes(node n, float shift);
    void    setNodePosition(node n, float x, float y, float z);
    void    setCoordY(node n, float* maxYLeaf, float* maxHeightLeaf);
};
/*@}*/
#endif



