#include "GraphHelper.h"

#include <tulip/TlpTools.h>
#include <tulip/Graph.h>
#include <tulip/NumericProperty.h>

#ifdef DEBUG
using std::cerr;
using std::endl;
#endif

vector<size_t> range(size_t n) {
  vector<size_t> range_vec(n);
  for (size_t i = 0; i < n; i++)
    range_vec[i] = i;
  return range_vec;
}

bool orderCSize(const size_t *A, const size_t *B) {

  if (A[1] == B[1]) {
    if (A[2] == B[2])
      return A[0] < B[0];
    else
      return A[2] > B[2];
  } else
    return A[1] > B[1];
}

size_t get_random_int(size_t /*from*/, size_t to, igraph_rng_t *) {
  // we assume from is always 0
  return static_cast<size_t>(tlp::randomUnsignedInteger(to));
}

void shuffle(vector<size_t> &v, igraph_rng_t *rng) {
  size_t n = v.size();
  if (n > 0) {
    for (size_t idx = n - 1; idx > 0; idx--) {
      size_t rand_idx = get_random_int(0, idx, rng);
      size_t tmp = v[idx];
      v[idx] = v[rand_idx];
      v[rand_idx] = tmp;
    }
  }
}

/****************************************************************************
  The binary Kullback-Leibler divergence.
****************************************************************************/
double KL(double q, double p) {
  double KL = 0.0;
  if (q > 0.0 && p > 0.0)
    KL += q * log(q / p);
  if (q < 1.0 && p < 1.0)
    KL += (1.0 - q) * log((1.0 - q) / (1.0 - p));
  return KL;
}

double KLL(double q, double p) {
  double KL = 0.0;
  if (q > 0.0 && p > 0.0)
    KL += q * log(q / p);
  if (q < 1.0 && p < 1.0)
    KL += (1.0 - q) * log((1.0 - q) / (1.0 - p));
  if (q < p)
    KL *= -1;
  return KL;
}

Graph::Graph(tlp::VectorGraph *graph, vector<double> const &edge_weights,
             vector<double> const &node_sizes, bool correct_self_loops, bool is_directed)
    : _graph(graph), _edge_weights(edge_weights), _node_sizes(node_sizes) {
  _correct_self_loops = correct_self_loops;
  _is_directed = is_directed;
  _total_size = 0;
  _node_self_weights.resize(vcount(), 0.0);
  this->init_admin();
}

Graph::Graph(tlp::Graph *graph, tlp::NumericProperty *edge_weights, bool is_directed) {
  tlp::VectorGraph *vgraph = new tlp::VectorGraph();
  _correct_self_loops = false;

  // init from graph nodes infos
  unsigned int n = graph->numberOfNodes();
  vgraph->addNodes(n);
  _node_sizes.resize(n, 1);
  _node_self_weights.resize(n, 0.0);
  _strength_in.resize(n, 0.0);
  if ((_is_directed = is_directed))
    _strength_out.resize(n, 0.0);
  _total_size = n;

  // build from graph edges infos
  _total_weight = 0.0;
  n = graph->numberOfEdges();
  vgraph->reserveAdj(n);
  _edge_weights.resize(n);
  _is_weighted = edge_weights != nullptr;
  auto edges = graph->edges();
  for (unsigned int i = 0; i < n; ++i) {
    auto e = edges[i];
    auto ends = graph->ends(e);
    auto from = graph->nodePos(ends.first);
    auto to = graph->nodePos(ends.second);
    // add edge
    vgraph->addEdge(tlp::node(from), tlp::node(to));
    double w = edge_weights ? edge_weights->getEdgeDoubleValue(e) : 1;
    // set weights
    _edge_weights[i] = w;
    _total_weight += w;
    if (from == to) {
      _correct_self_loops = true;
      _node_self_weights[from] = w;
    }
    // and strengths
    _strength_in[to] += w;
    if (is_directed)
      _strength_out[from] += w;
    else
      _strength_in[from] += w;
  }
  _graph = vgraph;
  this->init_admin();
}

Graph::~Graph() {
  delete _graph;
}

double Graph::possible_edges() {
  return this->possible_edges(this->vcount());
}

double Graph::possible_edges(double n) {
  double possible_edges = n * (n - 1);
  if (!this->is_directed())
    possible_edges /= 2;
  if (this->correct_self_loops())
    possible_edges += n;

  return possible_edges;
}

void Graph::set_defaults() {
  this->set_default_edge_weight();
  this->set_default_node_size();
}

void Graph::set_default_edge_weight() {
  size_t m = this->ecount();

  // Set default edge weight of 1.0
  this->_edge_weights.clear();
  this->_edge_weights.resize(m);
  fill(this->_edge_weights.begin(), this->_edge_weights.end(), 1.0);
  this->_is_weighted = false;
}

void Graph::set_default_node_size() {
  size_t n = this->vcount();

  // Set default node size of 1
  this->_node_sizes.clear();
  this->_node_sizes.resize(n);
  fill(this->_node_sizes.begin(), this->_node_sizes.end(), 1);
}

