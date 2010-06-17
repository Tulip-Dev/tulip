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
#ifndef _CONTEXT_H_
#define _CONTEXT_H_
#ifndef DOXYGEN_NOTFOR_DEVEL

#include <string>

#include <tulip/tulipconf.h>

namespace tlp {

class Renderer;

class TLP_GL_SCOPE Context {
  friend class TextRenderer;
  std::string font_name;
  int size;
  unsigned char c[3]; // 0:red, 1:green, 2:blue
  Renderer * renderer;
  Context(std::string str="", int s=0, unsigned char r=255, unsigned char v=255, unsigned char b=255);

 public:  
  inline std::string getFontName() const{ return font_name; }
  inline int getSize() const{ return size; }
  void getColor(unsigned char& r, unsigned char& v, unsigned char& b) const;
  inline void setFontName(std::string str){ font_name = str; }
  inline void setSize(int s){ size = s; }
  void setColor(unsigned char r, unsigned char v, unsigned char b){ c[0] = r; c[1] = v; c[2] = b;}  
  friend bool operator==(Context& a, Context& b);
  Renderer& getRenderer() const;  
};

}

#endif //DOXYGEN_NOTFOR_DEVEL
#endif
