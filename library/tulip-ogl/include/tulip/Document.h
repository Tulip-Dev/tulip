#ifndef _DOCUMENT_H_
#define _DOCUMENT_H_

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

#endif
