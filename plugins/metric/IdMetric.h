//-*-c++-*-
/**
 Author: David Auber
 Email : auber@labri.fr
 Last modification : 20/08/2001
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/

#ifndef _IDMETRIC_H
#define _IDMETRIC_H

#include <tulip/TulipPlugin.h>
///
class IdMetric:public Metric
{ 
public:
  IdMetric(const PropertyContext &);
  ~IdMetric();
  double getNodeValue(const node n);
  double getEdgeValue(const edge n);
};



#endif











