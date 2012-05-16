/*
 * $Revision: 2299 $
 * 
 * last checkin:
 *   $Author: gutwenger $ 
 *   $Date: 2012-05-07 15:57:08 +0200 (Mon, 07 May 2012) $ 
 ***************************************************************/
 
/** \file
 * \brief Declaration of PlanarModule which implements a planarity
 *        test and planar embedding algorithm.
 * 
 * \author Sebastian Leipert
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

#ifndef OGDF_PLANAR_MODULE_H
#define OGDF_PLANAR_MODULE_H

//=========================================================
// Main functions:
//
// planarityTest(Graph &G)  Tests a graph for planarity.
//
// planarEmbed(Graph &G)  Tests a graph for planarity and returns
//                        a planar embedding if G is planar.
//
//=========================================================

#include <ogdf/basic/EdgeArray.h>
#include <ogdf/basic/NodeArray.h>
#include <ogdf/basic/SList.h>

namespace ogdf {


class OGDF_EXPORT PlanarModule{

public:

	PlanarModule() {};
	~PlanarModule() {};

	// Returns true, if G is planar, false otherwise.
	bool planarityTest(Graph &G);
	bool planarityTest(const Graph &G);

	// Returns true, if G is planar, false otherwise.
	// If true, G contains a planar embedding.
	bool planarEmbed(Graph &G){return preparation(G,true);}

private:

	// Prepares the planarity test and the planar embedding
	bool preparation(Graph &G,bool embed);

	// Performs a planarity test on a biconnected component
	// of G. numbering contains an st-numbering of the component.
	bool doTest(Graph &G,NodeArray<int> &numbering);

	// Performs a planarity test on a biconnected component
	// of G and embedds it planar. 
	// numbering contains an st-numbering of the component.
	bool doEmbed(Graph &G,
				 NodeArray<int>  &numbering,
				 EdgeArray<edge> &backTableEdges,
				 EdgeArray<edge> &forwardTableEdges);

	// Used by doEmbed. Computes an entire embedding from an
	// upward embedding.
	void entireEmbed(Graph &G,
					 NodeArray<SListPure<adjEntry> > &entireEmbedding,
					 NodeArray<SListIterator<adjEntry> > &adjMarker,
					 NodeArray<bool> &mark,
					 node v);

	void prepareParallelEdges(Graph &G);


	//private Members for handling parallel edges
	EdgeArray<ListPure<edge> > m_parallelEdges;
	EdgeArray<bool> m_isParallel;
	int	m_parallelCount;



};

}
#endif
