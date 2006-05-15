#ifndef _FONTS_H_
#define _FONTS_H_

#include <string>

namespace tlp {

enum FontMode{TLP_BITMAP, TLP_PIXMAP, TLP_OUTLINE, TLP_POLYGON, TLP_EXTRUDE, TLP_TEXTURE, TLP_TLPPIXMAP};

class Fonts {
  friend class TextRenderer;

 public:
  virtual void drawString(const std::string str, int indice=-1)const=0;
  virtual float getAdvance(const  std::string str, int index=-1)const=0;
  virtual float getAscender(int index=-1)const=0;
  virtual float getDescender(int index=-1)const=0; 
  virtual bool ActiveFont(int indice)=0;
  virtual bool ActiveFont(FontMode t, int s, const std::string f, float d=0)=0;
  virtual int AddFont(FontMode type, int size, const std::string f, float depth=0)=0;
  virtual int searchFont(FontMode type, int size, const std::string f, float depth=0)const=0;
};

}

#endif
