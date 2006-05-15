#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <cmath>
#include "tulip/Bezier.h"

void tlp::Bezier3(double (&p)[3], const double p1[3], const double p2[3],
		    const double p3[3], double mu) {
  double mum1,mum12,mu2;
  mu2 = mu * mu;
  mum1 = 1 - mu;
  mum12 = mum1 * mum1;

  p[0] = p1[0]*mum12 + 2*p2[0]*mum1*mu + p3[0]*mu2;
  p[1] = p1[1]*mum12 + 2*p2[1]*mum1*mu + p3[1]*mu2;
  p[2] = p1[2]*mum12 + 2*p2[2]*mum1*mu + p3[2]*mu2;
}

void tlp::Bezier4(double (&p)[3], const double p1[3], const double p2[3],
		    const double p3[3], const double p4[3], double mu) {
 double mum1,mum13,mu3;
 mum1 = 1 - mu;
 mum13 = mum1 * mum1 * mum1;
 mu3 = mu * mu * mu;

 p[0]= mum13*p1[0] + 3*mu*mum1*mum1*p2[0]+ 3*mu*mu*mum1*p3[0] + mu3*p4[0];
 p[1]= mum13*p1[1] + 3*mu*mum1*mum1*p2[1]+ 3*mu*mu*mum1*p3[1] + mu3*p4[1];
 p[2]= mum13*p1[2] + 3*mu*mum1*mum1*p2[2]+ 3*mu*mu*mum1*p3[2] + mu3*p4[2];
}

/*
 * IMPORTANT: the LAST point is NOT computed
 */
void tlp::Bezier(double (&p)[3], const double (*points)[3], unsigned int size, double mu) {
  int n=size-1;
  if (n==2) {
    return Bezier3(p, points[0], points[1], points[2], mu);
  }
  else if (n==3) {
    return Bezier4(p, points[0], points[1], points[2], points[3], mu);
  }

  int k,kn,nn,nkn;
  double blend,muk,munk;

  p[0] = p[1] = p[2] = 0;
  muk = 1;
  munk = pow(1.-mu, (double)n);

  for (k=0;k<=n;k++) {
    nn = n;
    kn = k;
    nkn = n - k;
    blend = muk * munk;
    muk *= mu;
    munk /= (1-mu);
    while (nn >= 1) {
      blend *= nn;
      nn--;
      if (kn > 1) {
	blend /= (double)kn;
	kn--;
      }
      if (nkn > 1) {
	blend /= (double)nkn;
	nkn--;
      }
    }
    p[0] += points[k][0] * blend;
    p[1] += points[k][1] * blend;
    p[2] += points[k][2] * blend;
  }
}
