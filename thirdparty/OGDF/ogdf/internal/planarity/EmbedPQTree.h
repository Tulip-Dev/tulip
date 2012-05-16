/*
 * $Revision: 2299 $
 * 
 * last checkin:
 *   $Author: gutwenger $ 
 *   $Date: 2012-05-07 15:57:08 +0200 (Mon, 07 May 2012) $ 
 ***************************************************************/
 
/** \file
 * \brief Declaration of the class EmbedPQTree.
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



#ifndef OGDF_EMBED_PQTREE_H
#define OGDF_EMBED_PQTREE_H

#include <ogdf/internal/planarity/PQTree.h>
#include <ogdf/basic/Graph.h>
#include <ogdf/basic/SList.h>
#include <ogdf/internal/planarity/PlanarLeafKey.h>
#include <ogdf/internal/planarity/EmbedIndicator.h>

namespace ogdf {

typedef PQBasicKey<edge,indInfo*,bool> *PtrPQBasicKeyEIB;

template<>
inline bool doDestruction<PtrPQBasicKeyEIB>(const PtrPQBasicKeyEIB*) { return false; }


typedef PlanarLeafKey<indInfo*> *PtrPlanarLeafKeyI;

template<>
inline bool doDestruction<PtrPlanarLeafKeyI>(const PtrPlanarLeafKeyI*) { return false; }


class EmbedPQTree: public PQTree<edge,indInfo*,bool>
{
public:

	EmbedPQTree() : PQTree<edge,indInfo*,bool>() { }

	virtual ~EmbedPQTree() { }

    virtual void emptyAllPertinentNodes();

    virtual void clientDefinedEmptyNode(PQNode<edge,indInfo*,bool>* nodePtr);

	virtual int Initialize(SListPure<PlanarLeafKey<indInfo*>*> &leafKeys);
		
	void ReplaceRoot(
		SListPure<PlanarLeafKey<indInfo*>*> &leafKeys,
 		SListPure<edge> &frontier,
		SListPure<node> &opposed,
		SListPure<node> &nonOpposed,
		node v);

    virtual bool Reduction(SListPure<PlanarLeafKey<indInfo*>*> &leafKeys);

	PQNode<edge,indInfo*,bool>* scanSibLeft(PQNode<edge,indInfo*,bool> *nodePtr) const {
		return clientSibLeft(nodePtr);
	} 

	PQNode<edge,indInfo*,bool>* scanSibRight(PQNode<edge,indInfo*,bool> *nodePtr) const {
		return clientSibRight(nodePtr);
	} 

	PQNode<edge,indInfo*,bool>* scanLeftEndmost(PQNode<edge,indInfo*,bool> *nodePtr) const {
		return clientLeftEndmost(nodePtr);
	} 

	PQNode<edge,indInfo*,bool>* scanRightEndmost(PQNode<edge,indInfo*,bool> *nodePtr) const {
		return clientRightEndmost(nodePtr);
	} 

	PQNode<edge,indInfo*,bool>* scanNextSib(
		PQNode<edge,indInfo*,bool> *nodePtr, 
		PQNode<edge,indInfo*,bool> *other) {
			return clientNextSib(nodePtr,other);
	} 

	virtual void getFront(
		PQNode<edge,indInfo*,bool>* nodePtr,
		SListPure<PQBasicKey<edge,indInfo*,bool>*> &leafKeys);

protected:

	virtual PQNode<edge,indInfo*,bool>*
		clientSibLeft(PQNode<edge,indInfo*,bool> *nodePtr) const;

	virtual PQNode<edge,indInfo*,bool>*
		clientSibRight(PQNode<edge,indInfo*,bool> *nodePtr) const; 

	virtual PQNode<edge,indInfo*,bool>*
		clientLeftEndmost(PQNode<edge,indInfo*,bool> *nodePtr) const;

	virtual PQNode<edge,indInfo*,bool>*
		clientRightEndmost(PQNode<edge,indInfo*,bool> *nodePtr) const;

	virtual PQNode<edge,indInfo*,bool>*
		clientNextSib(PQNode<edge,indInfo*,bool> *nodePtr, 
					  PQNode<edge,indInfo*,bool> *other) const;
	virtual const char* 
		clientPrintStatus(PQNode<edge,indInfo*,bool> *nodePtr);

	virtual void front(
		PQNode<edge,indInfo*,bool>* nodePtr,
		SListPure<PQBasicKey<edge,indInfo*,bool>*> &leafKeys);

private:

	void ReplaceFullRoot(
		SListPure<PlanarLeafKey<indInfo*>*> &leafKeys,
		SListPure<PQBasicKey<edge,indInfo*,bool>*> &frontier,
		node v,
		bool addIndicator = false,
		PQNode<edge,indInfo*,bool> *opposite = 0);

	void ReplacePartialRoot(
		SListPure<PlanarLeafKey<indInfo*>*> &leafKeys,
		SListPure<PQBasicKey<edge,indInfo*,bool>*> &frontier,
		node v);
};	

}

#endif
