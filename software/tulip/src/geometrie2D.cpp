/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <geometrie2D.h>

//---------------------------------------------
// Computes the oriented angle BA,BC in the trigonometric direction
//---------------------------------------------
double angleABC(const double xA,const double yA,
		const double xB,const double yB,
		const double xC,const double yC)
{
  double cosAlpha,sens,alpha=0;
  
  cosAlpha =  ((xA-xB)*(xC-xB)+(yA-yB)*(yC-yB))
    /
    (sqrt(pow(xA-xB,2)+pow(yA-yB,2))*sqrt(pow(xC-xB,2)+pow(yC-yB,2)));

  sens=(xA-xB)*(yC-yB)-(xC-xB)*(yA-yB);
  alpha=acos(cosAlpha);
  if (sens<0) alpha=2*M_PI-alpha;
  return alpha;
}
//---------------------------------------------
// rotation of the point B around the point A, of angle Alpha.
//---------------------------------------------
void rotABAlpha(const double xA,const double yA,double &xB, double &yB,
		const double alpha)
{
  double tmpX;
  xB=xB-xA;
  yB=yB-yA;
  tmpX=xB*cos(alpha)-yB*sin(alpha);
  yB=xB*sin(alpha)+yB*cos(alpha);
  xB=tmpX;
  xB=xB+xA;
  yB=yB+yA;
}

void projABCircle(const double xA,const double yA,double &xB, double &yB,
		const double r)
{
  double a,b,x1,x2,y11,y12,y21,y22,xx1,yy1,xx2,yy2,tmpDouble;
  a=b=x1=x2=y11=y12=y21=y22=xx1=yy1=xx2=yy2=tmpDouble=0;
  double A,B,C,Delta;
  double d1,d2;
  int xx1OK=false;
  if (fabs(xA-xB) < 0.001)
  {
    a=0;
    b=xB;
    xx1=xB;
    xx2=xB;
    yy1=sqrt(pow(r,2)-pow(xB,2));
    yy2=-yy1;
  }
  else
  {
    a=(yA-yB)/(xA-xB);
    b=yB-a*xB;
    
    A=pow(a,2)+1.0;
    B=2.0*a*b;
    C=pow(b,2)-pow(r,2);
    Delta=pow(B,2)-4.0*A*C;

    x1=(-B+sqrt(Delta))/(2.0*A);
    x2=(-B-sqrt(Delta))/(2.0*A);

    //  cout << "\nx1 :" << x1 << " x2 : " << x2 << "\n";

    y11=sqrt(pow(r,2)-pow(x1,2));
    y12=-y11;

    y21=sqrt(pow(r,2)-pow(x2,2));
    y22=-y21;

    tmpDouble=fabs(a*x1+b-y11);
    if (tmpDouble<0.000001) {
      if (!xx1OK)
      {
        xx1=x1;
        yy1=y11;
        xx1OK=true;
      }
      else
      {
        xx2=x1;
        yy2=y11;
      }
    }
    
    tmpDouble=fabs(a*x1+b-y12);
    if (tmpDouble<0.000001) {
      if (!xx1OK)
      {
        xx1=x1;
        yy1=y12;
        xx1OK=true;
      }
      else
      {
        xx2=x1;
        yy2=y12;
      }
    }
    
    tmpDouble=fabs(a*x2+b-y21);
    if (tmpDouble<0.000001) {
      if (!xx1OK)
      {
        xx1=x2;
        yy1=y21;
        xx1OK=true;
      }
      else
      {
        xx2=x2;
        yy2=y21;
      }
    }
    
    tmpDouble=fabs(a*x2+b-y22);
    if (tmpDouble<0.000001) {
      if (!xx1OK)
      {
        xx1=x2;
        yy1=y22;
        xx1OK=true;
      }
      else
      {
        xx2=x2;
        yy2=y22;
      }
    }
  }

  d1=pow(xx1-xB,2)+pow(yy1-yB,2);
  d2=pow(xx2-xB,2)+pow(yy2-yB,2);
  if (d1<d2) {
    xB=xx1;yB=yy1;
  }
  else {
    xB=xx2;yB=yy2;
  }
}


void rotate(float calpha,float salpha ,point *p)
{
  point tmp;
  tmp.x=((calpha)*p->x-(salpha)*p->y);
  tmp.y=((salpha)*p->x+(calpha)*p->y);
  *p=tmp;
}

