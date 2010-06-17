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
#ifndef _FONTS_H_
#define _FONTS_H_
#ifndef DOXYGEN_NOTFOR_DEVEL

#include <string>

#include <tulip/tulipconf.h>

namespace tlp {

enum FontMode{TLP_BITMAP, TLP_PIXMAP, TLP_OUTLINE, TLP_POLYGON, TLP_EXTRUDE, TLP_TEXTURE, TLP_TLPPIXMAP};

class TLP_GL_SCOPE Fonts {
  friend class TextRenderer;

 public:
  virtual ~Fonts() {}
  virtual void drawString(const std::string &str, int indice=-1) const = 0;
  virtual float getAdvance(const  std::string &str, int index=-1) const = 0;
  virtual float getAscender(int index = -1) const = 0;
  virtual float getDescender(int index = -1) const = 0; 
  virtual bool ActiveFont(int indice) = 0;
  virtual bool ActiveFont(FontMode t, int s, const std::string &f, float d = 0) = 0;
  virtual int AddFont(FontMode type, int size, const std::string &f, float depth=0) = 0;
  virtual int searchFont(FontMode type, int size, const std::string &f, float depth=0) const = 0;
};

}

#endif //DOXYGEN_NOTFOR_DEVEL
#endif
