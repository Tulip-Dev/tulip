//-*-c++-*-
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "tulip/Size.h"
#include <cmath>

float Size::norm() const {return sqrt(h*h+w*w+d*d);}
