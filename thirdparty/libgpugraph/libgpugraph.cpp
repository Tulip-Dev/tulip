#include "libgpugraph.h"
#include "fragmentShaderHeaderSource.h"
#include <cassert>
#include <stack>
#include <sstream>
#include <vector>
#include <math.h>
#include <map>
#include <string.h>

using namespace std;

// for error output
static ostream* errorStream;

#define printOpenGLError() printOglError(__FILE__, __LINE__)

static void printOglError(char *file, int line) {
  GLenum glErr;

  glErr = glGetError();
  while (glErr != GL_NO_ERROR) {
    (*errorStream) << "Error in " << file << "@"  << line << ":"
		   << gluErrorString(glErr) << endl;
    glErr = glGetError();
  }
}

#if !defined(NDEBUG)
#define GL_CHECK(glcall) (glcall, printOglError(__FILE__, __LINE__))
#else
#define GL_CHECK(glcall) glcall
#endif

bool checkFrameBufferStatus(char* file, int line) {
  GLenum status;
  status = (GLenum) glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
  if (status == GL_FRAMEBUFFER_COMPLETE_EXT)
    return true;
  (*errorStream) << "Error in " << file << "@" << line << ": ";
  switch(status) {
  case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
    (*errorStream) << "Framebuffer incomplete, incomplete attachment" << endl;
    return false;
  case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
    (*errorStream) << "Framebuffer incomplete, missing attachment" << endl;
    return false;
  case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
    (*errorStream) << "Framebuffer incomplete, attached images must have same dimensions" << endl;
    return false;
  case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
    (*errorStream) << "Framebuffer incomplete, attached images must have same format" << endl;
    return false;
  case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
    (*errorStream) << "Framebuffer incomplete, missing draw buffer" << endl;
    return false;
  case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
    (*errorStream) << "Framebuffer incomplete, missing read buffer" << endl;
    return false;
  case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
    (*errorStream) << "Unsupported framebuffer format" << endl;
    return false;
  }
  return false;
}

#define CHECK_FRAME_BUFFER_STATUS() checkFrameBufferStatus(__FILE__, __LINE__)

//----------------
// the Gpu classes
//----------------
class GpuGraph {
public:
  unsigned int nbNodes;
  unsigned int nbEdges;
  float nSize;  // ceil(sqrt(nbNodes))
  float eSize;  // ceil(sqrt(nbEdges))
  float aSize;  // ceil(sqrt(2 * nbEdges))
  // in degree + out degree +
  // adjcency_lists first position (x, y)
  vec4* degrees_and_adj_pos;
  vec4* adjacency_lists;
  vec4* edges;

  GpuGraph(unsigned int nNodes, unsigned int nEdges);
  ~GpuGraph();
};

class GpuProperty {
public:
  string varName;
  GLuint texId; // the corresponding texture id
  GLuint predefinedTexId; // the corresponding predefined one (GL_TEXTURE...)
  unsigned int nbNodes; // number of nodes
  unsigned int nbEdges; // number of edges
  unsigned int valSize; // sizeof(value)
  GLint internal_format; // internal format in glTexImage2D
  GLint format; // format in glTexImage2D
  GLint type; // type in glTexImage2D
  unsigned char *values; // all nodes and eges values
  bool dataOnGpu; // indicates which data have been transferred to gpu

  GpuProperty(GLuint textureID, unsigned int nbNodes, unsigned int nbEdges,
	      unsigned int valsz, GLint int_format, GLint format, GLint type);
  ~GpuProperty();
};

class GpuProgram {
public:
  // the graph variable name
  string graphName;
  // the graph
  GpuGraph *graph;
  // the program id
  GLuint id;
  // the user fragment shader id
  GLuint fragId;
  // the main function fragment shader id
  GLuint mainId;
  // the iteration mode
  GpuIterationMode mode;
  // the result property
  GpuProperty *outProperty;
  
  GpuProgram(const string& graphName, GLuint gid,
	     GLuint fid, GpuIterationMode iterMode);
  ~GpuProgram();
};

/*
// the IDs of the predefined shaders
static GLuint vertexShaderID;

// the source of the immutable vertex shader
#define VERTEX_SHADER_NB_LINES 4
static const char* vertexShaderSource[VERTEX_SHADER_NB_LINES] = {
"void main() {",	
"  gl_TexCoord[0] = gl_MultiTexCoord0;",
"  gl_Position = gl_ModelViewProjectionMatrix*gl_Vertex;",
"}"
};
*/
/*
static GLuint fragmentShaderID;

// this variable holds the source
// of the gpu graph API fragment shader
// must be defined in fragmentShaderHeaderSource.h
// extern const char* fragmentShaderHeaderSource[];
*/

// shader compilation
// for internal use only
static bool compileShader(GLuint shaderID, const string& sName) {
  // compile it
  GL_CHECK(glCompileShader(shaderID));
  // check if compilation is OK
  GLint status;
  GL_CHECK(glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status));
  if (status != GL_TRUE) {
    (*errorStream) << "Compilation of " <<
      sName.c_str() << " failed :" << endl;
    // retrieve and output info log
    GLint infoLength;
    GL_CHECK(glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLength));
    char* log = new char[infoLength];
    GL_CHECK(glGetShaderInfoLog(shaderID, infoLength, 0, log));
    (*errorStream) << log << endl;
    delete log;
    return false;
  }
  return true;
}

// API global variables
// the frame buffer object to get the result
static GLuint gpuFBO;

// the current program
static GpuProgram* currentGpuProgram = NULL;

// a stack to manage the texture ids
// needed by the GpuProperty
static stack<GLuint> freeTextureIDs;

// API initialization
bool GpuInit(ostream& eStream) {
  // check if it is not already inited
  if (!freeTextureIDs.empty()) {
    eStream << "GpuInit failed: already inited" << endl;
    return false;
  }

  errorStream = &eStream;

  // check support of glsl
  GLenum status = glewInit();
  if (status != GLEW_OK){
    (*errorStream) << "GpuInit failed: " << glewGetErrorString(status) << endl;
    return false;
  }
    
  if (!glewIsSupported("GL_VERSION_2_0")) {
    (*errorStream) << "GpuInit failed: OpenGL 2.0 not supported" << endl;
    return false;
  }

  GLint maxColor;
  glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS_EXT, &maxColor);
  if ( maxColor < 1 ) {
    (*errorStream) << "GpuInit failed: no GL_COLOR_ATTACHEMENT" << endl;
    return false;
  }

  // check number of available textures
  // our API needs 4 textures for graph data encoding
  // + 1 texture for a property encoding
  GLint max_tex_units;
  GL_CHECK(glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max_tex_units));
  if (max_tex_units < 5) {
    (*errorStream) << "GpuInit failed: only " << max_tex_units
		   << " textures available" << endl;
    return false;
  }

  // initialize the frame buffer
  GL_CHECK(glGenFramebuffersEXT(1, &gpuFBO)); 

  // initialize the texture ids stack
  for(int i = 0; i < max_tex_units - 4; ++i)
    freeTextureIDs.push(GL_TEXTURE4 + i);

  /*
  // create our immutable vertex shader
  vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
  // set source code
  GL_CHECK(glShaderSource(vertexShaderID, VERTEX_SHADER_NB_LINES,
			  vertexShaderSource, 0));
  // compile it
  if (!compileShader(vertexShaderID, "vertex shader"))
  return false;
  */
  /*
  // create our API header fragment shader
  fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

  // count the number of lines
  unsigned int nbLines = 0;
  for(; fragmentShaderHeaderSource[nbLines]; ++nbLines);
  // set source code
  GL_CHECK(glShaderSource(fragmentShaderID, nbLines,
  fragmentShaderHeaderSource, 0));
  // compile it
  compileShader(fragmentShaderID, "fragment shader API");
  */

  return  true; 
}

