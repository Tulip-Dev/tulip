/** \file
 * \brief Implements class BiconnectedShellingOrder which computes
 * a shelling order for a biconnected planar graph.
 *
 * \author Carsten Gutwenger
 *
 * \par License:
 * This file is part of the Open Graph Drawing Framework (OGDF).
 *
 * \par
 * Copyright (C)<br>
 * See README.md in the OGDF root directory for details.
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
 * License along with this program; if not, see
 * http://www.gnu.org/copyleft/gpl.html
 */


#include <ogdf/planarlayout/BiconnectedShellingOrder.h>
#include <ogdf/basic/CombinatorialEmbedding.h>
#include <ogdf/basic/FaceArray.h>
#include <ogdf/basic/SList.h>

#include <ogdf/basic/extended_graph_alg.h>
#include <ogdf/basic/simple_graph_alg.h>


//#define OUTPUT_BSO

namespace ogdf {

// pair of node v and list itrator it
struct PairFaceItem;

struct PairNodeItem
{
	// constructor
	PairNodeItem() { }

	explicit PairNodeItem(node v, ListIterator<PairFaceItem> it = ListIterator<PairFaceItem>()) : m_v(v), m_it(it) { }

	node m_v;
	ListIterator<PairFaceItem> m_it;
};

// pair of face f and list iterator it
struct PairFaceItem
{
	// constructor
	PairFaceItem() : m_f(nullptr), m_it(nullptr) { }

	explicit PairFaceItem(face f) : m_f(f), m_it(nullptr) { }

	PairFaceItem(face f, ListIterator<PairNodeItem> it) : m_f(f), m_it(it) { }

	face m_f;
	ListIterator<PairNodeItem> m_it;
};


// defines (as shortcuts)
// initialization of a variable
#define INIT_VAR(x,var,val) \
	var[x] = (val);          \
	setUpdate(x);

// decrement of a variable
#define DEC_VAR(x,var)     \
	--var[x];               \
	setUpdate(x);

// increment of a variable
#define INC_VAR(x,var)     \
	++var[x];               \
	setUpdate(x);


class ComputeBicOrder
{
public:
	// types of structures to be removed
	enum class CandidateType { Face, Node, Edge };


	// constructor
	ComputeBicOrder (
		const Graph &G,                 // biconnected planar graph
		ConstCombinatorialEmbedding &E, // combinatorial embedding of G
		face extFace,              // external face
		double baseRatio);         // size of base (baseRatio * size(extFace)

	// returns external face
	face externalFace() { return m_extFace; }

	// returns face left of adj
	face left(adjEntry adj) { return m_pEmbedding->leftFace(adj); }

	// returns face right of adj
	face right(adjEntry adj) { return m_pEmbedding->rightFace(adj); }

	// if v = c_i, returns face right of c_i -> c_i+1
	face right(node v) { return left(m_nextSucc[v]); }

	// if v = c_i, returns face left of c_i -> c_i-1
	face left(node v) { return right(m_prevPred[v]); }

	// returns number of virtual edges adjacent to v
	int virte (node v);

	// returns successor of v on contour (if v=c_i returns c_i+1)
	node next(node v) { return m_next[v]; }

	// returns predecessor of v on contour (if v=c_i returns c_ii1)
	node prev(node v) { return m_prev[v]; }

	// returns true <=> f contains a virtual edge
	bool cutv(face f) { return m_virtSrc[f] != nullptr; }

	// returns true <=> f is a possible next face, i.e
	//   outv(f) >= 3 and outv(f) = oute(f)+1
	bool isPossFace(face f) {
		return f != externalFace()
		    && m_outv[f] >= 3
		    && m_outv[f] == m_oute[f] + 1;
	}

	// returns true <=> v is a possible next node, i.e.
	//   cutf(v) <= 1, cutf(v) = virte(v), numsf(v) = 0 and deg(v) >= 3
	bool isPossNode(node v) {  // precond.: v on C_k'
		return !m_onBase[v]
		    && m_cutf[v] <= 1
		    && m_cutf[v] == virte(v)
		    && m_numsf[v] == 0
		    && m_deg[v] >= 3;
	}

	// returns true <=> v=c_i and c_i -> c_i+1 is a possible next virtual edge, i.e.
	//   c_i -> c_i+1 is a virtual edge and (deg(c_i) = 2 and c_i != vLeft) or
	//   deg(c_i+1) = 2 and c_i+1 != vRight)
	bool isPossVirt(node v) {  // precond.: v on C_k'
		return m_virtEdge[v] && ((m_deg[v] == 2 && v != m_vLeft) ||
			(m_deg[next(v)] == 2 && next(v) != m_vRight));
	}

	// stores the next candidate in m_nextType and m_nextF, m_nextV or
	// m_nextE (depending on type)
	// returns true <=> there is a next candidate
	bool getPossible();

	// returns the type of the next candidate
	CandidateType nextPoss() { return m_nextType; }

	// puts nodes on base into shelling order set V
	void setV1(ShellingOrderSet &V);

	// initializes possible nodes and faces
	void initPossibles();

