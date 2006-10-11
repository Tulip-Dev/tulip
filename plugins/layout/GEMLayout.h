

/** \addtogroup layout */
/*@{*/
/// GEM3D.h - An implementation of a spring-embedder layout.
/** This plugin is an implementation of the GEM-2d layout
 *  algorithm first published as:
 *
 *  A. Frick, A. Ludwig, and H. Mehldau, \n
 *  "A fast, adaptive layout algorithm for undirected graphs", \n
 *  In R. Tamassia and I. Tollis (Eds), \n 
 *  Graph Drawing'94, \n
 *  Volume 894 of Lecture Notes in Computer Science, Springer Verlag, 1995.
 *
 *  <b>HISTORY</b>
 *
 *  The implementation started life as the public-domain 
 *  code produced by Arne Frick and released at
 *
 *  www.frick-consulting.de/publications.html
 *
 *  The core "embedder" part of the algorithm was used in the
 *  implementation of a Java plugin for the CWI "Royere"  tool,
 *  and then this code was ported to C++ to make the 
 *  implementation given here.
 *
 *
 *  \note The embedder algorithm described by Frick involves three
 *  phases: insertion, arrangement, and optimization.  Only
 *  the first two of these phases are included here. 
 *  Experiments with the Java implementation showed that the
 *  optimization phase consumed significantly more time than
 *  the first two and produced apparently marginal improvements
 *  in the final layout.\n
 *  As GEM, like other spring-embedder algorithms, is 
 *  computationally expensive, I have tried to optimize the
 *  loops somewhat by storing all necessary node information
 *  into two arrays: "GemProp" carries the (scalar) values 
 *  associated with each node by the layout algorithm, while
 *  "Adjacent" is an array of vectors, one vector per node,
 *  giving the index (integer) of each node.
 *
 * \note The new version has been reimplemented to manage edge length
 * it merges the 3D stuff and removes the use if integers (new CPU do not
 * require it anymore).
 *
 *  \author David Duke, University of Bath, UK: Email: D.Duke@bath.ac.uk
 *  \author David Auber,University of Bordeauw, FR: Email: david.auber@labri.fr
 *  Version 0.1: 23 July 2001.
 *  Version 0.2: September 2006
 */
 
#ifndef Tulip_GEMLayout_H
#define Tulip_GEMLayout_H

#include <queue>
#include <vector>
#if (__GNUC__ < 3)
#include <hash_map>
#else
#include <ext/hash_map>
#endif

#include <tulip/TulipPlugin.h>
using namespace tlp;
using namespace std;
using namespace stdext;

class GEMLayout : public tlp::LayoutAlgorithm
{ 
public:
    GEMLayout(const tlp::PropertyContext &);
    ~GEMLayout();
    bool run();

private: 
    Coord computeForces(unsigned int v, 
			float shake, 
			float gravity, 
			bool testPlaced);
    
    struct GEMparticule {
      node n;
      Coord pos;        // position
      int in;
      Coord imp;        // impulse
      float dir;        // direction
      float heat;        // heat
      float mass;        // weight = nr incident edges
      unsigned int id;
      GEMparticule(float m = 0) {
	pos.fill(0);
	imp.fill(0);
	dir  = 0.0;
	heat = 0;
	mass = m;
      }
    };
    
    /*
     * Functions used to implement the GEM3D layout.
     */
     
    unsigned int select();
    void vertexdata_init(const float starttemp);
    void insert();
    void displace(int v, Coord imp);
    void a_round();
    void arrange();
    void updateLayout();
        
    std::vector<GEMparticule> _particules;
    vector<int> _map; //for random selection
    MutableContainer<GEMparticule *> _nodeToParticules;

    /*
     * GEM3D variables
     */

    unsigned long Iteration;
    float         _temperature;
    Coord         _center;
    float         _maxtemp;
    float         _oscillation, _rotation;

    
    /*
     * Following parameters can be initialised in the original GEM3D
     * from a configuration file.  Here they are hard-wired, but
     * this could be replaced by configuration from a file.
     */
    float    i_maxtemp;
    float    i_starttemp;
    float    i_finaltemp;
    int      i_maxiter;
    float    i_gravity;
    float    i_oscillation;
    float    i_rotation;
    float    i_shake;

    float    a_maxtemp;
    float    a_starttemp;
    float    a_finaltemp;
    int      a_maxiter;
    float    a_gravity;
    float    a_oscillation;
    float    a_rotation;
    float    a_shake;

    
    unsigned int _dim; //2 or 3;
    unsigned int _nbNodes; //number of nodes in the graph
    bool _useLength; //if we manage edge length
    tlp::DoubleProperty * metric; //metric for edge length

};
/*@}*/
#endif
