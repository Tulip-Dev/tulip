//-*-c++-*-
/*
 Author: Romain BOURQUI
 Email : bourqui@labri.fr
 Last modification : 02/11/2005
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/

//#ifndef Bfs_H
//#define Bfs_H

#include <tulip/MutableContainer.h>

class Bfs {
 public :
  
  Bfs(SuperGraph * G, SelectionProxy * resultatAlgoSelection);
  ~Bfs();

  SuperGraph * graph;

 private :

  void computeBfs(SuperGraph * G,SelectionProxy * resultatAlgoSelection, node root);  

  SelectionProxy * s_proxy;
  unsigned int nbNodes;
  MutableContainer<bool>::MutableContainer selectedNodes;
  MutableContainer<bool>::MutableContainer selectedEdges;

 };  
