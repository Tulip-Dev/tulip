//-*-c-*-
// The API works with 3 major types
// Graph, node, edge

// a node is build from a vec2
// corresponding to the position of the
// node's infos in the degrees and adjacency_lists_pos structures
// we define a specific type to ease forEach implementation
struct node {
  vec2 pos;
};

#define NODE_TO_VEC2(n) n.pos
#define VEC2_TO_NODE(v) node(v)

// an edge is build from a vec2
// corresponding to the position of the
// egde's infos (source & target nodes) in the list of edges structure
// we define a specific type to ease forEach implementation
struct edge {
  vec2 pos;
};

#define EDGE_TO_VEC2(e) e.pos
#define VEC2_TO_EDGE(v) edge(v)

// declaration of a graph structure
struct Graph {
  // the number of nodes
  float nbNodes;
  // the number of edges
  float nbEdges;
  // precomputed values
  // ceil(sqrt(nbNodes))
  float nSize;
  // ceil(sqrt(nbEdges))
  float eSize;
  // ceil(sqrt(2 * nbEdges))
  float aSize;
  // 1.0/aSize
  float incr;
  // (aSize - 1)/ aSize
  float x_max;
};

// the degrees and adjacency lists position for each node
// each element is a vec4
// elt.x is for inDegree
// elt.y is for outDegree
// elt.zw gives the position of a node's first
// corresponding edge in the adjacency lists,
// the dimension of the used texture is
// ceil(sqrt(nbNodes)) x ceil(sqrt(nbNodes))
uniform sampler2D degrees_and_adj_pos;

// the adjacency lists
// each element is an edge i.e a vec2 given a position
// in the egdes lists,
// from the cpu point of view an edge is stored as
// a couple of unsigned 16 bits integers
// the dimension of the used texture is
// nbEdges x nbEdges
uniform sampler2D adjacency_lists;

// the edges list
// each element is a vec4,
// v.xy is the source node
// v.zw is the target node
// from the cpu point of view an elt is stored as
// 2 couples of unsigned 16 bits integers
// the dimension of the used texture is
// ceil(sqrt(nbEdges)) x ceil(sqrt(nbEdges))
uniform sampler2D edges;

// translate an xy struct ([0,1]x[0,1]) pos
// in a texture into an index [0, nbElts - 1] in a 1-dim array
// pos is a vec2, size is ceil(sqrt(nbElts))
#define POS_TO_INDEX(pos, size) (floor(pos.x * size) + floor(pos.y * size) * size)

// these ones are for debugging purpose
#define NODE_ID(n, g) POS_TO_INDEX(NODE_TO_VEC2(n), g.nSize)
#define EDGE_ID(n, g) POS_TO_INDEX(EDGE_TO_VEC2(n), g.eSize)

//-----------------------------------------------------------
// Public basic functions to get Graph infos 
//-----------------------------------------------------------
// the number of nodes in a graph
int numberOfNodes(const Graph g) {
  return int(g.nbNodes);
}

// the number of edges in a graph
int numberOfEdges(const Graph g) {
  return int(g.nbEdges);
}

//-----------------------------------------------------------
// Public basic functions to get node infos 
//-----------------------------------------------------------
// some macros to deal with the vec2 object
// holding in and out degrees
#define DEG(n) texture2D(degrees_and_adj_pos, n)

#define INDEG(d) d.x

#define OUTDEG(d) d.y

// the in degree of a node
int indeg(const node n, const Graph g) {
  return int(INDEG(DEG(NODE_TO_VEC2(n))));
}

// the out degree of a node
int outdeg(const node n, const Graph g) {
  return int(OUTDEG(DEG(NODE_TO_VEC2(n))));
}

// the inout degree of a node
int deg(const node n, const Graph g) {
  vec2 d = DEG(NODE_TO_VEC2(n)).xy;
  return int(INDEG(d) + OUTDEG(d));
}

// check if a node belongs to the graph
bool isValid(const node n, const Graph g) {
  return POS_TO_INDEX(NODE_TO_VEC2(n), g.nSize) < g.nbNodes;
}

// to get the id
int id(const node n, const Graph g) {
  return int(NODE_ID(n, g));
}

//-----------------------------------------------------------
// Public basic functions to get edge infos 
//-----------------------------------------------------------
// some macros to deal with the vec4 object
// holding the source and target nodes
#define EDGE(pos) texture2D(edges, pos)

