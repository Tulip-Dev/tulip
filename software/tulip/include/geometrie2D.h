/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
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
#include <math.h>
#ifndef GEO_2D
#define GEO_2D

#ifndef M_PI
#define M_PI 3.14159265
#endif

typedef struct {double x,y;} point;
//---------------------------------------------
// Calcule l'angle orienté BA,BC dans le sens
// trigonométrique
//---------------------------------------------
double angleABC(const double xA,const double yA,
		const double xB,const double yB,
		const double xC,const double yC);
//---------------------------------------------
// rotation du point B autour du point A de l'angle 
// Alpha.
//---------------------------------------------
void rotABAlpha(const double xA,const double yA,double &xB, double &yB,
		const double alpha);
void projABCircle(const double xA,const double yA,double &xB, double &yB,
		const double r);
void rotate(float calpha,float salpha ,point *p);
#endif





