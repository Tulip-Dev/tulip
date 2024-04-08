#ifndef GRAPHHELPER_INCLUDED
#define GRAPHHELPER_INCLUDED

//#include <igraph/igraph.h>
// we only need the igraph_neimode_t definition
typedef enum { IGRAPH_OUT = 1, IGRAPH_IN = 2, IGRAPH_ALL = 3 } igraph_neimode_t;
// and a igraph_rng_t pseudo definition
#include <cstddef>
#include <cmath>
typedef size_t igraph_rng_t;
// hide some igraph randomize functions
#define igraph_rng_seed(arg1, arg2)
#define igraph_rng_init(arg1, arg2)
#define igraph_rng_destroy(arg)

//#include "libleidenalg_export.h"
#define LIBLEIDENALG_EXPORT

#include <vector>
#include <set>
#include <exception>
#include <deque>

#include <cstdbool>

//#ifdef DEBUG
#include <iostream>
using std::cerr;
using std::endl;
//#endif

class MutableVertexPartition;

using std::deque;
using std::make_pair;
using std::pair;
using std::set;
using std::vector;

vector<size_t> range(size_t n);

bool orderCSize(const size_t *A, const size_t *B);

double KL(double q, double p);
double KLL(double q, double p);

template <class T>
T sum(vector<T> vec) {
  T sum_of_elems = T();
  for (T x : vec)
    sum_of_elems += x;
  return sum_of_elems;
}

class Exception : public std::exception {
public:
  Exception(const char *str) {
    this->str = str;
  }

  virtual const char *what() const throw() {
    return this->str;
  }

private:
  const char *str;
};

size_t get_random_int(size_t from, size_t to, igraph_rng_t *rng);

void shuffle(vector<size_t> &v, igraph_rng_t *rng);

namespace tlp {

class Graph;
class NumericProperty;

} // namespace tlp

#include <tulip/vectorgraph.h>

class LIBLEIDENALG_EXPORT Graph {
public:
  Graph(tlp::VectorGraph *graph, vector<double> const &edge_weights,
        vector<double> const &node_sizes, bool correct_self_loops, bool is_directed);
  Graph(tlp::Graph *graph, tlp::NumericProperty *edge_weights, bool is_directed);
  ~Graph();

  double possible_edges();
  double possible_edges(double n);

  Graph *collapse_graph(MutableVertexPartition *partition);

  vector<size_t> const &get_neighbour_edges(size_t v, igraph_neimode_t mode);
  vector<size_t> const &get_neighbours(size_t v, igraph_neimode_t mode);
  size_t get_random_neighbour(size_t v, igraph_neimode_t mode, igraph_rng_t *rng);

  inline size_t get_random_node(igraph_rng_t *rng) {
    return get_random_int(0, this->vcount() - 1, rng);
  };

  inline size_t vcount() {
    return _graph->numberOfNodes();
  }
  inline size_t ecount() {
    return _graph->numberOfNodes();
  }
  inline double total_weight() {
    return this->_total_weight;
  };
  inline double total_size() {
    return this->_total_size;
  };
  inline bool is_directed() {
    return this->_is_directed;
  };
  inline double density() {
    return this->_density;
  };
  inline bool correct_self_loops() {
    return this->_correct_self_loops;
  };
  inline bool is_weighted() {
    return this->_is_weighted;
  };

  inline double edge_weight(size_t e) {
    return this->_edge_weights[e];
  };

  inline void edge(size_t eid, size_t &from, size_t &to) {
    auto ends = _graph->ends(tlp::edge(eid));
    from = ends.first;
    to = ends.second;
  }

  inline vector<size_t> edge(size_t e) {
    vector<size_t> edge(2);
    this->edge(e, edge[0], edge[1]);
    return edge;
  }

  inline double node_size(size_t v) {
    return this->_node_sizes[v];
  };

  inline double node_self_weight(size_t v) {
    return this->_node_self_weights[v];
  };

  inline size_t degree(size_t v, igraph_neimode_t mode) {
    if (mode == IGRAPH_IN || !this->is_directed())
      return _graph->indeg(tlp::node(v));
    if (mode == IGRAPH_OUT)
      return _graph->outdeg(tlp::node(v));
    return _graph->deg(tlp::node(v));
  };

  inline double strength(size_t v, igraph_neimode_t mode) {
    if (mode == IGRAPH_IN || !this->is_directed())
      return this->_strength_in[v];
    return this->_strength_out[v];
  };

protected:
  tlp::VectorGraph *_graph;

  // Utility variables to easily access the strength of each node
  vector<double> _strength_in;
  vector<double> _strength_out;

  vector<double> _edge_weights;      // Used for the weight of the edges.
  vector<double> _node_sizes;        // Used for the size of the nodes.
  vector<double> _node_self_weights; // Used for the self weight of the nodes.

  void cache_neighbours(size_t v, igraph_neimode_t mode);
  vector<size_t> _cached_neighs_from;
  size_t _current_node_cache_neigh_from;
  vector<size_t> _cached_neighs_to;
  size_t _current_node_cache_neigh_to;
  vector<size_t> _cached_neighs_all;
  size_t _current_node_cache_neigh_all;

  void cache_neighbour_edges(size_t v, igraph_neimode_t mode);
  vector<size_t> _cached_neigh_edges_from;
  size_t _current_node_cache_neigh_edges_from;
  vector<size_t> _cached_neigh_edges_to;
  size_t _current_node_cache_neigh_edges_to;
  vector<size_t> _cached_neigh_edges_all;
  size_t _current_node_cache_neigh_edges_all;

  double _total_weight;
  double _total_size;
  bool _is_weighted;
  bool _is_directed;

  bool _correct_self_loops;
  double _density;

  void init_admin();
  void set_defaults();
  void set_default_edge_weight();
  void set_default_node_size();
  void set_self_weights();
};

// We need this ugly way to include the MutableVertexPartition
// to overcome a circular linkage problem.
#include "MutableVertexPartition.h"

#endif // GRAPHHELPER_INCLUDED