#define SOURCE(pos) node(EDGE(pos).xy)

#define TARGET(pos) node(EDGE(pos).zw)

// the source node of an edge
node source(const edge e, const Graph g) {
  return SOURCE(EDGE_TO_VEC2(e));
}

// the target node of an edge
node target(const edge e, const Graph g) {
  return TARGET(EDGE_TO_VEC2(e));
}

// check if an edge belongs to the graph
bool isValid(const edge e, const Graph g) {
  return POS_TO_INDEX(EDGE_TO_VEC2(e), g.eSize) < g.nbEdges;
}

// to get the id
int id(const edge e, const Graph g) {
  return int(EDGE_ID(e, g));
}

//----------------------------------------------------------
// forEach
//----------------------------------------------------------
// forEach à la tulip;
// it works because the getNext functions
// used below have 2 versions, for edge or node as 1st arg.
// The implementation may look strange
// but the first I choosed,
//
// for(Iterator it = set; getNext(elt, it);)
//
// did not work at all
#define forEach(elt, set) \
  Iterator it = set;	  \
  int loop_count1;	  \
  int loop_count2;	  \
  while(hasNext(it) && initForEachCounter(loop_count1))  \
  while(hasNext(it) && (++loop_count1 < 256) && initForEachCounter(loop_count2))  \
    while(hasNext(it) && (++loop_count2 < 256) && getNext(elt, it))

// for internal use only
bool initForEachCounter(inout int counter) {
  counter = 0;
  return true;
}

//----------------------------------------------------------
// Iterator structure
//----------------------------------------------------------
// This structure allows iteration
// on graph nodes or edges or
// in the adjacency_lists to get in/out edges
// and in/out nodes
struct Iterator {
  // all fields are private
  // the graph
  Graph graph;
  // flag for graph elts iteration
  bool iterateOnGraphElts;
  // the current pos when iterating
  vec2 current;
  // the number of remaining steps
  float steps;
  // the incr = graph.incr or 1.0/graph.nSize or 1.0/graph.eSize
  float incr;
};

// an iterator which does not iterate
#define null_iterator(g) Iterator(g, false, vec2(0.0, 0.0), 0.0, 0.0)

//-----------------------------------------------------------
// Private functions for Iterator management functions impl.
//----------------------------------------------------------
// internal function to get next pos to iterate in a set
// incr is 1.0/ceil(sqrt(number of elts))
vec2 getNextPos(vec2 pos, const float incr) {
  pos.x += incr;
  if (pos.x > 1.0) {
    pos.x = incr/2.0;
    pos.y += incr;
  }
  return pos;
}

// functions to get positions in the adjacency lists
// for internal use only
#define getAdjacencyListFirstPos(n, g) texture2D(degrees_and_adj_pos, NODE_TO_VEC2(n)).zw

// to get a new position from an existing position in an adjacency list
vec2 getAdjacencyListPos(vec2 pos, const float offset, const float incr) {
  for(float i = 0.0; i < offset; ++i) {
    pos.x += incr;
    if (pos.x > 1.0) {
      pos.x = incr/2.0;
      pos.y += incr;
    }
  }
  return pos;
}

//-----------------------------------------------------------
// Public functions for Iterator management
//----------------------------------------------------------
// to check if iteration can go on
#define hasNext(it) (it.steps > 0.0)

// to get the next edge via an Iterator
bool getNext(out edge e, inout Iterator it) {
  if (hasNext(it)) {
    if (it.iterateOnGraphElts)
      e.pos = it.current;
    else
      e.pos = texture2D(adjacency_lists, it.current).xy;
    it.current = getNextPos(it.current, it.graph.incr);
    --it.steps;
    return true;
  }
  return false;
}

// to get the next node via an Iterator
bool getNext(out node n, inout Iterator it) {
  if (hasNext(it)) {
    if (it.iterateOnGraphElts)
      n.pos = it.current;
    else
      n.pos = texture2D(adjacency_lists, it.current).zw;
     it.current = getNextPos(it.current, it.incr);
     --it.steps;
   return true;
  }
  return false;
}

//-------------------------------------------------------
// Public functions to get an Iterator on nodes or edges
//-------------------------------------------------------
Iterator getInEdges(const node n, const Graph g) {
  float nbInEdges = INDEG(DEG(NODE_TO_VEC2(n)));
  return Iterator(g, false, getAdjacencyListFirstPos(n, g),
		  nbInEdges, g.incr);
}

