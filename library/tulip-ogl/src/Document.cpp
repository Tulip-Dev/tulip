

#include "tulip/Document.h"

#include <iostream>
#include <cassert>

using namespace std;
using namespace tlp;

const Align DEFAULT_ALIGN=LEFT;

FLayout::~FLayout(){}
Frame::~Frame(){}

//***************************************************************************
// CLASSE Document
//***************************************************************************
//---------------------------------------------------------------------------
Document::Document() {
  //  cerr << __PRETTY_FUNCTION__ << ": Finised : "<< this << endl;
}
Document::Document(Context c, Align a){
  contexts.push(c);
  aligns.push(a);
}
//---------------------------------------------------------------------------
Document::~Document(){
  //  cerr << __PRETTY_FUNCTION__ << endl;
  for(unsigned int i = 0; i< blocks.size(); ++i){
    delete blocks[i];
  }
  //  while(!contexts.empty()) removeContext();
  //  while(!aligns.empty()) removeAlign();
  //  cerr << __PRETTY_FUNCTION__ << ": Finised : "<< this << endl;
}
//---------------------------------------------------------------------------
void Document::setDefaultAlign(){ aligns.push(DEFAULT_ALIGN); }
//---------------------------------------------------------------------------
void Document::setAlign(Align a){ 
  aligns.push(a); 
}
void Document::setContext(Context c){
  contexts.push(c);
}
//---------------------------------------------------------------------------
void Document::removeContext(){
  if(!contexts.empty())
    contexts.pop();
  else
    cerr<<" Document error : removeContext, pile vide!"<<endl;
}
//---------------------------------------------------------------------------
void Document::removeAlign(){
  if(!aligns.empty())
    aligns.pop();
  else
    cerr<<" Document error : removeAlign, pile vide!"<<endl;
}
//---------------------------------------------------------------------------
Context Document::getContext() const{
  assert(!contexts.empty());
  return contexts.top(); 
}
//---------------------------------------------------------------------------
Align Document::getAlign() const{
  assert(!aligns.empty());
  return aligns.top();
}
//---------------------------------------------------------------------------
void Document::draw(float w_max, float&w) const{
  w = 0;
  float w_global;
  int n = blocks.size();
  for(int i = 0; i<n; i++){
    blocks[i]->draw(w_max, w_global);
    if( w_global > w) w = w_global;
  }
  if(w < w_max) w = w_max;
}
//---------------------------------------------------------------------------
void Document::getBoundingBox(float w_max, float& h, float& w) const{
  float height, width;
  h = 0; w = 0;
  int n = blocks.size();
  for(int i = 0; i<n; i++){
    blocks.at(i)->getBoundingBox(w_max,height, width);
    if( width > w) w = width;
    h += height;
  }
  //  if(w < w_max) w = w_max;
}
//---------------------------------------------------------------------------
bool Document::addFrame(Frame* f){
  if(!f){
    cerr<< __PRETTY_FUNCTION__ << " ==> null pointer" << endl;
    return false;
  }
  blocks.push_back(f);
  return true;
}

//---------------------------------------------------------------------------
