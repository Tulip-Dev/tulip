//-*-c++-*-
#ifndef _STRENGTHMETRIC_H
#define _STRENGTHMETRIC_H
#include <ext/hash_set>
#include <tulip/TulipPlugin.h>

/** \addtogroup metric */
/*@{*/
/// StrengthMetric.h - An implementation of the strength parameter.
/** This plugin is an implementation of the Strength Parameter.
 *  algorithm first published in :
 *
 *  Y. Chiricota. F.Jourdan, an G.Melançon
 *  "Software component capture using graph clustering",
 *  "IWPC",
 *   2002.
 *  HISTORY:
 *
 *  26/02/2002 Verson 0.0.1: First implementation.
 *
 *  NOTES:
 *
 *  This algorithm works on general graphs. 
 *
 *  AUTHOR:
 *
 *  David Auber University Bordeaux I France: Email:auber@tulip-software.org
 *
 *  LICENCE:
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by  
 *  the Free Software Foundation; either version 2 of the License, or     
 *  (at your option) any later version.
 *
 */
class StrengthMetric:public Metric { 
public:
  StrengthMetric(const PropertyContext &);
  ~StrengthMetric();
  double getNodeValue(const node n);
  double getEdgeValue(const edge n);
  bool run();
private:
  double e(const stdext::hash_set<node> &U);
  double e(stdext::hash_set<node> &U, stdext::hash_set<node> &V);
  double s(stdext::hash_set<node> &U, stdext::hash_set<node> &V);
  double s(const stdext::hash_set<node> &U);
};
/*@}*/
#endif
