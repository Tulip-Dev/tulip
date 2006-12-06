#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <cmath>
#include <iostream>
#include <cassert>
#include <GL/gl.h>

#include <FTGLExtrdFont.h>
#include <FTGLOutlineFont.h>
#include <FTGLPolygonFont.h>
#include <FTGLTextureFont.h>
#include <FTGLPixmapFont.h>
#include <FTGLBitmapFont.h>
#include "tulip/TLPPixmapFont.h"
#include "tulip/GlRenderer.h"


using namespace std;
using namespace tlp;


//***************************************************************************
// CLASSE t_GlFonts
//***************************************************************************
//---------------------------------------------------------------------------
t_GlFonts::~t_GlFonts() {
  vector<_GlFonts>::iterator it=fonts.begin();
  for (;it!=fonts.end();++it)
    delete(it->font);
}
//---------------------------------------------------------------------------
int t_GlFonts::searchFont(FontMode type, int size, const string f, float depth) const{
  _GlFonts tmp = _GlFonts(type,size,f,depth);
  map<_GlFonts,int>::const_iterator it=fontsIndex.find(tmp);
  if (it!=fontsIndex.end()) {
    return it->second;
  }
  else
    return -1;
}
//---------------------------------------------------------------------------
int t_GlFonts::Add(FontMode t, int s, float d, const string f) {
  //  cerr << __PRETTY_FUNCTION__ << endl;
  _GlFonts tmp = _GlFonts(t,s,f,d);
  switch(t) {
  case TLP_BITMAP:
    tmp.font = new FTGLBitmapFont (f.c_str());
  	break;
  case TLP_PIXMAP:
    tmp.font = new FTGLPixmapFont (f.c_str());
    break;
  case TLP_OUTLINE:
    tmp.font = new FTGLOutlineFont (f.c_str());
    break;
  case TLP_POLYGON:
    tmp.font = new FTGLPolygonFont (f.c_str());
    break;
  case TLP_EXTRUDE:
    tmp.font = new FTGLExtrdFont (f.c_str());
    break;
  case TLP_TEXTURE:
    tmp.font = new FTGLTextureFont (f.c_str());
    break;
  case TLP_TLPPIXMAP:
    tmp.font = new TLPPixmapFont (f.c_str());
    break;
  }
  assert(!tmp.font->Error());  
  if (!tmp.font->FaceSize(s))
    cerr << __PRETTY_FUNCTION__ << " FaceSize error" << endl;
  tmp.font->Depth(d);
  if (!tmp.font->CharMap(ft_encoding_unicode))
    cerr << __PRETTY_FUNCTION__ << " CharMap error" << endl; 
  
  fonts.push_back(tmp);
  fontsIndex[tmp] = fonts.size()-1;
  return fonts.size()-1;
}

//---------------------------------------------------------------------------
_GlFonts t_GlFonts::operator[](unsigned int i) const {
  assert (i < fonts.size());
  return fonts[i];
}

//***************************************************************************
// CLASSE _GlFonts
//***************************************************************************
//---------------------------------------------------------------------------
_GlFonts::_GlFonts(FontMode type, int size, const std::string name, float depth) :
  type(type),size(size),file(name),font(0) {
  if (type==TLP_EXTRUDE)
    this->depth=depth;
  else
    this->depth=0;
}
bool _GlFonts::operator< (const _GlFonts &dest) const {
  if (type < dest.type) return true;
  if (type > dest.type) return false;
  if (size < dest.size) return true;
  if (size > dest.size) return false;
  if (file < dest.file) return true;
  if (file > dest.file) return false;
  if (depth < dest.depth) return true;
  //  if (depth < dest->depth) return false;  
  return false;
}
//***************************************************************************
//  CLASSE GlFonts
//***************************************************************************
//---------------------------------------------------------------------------
void GlRenderer::drawString(const string &str, int index) const{
  if(index == -1){
    if(active)
      ftfonts[current_font].font->Render(str.c_str());
    else
      cerr<<" GlRenderer warning : drawString, font non active"<<endl;
  }
  else{
    ftfonts[index].font->Render(str.c_str());
  }
}
//--------------------------------------------------------------------------- 
void GlRenderer::getBBox(const std::string &s, float& x1, float& y1, float& z1, float& x2, float& y2, float& z2){
  getBBox(s.c_str(), x1, y1, z1, x2, y2, z2);
}
//--------------------------------------------------------------------------- 
float GlRenderer::getAdvance(const string &str, int index) const{
  //  cerr << __PRETTY_FUNCTION__ << " " << index << " " << str << endl;
  if(index == -1){
    if(active)
      return ftfonts[current_font].font->Advance(str.c_str());
    else{
      cerr<<" GlRenderer error : getAdvance, probleme d'indice "<<endl;
      return 0;
    }
  }
  else
    return ftfonts[index].font->Advance(str.c_str());
}
//--------------------------------------------------------------------------- 
float GlRenderer::getAscender(int index) const{
  if(index == -1){
    if(active)
      return ftfonts[current_font].font->Ascender();
    else
      return 0;
  }
  else{
    return ftfonts[index].font->Ascender();
  }
}
//--------------------------------------------------------------------------- 
float GlRenderer::getDescender(int index) const{
  if(index == -1){
    if(active)
      return ftfonts[current_font].font->Descender();
    else
      return 0;
  }
  else{
    return ftfonts[index].font->Descender();
  }
}
//--------------------------------------------------------------------------- 
bool GlRenderer::ActiveFont(int index){
  current_font = index;
  active = true;
  return true;
}
//--------------------------------------------------------------------------- 
bool GlRenderer::ActiveFont(FontMode t, int s, const string &f, float d){
  int val = ftfonts.searchFont(t, s, f, d);
  if(val != -1) return false;
  current_font = val;
  active = true;
  return true;
}