	// removes next possible face
	void removeNextFace(ShellingOrderSet &V);
	// removes next possible node
	void removeNextNode(ShellingOrderSet &V);
	// removes next possible virtual edge
	void removeNextVirt(ShellingOrderSet &V);

	// updates variables of face and nodes contained in m_updateFaces and
	// m_updateNodes; also updates list of possible nodes, faces and virtual edges
	void doUpdate();

	// outputs contour and some node and face variables
	// for debugging only
	void print();

private:
	// if adj = w->v, puts edge (v,w) onto contour
	void edgeToContour(adjEntry adj);
	// puts virtual edge (v,w) onto contour and sets m_nextSucc[v] and m_prevPred[w]
	void virtToContour(node v, node w, adjEntry adjNextSucc, adjEntry adjPrevPred);
	// puts virtual edge (v,w) onto contour
	void virtToContour(node v, node w);

	// returns vertex cl of a face to be removed
	node getFaceCl(face f);

	void setOutv(node v);
	void setSeqp(node cl, node cr);
	void delOuterNode(node v);
	void decSeqp(node v);
	void putOnOuter(node v, face f);
	void delOuterRef(face f);

	// returns faces adjacent with v in list L
	void getAdjFaces(node v, SListPure<face> &L);
	// returns nodes adjacent with v in list L; nodes are sorted from left
	// to right
	void getAdjNodes(node v, SListPure<node> &L);

	// marks face f to be updated
	void setUpdate(face f);
	// marks node v to be updated
	void setUpdate(node v);

	void initVInFStruct(const ConstCombinatorialEmbedding &E);
	bool vInF(node v, face f);
	void delVInF(node v, face f);

	int getBaseChain(ConstCombinatorialEmbedding &E,
		face f,
		double baseRatio,
		adjEntry &adjLeft,
		adjEntry &adjRight);

	adjEntry findMaxBaseChain(ConstCombinatorialEmbedding &E,
		face f,
		int &length);

	const Graph                 *m_pGraph;     // the graph
	ConstCombinatorialEmbedding *m_pEmbedding; // the embedding of the graph

	face m_extFace; // the external face

	adjEntry m_adjLeft; // adjacency entry z_1 -> z_2 (if z_1,...,z_p is base)
	adjEntry m_adjRight; // adjacency entry z_p-1 -> z_p

	node m_vLeft, m_vRight; // left and right node on base
	int m_baseLength;       // length of base

	// next candidate to be removed
	CandidateType m_nextType; // type of next candidate
	face m_nextF; // next face (if m_nextType = typeFace)
	node m_nextV; // next node (if m_nextType = typeNode)
	node m_nextE; // next virtual edge (if m_nextType = typeEdge)

	// variables of nodes
	NodeArray<int>  m_deg;      // current degree
	NodeArray<int>  m_cutf;     // number of adjacent faces f with cutv(f) = true
	NodeArray<int>  m_numsf;    // number of adjacent faces f with outv(f) > seqp(f)+1
	NodeArray<bool> m_onOuter;  // true <=> v on contour
	NodeArray<bool> m_onBase;   // true <=> v on base

	// list iterator in m_possNodes containing node (or nil if not contained)
	NodeArray<ListIterator<node> > m_vLink;
	// list iterator in m_possVirt containing virtual edge at node (or nil if not contained)
	NodeArray<ListIterator<node> > m_virtLink;
	NodeArray<bool>	m_vUpdate;  // true <=> node marked to be updated
	NodeArray<ListPure<PairFaceItem> > m_inOutNodes;

	// variables of faces
	FaceArray<int>	m_outv,		// number of nodes contained in face on contour
					m_oute,		// number of edges contained in face on contour
					m_seqp;		// number of sequential pairs c_i,c_i+1 of face
	FaceArray<node>	m_virtSrc;	// if face contains virtual edge e then source(e), otherwise nil
	// list iterator in m_possFaces containing face (or nil if not contained)
	FaceArray<ListIterator<face> >	m_fLink;
	FaceArray<bool>	m_fUpdate;  // true <=> face marked to be updated
	FaceArray<bool> m_isSf;		// true <=> outv(f) > seqp(f)+1
	FaceArray<ListPure<PairNodeItem> > m_outerNodes;	// list of all nodes of face on contour

	// represantation of the contour
	NodeArray<node>	    m_next, m_prev;
	NodeArray<adjEntry>	m_nextSucc, m_prevPred;
	NodeArray<bool>	    m_virtEdge;	// virt_edge[c_i] = true <=> (c_i,c_i+1) virtuelle Kante

	// lists of possible faces, nodes and edges
	ListPure<face> m_possFaces; // faces with outv(f) >= 3 und outv(f) = oute(f)+1
	ListPure<node> m_possNodes; // nodes with cutf(v) <= 1, cutf(v) = virte(v), numsf(v) = 0 and deg(v) >= 3
	ListPure<node> m_possVirt;  // node v denotes virtual edge e = (v,next[v]), that satisfies (deg(v) = 2 and v != vLeft) or (deg(next(v)) = 2 and next(v) != vRight)