bool GpuEnd() {
  currentGpuProgram = NULL;
  return true;
}
  
// GpuProgram constructor
GpuProgram::GpuProgram(const string& name, GLuint gid, GLuint fid, GpuIterationMode iterMode)
  : graphName(name), graph(NULL), id(gid), fragId(fid), mainId(0),
    mode(iterMode), outProperty(NULL) {
}

// GpuProgram destructor
GpuProgram::~GpuProgram() {
  // delete the program
  GL_CHECK(glDeleteProgram(id));
  // delete the fragment shader
  GL_CHECK(glDeleteShader(fragId));
  // delete the main function fragment shader
  /* if (glIsShader(mainId))
     glDeleteShader(mainId); */
}

// destructor function
bool delGpuProgram(GpuProgram* prog) {
#if !defined(NDEBUG)
  assert(prog);
#endif
  if (currentGpuProgram == prog) {
    (*errorStream) << "delGpuProgram failed: program in use" << endl;
    return false;
  }
  delete prog;
  return true;
}

// API program creation
GpuProgram *genGpuProgram(const string& graphName, istream& inputStream,
			  GpuIterationMode iterMode) {
#if !defined(NDEBUG)
  assert(iterMode == NODES_ITERATION || iterMode == EDGES_ITERATION);
#endif

  // create the fragment shader
  GLuint fragID = glCreateShader(GL_FRAGMENT_SHADER);
  // get lines of code
  ostringstream ststream;
  string line;
  while (getline(inputStream, line)) {
    ststream << line << endl;
  }
  line = ststream.str();
  const char *lines[] = { line.c_str() };
  // set source code
  GL_CHECK(glShaderSource(fragID, 1, lines, 0));
  // compile it
  if (!compileShader(fragID, "user fragment shader")) {
    glDeleteShader(fragID);
    return NULL;
  }
  // create the program
  GLuint programID = glCreateProgram();
  // attach the fragment shader
  GL_CHECK(glAttachShader(programID, fragID));
  // finally link the program
  GL_CHECK(glLinkProgram(programID));
  // check if it is OK
  GLint status;
  GL_CHECK(glGetProgramiv(programID, GL_LINK_STATUS, &status));
  if (status == GL_FALSE) {
    (*errorStream) << "Program link failed:" << endl;
    // retrieve and output info log
    GLint infoLength;
    GL_CHECK(glGetProgramiv(programID  , GL_INFO_LOG_LENGTH, &infoLength));
    char* log = new char[infoLength];
    GL_CHECK(glGetProgramInfoLog(programID  , infoLength, 0, log));
    (*errorStream) << log << endl;
    // delete the log
    delete log;
    glDeleteShader(fragID);
    glDeleteProgram(programID);
    return false;
  }
  // the validity of graphName as a variable name should be checked first
  return new GpuProgram(graphName, programID, fragID, iterMode);
}

// the main function of the fragment shader
/* static char* fragmentShaderMainSource[] = {
   "struct node {",
   "float x, y;",
   "};",
   "struct edge {",
   "float x, y;",
   "};",
   "struct Graph {",
   "float nbNodes;",
   "float nbEdges;",
   "float nSize;",
   "float eSize;",
   "float neSize;",
   "float incr;",
   "float x_max;",
   "};",
   "#define currentNode node(gl_TexCoord[0].x, gl_TexCoord[0].y)",
   "#define currentEdge edge(gl_TexCoord[0].x, gl_TexCoord[0].y)",
   "bool isValid(node, Graph);",
   "bool isValid(edge, Graph);",
   "vec4 resultToVec4(bool val);",
   "vec4 resultToVec4(float val);",
   "vec4 resultToVec4(int val);",
   "vec4 resultToVec4(vec3 val);",
   "vec4 resultToVec4(vec4 val);",
   "bool doForElt(node);",
   "bool doForElt(edge);",
   "float doForElt(node);",
   "float doForElt(edge);",
   "int doForElt(node);",
   "int doForElt(edge);",
   "vec3 doForElt(node);",
   "vec3 doForElt(edge);",
   "vec4 doForElt(node);",
   "vec4 doForElt(edge);",
   "uniform Graph graph;",
   "void main() {",
   "", // this place is for elt declaration: node elt = currentNode;
   "", // this one is for check: if (isValid(elt, graph)) {
   "gl_FragColor = resultToVec4(doForElt(elt));",
   "  } else discard;", // invalid elts are discarded
   "}",
   0 // must be null terminated to find the number of lines
   };
   // number of available lines
   unsigned int fragmentShaderMainNbLines = 0;
*/

// the gpu textures holding the graph data
static GLuint tex_degrees_and_adj_pos;
static GLuint tex_adjacency_lists;
static GLuint tex_edges;

// function to fill the gpu side textures
static bool createTexture(GLuint* texID, GLuint predefinedTexID,
			  unsigned int sz, void* data,
			  GLint internal_format, GLint format, GLint type)  {
  // first check if there is enough memory
#if !defined(NDEBUG)
  GLint wd;
  glTexImage2D(GL_PROXY_TEXTURE_2D, 0, internal_format, sz, sz, 0,
	       format, type, 0);
  glGetTexLevelParameteriv(GL_PROXY_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &wd);
  if (wd == 0) {
    (*errorStream) << "not enough memory to create texture " << endl;
    return false;
  }
#endif

  // then create
  GL_CHECK(glGenTextures(1, texID));
  GL_CHECK(glActiveTexture(predefinedTexID));
  GL_CHECK(glBindTexture(GL_TEXTURE_2D, *texID));
  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP));
  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP));
  GL_CHECK(glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE));
  GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, internal_format, sz, sz, 0,
			format, type, data));

  return true;
}

// attach a texture to a uniform program variable
static bool bindTexture(const string& varName, GLuint predefinedTexID) {
  GLint loc = glGetUniformLocation(currentGpuProgram->id, varName.c_str());
  if (loc != -1) {
    // finally attach the texture to the current program variable
    GL_CHECK(glUniform1i(loc, predefinedTexID - GL_TEXTURE0));
    return true;
  }
  return false;
}
  
// GpuGraph constructor
GpuGraph::GpuGraph(unsigned int nNodes, unsigned int nEdges)
  : nbNodes(nNodes), nbEdges(nEdges) {
  nSize = ceil(sqrt((float)nNodes));
  eSize = ceil(sqrt((float)nEdges));
  aSize = ceil(sqrt((float)2 * nEdges));
  if (nEdges > 0) {
    degrees_and_adj_pos = new vec4[(int) (nSize * nSize)];
    adjacency_lists = new vec4[(int) (aSize * aSize)];
    edges = new vec4[(int) (eSize * eSize)];
  }
  else {
    degrees_and_adj_pos = NULL;
    adjacency_lists = NULL;
    edges = NULL;
  }
}

