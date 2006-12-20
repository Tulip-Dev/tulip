#include "tulip/Paragraph.h"
#include "tulip/Renderer.h"

#include <string.h>
#include <iostream>
#include <cassert>
#include <math.h>

#define H 3

using namespace std;
using namespace tlp;

//***************************************************************************
// CLASSE C_String
//***************************************************************************
//---------------------------------------------------------------------------
C_String::C_String(string str, Context* ctr):c(ctr),s(str){
}

C_String::C_String(const C_String& c_s):c(c_s.c),s(c_s.getString()){
}
//***************************************************************************
// CLASSE Paragraph
//***************************************************************************
//---------------------------------------------------------------------------
Paragraph::Paragraph(Context &context, Align a):_context(context) {
  alignement = a;
}
Paragraph::~Paragraph(){
  for(unsigned int i = 0; i<listOfcontexts.size(); ++i){
    delete listOfcontexts[i];
  }
}
//---------------------------------------------------------------------------
void Paragraph::drawLeft(float w_max, float x_depl, int debut, int fin) const{
#ifndef NDEBUG
  cerr << __PRETTY_FUNCTION__ << endl;
#endif
  int val;
  float decal = 0;
  
  for(int j = debut; j<=fin; ++j){  // on affiche chaque mot sur la ligne
    Context* c_current = myString.at(j).getContext();
    Renderer &renderer = c_current->getRenderer();
    val = renderer.searchFont(renderer.getMode(), c_current->getSize(),c_current->getFontName(), renderer.getDepth());
    assert(val != -1);
    
    unsigned char r, v, b;
    c_current->getColor(r, v, b);
    renderer.setColor(r, v, b);    
    renderer.ActiveFont(val);
    
    if(myString.at(j).getString() != ""){
       renderer.drawString(myString.at(j).getString());
       decal = renderer.getAdvance(myString.at(j).getString());
       renderer.translate(-decal, 0, 0);
    }

  }      
}
//---------------------------------------------------------------------------
void Paragraph::drawRight(float w_max, float x_depl, int debut, int fin) const{
  int j, val;
  float decal= 0;
  Context* c_current;

  for(j = debut; j<=fin; j++){  // on affiche chaque mot sur la ligne
    c_current = myString.at(j).getContext();
    Renderer &renderer = c_current->getRenderer();
    if (j==debut) renderer.translate(w_max - x_depl, 0, 0);

    val = renderer.searchFont(renderer.getMode(), c_current->getSize(), c_current->getFontName(), renderer.getDepth());
    assert(val != -1);
    
    unsigned char r, v, b;
    c_current->getColor(r, v, b);
    renderer.setColor(r, v, b);
    
    renderer.ActiveFont(val);
    if(myString.at(j).getString() != ""){
      renderer.drawString(myString.at(j).getString());
      decal = renderer.getAdvance(myString.at(j).getString().c_str());
    }
    renderer.translate(decal,0,0);
    if (j==fin)
      renderer.translate(-(w_max - x_depl), 0, 0);
  }
}
//---------------------------------------------------------------------------
void Paragraph::drawCenter(float w_max, float x_depl, int debut, int fin) const{
  int j, val;
  float decal= 0;
  Context* c_current;
  _context.getRenderer().translate((w_max - x_depl)/2., 0, 0);
  for(j = debut; j<=fin; j++){  // on affiche chaque mot sur la ligne
    c_current = myString.at(j).getContext();
    Renderer &renderer = c_current->getRenderer();
    val = renderer.searchFont(renderer.getMode(), c_current->getSize(), c_current->getFontName(), renderer.getDepth());
    assert(val != -1);
    unsigned char r, v, b;
    c_current->getColor(r, v, b);
    renderer.setColor(r, v, b);
    
    renderer.ActiveFont(val);
    if(myString.at(j).getString() != ""){
      renderer.drawString(myString.at(j).getString());
      decal = renderer.getAdvance(myString.at(j).getString().c_str());
    }
    renderer.translate(decal,0,0);
  }
  _context.getRenderer().translate(-(w_max - x_depl)/2., 0, 0);
}

