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