	ListPure <node> m_updateNodes; // list of nodes to be updated
	SListPure<face> m_updateFaces; // list of faces to be updated

	// deciding "v in F ?" in constant time
	NodeArray<List<PairFaceItem> > m_facesOf;
	FaceArray<List<PairNodeItem> > m_nodesOf;
};


void ComputeBicOrder::print()
{
	std::cout << "contour:\n";
	for(node v = m_vLeft; v != nullptr; v = m_next[v])
		std::cout << " " << v << "[" << m_prev[v] << "," << m_prevPred[v] <<
			" : " << m_next[v] << "," << m_nextSucc[v] <<
			"; " << m_virtEdge[v] << "]\n";

	std::cout << "node infos:\n";
	for(node v : m_pGraph->nodes)
		std::cout << v << ": deg = " << m_deg[v] << ", cutf = " << m_cutf[v] <<
			", numsf = " << m_numsf[v] << std::endl;

	std::cout << "face infos:\n";
	for(face f : m_pEmbedding->faces) {
		std::cout << f->index() << ": outv = " << m_outv[f] << ", oute = " <<
			m_oute[f] << ", seqp = " << m_seqp[f] << ", isSF = " <<
			m_isSf[f] << ", virtSrc = " << m_virtSrc[f] << std::endl;
	}
	std::cout << std::endl;
}


ComputeBicOrder::ComputeBicOrder(const Graph &G, // the graph
	ConstCombinatorialEmbedding &E,        // embedding of the graph
	face extFace,                          // the external face
	double baseRatio)                      // length of the base = baseRatio*size(extface)
{
	m_pGraph = &G;
	m_pEmbedding = &E;

#ifdef OUTPUT_BSO
	std::cout << "faces:" << std::endl;
	for(face fh : E.faces) {
		std::cout << fh->index() << ":";
		for(adjEntry adj : fh->entries)
			std::cout << " " << adj;
		std::cout << std::endl;
	}

	std::cout << "adjacency lists:" << std::endl;
	for(node vh : G.nodes) {
		std::cout << vh << ":";
		for(adjEntry adj : vh->adjEntries)
			std::cout << " " << adj;
		std::cout << std::endl;
	}
#endif

	m_vLink   .init(G, ListIterator<node>());
	m_virtLink.init(G, ListIterator<node>());

	m_extFace = extFace;

#ifdef OUTPUT_BSO
	std::cout << "external face = " << extFace->index() << std::endl;
#endif

	m_baseLength = getBaseChain(E, m_extFace, baseRatio, m_adjLeft, m_adjRight);
	m_vLeft      = m_adjLeft->theNode();
	m_vRight     = m_adjRight->twinNode();

#ifdef OUTPUT_BSO
	std::cout << "vLeft = " << m_vLeft << ", " << "vRight = " << m_vRight << std::endl;
#endif

	// initialization of node and face variables
	m_deg      	 .init (G);
	m_cutf     	 .init (G, 0);
	m_numsf    	 .init (G, 0);
	m_onOuter 	 .init (G, false);
	m_next       .init (G);
	m_prev       .init (G);
	m_nextSucc   .init (G);
	m_prevPred   .init (G);
	m_virtEdge	 .init (G, false);
	m_vUpdate    .init (G, false);
	m_inOutNodes .init (G);
	m_outv       .init (E, 0);
	m_oute       .init (E, 0);
	m_seqp       .init (E, 0);
	m_virtSrc    .init (E, nullptr);
	m_fLink      .init (E, ListIterator<face>());
	m_fUpdate    .init (E, false);
	m_isSf       .init (E, false);
	m_outerNodes .init (E);
	m_onBase     .init (G, false);

	initVInFStruct(E);

	// initialization of degree
	for(node v : G.nodes)
		m_deg[v] = v->degree();

	// initialization of m_onBase[v]
	adjEntry adj;
	for(adj = m_adjRight; adj != m_adjLeft; adj = adj->faceCyclePred())
		m_onBase[adj->theNode()] = true;
	m_onBase [m_vLeft] = m_onBase [m_vRight] = true;

	adj = m_adjLeft;
	do {
		node v = adj->theNode();
		for(adjEntry adj2 : v->adjEntries)
		{
			face f = E.rightFace(adj2);
			if (f != m_extFace) {
				m_outv[f] ++;
				putOnOuter(v,f);
			}
		}
		adj = adj->faceCyclePred();
	} while(adj != m_adjRight);

	for(adj = m_adjRight->faceCycleSucc(); adj != m_adjLeft; adj = adj->faceCycleSucc())
		m_oute[E.leftFace(adj)]++;

	m_onOuter [m_vLeft] = true;
	m_prevPred[m_vLeft] = m_nextSucc[m_vRight] = nullptr;
	m_prev[m_vLeft] = m_next[m_vRight] = nullptr;
	for (adj = m_adjLeft->faceCyclePred(); adj != m_adjRight; adj = adj->faceCyclePred())
	{
		node v = adj->twinNode();
		node w = adj->theNode();
		m_onOuter[w] = true;
		edgeToContour(adj);

		for(adjEntry adj2 : w->adjEntries)
		{
			face f = left(adj2);
			if (vInF(v,f))
				++m_seqp[f];
		}
	}

	for (node v = m_vLeft; v != nullptr; v = next(v))
	{
		for(adjEntry adjV : v->adjEntries) {
			face f = left(adjV);
			if ((m_isSf[f] = (m_outv[f] > m_seqp[f]+1)))
				++m_numsf[v];
		}
	}
}


void ComputeBicOrder::setV1(ShellingOrderSet &V)
{
	V = ShellingOrderSet(m_baseLength, nullptr, nullptr);

	int i;
	adjEntry adj;
	for (i = 1, adj = m_adjLeft; i <= m_baseLength;
		i++, adj = adj->faceCycleSucc())
	{
		V[i] = adj->theNode();
	}
}


void ComputeBicOrder::edgeToContour(adjEntry adj)
{
	node v = adj->twinNode(), w = adj->theNode();

	m_next     [v] = w;
	m_prev     [w] = v;
	m_nextSucc [v] = adj->twin()->cyclicSucc();
	m_prevPred [w] = adj->cyclicPred();
	m_virtEdge [v] = false;
}


void ComputeBicOrder::virtToContour(
	node v,
	node w,
	adjEntry adjNextSucc,
	adjEntry adjPrevPred)
{
	m_next     [v] = w;
	m_prev     [w] = v;
	m_nextSucc [v] = adjNextSucc;
	m_prevPred [w] = adjPrevPred;
	m_virtEdge [v] = true;
}


void ComputeBicOrder::virtToContour(node v, node w)
{
	m_next     [v] = w;
	m_prev     [w] = v;
	m_virtEdge [v] = true;
}


void ComputeBicOrder::putOnOuter(node v, face f)
{
	ListIterator<PairNodeItem> it;

	it = m_outerNodes[f].pushBack(PairNodeItem(v));
	(*it).m_it = m_inOutNodes[v].pushBack(PairFaceItem(f,it));
}


void ComputeBicOrder::delOuterRef(face f)
{
	ListPure<PairNodeItem> &L = m_outerNodes[f];
	PairNodeItem x;

	while (!L.empty()) {
		x = L.popFrontRet();
		m_inOutNodes[x.m_v].del(x.m_it);
	}
}


int ComputeBicOrder::virte(node v)
{
	int num = 0;

	if (m_onOuter[v])
	{
		if (m_virtEdge[v])
			num++;
		if (v != m_vLeft && m_virtEdge[prev(v)])
			num++;
	}
	return num;
}


void ComputeBicOrder::initVInFStruct(const ConstCombinatorialEmbedding &E)
{
	const Graph &G = E;

	m_facesOf.init(G);
	m_nodesOf.init(E);

	for (face f : E.faces)
	{
		for (adjEntry adj : f->entries) {
			node v = adj->theNode();

			ListIterator<PairFaceItem> it = m_facesOf[v].pushBack(PairFaceItem(f));
			(*it).m_it = m_nodesOf[f].pushBack(PairNodeItem(v, it));
		}
	}

	SListPure<node> smallV;
	for (node v : G.nodes) {
		if (m_facesOf[v].size() <= 5)
			smallV.pushBack(v);
	}

	SListPure<face> smallF;
	for (face f : E.faces) {
		if (m_nodesOf[f].size() <= 5)
			smallF.pushBack(f);
	}

	for (;;)
	{
		if (!smallV.empty()) {
			node v = smallV.popFrontRet();

			for (const PairFaceItem &f_it : m_facesOf[v]) {
				m_nodesOf[f_it.m_f].del(f_it.m_it);
				if (m_nodesOf[f_it.m_f].size() == 5)
					smallF.pushBack(f_it.m_f);
			}
		}
		else if (!smallF.empty()) {
			face f = smallF.popFrontRet();
			for (const PairNodeItem &v_it : m_nodesOf[f]) {
				m_facesOf[v_it.m_v].del(v_it.m_it);
				if (m_facesOf[v_it.m_v].size() == 5)
					smallV.pushBack(v_it.m_v);
			}
		}
		else
			break;
	}
}


bool ComputeBicOrder::vInF(node v, face f)
{
	for(const PairNodeItem &ni : m_nodesOf[f])
		if (ni.m_v == v) return true;

	for(const PairFaceItem &fi : m_facesOf[v])
		if (fi.m_f == f) return true;

	return false;
}


void ComputeBicOrder::delVInF(node v, face f)
{
	List<PairNodeItem> &L_f = m_nodesOf[f];
	List<PairFaceItem> &L_v = m_facesOf[v];

	ListIterator<PairNodeItem> itNI;
	for(itNI = L_f.begin(); itNI.valid(); ++itNI) {
		if ((*itNI).m_v == v) {
			L_f.del(itNI);
			return;
		}
	}

	ListIterator<PairFaceItem> itFI;
	for(itFI = L_v.begin(); itFI.valid(); ++itFI) {
		if ((*itFI).m_f == f) {
			L_v.del(itFI);
			return;
		}
	}
}


void ComputeBicOrder::initPossibles()
{
	for(face f : m_pEmbedding->faces) {
		if (isPossFace(f))
			m_fLink[f] = m_possFaces.pushBack(f);
	}

	for (node v = next(m_vLeft); v != m_vRight; v = next(v))
		if (isPossNode(v))
			m_vLink[v] = m_possNodes.pushBack(v);
}


bool ComputeBicOrder::getPossible()
{
	if (!m_possFaces.empty()) {
		m_nextType = CandidateType::Face;
		m_nextF = m_possFaces.popFrontRet();
		m_fLink[m_nextF] = ListIterator<face>();
		return true;

	} else if (!m_possNodes.empty()) {
		m_nextType = CandidateType::Node;
		m_nextV = m_possNodes.popFrontRet();
		m_vLink[m_nextV] = ListIterator<node>();
		return true;

	} else if (!m_possVirt.empty()) {
		m_nextType = CandidateType::Edge;
		m_nextE = m_possVirt.popFrontRet();
		m_virtLink[m_nextE] = ListIterator<node>();
		return true;

	} else
		return false;
}


node ComputeBicOrder::getFaceCl(face f)
{
	node v = nullptr;

	if (cutv(f)) {
		v = m_virtSrc [f];

	} else {
		for(adjEntry adj : f->entries) {
			if (m_onOuter[v = adj->theNode()] && m_deg[v] == 2)
				break;
		}
	}

	while (v != m_vLeft && m_deg[v] == 2)
		v = prev(v);

	return v;
}


void ComputeBicOrder::getAdjFaces(node v, SListPure<face> &L)
{
	L.clear();
	if (m_deg[v] <= 1) return;

	adjEntry adjEnd   = (v != m_vLeft)  ? m_prevPred[v] : m_adjLeft->cyclicPred();
	adjEntry adjStart = (v != m_vRight) ? m_nextSucc[v] : m_adjRight->twin()->cyclicSucc();

	if (left(adjStart) != m_extFace)
		L.pushBack(left(adjStart));

	if (m_deg[v] >= 3) {
		adjEntry adj;
		for (adj = adjStart; adj != adjEnd; adj = adj->cyclicSucc())
			L.pushBack(right(adj));

		L.pushBack(right(adjEnd));
	}
}


void ComputeBicOrder::getAdjNodes(node v, SListPure<node> &L)
{
	adjEntry adjEnd   = (v != m_vLeft)  ? m_prevPred[v] : m_adjLeft->cyclicPred();
	adjEntry adjStart = (v != m_vRight) ? m_nextSucc[v] : m_adjRight->twin()->cyclicSucc();

	L.clear();
	L.pushBack((v != m_vLeft) ? prev(v) : m_adjLeft->twinNode());

	if (m_deg[v] >= 3) {
		adjEntry adj;
		for (adj = adjEnd; adj != adjStart; adj = adj->cyclicPred())
			L.pushBack(adj->twinNode());
		L.pushBack(adjStart->twinNode());
	}
	L.pushBack((v != m_vRight) ? next(v) : m_adjRight->theNode());
}


void ComputeBicOrder::decSeqp(node v)
{
	node vNext = next(v);
	node vPrev = prev(v);

	SListPure<face> L;
	getAdjFaces(v, L);

	for (face f : L) {
		if (vInF(vNext, f))
			m_seqp[f]--;
		if (vInF(vPrev, f))
			m_seqp[f]--;
	}
}


void ComputeBicOrder::delOuterNode(node v)
{
	for(const PairFaceItem &fi : m_inOutNodes[v])
		m_outerNodes[fi.m_f].del(fi.m_it);
}


void ComputeBicOrder::setOutv(node v)
{
	SListPure<face> L;
	getAdjFaces(v, L);

	for (face f : L) {
		INC_VAR(f, m_outv)
			putOnOuter(v, f);
		if (cutv(f)) {
			INC_VAR(v, m_cutf)
		}
		if (m_isSf[f]) {
			INC_VAR(v, m_numsf)
		}
	}
}


void ComputeBicOrder::setSeqp(node cl, node cr)
{
	SListPure<face> L;

	node w;
	for (node v = cl; v != cr; v = w)
	{
		w = next(v);

		node wSmall, wBig;
		if (m_deg[v] < m_deg[w]) {
			wSmall = v;
			wBig   = w;
		} else {
			wSmall = w;
			wBig   = v;
		}

		getAdjFaces(wSmall, L);

		for(face f : L) {
			if (vInF(wBig,f)) {
				INC_VAR (f,m_seqp)
			}
		}
	}
}


void ComputeBicOrder::removeNextFace(ShellingOrderSet &V)
{
#ifdef OUTPUT_BSO
	std::cout << "remove next face: " << m_nextF->index() << std::endl;
#endif

	node cl = getFaceCl(m_nextF), cr, v;

	V = ShellingOrderSet(m_outv[m_nextF]-2);
	V.left(cl);

	int i;
	for (i = 1, cr = next(cl); cr != m_vRight && m_deg[cr] == 2; i++, cr = next(cr))
		V [i] = cr ;
	V.right (cr);
	V.leftAdj (m_virtEdge[cl]       ? nullptr : m_nextSucc[cl]->cyclicSucc()->twin());
	V.rightAdj(m_virtEdge[prev(cr)] ? nullptr : m_prevPred[cr]->cyclicPred()->twin());

	if (cutv(m_nextF) && next(m_virtSrc[m_nextF]) == cr)
		setUpdate(cr);

	if (cutv(m_nextF)) {
		DEC_VAR(cl,m_cutf)
		DEC_VAR(cr,m_cutf)
		v = m_virtSrc[m_nextF];
		if (v != cr) {
			m_possVirt.del(m_virtLink[v]);
			m_virtLink[v] = ListIterator<node>();
		}
	}

	adjEntry adj = m_nextSucc[cl]->twin();
	for( ; ; ) {
		edgeToContour(adj);

		if (adj->theNode() == cr)
			break;
		else {
			INIT_VAR(adj->theNode(),m_onOuter,true)
		}

		adj = adj->faceCyclePred();
	}
	DEC_VAR (cl,m_deg)
	DEC_VAR (cr,m_deg)

	for (v = cl; v != cr; v = next(v)) {
		INC_VAR(right(v),m_oute)
		if (v != cl)
			setOutv(v);
	}

	setSeqp(cl, cr);

	// possibly remove virtual edge
	if (cutv(m_nextF)) {
		if (m_virtSrc[m_nextF] == cl) {
			setUpdate(cl);
			m_virtEdge[cl] = false;
		}
		m_virtSrc[m_nextF] = nullptr;
	}
	delOuterRef(m_nextF);
}


void ComputeBicOrder::removeNextNode(ShellingOrderSet &V)
{
#ifdef OUTPUT_BSO
	std::cout << "remove next node: " << m_nextV << std::endl;
#endif

	node cl = prev(m_nextV);
	node cr = next(m_nextV);

	V = ShellingOrderSet(1);
	V[1] = m_nextV;

	if (m_virtEdge[prev(m_nextV)]) {
		V.left(m_prevPred[m_nextV]->twinNode());
		V.leftAdj(m_prevPred[m_nextV]);
	} else {
		V.left(prev(m_nextV));
		V.leftAdj(m_prevPred[m_nextV]->cyclicPred());
	}

	if (m_virtEdge[m_nextV]) {
		V.right(m_nextSucc[m_nextV]->twinNode());
		V.rightAdj(m_nextSucc[m_nextV]);
	} else {
		V.right(next(m_nextV));
		V.rightAdj(m_nextSucc[m_nextV]->cyclicSucc());
	}

	node vVirt = nullptr;
	face fVirt = nullptr;
	if (m_virtEdge[prev(m_nextV)]) {
		INIT_VAR(prev(m_nextV), m_virtEdge, false)
		vVirt = cl;
		fVirt = left(m_nextV);
		m_virtSrc [fVirt] = nullptr;
	}

	if (m_virtEdge[m_nextV]) {
		if (m_virtLink[m_nextV].valid()) {
			m_possVirt.del(m_virtLink[m_nextV]);
			m_virtLink[m_nextV] = ListIterator<node>();
		}
		vVirt = cr;
		fVirt = right(m_nextV);
		m_virtSrc[fVirt] = nullptr;
	}

	SListPure<face> L;
	getAdjFaces(m_nextV, L);
	for(face f : L)
		--m_outv[f];

	SListPure<node> L_v;
	getAdjNodes(m_nextV, L_v);

	delOuterNode(m_nextV);
	--m_oute[left (m_nextV)];
	--m_oute[right(m_nextV)];
	decSeqp(m_nextV);

	for(node w : L_v) {
		m_onOuter[w] = true;
		DEC_VAR (w, m_deg)
	}

	face potF = nullptr;
	node w1 = L_v.popFrontRet();
	bool firstTime = true;
	adjEntry adj,adj2;
	for(node w : L_v)
	{
		if (firstTime) {
			adj2 = m_nextSucc[prev(m_nextV)];
			adj = m_prevPred[m_nextV];
			firstTime = false;

			if (prev(m_nextV) != m_vLeft) {
				face f = left(adj2);
				if (vInF(prev(prev(m_nextV)),f))
					potF = f;
			}

		} else {
			adj2 = adj->twin()->faceCyclePred()->twin();
			adj  = adj->cyclicPred();
		}

		for( ; ; )
		{
			node v = adj2->twinNode();

			if (v != w && m_onOuter[v])
			{
				face f = left(adj2);

				// possibly remove "v in F" relation
				if (adj2->theNode() != w1)
				{
					adjEntry adj1 = adj2->twin()->faceCycleSucc();
					do {
						delVInF(adj1->twinNode(),f);
						adj1 = adj1->faceCycleSucc();
					} while (adj1->theNode() != w1);
				}
				if (f == potF && adj2->theNode() != prev(m_nextV)) {
					DEC_VAR(f,m_seqp)
				}

				// insert new virtual edge
				virtToContour(adj2->theNode(), w, adj2, (w == next(m_nextV)) ?
					m_prevPred[w] : adj->twin()->cyclicPred());

				setUpdate(f);

				INC_VAR(adj2->theNode(),m_deg)
				INC_VAR(w,m_deg)

				if (f != fVirt) {
					for(const PairNodeItem &ni : m_outerNodes[f]) {
						INC_VAR(ni.m_v, m_cutf);
					}
				}
				m_virtSrc[f] = adj2->theNode();

				break;
			}

			edgeToContour(adj2->twin());

			if (v == w) {
				delOuterRef(left(adj2));
				break;
			}
			INIT_VAR(v,m_onOuter,true)
			if (adj2->theNode() == cl)
			{
				ListIterator<PairNodeItem> it, itSucc;
				ListPure<PairNodeItem> &listPNI = m_outerNodes[left(adj2)];
				for(it = listPNI.begin(); it.valid(); it = itSucc) {
					itSucc = it.succ();
					if ((*it).m_v == cl) {
						m_inOutNodes[cl].del((*it).m_it);
						listPNI.del(it);
						break;
					}
				}
				m_outv[left(adj2)]--;
			}
			adj2 = adj2->twin()->faceCyclePred()->twin();
		}
		w1 = w;
	}

	for (node v = cl; v != cr; v = next(v)) {
		INC_VAR(right(v),m_oute)
		if (v != cl)
			setOutv(v);
	}

	setSeqp(cl,cr);

	if ((vVirt != nullptr && m_virtSrc[fVirt] == nullptr) ||
		(vVirt ==  cl && m_virtSrc[fVirt] != cl)) {
		DEC_VAR(vVirt,m_cutf)
	}
}


void ComputeBicOrder::removeNextVirt(ShellingOrderSet &V)
{
#ifdef OUTPUT_BSO
	std::cout << "remove next virt: " << m_nextE << std::endl;
#endif

	node v, cl = m_nextE, cr = next(m_nextE);
	int i = 0;

	while (m_deg[cl] == 2 && cl != m_vLeft)
		{ cl = prev(cl); i++; }
	while (m_deg[cr] == 2 && cr != m_vRight)
		{ cr = next(cr); i++; }

	V = ShellingOrderSet(i,m_virtEdge[cl] ? nullptr : m_prevPred[next(cl)],
		m_virtEdge[prev(cr)] ? nullptr : m_nextSucc[prev(cr)]);
	for (i = 1, v = next(cl); v != cr; v = next(v)) {
		V[i++] = v;
		delOuterNode(v);
	}
	V.left (cl);
	V.right(cr);

	face f = right(cl);
	m_virtSrc[f] = cl;

	virtToContour(cl, cr);

	INIT_VAR(f,m_outv,(m_outv[f] - V.len()))
	INIT_VAR(f,m_oute,(m_oute[f] - V.len()))
	INIT_VAR(f,m_seqp,(m_seqp[f] - V.len()-1))
	setSeqp(cl,cr);
	setUpdate(cl);
	setUpdate(cr);
}


void ComputeBicOrder::setUpdate(node v)
{
	if (!m_vUpdate[v]) {
		m_updateNodes.pushBack(v);
		m_vUpdate[v] = true;
	}
}


void ComputeBicOrder::setUpdate(face f)
{
	if (!m_fUpdate[f]) {
		m_updateFaces.pushBack(f);
		m_fUpdate[f] = true;
	}
}


void ComputeBicOrder::doUpdate()
{
	while (!m_updateFaces.empty())
	{
		face f = m_updateFaces.popFrontRet();
		m_fUpdate[f] = false;
		bool isSeperatingFace = (m_outv[f] > m_seqp[f]+1);
		if (isSeperatingFace != m_isSf[f])
		{
			for(const PairNodeItem &ni : m_outerNodes[f])
			{
				if (isSeperatingFace) {
					INC_VAR(ni.m_v,m_numsf)
				} else {
					DEC_VAR(ni.m_v,m_numsf)
				}
			}
			m_isSf[f] = isSeperatingFace;
		}
		bool possible = isPossFace(f);
		if (possible && !m_fLink[f].valid())
			m_fLink[f] = m_possFaces.pushBack(f);
		else if (!possible && m_fLink[f].valid()) {
			m_possFaces.del(m_fLink[f]);
			m_fLink[f] = ListIterator<face>();
		}
	}

	for (node v : reverse(m_updateNodes)) {
		if (v != m_vLeft && m_virtEdge[prev(v)])
			setUpdate(prev(v));
	}

	while (!m_updateNodes.empty())
	{
		node v = m_updateNodes.popFrontRet();
		m_vUpdate[v] = false;

		bool possible = isPossNode(v);
		if (possible && !m_vLink[v].valid())
			m_vLink[v] = m_possNodes.pushBack(v);
		else if (!possible && m_vLink[v].valid()) {
			m_possNodes.del(m_vLink[v]);
			m_vLink[v] = ListIterator<node>();
		}
		possible = isPossVirt(v);
		if (possible && !m_virtLink[v].valid())
			m_virtLink[v] = m_possVirt.pushBack(v);
		else if (!possible && m_virtLink[v].valid()) {
			m_possVirt.del(m_virtLink[v]);
			m_virtLink[v] = ListIterator<node>();
		}
	}
}


int ComputeBicOrder::getBaseChain(ConstCombinatorialEmbedding &E,
	face f,
	double baseRatio,
	adjEntry &adjLeft,
	adjEntry &adjRight)
{
	int len;
	adjLeft = findMaxBaseChain(E, f, len);
	len = max(2, min(len, (int)(baseRatio*f->size()+0.5)));

	adjRight = adjLeft;
	for (int i = 2; i < len; i++)
		adjRight = adjRight->faceCycleSucc();

	return len;
}


struct QType
{
	QType (adjEntry adj, int i) {
		m_start = adj;
		m_limit = i;
	}
	QType () {
		m_start = nullptr;
		m_limit = 0;
	}