//---------------------------------------------------------------------------
void Paragraph::draw(float w_max, float& w) const{
  w = 0;
  int n = myString.size(); // nb de chaines à traiter
  int val; // valeur contenant l'identifiant de la police
  int i_deb=0, i, taille=1; // i_deb : contient l'indice du premier mot à afficher
  bool do_test=false;
  float maxHeightSup=0, maxHeightInf=0, x_depl=0; // x_depl : longueur à afficher
  float HeightSup=0, HeightInf=0; // hauteur pour un mot
  string s;
  float size_s=0, cumul_s = 0;
  Context* c_current = 0;
  for(i=0; i< n; i++){
    c_current = myString.at(i).getContext();
    Renderer &renderer = c_current->getRenderer();
    // recherche de la police
    val = renderer.searchFont(renderer.getMode(), c_current->getSize(), c_current->getFontName(), renderer.getDepth());
    if(val == -1) // jout de la police si elle n'existe pas
      val = renderer.AddFont(renderer.getMode(), c_current->getSize(), c_current->getFontName(), renderer.getDepth());
    s = myString.at(i).getString(); // on récupère le mot;
    
    if(s != ""){
      cumul_s += renderer.getAdvance(s.c_str(), val); // on recherche sa longueur
            
      int length_s = s.size();
      if(s[length_s-1] == ' ')
	do_test = true;
      else{
	do_test = false;
	taille++;
      }
    }
    else
      do_test = true;
      
    if(HeightSup<renderer.getAscender(val))
      HeightSup = renderer.getAscender(val);
    if(HeightInf<fabs(renderer.getDescender(val)))
      HeightInf = fabs(renderer.getDescender(val));

    if(do_test){
      size_s = cumul_s;
      if((x_depl + size_s <= w_max) && (s != "")){ // test si la longeur cumulée va dépasser
	if(maxHeightSup<HeightSup) maxHeightSup = HeightSup;
	if(maxHeightInf<HeightInf) maxHeightInf = HeightInf;
	x_depl += size_s;
      }
      else{
	renderer.translate(0,-(maxHeightSup),0); 
	float sav_inf = maxHeightInf;
	maxHeightSup =  HeightSup;
	maxHeightInf =  HeightInf;      
	switch(alignement){
	case LEFT:      drawLeft(w_max, x_depl, i_deb, i-taille);   break;
	case RIGHT:     drawRight(w_max, x_depl, i_deb, i-taille);  break;
	case CENTER:    drawCenter(w_max, x_depl, i_deb, i-taille); break;
	case JUSTIFIED: drawLeft(w_max, x_depl, i_deb, i-taille); // A FAIRE       !!!!!!!!!!!!!!!!!!!!!!!!!!!
	}
	// retour à la ligne
	// renderer.translate(-x_depl,-(sav_inf+H),0);
	renderer.translate(0,-(sav_inf+H),0);
	if(x_depl > w) w = x_depl;	
	i_deb = i-taille+1; // maj du valeur
	x_depl = size_s;
      }
      taille = 1;
      cumul_s = 0;
      HeightSup = 0;
      HeightInf = 0;
    }
  }
  
  // derniere ligne à afficher ou première s'il y en a eu qu'une ...
  _context.getRenderer().translate(0,-(maxHeightSup),0);
  
  switch(alignement){
  case LEFT:      drawLeft(w_max, x_depl, i_deb, n-1);    break;
  case RIGHT:     drawRight(w_max, x_depl, i_deb, n-1);   break;
  case CENTER:    drawCenter(w_max, x_depl, i_deb, n-1);  break;
  case JUSTIFIED: drawLeft(w_max, x_depl, i_deb, i-1); // A FAIRE         !!!!!!!!!!!!!!!!!!!!!!!!!!!
  }
  
  // retour à la ligne
  _context.getRenderer().translate(0 ,-(maxHeightInf+H),0);
  if(x_depl > w) w = x_depl;
}

//---------------------------------------------------------------------------
void Paragraph::getBoundingBox(float w_max, float& h, float& w) const{
  w = 0;
  h = 0;
  int n = myString.size(); // nb de chaines à traiter
  int val; // valeur contenant l'identifiant de la police
  int i_deb=0, i, taille=1; // i_deb : contient l'indice du premier mot à afficher
  bool do_test=false;
  float maxHeightSup=0, maxHeightInf=0, x_depl=0; // x_depl : longueur à afficher
  float HeightSup=0, HeightInf=0; // hauteur pour un mot
  string s;
  float size_s=0, cumul_s = 0;
  Context* c_current = 0;
  for(i=0; i< n; i++){
    c_current = myString.at(i).getContext();
    Renderer &renderer = c_current->getRenderer();
    // recherche de la police
    val = renderer.searchFont(renderer.getMode(), c_current->getSize(), c_current->getFontName(), renderer.getDepth());
    if(val == -1) // ajoute de la police si elle n'existe pas
      val = renderer.AddFont(renderer.getMode(), c_current->getSize(), c_current->getFontName(), renderer.getDepth());
    if (val == -1) {cerr << __PRETTY_FUNCTION__ << " ==> Add font failed" << endl;return;}
    s = myString.at(i).getString(); // on récupère le mot;
    if(s != ""){
      cumul_s += renderer.getAdvance(s.c_str(), val); // on recherche sa longueur
      int length_s = s.size();
      if(s[length_s-1] == ' ')	do_test = true;
      else{
	do_test = false;
	taille++;
      }
    }
    else
      do_test = true;    
    if(HeightSup<renderer.getAscender(val))
      HeightSup = renderer.getAscender(val);
    if(HeightInf<fabs(renderer.getDescender(val)))
      HeightInf = fabs(renderer.getDescender(val));
    
    if(do_test){
      size_s = cumul_s;
      if((x_depl + size_s <= w_max) && (s != "")){ // test si la longeur cumulée va dépasser
	if(maxHeightSup<HeightSup) maxHeightSup = HeightSup;
	if(maxHeightInf<HeightInf) maxHeightInf = HeightInf;
	x_depl += size_s;

      }
      else{
	h += maxHeightSup+maxHeightInf+H; 
	maxHeightSup = HeightSup;
	maxHeightInf = HeightInf;      
	if(x_depl > w) w = x_depl;
	i_deb = i-taille+1; // maj du valeur
	x_depl = size_s;
      }
      taille = 1;
      cumul_s = 0;
      HeightSup = 0;
      HeightInf = 0;
    }
  }
  // derniere ligne à afficher ou première s'il y en a eu qu'une ...
  h += maxHeightSup+maxHeightInf+H; 
  if(x_depl > w) w = x_depl; 
}
//---------------------------------------------------------------------------
bool Paragraph::addString(string s, Context c){
  Context* ctr;
  ctr = findContext(c); 
  if(!ctr) {
    ctr = new Context(c);
    listOfcontexts.push_back(ctr);
  }
  C_String c_s = C_String(s,ctr);
  myString.push_back(c_s);
  return true;
}
//---------------------------------------------------------------------------
Context* Paragraph::findContext(Context& c){
  int n = listOfcontexts.size();
  for(int i=0; i< n; i++) {
    if(c ==  *(listOfcontexts[i]))
      return listOfcontexts[i];
  }
  return 0;
}
