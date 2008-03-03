#ifndef LIBGPUGRAPH_H
#define LIBGPUGRAPH_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <string>
#include <GL/glew.h>
#include <GL/gl.h>

#ifdef _WIN32
# 	ifdef DLL_TULIP_GL
#		define TLP_GPU_SCOPE       __declspec(dllexport)
# 	else
#		define TLP_GPU_SCOPE       __declspec(dllimport)
# 	endif
#endif
#ifndef TLP_GPU_SCOPE
#define TLP_GPU_SCOPE           
#endif


// An example of the use of the following API with Tulip
/*

// initialization
if (GpuInit() != true)
return;

// program creation
ifstream* programStream = new ifstream(fileName);
// parameters are the name of gpu graph uniform variable
// an input stream to get the program specific code
// and the iteration mode (NODES_ITERATION or EDGES_ITERATION)
GpuProgram* prog = genGpuProgram("graph", programStream);
if (!prog)
return;
delete programStream;

// gpu graph creation from an already existing Tulip graph
// all the data concerning the future use of the gpu graph are collected
// and transferred on the gpu
GpuGraph *gpuGraph = genGpuGraph(graph);

// get some input values from a graph property
LayoutProperty* layoutProp = graph->getProperty<BooleanProperty>("viewLayout");
// all the data concerning the future use of the gpu property are collected
GpuProperty* gpuLayout = genGpuProperty(layoutProp);

// tell our program is the current one
// and it will iterate on gpuGraph nodes
if (!attachGpuProgram(prog, gpuGraph))
return;

// transfer the property collected data
// to the uniform variable named "layout" in the current program
if (!setGpuParameter("layout", gpuLayout))
return;

// set other current program uniform variables
if (!setGpuParameter("flag", true))
return;

if (!setGpuParameter("initial_coord", tlp::Coord(0, 0, 0)))
return;

// set the out property
if (!setGpuOutProperty(gpuLayout))
return;

// launch the computation
if (!computeGpuProgram())
return;

// store the computed values in the viewLayout property of our graph
if (!getGpuOuPropertyValues(layoutProp, graph))
return;

// reset opengl in it's 'normal' state
if (!detachGpuProgram())
return;

// delete all
delGpuProgram(prog);
delGpuGraph(gpuGraph);
delGpuProperty(gpuLayout);

*/

// define some macros to add some useful types
#define VEC2(type, eltType)			\
  union type {					\
    eltType array[2];				\
    struct {					\
      eltType r;				\
      eltType g;				\
    } color;					\
    struct {					\
      eltType x;				\
      eltType y;				\
    } pos;					\
    struct {					\
      eltType s;				\
      eltType t;				\
    } tcoord;					\
  }

#define VEC3(type, eltType)			\
  union type {					\
    eltType array[3];				\
    struct {					\
      eltType r;				\
      eltType g;				\
      eltType b;				\
    } color;					\
    struct {					\
      eltType x;				\
      eltType y;				\
      eltType z;				\
    } pos;					\
    struct {					\
      eltType s;				\
      eltType t;				\
      eltType p;				\
    } tcoord;					\
  }

#define VEC4(type, eltType)			\
  union type {					\
    eltType array[4];				\
    struct {					\
      eltType r;				\
      eltType g;				\
      eltType b;				\
      eltType a;				\
    } color;					\
    struct {					\
      eltType x;				\
      eltType y;				\
      eltType z;				\
      eltType w;				\
    } pos;					\
    struct {					\
      eltType s;				\
      eltType t;				\
      eltType p;				\
      eltType q;				\
    } tcoord;					\
  }

// add boolean structures
VEC2(bvec2, bool);
VEC3(bvec3, bool);
VEC4(bvec4, bool);

// add float structures
VEC2(vec2, float);
VEC3(vec3, float);
VEC4(vec4, float);

// add integer structures
VEC2(ivec2, int);
VEC3(ivec3, int);
VEC4(ivec4, int);

// add matrix types
typedef float mat2[2][2];
typedef float mat3[3][3];
typedef float mat4[4][4];

// API initialization
// indicates the errorStream used by the API,
// check OpenGl version, and the number of texture image units
// available (min = 5), then initialize the predefined shaders
TLP_GPU_SCOPE bool GpuInit(std::ostream& errorStream = std::cerr);

// API end of use
// free allocated resources
TLP_GPU_SCOPE bool GpuEnd();

//-------------------
// Graph management
//-------------------
class GpuGraph;

// a simple struct to hold node's infos
struct CpuNode {
  int id;
  unsigned int indeg;
  unsigned int outdeg;
};

// a simple struct to hold edges's infos
struct CpuEdge {
  int source; // id of source node
  int target; // id of target node
};

// for a simple set of nodes
TLP_GPU_SCOPE GpuGraph* genGpuGraph(unsigned int nbNodes);

