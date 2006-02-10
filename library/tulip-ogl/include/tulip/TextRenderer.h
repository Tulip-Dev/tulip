#ifndef _TEXTRENDERER_H_
#define _TEXTRENDERER_H_

#include <string>
#include <libxml/parser.h>
#include <tulip/tulipconf.h>
#include <tulip/Context.h>
#include <tulip/Fonts.h>

enum TextMode{XML,VERBATIM};

class Paragraph;
class Document;
class Parser;


class TLP_GL_SCOPE TextRenderer{

 public:
  TextRenderer();
  ~TextRenderer();
  void draw(float w_max, float& w) const;
  void getBoundingBox(float w_max, float& h, float& w) const;
  void setString(std::string str, TextMode mode);
  void setContext(std::string str, int s, unsigned char r, unsigned char v, unsigned char b);
  void setColor(unsigned char r, unsigned char g, unsigned char b);
  void setMode(FontMode  m);

 private:  
  Document* doc;
  Context c;
  void initTextManager(std::string str);
  Paragraph* textXMLManager(Document* d, Paragraph* f, char* text);
  void initTextXMLManager(Parser* P, xmlNodePtr courant, Document* d);
  void finalTextXMLManager(Paragraph* f);
};
#endif
