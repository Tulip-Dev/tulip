#ifndef VS_COMPAT_H
#define VS_COMPAT_H
#include <cmath>

//This file contains all the visual studio missing functions and workaround
//MSVC needs explicit casting of ints ot double, float or long double. Let's just pretend he does not.
static  double sqrt(int i) {
  return sqrt(static_cast<double>(i));
}
static double sqrt(unsigned int i) {
  return sqrt(static_cast<double>(i));
}

static double log(int i) {
  return log(static_cast<double>(i));
}
static double log(unsigned int i) {
  return log(static_cast<double>(i));
}

static double floor(int i) {
  return floor(static_cast<double>(i));
}
static double floor(unsigned int i) {
  return floor(static_cast<double>(i));
}

static double fabs(int i) {
  return fabs(static_cast<double>(i));
}

static double fabs(unsigned int i) {
  return fabs(static_cast<double>(i));
}

////////////////////////////////////
static double round(double d) {
  return floor(d + 0.5);
}

static float cbrt( float arg ) {
  return pow(arg, static_cast<float>(1.0/3));
}
static  double cbrt( double arg ) {
  return pow(arg, static_cast<double>(1.0/3));
}
static long double cbrt( long double arg ) {
  return pow(arg, static_cast<long double>(1.0/3));
}
static double rint(double arg) {
  return arg > 0 ? std::floor(arg) : std::ceil(arg);
}

static float rint(float arg) {
  return arg > 0 ? std::floor(arg) : std::ceil(arg);
}
static long double rint(long double arg) {
  return arg > 0 ? std::floor(arg) : std::ceil(arg);
}

#define isnan(x) _isnan(x)
#define isinf(x) !_finite(x)

#define strcasecmp stricmp  //strcasecmp does not exists for VC, workaround

#endif // VS_COMPAT_H