// the in nodes are the source of the in edges
#define getInNodes(n, g) getInEdges(n, g)

Iterator getOutEdges(node n, Graph g) {
  vec4 d = DEG(NODE_TO_VEC2(n));
  return Iterator(g, false,
		  getAdjacencyListPos(getAdjacencyListFirstPos(n, g),
				      INDEG(d), g.incr),
		  OUTDEG(d), g.incr);
}

// the out nodes are the target of the out edges
#define getOutNodes(n, g) getOutEdges(n, g)

Iterator getInOutEdges(const node n, const Graph g) {
  vec4 d = DEG(NODE_TO_VEC2(n));
  return Iterator(g, false, getAdjacencyListFirstPos(n, g),
		  INDEG(d) + OUTDEG(d), g.incr);
}

// the in/out nodes are the source/target of the in/out edges
#define getInOutNodes(n, g) getInOutEdges(n, g)

Iterator getEdges(Graph g) {
  if (g.nbEdges > 0.0) {
    float incr = 1.0/g.eSize;
    return Iterator(g, true, vec2(incr/2.0, incr/2.0), g.nbEdges, incr);
  }
  return null_iterator(g);
}

Iterator getNodes(Graph g) {
  if (g.nbNodes > 0.0) {
    float incr = 1.0/g.nSize;
    return Iterator(g, true, vec2(incr/2.0, incr/2.0), g.nbNodes, incr);
  }
  return null_iterator(g);
}

//-------------------------------------------------------
// Property management
//-------------------------------------------------------
// a property is a struct with 2 fields
// the first field is a bool indicating if there values for nodes
// the second field is a texture (sampler2D)
// holding the values
#define DECLARE_PROPERTY_STRUCT(st_name)\
  struct st_name {			\
    bool hasNodeValues;			\
    sampler2D values;			\
  }

// BooleanProperty
//----------------
// from the cpu point of view
// the boolean values are stored as unsigned 8bit integers
DECLARE_PROPERTY_STRUCT(BooleanProperty);

// ColorProperty
//--------------
// from the cpu point of view
// the color values are stored as four 32bits float (rgba)
DECLARE_PROPERTY_STRUCT(ColorProperty);

// Color type
#define Color vec4

// DoubleProperty
//---------------
// from the cpu point of view
// the double values are stored as 32bit floats
DECLARE_PROPERTY_STRUCT(DoubleProperty);

// FloatProperty
//--------------
#define FloatProperty DoubleProperty

// IntegerProperty
//---------------
// from the cpu point of view
// the integer values are stored as 32 bits integers
DECLARE_PROPERTY_STRUCT(IntegerProperty);

// LayoutProperty
//---------------
// from the cpu point of view
// the layout values are stored as three floats
DECLARE_PROPERTY_STRUCT(LayoutProperty);

// Coord type
#define Coord vec3

// SizeProperty
//-------------
// from the cpu point of view
// the size values are stored as three floats
DECLARE_PROPERTY_STRUCT(SizeProperty);

// Size type
#define Size vec3

// Vec2Property
//-------------
DECLARE_PROPERTY_STRUCT(Vec2Property);

// Vec3Property
//-------------
#define Vec3Property SizeProperty

// Vec4Property
//-------------
#define Vec4Property ColorProperty

//-------------------------------------------------------
// Private functions and macros for implementation of
// functions to get a value from a property
//-------------------------------------------------------

// we need an internal function to
// translate a node into a position in the 'values' field 
vec2 translateToValuePos(const node n, const Graph g, int hasNodeValues) {
  return NODE_TO_VEC2(n);
}

// same thing for an edge
vec2 translateToValuePos(const edge e, const Graph g, int hasNodeValues) {
  if (hasNodeValues = 0)
    return EDGE_TO_VEC2(e);

  float index = POS_TO_INDEX(EDGE_TO_VEC2(e), g.eSize) + g.nbNodes;
  float size = g.nbNodes + g.nbEdges;
  return vec2(mod(index, size)/size, floor(index/size)/size);
}

#define GETVALUE(elt, g, prop)  texture2D(prop.values, translateToValuePos(elt, g, prop.hasNodeValues))