// GpuGraph destructor
GpuGraph::~GpuGraph() {
  if (degrees_and_adj_pos) {
    delete[] degrees_and_adj_pos;
    delete[] adjacency_lists;
    delete[] edges;
  }
}

// destructor function
bool delGpuGraph(GpuGraph *graph) {
#if !defined(NDEBUG)
  assert(graph);
  assert(!currentGpuProgram || currentGpuProgram->graph != graph);
#endif
  delete graph;
  return true;
}

// function to convert an 1D integer index belonging to [0, size*size]
// to a 2D float position belonging to [0, 1]x[0, 1]
void indexToPos(unsigned int index, unsigned int size, vec2 *val) {
  val->pos.x = 1.0/(2.0 * size) + ((float) (index%size))/size;
  val->pos.y = 1.0/(2.0 * size) + ((float) (index/size))/size;
}

// for a number of nodes
GpuGraph* genGpuGraph(unsigned int nbNodes) {
  if (nbNodes == 0) {
    (*errorStream) << "genGpuGraph failed: no node" << endl;
    return NULL;
  }
  // create a gpu graph object
  return new GpuGraph(nbNodes, 0);
}

// a class to hold the node's infos when filling
// the gpu graph associated textures
struct GpuNodeInfos {
  vec2 pos; // the node position
  // x = in degree, y = out degree,
  // (z,w) first position in adjacency_lists
  vec4 degrees_and_adj_pos;
  vec4* adj_list; // the node adjacency list
  unsigned int in_list_index; // the first free index for the in edges
  unsigned int out_list_index; // the first free index for th out edges

  GpuNodeInfos() {
    adj_list = NULL;
    in_list_index = 0;
  }

  ~GpuNodeInfos() {
    if (adj_list)
      delete [] adj_list;
  }

  // return the node position
  vec2* getPos() {
    return &pos;
  }

  // initialization of node's adjacency list infos
  void init(unsigned int indeg, unsigned int outdeg,
	    unsigned int& first_adj_index, unsigned int graphAdjSize) {
    degrees_and_adj_pos.pos.x = (float) indeg;
    degrees_and_adj_pos.pos.y = (float) outdeg;
    // in edges then out edges
    out_list_index = indeg;
    unsigned int degree = indeg + outdeg;
    if (degree) {
      adj_list = new vec4[degree];
      // get the corresponding adjacency lists firs pos
      indexToPos(first_adj_index, graphAdjSize,
		 (vec2 *) &degrees_and_adj_pos.pos.z);
      first_adj_index += degree;
    }
  }

  // fill infos for an in edge
  void addInEdge(vec2* edge_pos, vec2* target_pos) {
#if !defined(NDEBUG)
    // check if the edge can be correctly inserted
    assert(in_list_index < degrees_and_adj_pos.pos.x);
#endif
    // xy holds the edge position
    memcpy(&adj_list[in_list_index].pos.x, edge_pos, sizeof(vec2));
    // zw holds the target node position
    memcpy(&adj_list[in_list_index].pos.z, target_pos, sizeof(vec2));
    ++in_list_index;
  }

  // fill infos for an out edge
  void addOutEdge(vec2* edge_pos, vec2* source_pos) {
#if !defined(NDEBUG)
    // check if the edge can be correctly inserted
    assert(out_list_index < (degrees_and_adj_pos.pos.x + degrees_and_adj_pos.pos.y));
#endif
    // xy holds the edge position
    memcpy(&adj_list[out_list_index].pos.x, edge_pos, sizeof(vec2));
    // zw holds the source node position
    memcpy(&adj_list[out_list_index].pos.z, source_pos, sizeof(vec2));
    ++out_list_index;
  }
};  

// gpu graph creation from a set of nodes and edges
GpuGraph* genGpuGraph(CpuNode *nodes, unsigned int nbNodes,
		      CpuEdge *edges, unsigned int nbEdges) {
  if (nbNodes == 0 || nodes == NULL) {
    (*errorStream) << "genGpuGraph failed: no node" << endl;
    return NULL;
  } else if (nbEdges == 0 || edges == NULL) {
    (*errorStream) << "genGpuGraph failed: no edge" << endl;
    return NULL;
  }

  // create a gpu graph object
  GpuGraph *gpuGraph = new GpuGraph(nbNodes, nbEdges);

  unsigned int nSize = (unsigned int) gpuGraph->nSize;
  unsigned int aSize = (unsigned int) gpuGraph->aSize;
  std::map<int,  GpuNodeInfos *> mapNodes;
  GpuNodeInfos* nodesInfos = new GpuNodeInfos[nbNodes];
  unsigned int i, first_adj_index;
  // first for every node init the GpuNodeInfos
  for (i = first_adj_index = 0; i < nbNodes; ++i) {
    GpuNodeInfos* nodeInfos = &nodesInfos[i];
    nodeInfos->init(nodes[i].indeg, nodes[i].outdeg, first_adj_index, aSize);
    // get the corresponding node pos
    indexToPos(i, nSize, nodeInfos->getPos());
    mapNodes[nodes[i].id] = nodeInfos;
  }
#if !defined(NDEBUG)
  assert(first_adj_index <=
	 (unsigned int) (gpuGraph->aSize * gpuGraph->aSize));
#endif


  unsigned int eSize = (unsigned int) gpuGraph->eSize;
  vec2* edgesPos = new vec2[nbEdges];
  // same for every edge
  for (i = 0; i < nbEdges; ++i)
    indexToPos(i, eSize, &edgesPos[i]);

  // fill edges and record nodes adjacency_list
  vec4 *gpu_edges = gpuGraph->edges;
  for (i = 0; i < nbEdges; ++i) {
    GpuNodeInfos* sourceInfos = mapNodes[edges[i].source];
    GpuNodeInfos* targetInfos = mapNodes[edges[i].target];
    vec2* sourcePos = sourceInfos->getPos();
    // xy holds the source node position
    memcpy(&gpu_edges[i].pos.x, sourcePos, sizeof(vec2));
    // zw holds the target node position
    vec2* targetPos = targetInfos->getPos();
    memcpy(&gpu_edges[i].pos.z, targetPos, sizeof(vec2));
    // record infos for source node
    sourceInfos->addOutEdge(&edgesPos[i], targetPos);
    // record infos for target node
    targetInfos->addInEdge(&edgesPos[i], sourcePos);
  }

  vec4 *degrees_and_adj_pos = gpuGraph->degrees_and_adj_pos;
  vec4 *adjacency_lists = gpuGraph->adjacency_lists;
  // fill degrees & adjacency_lists
  for (i = 0; i < nbNodes; ++i) {
    GpuNodeInfos* nodeInfos = &nodesInfos[i];
    // fill degrees_and_adj_pos
    degrees_and_adj_pos[i] = nodeInfos->degrees_and_adj_pos;
    // fill the adjacency_lists
    unsigned int degree =
      (unsigned int) (nodeInfos->degrees_and_adj_pos.pos.x + nodeInfos->degrees_and_adj_pos.pos.y);
    if (degree) {
      memcpy(adjacency_lists, nodeInfos->adj_list, degree * sizeof(vec4));
      adjacency_lists += degree;
    }
  }

  if (nbEdges) {
    // create the gpu textures with graph data
    createTexture(&tex_degrees_and_adj_pos, GL_TEXTURE0,
		  (unsigned int) gpuGraph->nSize, gpuGraph->degrees_and_adj_pos,
		  GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT);
    createTexture(&tex_adjacency_lists, GL_TEXTURE2,
		  (unsigned int) gpuGraph->aSize, gpuGraph->adjacency_lists,
		  GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT);
    createTexture(&tex_edges, GL_TEXTURE3,
		  (unsigned int) gpuGraph->eSize,
		  gpuGraph->edges,
		  GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT);
  }

#if !defined(NDEBUG)
  assert((adjacency_lists - gpuGraph->adjacency_lists) <=
	 (gpuGraph->aSize * gpuGraph->aSize));
#endif

  delete[] nodesInfos;
  delete[] edgesPos;
  return gpuGraph;
}

