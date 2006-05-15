#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <cmath>
#include "tulip/Spline.h"

using namespace tlp;

static void SplinePoint(int *u, unsigned int n, unsigned int t, double v,
                        const double (*control)[3], double (&output)[3]);
static double SplineBlend(unsigned int k, unsigned int t, int *u, double v);
static void SplineKnots(int *u, unsigned int n, unsigned int t);

void tlp::Spline3(double (&p)[3], const double (*point)[3], unsigned int size, double mu)
{
  int u[size+3];
  size--;
  SplineKnots(u, size, 3);
  SplinePoint(u, size, 3, mu * (size-1), point, p);
}

void tlp::Spline4(double (&p)[3], const double (*point)[3], unsigned int size, double mu)
{
  int u[size+4];
  size--;
  SplineKnots(u, size, 4);
  SplinePoint(u, size, 4, mu * (size-2), point, p);
}

/*
  This returns the point "output" on the spline curve.
  The parameter "v" indicates the position, it ranges from 0 to n-t+2
   
*/
void SplinePoint(int *u, unsigned int n, unsigned int t, double v, const double (*control)[3], double (&output)[3])
{
  output[0] = output[1] = output[2] = 0;

  for (unsigned int k=0; k<=n; ++k) {
    double b = SplineBlend(k,t,u,v);
    output[0] += control[k][0] * b;
    output[1] += control[k][1] * b;
    output[2] += control[k][2] * b;
  }
}

/*
  Calculate the blending value, this is done recursively.
   
  If the numerator and denominator are 0 the expression is 0.
  If the deonimator is 0 the expression is 0
*/
double SplineBlend(unsigned int k, unsigned int t, int *u, double v)
{
  double value;

  if (t == 1) {
    if ((u[k] <= v) && (v < u[k+1]))
      value = 1;
    else
      value = 0;
  } else {
    if ((u[k+t-1] == u[k]) && (u[k+t] == u[k+1]))
      value = 0;
    else if (u[k+t-1] == u[k]) 
      value = (u[k+t] - v) / (u[k+t] - u[k+1]) * SplineBlend(k+1,t-1,u,v);
    else if (u[k+t] == u[k+1])
      value = (v - u[k]) / (u[k+t-1] - u[k]) * SplineBlend(k,t-1,u,v);
    else
      value = (v - u[k]) / (u[k+t-1] - u[k]) * SplineBlend(k,t-1,u,v) + 
        (u[k+t] - v) / (u[k+t] - u[k+1]) * SplineBlend(k+1,t-1,u,v);
  }
  return(value);
}

/*
  The positions of the subintervals of v and breakpoints, the position
  on the curve are called knots. Breakpoints can be uniformly defined
  by setting u[j] = j, a more useful series of breakpoints are defined
  by the function below. This set of breakpoints localises changes to
  the vicinity of the control point being modified.
*/
void SplineKnots(int *u, unsigned int n, unsigned int t)
{
  unsigned int j;

  for (j=0; j<=n+t; ++j) {
    if (j < t)
      u[j] = 0;
    else if (j <= n)
      u[j] = j - t + 1;
    else if (j > n)
      u[j] = n - t + 2;	
  }
}