void Graph::init_admin() {

  size_t m = this->ecount();
  size_t n = this->vcount();

  if (_total_size != n) {
    this->_strength_in.resize(n, 0.0);

    if (this->_is_directed)
      this->_strength_out.resize(n, 0.0);

    // Determine total weight in the graph.
    this->_total_weight = 0.0;
    for (size_t e = 0; e < m; e++) {
      double w = this->edge_weight(e);
      this->_total_weight += w;

      size_t from, to;
      this->edge(e, from, to);

      if (this->is_directed()) {
        this->_strength_in[to] += w;
        this->_strength_out[from] += w;
      } else {
        // we only compute strength_in and degree_in for undirected graphs
        this->_strength_in[to] += w;
        this->_strength_in[from] += w;
      }
    }

    // Make sure to multiply by 2 for undirected graphs
    // if (!this->is_directed())
    //  this->_total_weight *= 2.0;

    this->_total_size = 0;
    for (size_t v = 0; v < n; v++)
      this->_total_size += this->node_size(v);
  }

  // Calculate density;
  double w = _total_weight;
  double n_size = _total_size;

  // For now we default to not correcting self loops.
  // this->_correct_self_loops = false; (remove this as this is set in the constructor)

  double normalise = 0.0;
  if (this->_correct_self_loops)
    normalise = n_size * n_size;
  else
    normalise = n_size * (n_size - 1);

  if (this->is_directed())
    this->_density = w / normalise;
  else
    this->_density = 2 * w / normalise;

  this->_current_node_cache_neigh_edges_from = n + 1;
  this->_current_node_cache_neigh_edges_to = n + 1;
  this->_current_node_cache_neigh_edges_all = n + 1;

  this->_current_node_cache_neigh_from = n + 1;
  this->_current_node_cache_neigh_to = n + 1;
  this->_current_node_cache_neigh_all = n + 1;
}

void Graph::cache_neighbour_edges(size_t v, igraph_neimode_t mode) {
  size_t degree = this->degree(v, mode);

  vector<size_t> *_cached_neigh_edges = nullptr;
  tlp::Iterator<tlp::edge> *it = nullptr;
  switch (mode) {
  case IGRAPH_IN:
    this->_current_node_cache_neigh_edges_from = v;
    _cached_neigh_edges = &(this->_cached_neigh_edges_from);
    it = _graph->getInEdges(tlp::node(v));
    break;
  case IGRAPH_OUT:
    this->_current_node_cache_neigh_edges_to = v;
    _cached_neigh_edges = &(this->_cached_neigh_edges_to);
    it = _graph->getOutEdges(tlp::node(v));
    break;
  case IGRAPH_ALL:
    this->_current_node_cache_neigh_edges_all = v;
    _cached_neigh_edges = &(this->_cached_neigh_edges_all);
    it = _graph->getInOutEdges(tlp::node(v));
    break;
  }

  _cached_neigh_edges->clear();
  _cached_neigh_edges->reserve(degree);
  while (it->hasNext()) {
    _cached_neigh_edges->push_back(it->next().id);
  }
  delete it;
}

vector<size_t> const &Graph::get_neighbour_edges(size_t v, igraph_neimode_t mode) {
  if (!this->is_directed())
    mode = IGRAPH_ALL; // igraph ignores mode for undirected graphs

  switch (mode) {
  case IGRAPH_IN:
    if (this->_current_node_cache_neigh_edges_from != v) {
      cache_neighbour_edges(v, mode);
      this->_current_node_cache_neigh_edges_from = v;
    }
    return this->_cached_neigh_edges_from;
  case IGRAPH_OUT:
    if (this->_current_node_cache_neigh_edges_to != v) {
      cache_neighbour_edges(v, mode);
      this->_current_node_cache_neigh_edges_to = v;
    }
    return this->_cached_neigh_edges_to;
  case IGRAPH_ALL:
  default:
    if (this->_current_node_cache_neigh_edges_all != v) {
      cache_neighbour_edges(v, mode);
      this->_current_node_cache_neigh_edges_all = v;
    }
    return this->_cached_neigh_edges_all;
  }
}

void Graph::cache_neighbours(size_t v, igraph_neimode_t mode) {
  size_t degree = this->degree(v, mode);

  vector<size_t> *_cached_neighs = nullptr;
  tlp::Iterator<tlp::node> *it = nullptr;
  switch (mode) {
  case IGRAPH_IN:
    this->_current_node_cache_neigh_from = v;
    _cached_neighs = &(this->_cached_neighs_from);
    it = _graph->getInNodes(tlp::node(v));
    break;
  case IGRAPH_OUT:
    this->_current_node_cache_neigh_to = v;
    _cached_neighs = &(this->_cached_neighs_to);
    it = _graph->getOutNodes(tlp::node(v));
    break;
  case IGRAPH_ALL:
    this->_current_node_cache_neigh_all = v;
    _cached_neighs = &(this->_cached_neighs_all);
    it = _graph->getOutNodes(tlp::node(v));
    break;
  }

  _cached_neighs->clear();
  _cached_neighs->reserve(degree);
  while (it->hasNext()) {
    _cached_neighs->push_back(it->next().id);
  }
  delete it;
}