// function to iterate on gpu graph elts
// get nodes iterator
GpuEltIterator getGpuNodes(GpuGraph* graph) {
#if !defined(NDEBUG)
  assert(graph);
#endif
  GpuEltIterator it;
  it.current = 0;
  it.steps = graph->nbNodes;
  it.size = (unsigned int) graph->nSize;
  return it;
}

// get edges iterator
GpuEltIterator getGpuEdges(GpuGraph* graph) {
#if !defined(NDEBUG)
  assert(graph);
#endif
  GpuEltIterator it;
  it.current = 0;
  it.steps = graph->nbEdges;
  it.size = (unsigned int) graph->eSize;
  return it;
}

// check for next elt
bool hasNext(GpuEltIterator& it) {
  return it.steps > it.current;
}

// get next elt
void getNext(GpuElt& elt, GpuEltIterator& it) {
  indexToPos(it.current, it.size, &elt.pos);
  ++it.current;
}
  
//------------------------
// Gpu property management
//------------------------
// GpuProperty constructor
GpuProperty::GpuProperty(GLuint predefTexID, unsigned int nNodes, unsigned int nEdges,
			 unsigned int sz, GLint int_format, GLint format, GLint type)
  : varName(""), texId(0), predefinedTexId(predefTexID),
    nbNodes(nNodes), nbEdges(nEdges), valSize(sz),
    internal_format(int_format), format(format), type(type) {
  // size is ceil(sqrt(nNodes)) + ceil(sqrt(nEdges))
  // because we allow to transfer values for nodes, edges or both
  unsigned int size = (unsigned int) (ceil(sqrt((float)nNodes)) + ceil(sqrt((float)nEdges)));
  values = new unsigned char[size * size * sz];
  dataOnGpu = false;
}

// GpuProperty destructor
GpuProperty::~GpuProperty() {
  delete[] values;
  freeTextureIDs.push(predefinedTexId);
  GL_CHECK(glDeleteTextures(1, &predefinedTexId));
}

// destructor function
bool delGpuProperty(GpuProperty *prop) {
#if !defined(NDEBUG)
  assert(prop);
#endif
  delete prop;
  return true;
}

// a low level function to
// allocate a GpuProperty with a free texture id
GpuProperty *genGpuProperty(unsigned int nNodes, unsigned int nEdges,
			    unsigned int valSize, GLint int_format,
			    GLint format, GLint type) {
  // first try to get free texture id
  if (freeTextureIDs.empty()) {
    (*errorStream) << "genGpuProperty failed: no texture image unit available" << endl;
    return NULL;
  }
  GLuint textureID = freeTextureIDs.top();
  freeTextureIDs.pop();
  GpuProperty *gpuProp =
    new GpuProperty(textureID, nNodes, nEdges, valSize, int_format, format, type);
  unsigned int size =
    (unsigned int) ceil(sqrt((float)gpuProp->nbNodes + gpuProp->nbEdges));
  // create its associated texture
  if (!createTexture(&gpuProp->texId, textureID, size, 0, int_format, format, type))
    return NULL;

  return gpuProp;
}

// a low level function to access to the property associated values
void *getGpuPropertyValues(GpuProperty *gpuProp) {
#if !defined(NDEBUG)
  assert(gpuProp);
#endif
  return gpuProp->values;
}

// clone an existing GpuProperty
GpuProperty* genGpuProperty(GpuProperty* gProp) {
  GpuProperty* gpuProp =
    genGpuProperty(gProp->nbNodes, gProp->nbEdges,
		   gProp->valSize, gProp->internal_format, gProp->format,
		   gProp->type);
  if (!gpuProp)
    return NULL;

  // copy values
  unsigned int size =
    (unsigned int) (ceil(sqrt((float)gProp->nbNodes)) +
		    ceil(sqrt((float)gProp->nbEdges))) * gProp->valSize;
  memcpy(gpuProp->values, gProp->values, size);
  
  return gpuProp;
}

// GpuProperty creation from an array of bool values
GpuProperty* genGpuProperty(bool *values, unsigned int nbNodeValues,
			    unsigned int nbEdgeValues) {
  // create the object
  GpuProperty *gpuProp = genGpuProperty(nbNodeValues, nbEdgeValues,
					sizeof(float), GL_RGBA32F_ARB,
					GL_RED,	GL_FLOAT);
  if (!gpuProp)
    return NULL;
  if (values) {
    float* gpuValues = (float*) gpuProp->values;
    unsigned int i;
    for (i = 0; i < nbNodeValues; ++i) {
      *gpuValues = *values ? 1. : 0.;
      ++gpuValues;
      ++values;
    }
    for (i = 0; i < nbEdgeValues; ++i) {
      *gpuValues = *values ? 1. : 0.;
      ++gpuValues;
      ++values;
    }
  }
  return gpuProp;
}

// GpuProperty creation from an array of float values
GpuProperty* genGpuProperty(float *values, unsigned int nbNodeValues,
			    unsigned int nbEdgeValues) {
  // create the object
  GpuProperty *gpuProp = genGpuProperty(nbNodeValues, nbEdgeValues,
					sizeof(float), GL_RGBA32F_ARB,
					GL_RED,	GL_FLOAT);
  if (!gpuProp)
    return NULL;
  if (values) {
    memcpy(gpuProp->values, values,
	   (nbNodeValues + nbEdgeValues) * sizeof(float));
  }
  return gpuProp;
}

// GpuProperty creation from an array of int values
GpuProperty* genGpuProperty(int *values, unsigned int nbNodeValues,
			    unsigned int nbEdgeValues) {
  // create the object
  GpuProperty *gpuProp = genGpuProperty(nbNodeValues, nbEdgeValues,
					sizeof(float), GL_RGBA32F_ARB,
					GL_RED,	GL_FLOAT);
  if (!gpuProp)
    return NULL;
  if (values) {
    float* gpuValues = (float*) gpuProp->values;
    unsigned int i;
    for (i = 0; i < nbNodeValues; ++i) {
      *gpuValues = (float) *values;
      ++gpuValues;
      ++values;
    }
    for (i = 0; i < nbEdgeValues; ++i) {
      *gpuValues = (float) *values;
      ++gpuValues;
      ++values;
    }
  }
  return gpuProp;
}