//-------------------------------------------------------
// Public functions for the management of properties
//-------------------------------------------------------
// get a bool value for a node
bool getNodeValue(const node n, const Graph g, const BooleanProperty prop) {
  if (GETVALUE(n, g, prop).x == 0.0)
    return false;
  return true;
}

// get a bool value for an edge
bool getEdgeValue(const edge e, const Graph g, const BooleanProperty prop) {
  if (GETVALUE(e, g, prop).x == 0.0)
    return false;
  return true;
}

// get a color (vec4) value for a node
Color getNodeValue(const node n, const Graph g, const ColorProperty prop) {
  return GETVALUE(n, g, prop);
}

// get a color (vec4) value for an edge
Color getEdgeValue(const edge e, const Graph g, const ColorProperty prop) {
  return GETVALUE(e, g, prop);
}

// get a float value for a node
float getNodeValue(const node n, const Graph g, const DoubleProperty prop) {
  return GETVALUE(n, g, prop).x;
}

// get a float value for an edge
float getEdgeValue(const edge e, const Graph g, const DoubleProperty prop) {
  return GETVALUE(e, g, prop).x;
}

// get an int value for a node
int getNodeValue(const node n, const Graph g, const IntegerProperty prop) {
  return int(GETVALUE(n, g, prop).x);
}

// get an int value for an edge
int getEdgeValue(const edge e, const Graph g, const IntegerProperty prop) {
  return int(GETVALUE(e, g, prop).x);
}

// get a layout position (vec3) value for a node
Coord getNodeValue(const node n, const Graph g, const LayoutProperty prop) {
  return texture2D(prop.values,
		   translateToValuePos(n, g, prop.hasNodeValues)).xyz;
}

// vec3 getEdgeValue(edge e, Graph g, LayoutProperty prop) does not exist

// get a size (vec3) value for a node
Size getNodeValue(const node n, const Graph g, const SizeProperty prop) {
  return texture2D(prop.values,
		   translateToValuePos(n, g, prop.hasNodeValues)).xyz;
}

// get a size (vec3) value for a edge
Size getNodeValue(const edge e, const Graph g, const SizeProperty prop) {
  return texture2D(prop.values,
		   translateToValuePos(e, g, prop.hasNodeValues)).xyz;
}

// get a vec2 value for a node
vec2 getNodeValue(const node n, const Graph g, const Vec2Property prop) {
  return texture2D(prop.values,
		   translateToValuePos(n, g, prop.hasNodeValues)).xy;
}

// get a vec2 value for a edge
vec2 getNodeValue(const edge e, const Graph g, const Vec2Property prop) {
  return texture2D(prop.values,
		   translateToValuePos(e, g, prop.hasNodeValues)).xy;
}

// the fragment shader current elts (used for main function invocation)
//---------------------------------------------------------------------
#define currentNode node(gl_TexCoord[0].xy)
#define currentEdge edge(gl_TexCoord[0].xy)

// some functions to handle the result
vec4 resultToVec4(const bool val) {
  return vec4(val, 0.0, 0.0, 0.0);
}

vec4 resultToVec4(const float val) {
  return vec4(val, 0.0, 0.0, 0.0);
}

vec4 resultToVec4(const int val) {
  return vec4(val, 0.0, 0.0, 0.0);
}

vec4 resultToVec4(const vec2 val) {
  return vec4(val, 0.0, 0.0);
}

vec4 resultToVec4(const vec3 val) {
  return vec4(val, 0.0);
}

vec4 resultToVec4(const vec4 val) {
  return val;
}

// The CPU-side API will automatically generate the main function
// of the shaders
//
// always the same for the vertex shader
//
// void main() {		
//	gl_TexCoord[0] = gl_MultiTexCoord0;
//
//	gl_Position = gl_ModelViewProjectionMatrix*gl_Vertex;
// }
//
// for the fragment shader it depends of the name of the graph variable
// the type of iteration (through nodes or edges)
// for nodes iterations with a graph named 'graph'
//
// void main() {
//	node n = currentNode;
//	if (isValid(node, graph)) {
//	  gl_FragColor= resultToVec4(doForElt(currentNode));
//      }
//	// invalid fragment
//	discard;
// }
// So to iterate on nodes, the developer has only to defined
//
// type doForElt(node n);
//
// and/or to iterate on edges
//
// type doForElt(edge e)
//
// where type is one of bool, float, int,
// vec2, vec3 (Coord or Size), vec4 (Color)
