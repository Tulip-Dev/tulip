#ifndef BEZIER_H
#define BEZIER_H

namespace tlp {

/*
 * Compute the position of a point 'p' at mu (0 <= mu < 1)
 * along Bezier curve defined by control points 'points'
 * IMPORTANT: THE LAST POINT IS NOT COMPUTED
 */
void Bezier(double (&p)[3], const double (*points)[3], unsigned int size, double mu);

/*
 * optimized version with 3 or four control points. General Bezier uses the functions in that case.
 */
void Bezier3(double (&p)[3], const double p1[3], const double p2[3], const double p3[3], double mu);
void Bezier4(double (&p)[3], const double p1[3], const double p2[3], const double p3[3], const double p4[3], double mu);

}

#endif //BEZIER_H