// GpuProperty creation from an array of bvec2 values
GpuProperty* genGpuProperty(bvec2 *values, unsigned int nbNodeValues,
			    unsigned int nbEdgeValues) {
  // create the object
  GpuProperty *gpuProp = genGpuProperty(nbNodeValues, nbEdgeValues,
					3 * sizeof(float), GL_RGBA32F_ARB,
					GL_RGB,	GL_FLOAT);
  if (!gpuProp)
    return NULL;
  if (values) {
    float* bvalues = (float*) gpuProp->values;
    unsigned int i;
    for (i = 0; i < nbNodeValues; ++i) {
      bvalues[0] = values->array[0] ? 1. : 0.;
      bvalues[1] = values->array[1] ? 1. : 0.;
      bvalues += 3;
      ++values;
    }
    for (i = 0; i < nbEdgeValues; ++i) {
      bvalues[0] = values->array[0] ? 1. : 0.;
      bvalues[1] = values->array[1] ? 1. : 0.;
      bvalues += 3;
      ++values;
    }
  }
  return gpuProp;
}

// GpuProperty creation from an array of bvec3 values
GpuProperty* genGpuProperty(bvec3 *values, unsigned int nbNodeValues,
			    unsigned int nbEdgeValues) {
  // create the object
  GpuProperty *gpuProp = genGpuProperty(nbNodeValues, nbEdgeValues,
					3 * sizeof(float), GL_RGBA32F_ARB,
					GL_RGB,	GL_FLOAT);
  if (!gpuProp)
    return NULL;
  if (values) {
    memcpy(gpuProp->values, values,
	   (nbNodeValues + nbEdgeValues) * sizeof(bvec3));
  }
  return gpuProp;
}

// GpuProperty creation from an array of bvec4 values
GpuProperty* genGpuProperty(bvec4 *values, unsigned int nbNodeValues,
			    unsigned int nbEdgeValues) {
  // create the object
  GpuProperty *gpuProp = genGpuProperty(nbNodeValues, nbEdgeValues,
					4 * sizeof(float), GL_RGBA32F_ARB,
					GL_RGBA, GL_FLOAT);
  if (!gpuProp)
    return NULL;
  if (values) {
    memcpy(gpuProp->values, values,
	   (nbNodeValues + nbEdgeValues) * sizeof(bvec4));
  }
  return gpuProp;
}

// GpuProperty creation from an array of vec2 values
GpuProperty* genGpuProperty(vec2 *values, unsigned int nbNodeValues,
			    unsigned int nbEdgeValues) {
  // create the object
  GpuProperty *gpuProp = genGpuProperty(nbNodeValues, nbEdgeValues,
					3 * sizeof(float), GL_RGBA32F_ARB,
					GL_RGB,	GL_FLOAT);
  if (!gpuProp)
    return NULL;
  if (values) {
    vec3* vvalues = (vec3 *) gpuProp->values;
    unsigned int i;
    for (i = 0; i < nbNodeValues; ++i) {
      memcpy(vvalues, values, sizeof(vec2));
      ++vvalues;
      ++values;
    }
    for (i = 0; i < nbEdgeValues; ++i) {
      memcpy(vvalues, values, sizeof(vec2));
      ++vvalues;
      ++values;
    }
  }
  return gpuProp;
}

// GpuProperty creation from an array of vec3 values
GpuProperty* genGpuProperty(vec3 *values, unsigned int nbNodeValues,
			    unsigned int nbEdgeValues) {
  // create the object
  GpuProperty *gpuProp = genGpuProperty(nbNodeValues, nbEdgeValues,
					3 * sizeof(float), GL_RGBA32F_ARB,
					GL_RGB,	GL_FLOAT);
  if (!gpuProp)
    return NULL;
  if (values) {
    memcpy(gpuProp->values, values,
	   (nbNodeValues + nbEdgeValues) * sizeof(vec3));
  }
  return gpuProp;
}

// GpuProperty creation from an array of vec4 values
GpuProperty* genGpuProperty(vec4 *values, unsigned int nbNodeValues,
			    unsigned int nbEdgeValues) {
  // create the object
  GpuProperty *gpuProp = genGpuProperty(nbNodeValues, nbEdgeValues,
					4 * sizeof(float), GL_RGBA32F_ARB,
					GL_RGBA, GL_FLOAT);
  if (!gpuProp)
    return NULL;
  if (values) {
    memcpy(gpuProp->values, values,
	   (nbNodeValues + nbEdgeValues) * sizeof(vec4));
  }
  return gpuProp;
}

// GpuProperty creation from an array of ivec2 values
GpuProperty* genGpuProperty(ivec2 *values, unsigned int nbNodeValues,
			    unsigned int nbEdgeValues) {
  // create the object
  GpuProperty *gpuProp = genGpuProperty(nbNodeValues, nbEdgeValues,
					3 * sizeof(float), GL_RGBA32F_ARB,
					GL_RGB,	GL_FLOAT);
  if (!gpuProp)
    return NULL;
  if (values) {
    vec3* vvalues = (vec3 *) gpuProp->values;
    unsigned int i;
    for (i = 0; i < nbNodeValues; ++i) {
      vvalues->array[0] = (float) values->array[0];
      vvalues->array[1] = (float) values->array[1];
      ++vvalues;
      ++values;
    }
    for (i = 0; i < nbEdgeValues; ++i) {
      vvalues->array[0] = (float) values->array[0];
      vvalues->array[1] = (float) values->array[1];
      ++vvalues;
      ++values;
    }
  }
  return gpuProp;
}

// GpuProperty creation from an array of ivec3 values
GpuProperty* genGpuProperty(ivec3 *values, unsigned int nbNodeValues,
			    unsigned int nbEdgeValues) {
  // create the object
  GpuProperty *gpuProp = genGpuProperty(nbNodeValues,
					nbEdgeValues,
					3 * sizeof(float), GL_RGBA32F_ARB,
					GL_RGB,	GL_FLOAT);
  if (!gpuProp)
    return NULL;
  if (values) {
    vec3* vvalues = (vec3 *) gpuProp->values;
    unsigned int i;
    for (i = 0; i < nbNodeValues; ++i) {
      vvalues->array[0] = (float) values->array[0];
      vvalues->array[1] = (float) values->array[1];
      vvalues->array[2] = (float) values->array[2];
      ++vvalues;
      ++values;
    }
    for (i = 0; i < nbEdgeValues; ++i) {
      vvalues->array[0] = (float) values->array[0];
      vvalues->array[1] = (float) values->array[1];
      vvalues->array[2] = (float) values->array[2];
      ++vvalues;
      ++values;
    }
  }
  return gpuProp;
}

// GpuProperty creation from an array of ivec4 values
GpuProperty* genGpuProperty(ivec4 *values, unsigned int nbNodeValues,
			    unsigned int nbEdgeValues) {
  // create the object
  GpuProperty *gpuProp = genGpuProperty(nbNodeValues, nbEdgeValues,
					4 *sizeof(float), GL_RGBA32F_ARB,
					GL_RGBA, GL_FLOAT);
  if (!gpuProp)
    return NULL;
  if (values) {
    vec4* vvalues = (vec4 *) gpuProp->values;
    unsigned int i;
    for (i = 0; i < nbNodeValues; ++i) {
      vvalues->array[0] = (float) values->array[0];
      vvalues->array[1] = (float) values->array[1];
      vvalues->array[2] = (float) values->array[2];
      vvalues->array[3] = (float) values->array[3];
      ++vvalues;
      ++values;
    }
    for (i = 0; i < nbEdgeValues; ++i) {
      vvalues->array[0] = (float) values->array[0];
      vvalues->array[1] = (float) values->array[1];
      vvalues->array[2] = (float) values->array[2];
      vvalues->array[3] = (float) values->array[3];
      ++vvalues;
      ++values;
    }
  }
  return gpuProp;
}

