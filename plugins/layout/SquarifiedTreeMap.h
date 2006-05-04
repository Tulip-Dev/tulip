#ifndef SQUARIFIEDTREEMAP_H
#define SQUARIFIEDTREEMAP_h

#if (__GNUC__ < 3)
#include <hash_map>
#else
#include <ext/hash_map>
#endif

#include <vector>
#include <utility>
#include "tulip/TulipPlugin.h"
#include "tulip/RectangleArea.h"

typedef std::vector<node> vecNode;
typedef stdext::hash_map<node, float> mapNode;
typedef std::pair<node, float> pairNodeF;
typedef std::vector<pairNodeF> pairVector;
typedef pairVector::iterator pairIterator;

/** \addtogroup layout */
/*@{*/
/// SquarifiedTreeMap.h - An implementation of a squarified treemap layout.
/** This plugin is an implementation of a Squarified treemap layout.
 * 
 *  Squarified Treemaps : \n
 *  Bruls, M., Huizing, K., & van Wijk, J. J. \n
 *  In Proc. of Joint Eurographics and IEEE TCVG Symp. on Visualization \n
 *  (TCVG 2000) IEEE Press, pp. 33-42.
 * 
 *  \note This algorith only works on tree.
 *  Let n be the number of nodes, the algorithm complexity is in O(n).
 *
 *  \author Julien Testut, Antony Durand, Pascal Ollier, Yashvin Nababsing, \n 
 *  Sebastien Leclerc, Thibault Ruchon, Eric Dauchier \n
 *  University Bordeaux I France
 */
class SquarifiedTreeMap: public LayoutAlgorithm {
   friend class SquarifiedTreeMapUnitTests; 

public:
    SquarifiedTreeMap(const PropertyContext&);
    ~SquarifiedTreeMap();

    bool check(std::string&);
    bool run();
    void reset();

private:
    SizeProperty*     size;   
    DoubleProperty*    metric;
    IntegerProperty*   glyph;
    mapNode    sumChildrenMetric;
    float      aspectRatio;
    
    void    layRow(pairIterator firstChildNode, pairIterator endChildNode,
                   int depth, RectangleArea rectArea, float listMetric);
    void    squarify(node n, RectangleArea rectArea, int depth);
    float   findWorstRatio(float metric1, float metric2, float listMetric, 
                           const RectangleArea& rectArea);
    float   initializeMapSum(node n);
    bool    verifyMetricIsPositive();       

};
/*@}*/
#endif
