/*
 * $Revision: 2299 $
 * 
 * last checkin:
 *   $Author: gutwenger $ 
 *   $Date: 2012-05-07 15:57:08 +0200 (Mon, 07 May 2012) $ 
 ***************************************************************/
 
/** \file
 * \brief Declaration of extended graph algorithms
 * 
 * \author Sebastian Leipert, Karsten Klein
 * 
 * \par License:
 * This file is part of the Open Graph Drawing Framework (OGDF).
 *
 * Copyright (C). All rights reserved.
 * See README.txt in the root directory of the OGDF installation for details.
 * 
 * \par
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * Version 2 or 3 as published by the Free Software Foundation;
 * see the file LICENSE.txt included in the packaging of this file
 * for details.
 * 
 * \par
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * \par
 * You should have received a copy of the GNU General Public 
 * License along with this program; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 * 
 * \see  http://www.gnu.org/copyleft/gpl.html
 ***************************************************************/

#ifdef _MSC_VER
#pragma once
#endif

#ifndef OGDF_EXTENDED_GRAPH_ALG_H
#define OGDF_EXTENDED_GRAPH_ALG_H


#include <ogdf/cluster/ClusterGraph.h>
#include <ogdf/basic/BinaryHeap2.h>
#include <limits>

namespace ogdf {


//---------------------------------------------------------
// Methods for induced subgraphs
//---------------------------------------------------------

//! Computes the induced subgraph of nodes.
template<class LISTITERATOR>
void inducedSubGraph(const Graph &G, LISTITERATOR start, Graph &subGraph)
{
	NodeArray<node> nodeTableOrig2New;
	inducedSubGraph(G,start,subGraph,nodeTableOrig2New);
}

//! Computes the induced subgraph of nodes.
template<class LISTITERATOR>
void inducedSubGraph(const Graph &G,
					 LISTITERATOR start,
					 Graph &subGraph,
					 NodeArray<node> &nodeTableOrig2New)
{
	subGraph.clear();
	nodeTableOrig2New.init(G,0);

	EdgeArray<bool> mark(G,false);

	LISTITERATOR its;
	for (its = start; its.valid(); its++)
	{
		node w = (*its);
		OGDF_ASSERT(w != 0 && w->graphOf() == &G);
		nodeTableOrig2New[w] = subGraph.newNode();

		adjEntry adj = w->firstAdj();
		forall_adj(adj,w)
		{
			edge e = adj->theEdge();
			if (nodeTableOrig2New[e->source()] && nodeTableOrig2New[e->target()] && !mark[e])
			{
				subGraph.newEdge(nodeTableOrig2New[e->source()],nodeTableOrig2New[e->target()]);
				mark[e] = true;
			}
		}
	}
}
					 

//! Computes the induced subgraph of nodes.
template<class LISTITERATOR>
void inducedSubGraph(const Graph &G,
					 LISTITERATOR start,
					 Graph &subGraph,
					 NodeArray<node> &nodeTableOrig2New,
					 EdgeArray<edge> &edgeTableOrig2New)
{
	subGraph.clear();
	nodeTableOrig2New.init(G,0);
	edgeTableOrig2New.init(G,0);

	EdgeArray<bool> mark(G,false);

	LISTITERATOR its;
	for (its = start; its.valid(); its++)
	{
		node w = (*its);
		OGDF_ASSERT(w != 0 && w->graphOf() == &G);
		nodeTableOrig2New[w] = subGraph.newNode();

		adjEntry adj = w->firstAdj();
		forall_adj(adj,w)
		{
			edge e = adj->theEdge();
			if (nodeTableOrig2New[e->source()] && 
				nodeTableOrig2New[e->target()] && 
				!mark[e])
			{
				edgeTableOrig2New[e] = 
					subGraph.newEdge(
						nodeTableOrig2New[e->source()],
						nodeTableOrig2New[e->target()]);
				mark[e] = true;
			}
		}
	}
}


template<class NODELISTITERATOR,class EDGELIST>
void inducedSubgraph(Graph &G,NODELISTITERATOR &it,EDGELIST &E)
{
	NODELISTITERATOR itBegin = it;
	NodeArray<bool>  mark(G,false);

	for (;it.valid();it++)
		mark[(*it)] = true;
	it = itBegin;
	for (;it.valid();it++)
	{
		node v = (*it);
		adjEntry adj;
		forall_adj(adj,v)
		{
			edge e = adj->theEdge();
			if (mark[e->source()] && mark[e->target()])
				E.pushBack(e);
		}
	}
}


//---------------------------------------------------------
// Methods for clustered graphs
//---------------------------------------------------------


// true <=> C is C-connected
OGDF_EXPORT bool isCConnected(const ClusterGraph &C);

//connect clusters to achieve c-connectivity (probably loosing planarity)
//GG is underlying graph of C, returns inserted edges in addededges
//simple: only connect underlying cluster subgraph without crossing check
OGDF_EXPORT void makeCConnected(
	ClusterGraph& C,
	Graph& GG,
	List<edge>& addedEdges,
	bool simple = true);




//---------------------------------------------------------
// Methods for st-numbering
//---------------------------------------------------------


// Computes an st-Numbering.
// Precondition: G must be biconnected and simple.
// Exception: the Graph is allowed to have isolated nodes.
// The st-numbers are stored in NodeArray. Return value is 
// the number t. It is 0, if the computation was unsuccessful.
// The nodes s and t may be specified. In this case 
// s and t have to be adjacent.
// If s and t are set 0 and parameter randomized is set to true,
// the st edge is chosen to be a random edge in G.
OGDF_EXPORT int stNumber(const Graph &G,
	NodeArray<int> &numbering,
	node s = 0,
	node t = 0,
	bool randomized = false);

// Tests, whether a numbering is an st-numbering
// Precondition: G must be biconnected and simple.
// Exception: the Graph is allowed to have isolated nodes.
OGDF_EXPORT bool testSTnumber(const Graph &G, NodeArray<int> &st_no,int max);


//---------------------------------------------------------
// Methods for minimum spanning tree computation
//---------------------------------------------------------
//! Computes a minimum spanning tree using Prim's algorithm
/** Computes a minimum spanning tree MST of graph \a G with respect
 *  to edge weights in \a weight using Prim's algorithm.
 *  If an edge is in MST, the corresponding value in \a isInTree
 *  is set to true, otherwise to false.
 *  Returns the sum of the edge weights in the computed tree.
 * */ 
template<typename T>
T computeMinST(const Graph &G, const EdgeArray<T> &weight, EdgeArray<bool> &isInTree){
	NodeArray<edge> pred(G, 0);
	return computeMinST(G.firstNode(), G, weight, pred, isInTree);
}

template<typename T>
T computeMinST(const Graph &G, const EdgeArray<T> &weight, NodeArray<edge> &pred, EdgeArray<bool> &isInTree){
	return computeMinST(G.firstNode(), G, weight, pred, isInTree);
}

template<typename T>
T computeMinST(node s, const Graph &G, const EdgeArray<T> &weight, NodeArray<edge> &pred, EdgeArray<bool> &isInTree)
{
	//our priority queue storing the front vertices
	BinaryHeap2<T, node> pq;

	//initialize tree flag for edges
	edge e;
	forall_edges(e, G)
	{
		isInTree[e] = false;
	}

	//array to save the position information from pq
	int* pqpos = new int[G.numberOfNodes()];

	int i = 0;
	NodeArray<int> vIndex(G);
	//array that tells us if node is already processed
	NodeArray<bool> processed(G);
	//predecessor of node v is given by an edge (w,v)

	//insert nodes into pr
	node v = G.firstNode();
	T prio = std::numeric_limits<T>::max();;
	while (v != 0)
	{
		vIndex[v] = i;
		pq.insert(v, prio, &(pqpos[i++]));
		processed[v] = false;
		pred[v] = 0;
		v = v->succ();
	}//while all nodes
	// decrease start node
	pq.decreaseKey(pqpos[vIndex[s]], 0.0);

	//extract the nodes again along a minimum ST
	while (!pq.empty())
	{
		v = pq.extractMin();
		processed[v] = true;
		forall_adj_edges(e, v)
		{
			node w = e->opposite(v);

			int posofw = pqpos[vIndex[w]];
			if ((!processed[w]) && (weight[e] < pq.getPriority(posofw)))
			{
				pq.decreaseKey(posofw, weight[e]);
				pred[w] = e;
			}//if improvement
		}
	}//while pq

	//only for connected graphs
	int rootcount = 0;
	T treeWeight = 0.0;
	forall_nodes(v, G)
	{
		if (pred[v] == 0) rootcount++;
		else
		{
			isInTree[pred[v]] = true;
			treeWeight += weight[pred[v]];
		}
	}
	OGDF_ASSERT(rootcount == 1);

	delete[] pqpos;
	return treeWeight;
}//computeMinST

} // end namespace ogdf


#endif
