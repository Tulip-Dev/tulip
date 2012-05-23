/*
 * $Revision: 2299 $
 * 
 * last checkin:
 *   $Author: gutwenger $ 
 *   $Date: 2012-05-07 15:57:08 +0200 (Mon, 07 May 2012) $ 
 ***************************************************************/
 
/** \file
 * \brief Mathematical Helpers
 * 
 * \author Markus Chimani
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
 
#ifndef OGDF_MATH_H
#define OGDF_MATH_H

#include <ogdf/basic/basic.h>
#include <math.h>

namespace ogdf {


#define DOUBLE_EPS 0.000001


class OGDF_EXPORT Math {
public:
	static const double pi;
	static const double pi_2;
	static const double pi_4;
	static const double two_pi;
	static const double e;

	inline static int binomial(int n, int k) {
		if(k>n/2) k = n-k;
		if(k == 0) return 1;
		int r = n;
		for(int i = 2; i<=k; ++i)
			r = (r * (n+1-i))/i;
		return r;
	}

	inline static double binomial(double n, double k) {
		if(k>n/2) k = n-k;
		if(k == 0) return 1.0;
		double r = n;
		for(int i = 2; i<=k; ++i)
			r = (r * (n+1-i))/i;
		return r;
	}

	static int factorial(int n) {
		int r = 1;
		for(; n>1; --n) r *= n;
		return r;
	}

	static double factorial(double n) {
		double r = 1;
		for(; n>1; --n) r *= n;
		return r;
	}

	inline bool equald(double a, double b) {
		double d = a-b;
		return d < DOUBLE_EPS && d > -DOUBLE_EPS;
	}

	/*!
	 * \brief A fast method to obtain the rounded down binary logarithm of an 32-bit integer
	 *
	 * This is based on http://en.wikipedia.org/wiki/Binary_logarithm
	 * @param v The number of which the binary logarithm is to be determined
	 * @return The rounded down logarithm base 2 if v is positive, -1 otherwise
	 */
	inline static int floorLog2(int v) {
		if (v <= 0) {
			return -1;
		} else {
			int result = 0;
			if (v >= (1 << 16)) {
				v >>= 16;
				result += 16;
			}
			if (v >= (1 << 8)) {
				v >>= 8;
				result += 8;
			}
			if (v >= (1 << 4)) {
				v >>= 4;
				result += 4;
			}
			if (v >= (1 << 2)) {
				v >>= 2;
				result += 2;
			}
			if (v >= (1 << 1)) {
				result += 1;
			}
			return result;
		}
	}
};


}

#endif // OGDF_MATH_H
