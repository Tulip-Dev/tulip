/*
 * $Revision: 2299 $
 * 
 * last checkin:
 *   $Author: gutwenger $ 
 *   $Date: 2012-05-07 15:57:08 +0200 (Mon, 07 May 2012) $ 
 ***************************************************************/
 
/** \file
 * \brief Declaration of the class EmbedKey. 
 * 
 * Implements the Key of the direction Indicator. Used by class EmbedPQTree.
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



#ifndef OGDF_EMBED_KEY_H
#define OGDF_EMBED_KEY_H

#include <ogdf/internal/planarity/PQNodeKey.h>

namespace ogdf{


class EmbedPQTree;
class embedKey;


class indInfo
{
	friend class EmbedPQTree;
	friend class embedKey;

public:
	indInfo(node w) { v = w; changeDir = false; }
	~indInfo() { }

	void resetAssociatedNode(node w) { v = w; }
	node getAssociatedNode() { return v; }

private:
	node v;
	bool changeDir;


   OGDF_NEW_DELETE
};


class embedKey : public PQNodeKey<edge,indInfo*,bool>
{
public: 
	
	embedKey(indInfo* i) : PQNodeKey<edge,indInfo*,bool>(i) { }

	virtual ~embedKey() { }

	const char* print() {
		if (!m_printString) 
			m_printString = new char[128]; 
		int sId = m_userStructInfo->v->index();
		ogdf::sprintf(m_printString, 128, " Indicator %d", sId);
		return m_printString;
	}
};

}
#endif