//------------------------------------
// functions to set program parameters
//------------------------------------
// this one retrieve the current parameter location
// in the current program
static GLint getParamLocation(const string& paramName) {
  if (!currentGpuProgram) {
    (*errorStream) << "setGpuParameter failed: no current program" << endl;
    return -1;
  }
  GLint loc = glGetUniformLocation(currentGpuProgram->id, paramName.c_str());
  if (loc == -1)
    (*errorStream) << "uniform variable " << paramName.c_str()
		   << " does not exist" << endl;
  return loc;
}

// transfer the property data to the corresponding gpu variable
bool setGpuParameter(const string& varName, GpuProperty *gpuProp) {
#if !defined(NDEBUG)
  if (!currentGpuProgram) {
    (*errorStream) << "setGpuParameter failed: no current program" << endl;
    return false;
  }
  if (!gpuProp) {
    (*errorStream) << "setGpuParameter failed: property parameter is null" << endl;
    return false;
  }
  if (varName.empty()) {
    (*errorStream) << "setGpuParameter failed: parameter name is empty" << endl;
    return false;
  }
#endif
  // attach the texture to the current program variable
  if (!bindTexture((varName + ".values").c_str(), gpuProp->predefinedTexId)) {
    (*errorStream) << "uniform property variable " << varName.c_str()
		   << " does not exist" << endl;
    return false;
  } 

  // assign hasNodeValues
  GL_CHECK(glUniform1i(glGetUniformLocation(currentGpuProgram->id,
					    (varName + ".hasNodeValues").c_str()),
		       gpuProp->nbNodes ? 1 : 0));
  // if needed transfer the prop values on the gpu
  if (!gpuProp->dataOnGpu) {
    // compute size
    unsigned int size =
      (unsigned int) ceil(sqrt((float)gpuProp->nbNodes + gpuProp->nbEdges));
    gpuProp->dataOnGpu = true;
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, gpuProp->texId));
    GL_CHECK(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, size, size,
			     gpuProp->format, gpuProp->type, gpuProp->values));
  }
  gpuProp->varName = varName;

  return true;
}

// reset of a GpuProperty
// the cpu stored values are retransferred on Gpu
bool resetGpuProperty(GpuProperty *gpuProp) {
#if !defined(NDEBUG)
  if (!gpuProp) {
    (*errorStream) << "setGpuParameter failed: property parameter is null" << endl;
    return false;
  }
#endif
  // compute size
  unsigned int size =
    (unsigned int) ceil(sqrt((float)gpuProp->nbNodes + gpuProp->nbEdges));
  gpuProp->dataOnGpu = true;
  GL_CHECK(glBindTexture(GL_TEXTURE_2D, gpuProp->texId));
  GL_CHECK(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, size, size,
			   gpuProp->format, gpuProp->type, gpuProp->values));
  return true;
}

// a useful macro for basic types
#define SET_GPU_PARAM(param, val, function)			\
  GLint loc = getParamLocation(vName);				\
  if (loc == -1)						\
    return false;						\
  GL_CHECK(function(loc, val));					\
  return true

// function for bool parameter
bool setGpuParameter(const string& vName, bool value) {
  SET_GPU_PARAM(vName, value, glUniform1i); 
}

// the corresponding function
bool setGpuParameter(const string& vName, float value) {
  SET_GPU_PARAM(vName, value, glUniform1f);
}

// the function for int parameter
bool setGpuParameter(const string& vName, int value) {
  SET_GPU_PARAM(vName, value, glUniform1i);
}

#define SET_GPU_PARAM_ARRAY(param, val, nbVal, valType, bindFunc)		\
  GLint loc = getParamLocation(vName);				\
  if (loc == -1)						\
    return false;						\
  GL_CHECK(bindFunc(loc, nbVal, (const valType *) val));	\
  return true

// function for an array of bool
bool setGpuParameter(const string& vName, const bool* values, unsigned int nbValues) {
  SET_GPU_PARAM_ARRAY(vName, values, nbValues, GLint, glUniform1iv);
}

// function for an array of float
bool setGpuParameter(const string& vName, const float* values, unsigned int nbValues) {
  SET_GPU_PARAM_ARRAY(vName, values, nbValues, float, glUniform1fv);
}

// function for an array on int
bool setGpuParameter(const string& vName, const int* values, unsigned int nbValues) {
  SET_GPU_PARAM_ARRAY(vName, values, nbValues, GLint, glUniform1iv);
}

// function for a bvec2
bool setGpuParameter(const string& vName, const bvec2& val) {
  SET_GPU_PARAM_ARRAY(vName, val.array, 1, GLint, glUniform2iv);
}

// function for a vec2
bool setGpuParameter(const string& vName, const vec2& val) {
  SET_GPU_PARAM_ARRAY(vName, val.array, 1, float, glUniform2fv);
}

// function for an ivec2
bool setGpuParameter(const string& vName, const ivec2& val) {
  SET_GPU_PARAM_ARRAY(vName, val.array, 1, GLint, glUniform2iv);
}

// function for a bvec3
bool setGpuParameter(const string& vName, const bvec3& val) {
  SET_GPU_PARAM_ARRAY(vName, val.array, 1, GLint, glUniform3iv);
}

// function for a vec3
bool setGpuParameter(const string& vName, const vec3& val) {
  SET_GPU_PARAM_ARRAY(vName, val.array, 1, float, glUniform3fv);
}

// function for an ivec3
bool setGpuParameter(const string& vName, const ivec3& val) {
  SET_GPU_PARAM_ARRAY(vName, val.array, 1, GLint, glUniform3iv);
}

// function for a bvec4
bool setGpuParameter(const string& vName, const bvec4& val) {
  SET_GPU_PARAM_ARRAY(vName, val.array, 1, GLint, glUniform4iv);
}

// function for a bvec4
bool setGpuParameter(const string& vName, const vec4& val) {
  SET_GPU_PARAM_ARRAY(vName, val.array, 1, float, glUniform4fv);
}

// function for an ivec4
bool setGpuParameter(const string& vName, const ivec4& val) {
  SET_GPU_PARAM_ARRAY(vName, val.array, 1, GLint, glUniform4iv);
}

// function for a GpuElt
bool setGpuParameter(const string& vName, const GpuElt& elt) {
  return setGpuParameter(vName + ".pos", elt.pos);
}

#define SET_GPU_PARAM_MATRIX(param, val, bindFunc)		\
  GLint loc = getParamLocation(vName);				\
  if (loc == -1)						\
    return false;						\
  GL_CHECK(bindFunc(loc, 1, GL_TRUE, val));			\
  return true

// function for a mat2
bool setGpuParameter(const std::string& vName, const mat2& matrix) {
  SET_GPU_PARAM_MATRIX(vName, &matrix[0][0], glUniformMatrix2fv);
}

// function for a mat3
bool setGpuParameter(const std::string& vName, const mat3& matrix) {
  SET_GPU_PARAM_MATRIX(vName, &matrix[0][0], glUniformMatrix3fv);
}

