#ifndef _PARAGRAPH_H_
#define _PARAGRAPH_H_
#ifndef DOXYGEN_NOTFOR_DEVEL

#include "Context.h"
#include "Frame.h"

#include <string>
#include <vector>



namespace tlp {

typedef enum Align{LEFT, RIGHT, CENTER, JUSTIFIED}; // défini l'alignement du texte

class C_String{  // la classe utilisée pour le stockage un texte à afficher
  Context* c;
  std::string s;
 public:
  C_String(std::string str, Context* ctr);
  C_String(const C_String& c_s);
  // ce n'est pas la classe C_String qui détruit le contexte
  inline const std::string& getString() const {return s;}
  inline std::string* getStringPtr() {return &s;}
  inline Context* getContext()const {return c;}
};


class Paragraph : public Frame{
  std::vector<C_String> myString; // le texte à afficher
  std::vector<Context*> listOfcontexts; // tableau de contextes à utiliser
  Context* findContext(Context& c);  // recherche un contexte dans le vecteur
  Align alignement; // le type d'alignement défini

  // routines d'affichages en fonction de l'alignement
  void drawLeft(float w_max, float x_depl, int debut, int fin) const; 
  void drawRight(float w_max, float x_depl, int debut, int fin) const;
  void drawCenter(float w_max, float x_depl, int debut, int fin) const;
  Context &_context;
 public:
  Paragraph(Context &, Align a = LEFT);
  virtual ~Paragraph();
  void draw(float w_max, float& w) const;
  void getBoundingBox(float w_max, float& h, float& w) const;
  bool addString(std::string s, Context c);
  void setAlign(Align h){ alignement = h; }
  std::vector<C_String>* getMyString(){return &myString;}
};

}

#endif //DOXYGEN_NOTFOR_DEVEL
#endif
