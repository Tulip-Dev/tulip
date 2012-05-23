/*
 * $Revision: 2299 $
 * 
 * last checkin:
 *   $Author: gutwenger $ 
 *   $Date: 2012-05-07 15:57:08 +0200 (Mon, 07 May 2012) $ 
 ***************************************************************/
 
/** \file
 * \brief Declaration of class PlanarPQTree.
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


#ifndef OGDF_PLANAR_PQTREE_H
#define OGDF_PLANAR_PQTREE_H



#include <ogdf/internal/planarity/PQTree.h>
#include <ogdf/basic/Graph.h>
#include <ogdf/basic/SList.h>
#include <ogdf/internal/planarity/PlanarLeafKey.h>
#include <ogdf/internal/planarity/EmbedKey.h>


namespace ogdf {

class PlanarPQTree: public PQTree<edge,indInfo*,bool> {

public:

	PlanarPQTree() : PQTree<edge,indInfo*,bool>() { }

	virtual ~PlanarPQTree() { }

	//  Does a clean up after a reduction
    virtual void emptyAllPertinentNodes();

	// Initializes a new PQ-tree with a set of leaves.
	virtual int Initialize(SListPure<PlanarLeafKey<indInfo*>*> &leafKeys);
		
	// Replaces the pertinent subtree by a set of new leaves.
	void ReplaceRoot(SListPure<PlanarLeafKey<indInfo*>*> &leafKeys);

	// Reduces a set of leaves.
    virtual bool Reduction(SListPure<PlanarLeafKey<indInfo*>*> &leafKeys);

private:

	// Replaces a pertinet subtree by a set of new leaves if the root 
	// is full.
	void ReplaceFullRoot(SListPure<PlanarLeafKey<indInfo*>*> &leafKeys);

	// Replaces a pertinet subtree by a set of new leaves if the root 
	// is partial.
	void ReplacePartialRoot(SListPure<PlanarLeafKey<indInfo*>*> &leafKeys);

};	

}

#endif