// for a set of nodes and edges
TLP_GPU_SCOPE GpuGraph* genGpuGraph(CpuNode* nodes, unsigned int nbNodes,
				    CpuEdge* edges, unsigned int nbEdges);

// the destructor function
TLP_GPU_SCOPE bool delGpuGraph(GpuGraph *gpuGraph);

// equivalent of gpu structures
struct GpuElt {
  vec2 pos;
};

#define GpuNode GpuElt
#define GpuEdge GpuElt

// a structure to iterate on GpuGraph elts
struct GpuEltIterator {
  unsigned int current;
  unsigned int steps;
  unsigned int size;
};

// corresponding functions
// get nodes iterator
TLP_GPU_SCOPE GpuEltIterator getGpuNodes(GpuGraph* graph);

// get edges iterator
TLP_GPU_SCOPE GpuEltIterator getGpuEdges(GpuGraph* graph);

// check for next elt
TLP_GPU_SCOPE bool hasNext(GpuEltIterator& it);

// get next elt
TLP_GPU_SCOPE void getNext(GpuElt& elt, GpuEltIterator& it);

//--------------------
// Property management
//--------------------
class GpuProperty;

enum GpuValueType { NODE_VALUES, EDGE_VALUES, ALL_VALUES };

// low level function for creation of a GpuProperty
// it creates a property with associated values (see below)
// sized to store nbNodes + nbEdges values
// valSize is the size in char of a value (sizeof(...))
// see GlTexImage2D about internal_format, format and type
TLP_GPU_SCOPE GpuProperty *genGpuProperty(unsigned int nbNodes, unsigned int nbEdges,
					  unsigned int valSize, GLint internal_format,
					  GLint format, GLint type);

// low level function to get a pointer
// to the allocated memory area where the property associated values
// are stored
TLP_GPU_SCOPE void* getGpuPropertyValues(GpuProperty *prop);

// high level functions which associate
// an array of values to a gpu property variable
// and compute the needed infos
// first argument must be null if the property
// is for output only
// return NULL if there is no texture image unit available
TLP_GPU_SCOPE GpuProperty* genGpuProperty(bool *values,
					  unsigned int nbNodeValues,
					  unsigned int nbEdgesValues = 0);
TLP_GPU_SCOPE GpuProperty* genGpuProperty(float *values,
					  unsigned int nbNodeValues,
					  unsigned int nbEdgesValues = 0);
TLP_GPU_SCOPE GpuProperty* genGpuProperty(int *values,
					  unsigned int nbNodeValues,
					  unsigned int nbEdgesValues = 0);
TLP_GPU_SCOPE GpuProperty* genGpuProperty(bvec2 *values,
					  unsigned int nbNodeValues,
					  unsigned int nbEdgesValues = 0);
TLP_GPU_SCOPE GpuProperty* genGpuProperty(bvec3 *values,
					  unsigned int nbNodeValues,
					  unsigned int nbEdgesValues = 0);
TLP_GPU_SCOPE GpuProperty* genGpuProperty(bvec4 *values,
					  unsigned int nbNodeValues,
					  unsigned int nbEdgesValues = 0);
TLP_GPU_SCOPE GpuProperty* genGpuProperty(vec2 *values,
					  unsigned int nbNodeValues,
					  unsigned int nbEdgesValues = 0);
TLP_GPU_SCOPE GpuProperty* genGpuProperty(vec3 *values,
					  unsigned int nbNodeValues,
					  unsigned int nbEdgesValues = 0);
TLP_GPU_SCOPE GpuProperty* genGpuProperty(vec4 *values,
					  unsigned int nbNodeValues,
					  unsigned int nbEdgesValues = 0,
					  bool onGpu = true);
TLP_GPU_SCOPE GpuProperty* genGpuProperty(ivec2 *values,
					  unsigned int nbNodeValues,
					  unsigned int nbEdgesValues = 0);
TLP_GPU_SCOPE GpuProperty* genGpuProperty(ivec3 *values,
					  unsigned int nbNodeValues,
					  unsigned int nbEdgesValues = 0);
TLP_GPU_SCOPE GpuProperty* genGpuProperty(ivec4 *values,
					  unsigned int nbNodeValues,
					  unsigned int nbEdgesValues = 0);

// clone a property
TLP_GPU_SCOPE GpuProperty* genGpuProperty(GpuProperty* prop);

// delete function
TLP_GPU_SCOPE bool delGpuProperty(GpuProperty* prop);

//-------------------
// Program management
//-------------------
class GpuProgram;

enum GpuIterationMode{NODES_ITERATION, EDGES_ITERATION};

// create a gpu program with named graph variable
// code source from the input stream
// and iteration mode (NODES_ITERATION or EDGES_ITERATION)
// compile the corresponding fragment shader
// if this fails a null pointer is returned
TLP_GPU_SCOPE GpuProgram* genGpuProgram(const std::string& graphName,
					std::istream& inputStream,
					GpuIterationMode mode = NODES_ITERATION);

