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
///@cond DOXYGEN_HIDDEN

#ifndef TULIP_GRAPHITERATORS_H
#define TULIP_GRAPHITERATORS_H
#include <tulip/BasicIterators.h>
#include <tulip/tulipconf.h>

namespace tlp {

class Graph;
class GraphImpl;
class GraphView;

//============================================================
/// Out node iterator for GraphView
class TLP_SCOPE OutNodesIterator : public FactorNodeIterator, public MemoryPool<OutNodesIterator> {
private:
  Iterator<edge> *it;
#if !defined(NDEBUG) && !defined(_OPENMP)
  const GraphView *sg;
#endif
public:
  OutNodesIterator(const GraphView *sG, node n);
  ~OutNodesIterator() override;
  node next() override;
  bool hasNext() override;
};
//============================================================
/// In node iterator for GraphView
class InNodesIterator : public FactorNodeIterator, public MemoryPool<InNodesIterator> {
private:
  Iterator<edge> *it;
#if !defined(NDEBUG) && !defined(_OPENMP)
  const GraphView *sg;
#endif
public:
  InNodesIterator(const GraphView *sG, node n);
  ~InNodesIterator() override;
  node next() override;
  bool hasNext() override;
};
//============================================================
/// In Out node iterator for GraphView
class TLP_SCOPE InOutNodesIterator : public FactorNodeIterator,
                                     public MemoryPool<InOutNodesIterator> {
private:
  Iterator<edge> *it;
  node n;
#if !defined(NDEBUG) && !defined(_OPENMP)
  const GraphView *sg;
#endif
public:
  InOutNodesIterator(const GraphView *sG, node n);
  ~InOutNodesIterator() override;
  node next() override;
  bool hasNext() override;
};
//============================================================
/// Out edge iterator for GraphView
class TLP_SCOPE OutEdgesIterator : public FactorEdgeIterator, public MemoryPool<OutEdgesIterator> {
private:
  Iterator<edge> *it;
  edge curEdge;
  const GraphView *sg;

public:
  OutEdgesIterator(const GraphView *sG, node n);
  ~OutEdgesIterator() override;
  edge next() override;
  bool hasNext() override;

protected:
  void prepareNext();
};
//============================================================
/// In edge iterator for GraphView
class TLP_SCOPE InEdgesIterator : public FactorEdgeIterator, public MemoryPool<InEdgesIterator> {
private:
  Iterator<edge> *it;
  edge curEdge;
  const GraphView *sg;

public:
  InEdgesIterator(const GraphView *sG, node n);
  ~InEdgesIterator() override;
  edge next() override;
  bool hasNext() override;

protected:
  void prepareNext();
};
//============================================================
/// In Out edge iterator for GraphView
class TLP_SCOPE InOutEdgesIterator : public FactorEdgeIterator,
                                     public MemoryPool<InOutEdgesIterator> {
private:
  Iterator<edge> *it;
  edge curEdge;
  const GraphView *sg;

public:
  InOutEdgesIterator(const GraphView *sG, node n);
  ~InOutEdgesIterator() override;
  edge next() override;
  bool hasNext() override;

protected:
  void prepareNext();
};

//============================================================
// Iterator for the Graph
//============================================================
class TLP_SCOPE GraphNodeIterator : public NodeIterator
#if defined(NDEBUG) || defined(_OPENMP)
    ,
                                    public MemoryPool<GraphNodeIterator>
#endif
{
private:
#if !defined(NDEBUG) && !defined(_OPENMP)
  const Graph *graph;
  void *ito;
#endif
  Iterator<node> *itId;

public:
  GraphNodeIterator(const Graph *g, Iterator<node> *it);
  ~GraphNodeIterator() override;
  node next() override;
  bool hasNext() override;
};
//=============================================================
/// Edge iterator for data sg
class TLP_SCOPE GraphEdgeIterator : public EdgeIterator
#if defined(NDEBUG) || defined(_OPENMP)
    ,
                                    public MemoryPool<GraphEdgeIterator>
#endif
{
private:
#if !defined(NDEBUG) && !defined(_OPENMP)
  const Graph *graph;
  void *ito;
#endif
  Iterator<edge> *itId;

public:
  GraphEdgeIterator(const Graph *g, Iterator<edge> *it);
  ~GraphEdgeIterator() override;
  edge next() override;
  bool hasNext() override;
};
//============================================================
} // namespace tlp
#endif

///@endcond
