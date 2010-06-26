/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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
#ifndef _DOCUMENT_H_
#define _DOCUMENT_H_
#ifndef DOXYGEN_NOTFOR_DEVEL

#include "FLayout.h"
#include "Context.h"
#include "Paragraph.h" 

#include <vector>
#include <stack>

namespace tlp {

typedef std::vector<Frame*, std::allocator<Frame*> > VFrame; // tableau de Frames : contenu du document
typedef std::stack<Context> SContext; // la pile de context nécessaire lors d'un ajout de frames
typedef std::stack<Align> SAlign; // pile des alignements, dans le cas, où les <tags> sont imbriqués

class Document : public FLayout{

  VFrame blocks;
  SContext contexts;
  SAlign aligns;

 public:
  Document();
  Document(Context c, Align a);
  virtual ~Document();

  void setDefaultAlign();
  void setContext(Context c);
  void setAlign(Align a);
  void removeContext();
  void removeAlign();
  Context getContext() const;
  Align getAlign() const;
  void draw(float w_max, float& w) const;
  void getBoundingBox(float w_max, float& h, float& w) const;
  bool addFrame(Frame* f);
};

}

#endif //DOXYGEN_NOTFOR_DEVEL
#endif
