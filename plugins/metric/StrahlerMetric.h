//-*-c++-*-
#ifndef _STRAHLERGRAPH2METRIC_H
#define _STRAHLERGRAPH2METRIC_H

#include <tulip/TulipPlugin.h>
struct Strahler {
  Strahler(int stra=1,int sta=0,int used=0):strahler(stra),stacks(sta),usedStack(used){}
  int strahler;
  int stacks;
  int usedStack;
};

/** \addtogroup metric */
/*@{*/
/// StrahlerMetric.h - An implementation of the Strahler parameter.
/** This plugin is an implementation of the Strahler Parameter.
 *  algorithm first published as:
 *
 *  A.N. Strahler, \n
 *  "Hypsomic analysis of erosional topography",\n
 *  "Bulletin Geological Society of America 63,pages 1117-1142.",\n
 *   1952.\n
 *
 *  Extended to graphs in :
 *
 *  D. Auber, \n
 *  "Using Strahler numbers for real time visual exploration of huge graphs, \n
 *  ICCVG, International Conference on Computer Vision and Graphics, \n
 *  pages 56-69, \n
 *  2002, September.
 *
 *  <b> HISTORY</b>
 *
 *  - 01/12/1999 Verson 0.0.1: Initial release
 *
 *  - 06/11/2002 Verson 0.0.2: Documentation and code clean up
 *
 *
 *  \note This algorithm works on general graphs, if no node is selected,
 *  the algorithm automatically choose the nodes with higher outdegree as
 *  starting nodes.
 *
 *  The algorithm use the (bool)"viewSelection" property to determine
 *  the starting nodes.
 *
 *
 *  \author David Auber University Bordeaux I France: Email:auber@tulip-software.com
 *
 *  <b>LICENCE</b>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by  
 *  the Free Software Foundation; either version 2 of the License, or     
 *  (at your option) any later version.
 *
 */
class StrahlerMetric:public DoubleAlgorithm { 
public:
  StrahlerMetric(const PropertyContext &);
  bool run();
  bool check(std::string &);

private:
  Strahler topSortStrahler(node n,int &curPref, stdext::hash_map<node,int> &tofree, stdext::hash_map<node,int> &prefix,
			   stdext::hash_map<node,bool> &visited, stdext::hash_map<node,bool> &finished,
			   stdext::hash_map<node,Strahler> &cachedValues);
  bool allNodes;
};
/*@}*/
#endif











