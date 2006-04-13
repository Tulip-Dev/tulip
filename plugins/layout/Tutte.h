//-*-c++-*-
#ifndef _TutteLAYOUT_H
#define _TutteLAYOUT_H
#include <cmath>
#include <tulip/TulipPlugin.h>

/** \addtogroup layout */
/*@{*/
/// Tutte.h - An implementation of a 3-Connected graph layout.
/** This plugin is an implementation of the Tutte layout
 *  algorithm first published as:
 *
 *  W.T. Tutte,
 *  "How to Draw a Graph",
 *  "Proc. London Math. Soc.",
 *  "1963",
 *  pages 743-768.
 *
 *  <b> HISTORY </b>
 *
 *  - 01/12/1999 Verson 0.0.1: Initial release
 *  - 06/11/2002 Verson 0.0.2: Documentation and code clean up
 *
 *  \note This algorithm only works on 3 connected graphs.
 *
 *  \author David Auber University Bordeaux I France, Email : auber@tulip-software.org
 *
 *  <b> LICENCE </b>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by  
 *  the Free Software Foundation; either version 2 of the License, or     
 *  (at your option) any later version.
 *
*/
class Tutte:public LayoutAlgorithm {
public:
  Tutte(const PropertyContext &);
  ~Tutte();
  
  bool run();
  bool check(std::string &);
  void reset();
};
/*@}*/
#endif
