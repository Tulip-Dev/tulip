/*
 * $Revision: 2299 $
 * 
 * last checkin:
 *   $Author: gutwenger $ 
 *   $Date: 2012-05-07 15:57:08 +0200 (Mon, 07 May 2012) $ 
 ***************************************************************/
 
/** \file
 * \brief Declaration of interface for two-layer crossing
 *        minimization algorithms for Simultaneous Drawing.
 * 
 * \author Michael Schulz
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

#ifndef OGDF_TWO_LAYER_CROSS_MIN_SIMDRAW_H
#define OGDF_TWO_LAYER_CROSS_MIN_SIMDRAW_H



#include <ogdf/module/TwoLayerCrossMin.h>


namespace ogdf {


class OGDF_EXPORT TwoLayerCrossMinSimDraw : public TwoLayerCrossMin {
public:
  	//! Initializes a two-layer crossing minimization module.
  TwoLayerCrossMinSimDraw() : TwoLayerCrossMin() { };

	/**
	 * \brief Performs crossing minimization for level \a L.
	 *
	 * @param L is the level in the hierarchy on which nodes are permuted; the
	 *        neighbor level (fixed level) is determined by the hierarchy.
	 */
	virtual void call(Level &L, const EdgeArray<unsigned int> *esg) = 0;

};


} // end namespace ogdf


#endif
