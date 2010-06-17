/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
#include "tulip/Context.h"
#include "tulip/Table.h"
#include "tulip/Renderer.h"

#include <cassert>
#include <iostream>
using namespace std;
using namespace tlp;

const int H = 10;

Table::Table(Context &c, int row, int col): _context(c){
  assert((0<=row) && (0<=col));
  cellules.clear();

  vFrame tmp(col);
  for(int i = 0; i<row; i++){
    cellules.push_back(tmp);
    for(int j = 0; j<col; j++){
      cellules.at(i).at(j) = NULL;
    }
  }
 couleur[0] = couleur[1] = couleur[2] = 255;
}

Table::~Table(){
  int k = cellules.size();
  for(int i = 0; i< k; i++){
    int l = cellules.at(i).size();
    for(int j = 0; j< l; j++)
      if(cellules.at(i).at(j))
	delete cellules.at(i).at(j);
  }
}


void Table::draw(float w_max, float& w) const{
  w = 0;
  Renderer &renderer = _context.getRenderer();

  float w_local, w_global;

  float n_i, n_j;
  float max_h = 0, cumul_h=0;
  n_i = cellules.size();
    
  if(n_i != 0){
    renderer.setColor(couleur[0], couleur[1], couleur[2]);
    renderer.drawLine(0, 0, 0, w_max, 0, 0);
    
    for(int i = 0; i< n_i; i++){
      n_j = cellules.at(i).size();
      if(n_j != 0){
	w_global = 0;
	max_h = 0;
	float interval = w_max/(float)n_j;
	
	for(int j = 0; j<n_j; j++){ // décalage suivant la case à traiter
	  if(cellules.at(i).at(j)){
	    renderer.translate(j*interval+H,-H,0);
	    float h; // hauteur tracée pour cette case
	    
	    cellules.at(i).at(j)->draw(interval-H*2, w_local);
	    w_global += w_local+2*H;
	    cellules.at(i).at(j)->getBoundingBox(interval-H*2, h, w_local);
	    if(h>max_h) max_h = h;
	    renderer.translate(-(H+j*interval),h+H,0);
	  }
	} 
	renderer.translate(0,-(max_h+H*2),0);
	
	renderer.setColor(couleur[0], couleur[1], couleur[2]);
	renderer.drawLine(0, 0, 0, w_max, 0, 0);
	
	if(w_global>w) w = w_global;
      }
      cumul_h += max_h+2*H;
    }
    n_j = cellules.at(0).size();
    float interval = w_max/n_j;
    for(int i = 0; i <=n_j; i++){
      renderer.setColor(couleur[0], couleur[1], couleur[2]);
      renderer.drawLine(i*interval, cumul_h, 0, i*interval, 0, 0);
    }
    renderer.translate(0,-H,0);
    if( w <w_max ) w = w_max;
    else if(w > w_max) w = w_max;//cout<<" Draw : qu'estc-e que c'est que ce bordel   , w : "<<w<<"  w_max : "<<w_max<<endl;
  }
}

void Table::getBoundingBox(float w_max, float& h, float& w) const{
  w = 0;
  float w_local, w_global;

  int n_i, n_j;
  float max_h = 0, cumul_h=0;
  n_i = cellules.size();
  
  if(n_i != 0){
    
    for(int i = 0; i< n_i; i++){
      
      w_global = 0;
      n_j = cellules.at(i).size();
      max_h = 0;
      if(n_j != 0){
	float interval = w_max/(float)n_j;
	
	for(int j = 0; j<n_j; j++){
	  if(cellules.at(i).at(j)){
	    float h; // hauteur tracée pour cette case
	    cellules.at(i).at(j)->getBoundingBox(interval-H*2, h, w_local);
	    w_global += w_local+2*H;
	    if(h>max_h) max_h = h;
	  }
	} 
	cumul_h += max_h+2*H;
	if(w_global > w) w = w_global;
      }
    }
    h = cumul_h+H;
    if(w < w_max) w = w_max; 
  }
  else  h = 0;
}

bool Table::addFrame(Frame* f){
  int n = cellules.size();
  int i = 0;
  
  while(i<n){
    int k = cellules.at(i).size();
    int j = 0;
    
    while((j<k) && cellules.at(i).at(j)) ++j;
    
    if(j != k){
      cellules.at(i).at(j) = f;
      return true;
    }
    ++i;
  }    
  if(i == n) return false;
  return false;
}

bool Table::addFrame(Frame* f, int row, int col){
  assert(f);
  
  int n = cellules.size();
  assert((0<=row) && (row<n));

  n = cellules.at(row).size();
  assert((0<=col) && (col<n));
  if(!cellules.at(row).at(col)){
    cellules.at(row).at(col) = f;
    return true;
  }
  else{
    delete cellules.at(row).at(col);
    cellules.at(row).at(col) = f;
    return true;
  }
  return false;
}

void Table::getColor(unsigned char& r, unsigned char& v, unsigned char& b) const{ 
  r = couleur[0]; v = couleur[1]; b = couleur[2];
}
void Table::setColor(unsigned char r, unsigned char v, unsigned char b){ 
  couleur[0] = r; couleur[1] = v; couleur[2] = b;
}