// delete function
TLP_GPU_SCOPE bool delGpuProgram(GpuProgram *);

// a function to set a GpuProgram as the current program
// indicating the current graph 
TLP_GPU_SCOPE bool attachGpuProgram(GpuProgram* program, GpuGraph* graph);

// some functions to set a program parameter value
// must be called after attachGpuProgram
TLP_GPU_SCOPE bool setGpuParameter(const std::string& pName, GpuProperty *prop);
TLP_GPU_SCOPE bool setGpuParameter(const std::string& pName, bool value);
TLP_GPU_SCOPE bool setGpuParameter(const std::string& pName, const bool* values,
				   unsigned nbValues);
TLP_GPU_SCOPE bool setGpuParameter(const std::string& pName, float value);
TLP_GPU_SCOPE bool setGpuParameter(const std::string& pName, const float* values,
				   unsigned nbValues);
TLP_GPU_SCOPE bool setGpuParameter(const std::string& pName, int value);
TLP_GPU_SCOPE bool setGpuParameter(const std::string& pName, const int* values,
				   unsigned nbValues);
TLP_GPU_SCOPE bool setGpuParameter(const std::string& pName, const bvec2& val);
TLP_GPU_SCOPE bool setGpuParameter(const std::string& pName, const bvec3& val);
TLP_GPU_SCOPE bool setGpuParameter(const std::string& pName, const bvec4& val);
TLP_GPU_SCOPE bool setGpuParameter(const std::string& pName, const ivec2& val);
TLP_GPU_SCOPE bool setGpuParameter(const std::string& pName, const ivec3& val);
TLP_GPU_SCOPE bool setGpuParameter(const std::string& pName, const ivec4& val);
TLP_GPU_SCOPE bool setGpuParameter(const std::string& pName, const vec2& val);
TLP_GPU_SCOPE bool setGpuParameter(const std::string& pName, const vec3& val);
TLP_GPU_SCOPE bool setGpuParameter(const std::string& pName, const vec4& val);
TLP_GPU_SCOPE bool setGpuParameter(const std::string& pName, const GpuElt& elt);
TLP_GPU_SCOPE bool setGpuParameter(const std::string& pName, const mat2& matrix);
TLP_GPU_SCOPE bool setGpuParameter(const std::string& pName, const mat3& matrix);
TLP_GPU_SCOPE bool setGpuParameter(const std::string& pName, const mat4& matrix);

// specify the property to hold the result
TLP_GPU_SCOPE bool setGpuOutProperty(GpuProperty *prop);

// reset of a GpuProperty
// the cpu stored values are retransferred on Gpu
TLP_GPU_SCOPE bool resetGpuProperty(GpuProperty *prop);

// the launch of the computation
TLP_GPU_SCOPE bool computeGpuProgram();

// swap the current result property
// with the property given as parameter
TLP_GPU_SCOPE bool swapGpuOutProperty(GpuProperty *prop);

// to reset opengl to default program
TLP_GPU_SCOPE bool detachGpuProgram();

// a low level function to get the current program outProperty values
// it returns a pointer to the memory area where computed values are stored
// nbValues, valSize and valType are out arguments
// indicating if their number, size (of a value)
// and type (NODE_VALUES or EDGE_VALUES)
TLP_GPU_SCOPE void* getGpuOutPropertyValues(unsigned int& nbValues,
					    unsigned int& valSize,
					    GpuValueType& valType);

// some functions to get the values
// computed in the outProperty
TLP_GPU_SCOPE bool getGpuOutPropertyValues(bool *values, unsigned int nbValues);
TLP_GPU_SCOPE bool getGpuOutPropertyValues(float *values, unsigned int nbValues);
TLP_GPU_SCOPE bool getGpuOutPropertyValues(int *values, unsigned int nbValues);
TLP_GPU_SCOPE bool getGpuOutPropertyValues(bvec2 *values, unsigned int nbValues);
TLP_GPU_SCOPE bool getGpuOutPropertyValues(bvec3 *values, unsigned int nbValues);
TLP_GPU_SCOPE bool getGpuOutPropertyValues(bvec4 *values, unsigned int nbValues);
TLP_GPU_SCOPE bool getGpuOutPropertyValues(vec2 *values, unsigned int nbValues);
TLP_GPU_SCOPE bool getGpuOutPropertyValues(vec3 *values, unsigned int nbValues);
TLP_GPU_SCOPE bool getGpuOutPropertyValues(vec4 *values, unsigned int nbValues);
TLP_GPU_SCOPE bool getGpuOutPropertyValues(ivec2 *values, unsigned int nbValues);
TLP_GPU_SCOPE bool getGpuOutPropertyValues(ivec3 *values, unsigned int nbValues);
TLP_GPU_SCOPE bool getGpuOutPropertyValues(ivec4 *values, unsigned int nbValues);

#endif
