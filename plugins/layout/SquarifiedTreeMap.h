#ifndef SQUARIFIEDTREEMAP_H
#define SQUARIFIEDTREEMAP_h

#include <vector>
#include <utility>
#include <tulip/tuliphash.h>
#include "tulip/TulipPlugin.h"
#include "tulip/Rectangle.h"

typedef std::vector<tlp::node> VecNode;
typedef TLP_HASH_MAP<tlp::node, double> MapNode;
typedef std::pair<tlp::node, double> PairNodeF;
typedef std::vector<PairNodeF> PairVector;
/** \addtogroup layout */
/*@{*/
/// SquarifiedTreeMap.h - An implementation of a squarified treemap layout.
/** This plugin is an implementation of TreeMap and Squarified treemap layout.
 * 
 *  Squarified Treemaps : \n
 *  Bruls, M., Huizing, K., & van Wijk, J. J. \n
 *  In Proc. of Joint Eurographics and IEEE TCVG Symp. on Visualization \n
 *  (TCVG 2000) IEEE Press, pp. 33-42.
 *
 *  Shneiderman, B. (March 1991)
 *  Tree visualization with treemaps: a 2-d space-filling approach
 *  ACM Transactions on Graphics, vol. 11, 1 (Jan. 1992) 92-99.
 *  HCIL-91-03, CS-TR-2645, CAR-TR-548
 *
 *  \note This algorith only works on tree.
 *
 *  @version 1.0.0 complete rewrite, merge treemap and squarified in the same algorithm
 *  simplify implementation. The algorithm can be tune to go 2 or 3 time faster however, since the algorithm
 *  is fast enough, the code is written to be easily read and maintain.
 *  @author Auber David
 *
 *  @version 0.0.0
 *  @author Julien Testut, Antony Durand, Pascal Ollier, Yashvin Nababsing, \n
 *  Sebastien Leclerc, Thibault Ruchon, Eric Dauchier \n
 *  University Bordeaux I France
 */
class SquarifiedTreeMap: public tlp::LayoutAlgorithm {
   friend class SquarifiedTreeMapUnitTests; 

public:
    SquarifiedTreeMap(const tlp::PropertyContext&);
    ~SquarifiedTreeMap();

    bool check(std::string&);
    bool run();

private:
    tlp::SizeProperty*         size;
    tlp::DoubleProperty*       metric;
    tlp::IntegerProperty*      glyph;
    tlp::MutableContainer<double>    nodesSize;
    bool shneidermanTreeMap;
    double aspectRatio;
    /**
    * return a measure quality of row in which one wants ot add n
    * width is the width of the rectangle in which we create the row
    * length is the height of the rectangle in wich on creates the row
    * surface is sum of size of elements what belongs to the rectangle
    */
    double evaluateRow(const std::vector<tlp::node> &row, tlp::node n, double width, double length, double surface);
    void layoutRow(const std::vector<tlp::node> &row, const int depth, const tlp::Rectangle<double> &rectArea);
    void squarify(const std::vector<tlp::node> &toTreat, const tlp::Rectangle<double> &rectArea, const int depth);
    //change the rectangle to take into account space reserved for the drawing of borders and headers
    //the function currently fix adjust the size for the 2D windwows glyph.
    tlp::Rectangle<double> adjustRectangle(const tlp::Rectangle<double> &r) const;
    //return a vector containing children of n ordered in decreasing order of their size.
    std::vector<tlp::node> orderedChildren(const tlp::node n) const;
    /**
    * compute the size of each node in the tree
    * the size is the sum of all the size of all leaves descendant of a node
    * in the tree.
    */
    void   computeNodesSize(tlp::node n);
};
/*@}*/
#endif