// function for a mat4
bool setGpuParameter(const std::string& vName, const mat4& matrix) {
  SET_GPU_PARAM_MATRIX(vName, &matrix[0][0], glUniformMatrix4fv);
}

// beginning of the computation
bool attachGpuProgram(GpuProgram *prog, GpuGraph* graph) {
#if !defined(NDEBUG)
  if (currentGpuProgram) {
    (*errorStream) << "attachGpuProgram failed: end current program first" << endl;
    return false;
  }
  if (!graph) {
    (*errorStream) << "attachGpuProgram failed: graph parameter is null" << endl;
    return false;
  }    
#endif

  // save current opengl context
  GL_CHECK(glMatrixMode(GL_PROJECTION));
  GL_CHECK(glPushMatrix());
  GL_CHECK(glMatrixMode(GL_MODELVIEW));
  GL_CHECK(glPushMatrix());
  GL_CHECK(glPushAttrib(GL_ALL_ATTRIB_BITS));

  GL_CHECK(glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, gpuFBO));

  GL_CHECK(glMatrixMode(GL_PROJECTION));
  GL_CHECK(glLoadIdentity());

  // install the program as current
  GL_CHECK(glUseProgram(prog->id));
  currentGpuProgram = prog;

  // bind graph if needed
  if (prog->graph != graph) {
    // nbNodes
    GLint loc = glGetUniformLocation(prog->id,
				     (prog->graphName + ".nbNodes").c_str());
#if !defined(NDEBUG)
    if (loc == -1) {
      (*errorStream) << "uniform graph variable " << prog->graphName.c_str()
		     << " does not exist" << endl;
      detachGpuProgram();
      return false;
    }
#endif
    GL_CHECK(glUniform1f(loc, (float) graph->nbNodes));
    // nSize
    loc = glGetUniformLocation(prog->id,
			       (prog->graphName + ".nSize").c_str());
    GL_CHECK(glUniform1f(loc, graph->nSize));
    // nbEdges
    loc = glGetUniformLocation(prog->id,
			       (prog->graphName + ".nbEdges").c_str());
    GL_CHECK(glUniform1f(loc, (float) graph->nbEdges));
    // eSize
    loc = glGetUniformLocation(prog->id,
			       (prog->graphName + ".eSize").c_str());
    GL_CHECK(glUniform1f(loc, graph->eSize));
    // aSize
    loc = glGetUniformLocation(prog->id,
			       (prog->graphName + ".aSize").c_str());
    GL_CHECK(glUniform1f(loc, graph->aSize));

    // the other fields only have sense if the graph have edges
    if (graph->nbEdges) {
      // incr
      loc = glGetUniformLocation(prog->id,
				 (prog->graphName + ".incr").c_str());
      GL_CHECK(glUniform1f(loc, (float) 1.0/graph->aSize));
      // x_max
      loc = glGetUniformLocation(prog->id,
				 (prog->graphName + ".xmax").c_str());
      GL_CHECK(glUniform1f(loc, ((float) (graph->aSize - 1))/ graph->aSize));

      // attach textures
      bindTexture("degrees_and_adj_pos", GL_TEXTURE0);
      bindTexture("adjacency_lists", GL_TEXTURE2);
      bindTexture("edges", GL_TEXTURE3);
    }
    prog->graph = graph;
  }

  return true;
}

// attach a property as result of the current program
bool setGpuOutProperty(GpuProperty *prop) {
#if !defined(NDEBUG)
  if (!currentGpuProgram) {
    (*errorStream) << "setGpuProgramOutProperty failed: no current program" << endl;
    return false;
  }
  if (!prop) {
    (*errorStream) << "setGpuProgramOutProperty failed: argument is null" << endl;
    return false;
  }
#endif
  currentGpuProgram->outProperty = prop;
  
  return true;
}

// launch of the computation
// on nodes or edges
bool computeGpuProgram() {
  if (!currentGpuProgram) {
    (*errorStream)
      << "computeGpuProgram failed: attachGpuProgram has to be called first" << endl;
    return false;
  }
  if (!currentGpuProgram->outProperty) {
    (*errorStream) << "computeGpuProgram failed: current program has no out property" << endl;
    return false;
  }

  float size;
  if (currentGpuProgram->mode == NODES_ITERATION)
    size = currentGpuProgram->graph->nSize;
  else
    size = currentGpuProgram->graph->eSize;

  GL_CHECK(gluOrtho2D(0.0, size, 0.0, size));
    
  GL_CHECK(glMatrixMode(GL_MODELVIEW));
  GL_CHECK(glLoadIdentity());
  GL_CHECK(glViewport(0, 0, (unsigned int) size, (unsigned int) size));

  GL_CHECK(glEnable(GL_TEXTURE_2D));
  GL_CHECK(glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,
				     GL_COLOR_ATTACHMENT0_EXT,
				     GL_TEXTURE_2D,
				     currentGpuProgram->outProperty->texId, 0));
#if !defined(NDEBUG)
  if (!CHECK_FRAME_BUFFER_STATUS()) {
    return false;
  }
#endif

  GL_CHECK(glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT));
  GL_CHECK(glPolygonMode(GL_FRONT,GL_FILL));


  //  normalized texcoords
  glBegin(GL_QUADS);
  glTexCoord2f(0.0, 0.0);
  glVertex2f(0.0, 0.0);
  glTexCoord2f(1.0, 0.0);
  glVertex2f(size, 0.0);
  glTexCoord2f(1.0, 1.0);
  glVertex2f(size, size);
  glTexCoord2f(0.0, 1.0);
  glVertex2f(0.0, size);
  GL_CHECK(glEnd());

  return true;
}

// swap the result and property
bool swapGpuOutProperty(GpuProperty *gpuProp) {

#if !defined(NDEBUG)
  if (!gpuProp) {
    (*errorStream) << "swapGpuOutProperty failed: argument is null" << endl;
    return false;
  }
  if (!currentGpuProgram) {
    (*errorStream) << "swapGpuOutProperty failed: attachGpuProgram has to be called first" << endl;
    return false;
  }

  // check if gpuProp is 'compatible' with outProperty
  unsigned int pSize, rSize;
  if (currentGpuProgram->mode == NODES_ITERATION) {
    pSize = gpuProp->nbNodes;
    rSize = currentGpuProgram->outProperty->nbNodes;
  } else {
    pSize = gpuProp->nbEdges;
    rSize = currentGpuProgram->outProperty->nbEdges;
  } 

  if (rSize != pSize ||
      currentGpuProgram->outProperty->valSize != gpuProp->valSize ||
      currentGpuProgram->outProperty->internal_format != gpuProp->internal_format ||
      currentGpuProgram->outProperty->format != gpuProp->format ||
      currentGpuProgram->outProperty->type != gpuProp->type) {
    (*errorStream) << "swapGpuOutProperty failed: properties are not compatible" << endl;
    return false;
  }
#endif

  // swap properties
  GL_CHECK(glUniform1i(glGetUniformLocation(currentGpuProgram->id,
					    currentGpuProgram->outProperty->varName.c_str()),
		       gpuProp->texId));
  GL_CHECK(glUniform1i(glGetUniformLocation(currentGpuProgram->id,
					    gpuProp->varName.c_str()),
		       currentGpuProgram->outProperty->texId));
  currentGpuProgram->outProperty = gpuProp;
  return true;
}  

