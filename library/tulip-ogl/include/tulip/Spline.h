#ifndef SPLINE_H
#define SPLINE_H

namespace tlp {

/*
 * Compute a spline curve with degree 3
 */
void Spline3(double (&p)[3], const double (*point)[3], unsigned int size, double mu);

/*
 * Compute a spline curve with degree 4
 */
void Spline4(double (&p)[3], const double (*point)[3], unsigned int size, double mu);

}
#endif //SPLINE_H

