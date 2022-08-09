/*
 *
 * This file is part of Tulip (https://tulip.labri.fr)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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

// +-------------------------------------------------------------------------+
// | Tulip Python Bindings                                                   |
// | inspired from bindings by the Booggie project development team          |
// | (https://sourceforge.net/projects/booggie/)                             |
// +-------------------------------------------------------------------------+

%ModuleHeaderCode
#include <tulip/MapIterator.h>
namespace tlp {
typedef tlp::Iterator<tlp::node> IteratorNode;
typedef tlp::Iterator<tlp::edge> IteratorEdge;
}
%End

namespace tlp {

typedef tlp::Iterator<tlp::node> IteratorNode;
typedef tlp::Iterator<tlp::edge> IteratorEdge;

tlp::edge nextFaceEdge(tlp::Graph* g, tlp::edge source, tlp::node target);

class NodeMapIterator : tlp::IteratorNode {
%TypeHeaderCode
#include <tulip/MapIterator.h>
%End  
  
public :
  
  NodeMapIterator(tlp::Graph *sg, tlp::node source, tlp::node target);
  ~NodeMapIterator();

};

class EdgeMapIterator : tlp::IteratorEdge {
%TypeHeaderCode
#include <tulip/MapIterator.h>
%End    

public :
  
  EdgeMapIterator(tlp::Graph *sg, tlp::edge source, tlp::node target);
  ~EdgeMapIterator();
  
};

};