// a low level function to get the current program outProperty values
void* getGpuOutPropertyValues(unsigned int& nbValues,
			      unsigned int& valSize,
			      GpuValueType& valType) {
#if !defined(NDEBUG)
  if (!currentGpuProgram) {
    (*errorStream) << "getGpuOutPropertyValues failed: no current program" << endl;
    return NULL;
  }

  if (!currentGpuProgram->outProperty) {
    (*errorStream) << "getGpuOutPropertyValues failed: no current out property" << endl;
    return NULL;
  }
#endif

  float size;
  unsigned char *values = currentGpuProgram->outProperty->values;
  if (currentGpuProgram->mode == NODES_ITERATION) {
    size = currentGpuProgram->graph->nSize;
    valType = NODE_VALUES;
    nbValues = currentGpuProgram->outProperty->nbNodes;
  } else {
    size = currentGpuProgram->graph->eSize;
    values += currentGpuProgram->outProperty->valSize * currentGpuProgram->outProperty->nbNodes;
    valType = EDGE_VALUES;
    nbValues = currentGpuProgram->outProperty->nbEdges;
  }
  valSize = currentGpuProgram->outProperty->valSize;

  GL_CHECK(glReadBuffer(GL_COLOR_ATTACHMENT0_EXT));
  GL_CHECK(glReadPixels (0, 0, (unsigned int) size, (unsigned int) size,
			 currentGpuProgram->outProperty->format,
			 currentGpuProgram->outProperty->type,
			 values));
  
  return values;
}

// function to end the use of the current gpu program
bool detachGpuProgram() {
#if !defined(NDEBUG)
  if (!currentGpuProgram) {
    (*errorStream) << "detachGpuProgram failed: no current program" << endl;
    return false;
  }
#endif
  // end of gpu program use
  GL_CHECK(glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0));

  // restore previous context
  GL_CHECK(glPopAttrib());
  GL_CHECK(glMatrixMode(GL_MODELVIEW));
  GL_CHECK(glPopMatrix());
  GL_CHECK(glMatrixMode(GL_PROJECTION));
  GL_CHECK(glPopMatrix());

  // deinstall the program
  GL_CHECK(glUseProgram(0));
  currentGpuProgram = NULL;

  GL_CHECK(glDisable(GL_TEXTURE_2D));

  return true;
}

// internal function to hold some checks
static void* getGpuOutPropertyValues(unsigned int valSize,
				     unsigned int nbValues) {
  unsigned int nbVal;
  unsigned int vSize;
  GpuValueType type;

  void* values = getGpuOutPropertyValues(nbVal, vSize, type);
#if !defined(NDEBUG)
  assert(valSize == vSize);
  assert(nbValues == nbVal);
#endif
  return values;
}

// functions to get the result in an array of values
bool getGpuOutPropertyValues(bool *values, unsigned int nbValues) {
  // set property values with result ones
  float* rvalues =
    (float*) getGpuOutPropertyValues(sizeof(float), nbValues);

  if (!rvalues)
    return false;

  for (unsigned int i = 0; i < nbValues; ++i) {
    *values = *rvalues ? true : false;
    ++values;
    ++rvalues;
  }
  return true;
}

// an internal function to store the result values in an array
static bool getGpuOutPropertyValuesArray(void *values, unsigned int valSize,
					 unsigned int nbValues) {
  void *rvalues = getGpuOutPropertyValues(valSize, nbValues);
  if (!rvalues)
    return false;
  
  // set property values with result ones
  memcpy(values, rvalues, nbValues * valSize);

  return true;
}

bool getGpuOutPropertyValues(float *values, unsigned int nbValues) {
  return getGpuOutPropertyValuesArray(values, sizeof(float), nbValues);
}

bool getGpuOutPropertyValues(int *values, unsigned int nbValues) {
  // set property values with result ones
  float* rvalues =
    (float*) getGpuOutPropertyValues(sizeof(float), nbValues);

  if (!rvalues)
    return false;

  for (unsigned int i = 0; i < nbValues; ++i) {
    *values = (int) *rvalues;
    ++values;
    ++rvalues;
  }
  return true;
}

bool getGpuOutPropertyValues(bvec2 *values, unsigned int nbValues) {
  float* rvalues =
    // bvec2 are stored in RGB format with FLOAT values
    (float*) getGpuOutPropertyValues(3 * sizeof(float), nbValues);
  if (!rvalues)
    return false;

  for (unsigned int i = 0; i < nbValues; ++i) {
    values->array[0] = rvalues[0] ? true : false;
    values->array[1] = rvalues[1] ? true : false;
    ++values;
    rvalues += 3;
  }
  return true;
}

bool getGpuOutPropertyValues(bvec3 *values, unsigned int nbValues) {
  return getGpuOutPropertyValuesArray(values, sizeof(bvec3),
				      nbValues);
}

bool getGpuOutPropertyValues(bvec4 *values, unsigned int nbValues) {
  return getGpuOutPropertyValuesArray(values, sizeof(bvec4),
				      nbValues);
}

bool getGpuOutPropertyValues(vec2 *values, unsigned int nbValues) {
  float* rvalues =
    // vec2 are stored in RGB format
    (float*) getGpuOutPropertyValues(3 * sizeof(float), nbValues);
  if (!rvalues)
    return false;

  for (unsigned int i = 0; i < nbValues; ++i) {
    memcpy(values, rvalues, sizeof(vec2));
    ++values;
    rvalues += 3;
  }
  return true;
}

bool getGpuOutPropertyValues(vec3 *values, unsigned int nbValues) {
  return getGpuOutPropertyValuesArray(values, sizeof(vec3),
				      nbValues);
}

bool getGpuOutPropertyValues(vec4 *values, unsigned int nbValues) {
  return getGpuOutPropertyValuesArray(values, sizeof(vec4),
				      nbValues);
}

bool getGpuOutPropertyValues(ivec2 *values, unsigned int nbValues) {
  float* rvalues =
    // ivec2 are stored in RGB format with FLOAT values
    (float*) getGpuOutPropertyValues(3 * sizeof(float), nbValues);
  if (!rvalues)
    return false;

  for (unsigned int i = 0; i < nbValues; ++i) {
    values->array[0] = (int) rvalues[0];
    values->array[1] = (int) rvalues[1];
    ++values;
    rvalues += 3;
  }
  return true;
}

bool getGpuOutPropertyValues(ivec3 *values, unsigned int nbValues) {
  float* rvalues =
    (float*) getGpuOutPropertyValues(3 * sizeof(float), nbValues);
  if (!rvalues)
    return false;

  for (unsigned int i = 0; i < nbValues; ++i) {
    values->array[0] = (int) rvalues[0];
    values->array[1] = (int) rvalues[1];
    values->array[2] = (int) rvalues[2];
    ++values;
    rvalues += 3;
  }
  return true;
}

bool getGpuOutPropertyValues(ivec4 *values, unsigned int nbValues) {
  float* rvalues =
    (float*) getGpuOutPropertyValues(4 * sizeof(float), nbValues);
  if (!rvalues)
    return false;

  for (unsigned int i = 0; i < nbValues; ++i) {
    values->array[0] = (int) rvalues[0];
    values->array[1] = (int) rvalues[1];
    values->array[2] = (int) rvalues[2];
    values->array[3] = (int) rvalues[3];
    ++values;
    rvalues += 4;
  }
  return true;
}