	adjEntry m_start;
	int      m_limit;
};


adjEntry ComputeBicOrder::findMaxBaseChain(ConstCombinatorialEmbedding &E,
	face f,
	int &length)
{
	const Graph &G = (const Graph &) E;
	int p = f->size();

	NodeArray<int> num(G, -1);

	int i = 0;

	for (adjEntry adj : f->entries)
		num[adj->theNode()] = i++;

	Array<SListPure<int> > diag(0, p - 1);
	for (adjEntry adj : f->entries)
	{
		i = num[adj->theNode()];
		adjEntry adj2;
		for (adj2 = adj->cyclicPred(); adj2 != adj->cyclicSucc();
			adj2 = adj2->cyclicPred())
		{
			int j = num[adj2->twinNode()];
			if (j != -1)
				diag[i].pushBack(j);
		}
	}

	SListPure<QType> Q;
	Array<SListIterator<QType> > posInQ(0, p - 1, SListIterator<QType>());

	length = 0;
	bool firstRun = true;
	adjEntry adj = f->firstAdj();
	i = num[adj->theNode()];

	adjEntry adjStart = nullptr;
	do {
		if (posInQ[i].valid()) {
			adjEntry adj2 = Q.front().m_start;
			int d = (i - num[adj2->theNode()] + p) % p + 1;
			if (d > length || (d == length && adj2->theNode()->index() < adjStart->theNode()->index())) {
				length = d;
				adjStart = adj2;
			}
			SListIterator<QType> it, itLimit = posInQ[i];
			do {
				it = Q.begin();
				posInQ[(*it).m_limit] = SListIterator<QType>();
				Q.popFront();
			} while (it != itLimit);
		}

		int j = -1;
		if (diag[i].empty())
			j = (i - 2 + p) % p;
		else {
			int m = p;
			for (int k : diag[i]) {
				int d = (k - i + p) % p;
				if (d < m) {
					m = d;
					j = k;
				}
			}
			OGDF_ASSERT(j != -1);
			j = (j - 1 + p) % p;
			if (!firstRun) {
				posInQ[Q.back().m_limit] = nullptr;
				Q.back().m_limit = j;
				posInQ[j] = Q.backIterator();
			}
		}

		if (firstRun)
			posInQ[j] = Q.pushBack(QType(adj, j));

		adj = adj->faceCycleSucc();
		i = num[adj->theNode()];
		if (i == 0) firstRun = false;
	} while (!Q.empty());

	return adjStart;
}


void BiconnectedShellingOrder::doCall(const Graph &G,
	adjEntry adj,
	List<ShellingOrderSet> &partition)
{
	OGDF_ASSERT(isBiconnected(G));
	OGDF_ASSERT(G.representsCombEmbedding());

	ConstCombinatorialEmbedding E(G);

	face extFace = (adj != nullptr) ? E.rightFace(adj) : E.maximalFace();
	ComputeBicOrder cpo(G,E,extFace,m_baseRatio);

	cpo.initPossibles();

#ifdef OUTPUT_BSO
	std::cout << "after initialization:\n";
	cpo.print();
#endif

	while(cpo.getPossible())
	{
		switch(cpo.nextPoss())
		{
		case ComputeBicOrder::CandidateType::Face:
			partition.pushFront(ShellingOrderSet());
			cpo.removeNextFace(partition.front());
			break;

		case ComputeBicOrder::CandidateType::Node:
			partition.pushFront(ShellingOrderSet());
			cpo.removeNextNode(partition.front());
			break;

		case ComputeBicOrder::CandidateType::Edge:
			partition.pushFront(ShellingOrderSet());
			cpo.removeNextVirt(partition.front());
			break;
		}

		cpo.doUpdate();

#ifdef OUTPUT_BSO
		std::cout << "after update:\n";
		cpo.print();
#endif
	}

	partition.pushFront(ShellingOrderSet(2));
	cpo.setV1(partition.front());
}

}
