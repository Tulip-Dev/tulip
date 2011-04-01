%ModuleHeaderCode
#include <tulip/DrawingTools.h>
%End

namespace tlp { 
 
 tlp::BoundingBox computeBoundingBox(const tlp::Graph *graph,
                                     const tlp::LayoutProperty *layout,
                                     const tlp::SizeProperty *size,
                                     const tlp::DoubleProperty *rotation,
                                     const tlp::BooleanProperty *selection = 0);

 tlp::BoundingBox computeBoundingBox(tlp::Iterator<tlp::node> *itN,
									 tlp::Iterator<tlp::edge> *itE,
									 const tlp::LayoutProperty *layout,
									 const tlp::SizeProperty *size,
									 const tlp::DoubleProperty *rotation,
									 const tlp::BooleanProperty *selection = 0);

 std::pair<tlp::Coord, tlp::Coord> computeBoundingRadius(const tlp::Graph *graph,
                                                         const tlp::LayoutProperty *layout,
                                                         const tlp::SizeProperty *size,
                                                         const tlp::DoubleProperty *rotation,
                                                         const tlp::BooleanProperty *selection = 0);

 std::vector<tlp::Coord> computeConvexHull(const tlp::Graph *graph,
                                           const tlp::LayoutProperty *layout,
                                           const tlp::SizeProperty *size,
                                           const tlp::DoubleProperty *rotation,
                                           const tlp::BooleanProperty *selection = 0);	
                                
};