vector<size_t> const &Graph::get_neighbours(size_t v, igraph_neimode_t mode) {
  if (!this->is_directed())
    mode = IGRAPH_ALL; // igraph ignores mode for undirected graphs

  switch (mode) {
  case IGRAPH_IN:
    if (this->_current_node_cache_neigh_from != v) {
      cache_neighbours(v, mode);
      this->_current_node_cache_neigh_from = v;
    }
    return this->_cached_neighs_from;
  case IGRAPH_OUT:
    if (this->_current_node_cache_neigh_to != v) {
      cache_neighbours(v, mode);
      this->_current_node_cache_neigh_to = v;
    }
    return this->_cached_neighs_to;
  case IGRAPH_ALL:
  default:
    if (this->_current_node_cache_neigh_all != v) {
      cache_neighbours(v, mode);
      this->_current_node_cache_neigh_all = v;
    }
    return this->_cached_neighs_all;
  }
}

/********************************************************************************
 * This should return a random neighbour in O(1)
 ********************************************************************************/
size_t Graph::get_random_neighbour(size_t v, igraph_neimode_t mode, igraph_rng_t * /*rng*/) {
  size_t rand_neigh_idx = get_random_int(0, degree(v, mode), nullptr);
  tlp::Iterator<tlp::node> *it = nullptr;

  /*if (this->degree(v, mode) <= 0)
    throw Exception("Cannot select a random neighbour for an isolated node.");*/

  if (this->is_directed() && mode != IGRAPH_ALL) {
    if (mode == IGRAPH_OUT) {
      it = _graph->getOutNodes(tlp::node(v));
    } else if (mode == IGRAPH_IN) {
      it = _graph->getInNodes(tlp::node(v));
    }
  } else {
    it = _graph->getInOutNodes(tlp::node(v));
  }

  tlp::node rand_neigh;
  while (it->hasNext()) {
    rand_neigh = it->next();
    if (rand_neigh_idx == 0)
      break;
    --rand_neigh_idx;
  }
  delete it;
  return rand_neigh.id;
}

/****************************************************************************
  Creates a graph with communities as node and links as weights between
  communities.

  The weight of the edges in the new graph is simply the sum of the weight
  of the edges between the communities. The self weight of a node (i.e. the
  weight of its self loop) is the internal weight of a community. The size
  of a node in the new graph is simply the size of the community in the old
  graph.
*****************************************************************************/
Graph *Graph::collapse_graph(MutableVertexPartition *partition) {
  size_t n_collapsed = partition->n_communities();
  vector<vector<size_t>> community_memberships = partition->get_communities();

  vector<double> collapsed_weights;
  double total_collapsed_weight = 0.0;

  vector<double> edge_weight_to_community(n_collapsed, 0.0);
  vector<bool> neighbour_comm_added(n_collapsed, false);

  // new graph
  tlp::VectorGraph *n_graph = new tlp::VectorGraph();
  // add nodes
  n_graph->addNodes(n_collapsed);

  // build new graph
  for (size_t v_comm = 0; v_comm < n_collapsed; v_comm++) {
    vector<size_t> neighbour_communities;
    for (size_t v : community_memberships[v_comm]) {
      for (size_t e : this->get_neighbour_edges(v, IGRAPH_OUT)) {
        size_t from, to;
        this->edge(e, from, to);

        if (from != v) {
          // need to skip because IGRAPH_OUT is ignored for undirected graphs
          continue;
        }

        size_t u_comm = partition->membership(to);

        double w = this->edge_weight(e);
        // Self loops appear twice here if the graph is undirected, so divide by 2.0 in that case.
        if (from == to && !this->is_directed())
          w /= 2.0;

        if (!neighbour_comm_added[u_comm]) {
          neighbour_comm_added[u_comm] = true;
          neighbour_communities.push_back(u_comm);
        }
        edge_weight_to_community[u_comm] += w;
      }
    }

    for (size_t u_comm : neighbour_communities) {
      // add edge to new graph
      n_graph->addEdge(tlp::node(v_comm), tlp::node(u_comm));
      collapsed_weights.push_back(edge_weight_to_community[u_comm]);
      total_collapsed_weight += edge_weight_to_community[u_comm];

      // reset edge_weight_to_community to all 0.0 and neighbour_comm_added to all false
      edge_weight_to_community[u_comm] = 0.0;
      neighbour_comm_added[u_comm] = false;
    }
  }

  // Calculate new node sizes
  vector<double> csizes(n_collapsed, 0);
  for (size_t c = 0; c < n_collapsed; c++)
    csizes[c] = partition->csize(c);

  Graph *G =
      new Graph(n_graph, collapsed_weights, csizes, this->_correct_self_loops, this->_is_directed);
  return G;
}