int GlRenderer::searchFont(FontMode type, int size, const std::string &f, float depth)const {
  return ftfonts.searchFont(type,size,f.c_str(),depth);
}
//--------------------------------------------------------------------------- 
const char* GlRenderer::getFontFilename(int index)const{
  if(index == -1){
    if(active)
      return ftfonts[current_font].getFileName();
    else{
      cerr<<" GlRenderer error : getFontFilename, font non active "<<endl;
      return 0;      
    }
  }
  else{
    return ftfonts[index].getFileName();
  }
}
//--------------------------------------------------------------------------- 
int GlRenderer::getFontType(int index)const{
  if(index == -1){
    if(active)
      return ftfonts[current_font].getType();
    else{
      cerr<<" GlRenderer error : getFontType, font non active "<<endl;
      return 0;      
    }
  }
  else{
    return ftfonts[index].getType();
  }
}
//--------------------------------------------------------------------------- 
int GlRenderer::AddFont(FontMode type, int size, const string &f, float depth){
  //  cerr << __PRETTY_FUNCTION__ << " " << size << " " << f << " " << depth << endl;
  int val = ftfonts.searchFont(type, size, f, depth);
  if(val != -1) {
    cerr << __PRETTY_FUNCTION__ << " Font already loaded" << endl;
    return val;
  } 
  val = ftfonts.Add(type, size, depth, f);  // ne renvoie qu'une valeur juste
  return val;
}
//--------------------------------------------------------------------------- 
void GlRenderer::translate(float x, float y, float z) const {
  switch(mode){
  case TLP_BITMAP:
  case TLP_PIXMAP:
  case TLP_TLPPIXMAP:
    glBitmap(0,0,0,0, x,y, 0);
    break;
  case TLP_OUTLINE:
  case TLP_POLYGON:
  case TLP_EXTRUDE:
  case TLP_TEXTURE:
    glTranslatef(x,y,z);
    break;
  }
}

void GlRenderer::setColor(unsigned char r, unsigned char v, unsigned char b) const {
  glColor4ub(r, v, b ,255);
  
}

void GlRenderer::drawLine(float x1, float y1, float z1, float x2, float y2, float z2) const {
  switch(mode){
  case TLP_BITMAP:
  case TLP_PIXMAP:
  case TLP_TLPPIXMAP:
    GLfloat color[3];

    glGetFloatv(GL_CURRENT_COLOR, color);

    glBitmap(0, 0, 0, 0, x1, y1, 0);

    if(x1 == x2){
      int inc;
      float dy = fabs(y1-y2);
      if( y1 > y2) inc = -1;
      else inc = +1;
      for(int i=0; i<dy; i++){
	glDrawPixels(1, 1, GL_RGB, GL_FLOAT, color);
	glBitmap(0, 0, 0, 0, 0, inc, 0);
      }
      glBitmap(0, 0, 0, 0, 0,-1*inc*dy, 0);
    }
    else{
      int inc;
      float dx = fabs(x1-x2);
      if( x1 > x2 ) inc = -1;
      else inc = +1;
      for(int i=0; i<dx; i++){
	glDrawPixels(1, 1, GL_RGB, GL_FLOAT, color);
	glBitmap(0, 0, 0, 0, inc, 0, 0);
      } 
      glBitmap(0, 0, 0, 0, -1*inc*dx,0, 0);
    }
    
    glBitmap(0, 0, 0, 0, -x1, -y1, 0);

    break;
  case TLP_OUTLINE:
  case TLP_POLYGON:
  case TLP_EXTRUDE:
     glBegin(GL_LINES);
     glVertex3f(x1, y1, z1);
     glVertex3f(x2, y2, z2);
     glEnd();
     break;
  case TLP_TEXTURE:
    glDisable(GL_BLEND);
    glBegin(GL_LINES);
    glVertex3f(x1, y1, z1);
    glVertex3f(x2, y2, z2);
    glEnd();
    glEnable(GL_BLEND);
  }
}

