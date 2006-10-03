#ifndef _GLFONTS_H_
#define _GLFONTS_H_

#ifndef DOXYGEN_NOTFOR_DEVEL

#include <string>
#include <map>
#include <vector>
#include <tulip/Fonts.h>

class FTFont;

#define TAILLE 100
namespace tlp {

//---------------------------------------------------------------------------
class _GlFonts {
  friend class t_GlFonts;
  friend class GlRenderer;

 public:
  _GlFonts(FontMode type, int size, const std::string name, float depth);
  bool operator<(const _GlFonts &) const;

 protected:
  FontMode type;
  int size;
  float depth;
  std::string file;
  FTFont* font;    // appartient à FTGL  
  inline const char* getFileName() const {return file.c_str();}
  inline FontMode getType() const {return type;}
};

//---------------------------------------------------------------------------
class t_GlFonts {
  friend class GlRenderer;
  
 private:
  std::map<_GlFonts,int> fontsIndex;
  std::vector<_GlFonts> fonts;
 protected:
  //  t_GlFonts();
  ~t_GlFonts();

  int Add(FontMode t, int s, float d, const std::string f);
  int searchFont(FontMode type, int size, const std::string f, float depth) const;
  _GlFonts operator[](unsigned int i) const;
};

}

//---------------------------------------------------------------------------

#include <tulip/Renderer.h>

namespace tlp {

class GlRenderer : public Renderer {

 private:
  int current_font; // indice du tableau
  bool active;
  t_GlFonts ftfonts; // contient les différentes polices chargées

 public: 
  // constructeurs et destructeurs
  GlRenderer(){active = false;} // défini des valeurs par défault
  virtual ~GlRenderer(){}

  void drawString(const std::string &str, int index=-1)const;
  void getBBox(const std::string &s, float& x1, float& y1, float& z1, float& x2, float& y2, float& z2);
  float getAdvance(const  std::string &str, int index=-1)const;
  float getAscender(int index = -1)const;
  float getDescender(int index = -1)const; 
  bool ActiveFont(int index);
  bool ActiveFont(FontMode t, int s, const std::string &f, float d=0);
  int AddFont(FontMode type, int size, const std::string &f, float depth=0);
  int searchFont(FontMode type, int size, const std::string &f, float depth=0)const;
  const char* getFontFilename(int index = -1) const; 
  int getFontType(int index = -1) const;
  void translate(float x, float y, float z) const;
  void setColor(unsigned char r, unsigned char v, unsigned char b) const;
  void drawLine(float x1, float y1, float z1, float x2, float y2, float z2) const;
};

}
#endif //DOXYGEN_NOTFOR_DEVEL
#endif

