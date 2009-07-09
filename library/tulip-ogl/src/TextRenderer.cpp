#include <tulip/TlpTools.h>
#include "tulip/TextRenderer.h"
#include "tulip/Renderer.h"
#include "tulip/GlRenderer.h"
#include "tulip/Document.h"
#include "tulip/Paragraph.h"
#include "tulip/Hline.h"
#include "tulip/Table.h"
#include "tulip/Parser.h"

#include <iostream>
#include <cassert>
#include <stack>
#include <string.h>

using namespace std;
using namespace tlp;

const FontMode DEFAULT_FONTMODE = TLP_POLYGON;
const float DEFAULT_PROF = 20;

const int H1 = 20;
const int H2 = 15;
const int H3 = 10;
const int H4 =  5;

const int DEFAULT_SIZE = 20;

string getFontFile() {
  return tlp::TulipLibDir + "tlp/bitmaps/" + "font.ttf";
}

struct Cell{
  bool new_context;  // si vrai : ordonne un dépilement de contexte du Document courant
  xmlNodePtr pt; // contient le noeud à traiter de l'arbre
};

typedef stack<Cell*> sCell; // pile

//***************************************************************************
// CLASSE TextRenderer
//***************************************************************************
//---------------------------------------------------------------------------
TextRenderer::TextRenderer(): c(getFontFile(), DEFAULT_SIZE, ROUGE, VERT, BLEU) {
  c.renderer = new GlRenderer();
  doc = 0;
}
TextRenderer::~TextRenderer() {
  delete c.renderer;
  if (doc!=0) delete doc;
}
//---------------------------------------------------------------------------
void TextRenderer::initTextManager(const string &str){
  string s_local = "";
  Paragraph* f;
  for(unsigned int i=0; i<str.size(); ++i){
    switch(str[i]){
    case '\n':
      f = new Paragraph(c, doc->getAlign());
      f->addString(s_local + " ",doc->getContext());
      f->addString("", doc->getContext());
      s_local = "";
      doc->addFrame(f);
      break;
    case '\t':
      s_local = s_local + "  ";
      break;
    default:
      s_local.push_back(str[i]);
      break;
    }
  }
  if(s_local != ""){
    f = new Paragraph(c, doc->getAlign());
    f->addString(s_local + " ", doc->getContext());
    doc->addFrame(f);
  }
}
//---------------------------------------------------------------------------
void TextRenderer::draw(float w_max, float& w, int relPos) const{
  float h = 0;
  w = w_max;
  if(doc){
    doc->getBoundingBox(w_max, h, w);
    // Quick fix to center correctly text need more rigourus fix.
    float dx = -(w-3.5)/2.;
    float dy = (h-2.5)/2.;

    switch(relPos) {
    case ON_TOP:
      dy += h/2.;
      break;
    case ON_BOTTOM:
      dy -= h/2.;
      break;
    case ON_LEFT:
      dx -= w/2.;
      break;
    case ON_RIGHT:
      dx += w/2.;
    }
    c.getRenderer().translate(dx, dy, 0);
    doc->draw(w_max, w);
    if(w<w_max) w = w_max;
  }
}
//---------------------------------------------------------------------------
// pour éviter une meilleure reconnaissant des tags et attributs
char* nameManager(char* nom){ 
  char* tmp = nom;
  int n = strlen(tmp);
  for(int i= 0; i<n; i++) tmp[i] = tolower(tmp[i]); // met tout en minuscules
  nom = strtok(tmp," "); // enlève les espaces avant et après
  return (char*)nom;
}
//---------------------------------------------------------------------------
// sépare chaque mot et construit une frame Paragrphh correspondante
// d : le document dans lequel le paragraph sera inséré, f : paragraph déjà ouvert s'il y a 
// texte : -> à traiter
Paragraph* TextRenderer::textXMLManager(Document* d, Paragraph* f, char* text){
  bool prem_mot, dern_mot;
  char* p;
  
  int n = strlen(text); // test qui permet de savoir si un mot a été coupé
  if( (text[n-1] == ' ')||
      (text[n-1] == '\n')||
      (text[n-1] == '\t') )    dern_mot = false; 
  else    dern_mot = true; // le dernier mot n'est pas forcément entier

  if( (text[0] == ' ')||
      (text[0] == '\n')||
      (text[0] == '\t') )    prem_mot = false;
  else    prem_mot = true; // le premier mot n'est pas forcément entier
  
  p = strtok((char*)text, " \n\r\t");
  if(p != 0){
    if(!f) f = new Paragraph(c, d->getAlign());
    
    string str = p;
    if(!prem_mot)
      str = " "+str;
    
    p = strtok(0," \n\r\t");
    if(p || !dern_mot)
      str = str + " ";
    else
      str = str;
    
    
    if(str != "")
      f->addString(str, d->getContext());
    
    while(p != 0){
      str = p;
      p = strtok(0," \n\r\t");
      if(p || !dern_mot)
	str = " "+str + " ";
      else
	str = " "+str;
     
      f->addString(str, d->getContext());
    }
  }
  return f;
}
//---------------------------------------------------------------------------
void TextRenderer::finalTextXMLManager(Paragraph* f){
  std::vector<C_String>* myString = f->getMyString();
  int n = myString->size();
  string* s, *prec_s;
  prec_s = 0;
  for(int i=0; i< n; i++){
    s = myString->at(i).getStringPtr();
    if(strcmp(s->c_str(),"")){
      int length_s = s->size();
      if((*s)[length_s-1] == ' '){
	if((*s)[0] == ' '){
	  if(prec_s != 0){
	    int length_p = prec_s->size();
	    if((*prec_s)[length_p -1] != ' ')
	      prec_s->push_back(' ');
	  }
	  s->erase(0,1);
	}
      }
      else
	if((*s)[0] == ' '){
	  s->erase(0, 1);
	  if((prec_s != 0) && (strcmp(prec_s->c_str(),""))){
	    int length_p = prec_s->size();
	    if((*prec_s)[length_p -1] != ' ')
	      (*prec_s) = (*prec_s) + ' ';
	  }
	}
    }
    else{
      if((prec_s != 0) && (strcmp(prec_s->c_str(),""))){
	int length_p = prec_s->size();
	 if((*prec_s)[length_p -1] != ' ')
	  (*prec_s) = (*prec_s) + ' ';
      }
    }
    prec_s = s;
  }
  s = myString->at(n-1).getStringPtr();
  if (s->size()){
    int length_s = s->size();
    if((*s)[length_s-1] != ' '){
      s->push_back(' ');
    }
  }
}

//---------------------------------------------------------------------------
void TextRenderer::initTextXMLManager(Parser* P, xmlNodePtr courant, Document* d){
  assert(d && P);
  if(courant != 0){
    char* tmp = (char*)P->getName(courant); // on récupère le nom du noeud
    if(P->getType(courant) != XML_TEXT_NODE) // nameManager plante pour un texte
      tmp = nameManager(tmp);
    if( (P->getType(courant) ==  XML_TEXT_NODE) ||
	((P->getType(courant) ==  XML_ELEMENT_NODE) &&( !strcmp(tmp,"font")||!strcmp(tmp,"br")) ) ){ 
      // zone de texte : traitement itératif en priorité
      
      Paragraph* frame = 0;
      sCell pile; // contient les cellules à parcourir
      Cell* cell, *tmp_cell;
      
      cell = new Cell();
      cell->pt = courant;
      cell->new_context = false;
      pile.push(cell); // empile le premier état à regarder
      
      while(!pile.empty()){
	tmp_cell = pile.top(); // récupère la plus haute valeur
	pile.pop(); // on depile
	if(tmp_cell->new_context) d->removeContext(); // on a terminé une branche de type "font"
	if(tmp_cell->pt){ // le noeud n'est pas 0
	  
	  if(P->getType(tmp_cell->pt)== XML_TEXT_NODE){
	    frame = textXMLManager(d, frame,(char*)P->getContent(tmp_cell->pt));

	    cell = new Cell;
	    cell->new_context = false;
	    cell->pt = P->Next(tmp_cell->pt);
	    pile.push(cell);
	  }
	  else if(P->getType(tmp_cell->pt) == XML_ELEMENT_NODE){
	    
	    char* tmp = nameManager((char*)P->getName(tmp_cell->pt));
	    
	    if(!strcmp(tmp,"font")){
	      // changement de contexte
	      xmlAttr* properties;
	      xmlNodePtr contents;
	      
	      int size = d->getContext().getSize(); 
	      string font_name = d->getContext().getFontName();
	      
	      unsigned char r, v, b;
	      d->getContext().getColor(r, v, b);
	      
	      // -> traitement des attributs
	      properties = P->Attribut(tmp_cell->pt); 
	      if(properties != 0){
		do{
		  contents = P->Value(properties);
		  if(contents){
		    tmp = nameManager((char* )P->getName(properties));
		    
		    if(!strcmp(tmp,"size")){
		      char nb[2];
		      char* tmp = (char*)P->getContent(contents);
		      if(tmp[0] == '-'){
			sscanf(tmp+1,"%c",&nb[0]);
			nb[1] = '\0';
			if((atoi(nb) != 0) && (size - atoi(nb) > 0))
			  size -= atoi(nb);			  
			
		      }
		      else if(tmp[0] == '+'){
			sscanf(tmp+1,"%c",&nb[0]);
			nb[1] = '\0';
			if(atoi(nb) != 0)
			  size += atoi(nb);
		      }
		      else
			if(atoi((char*)P->getContent(contents)) != 0)
			  size = atoi((char*)P->getContent(contents));
		    }
		    else if(!strcmp(tmp,"face")){		     
		      font_name = (char*)(P->getContent(contents));
		    }
		    else if(!strcmp(tmp,"color")){
		      tmp = strtok((char*) P->getContent(contents)," ");
		      char c;
		      int val;
		      sscanf(tmp,"%c%x",&c,&val);
		      assert(c == '#');

		      r = (unsigned char)((val & 0xFF0000)>>16);
		      v = (unsigned char)((val & 0x00FF00)>>8);
		      b = (unsigned char)(val & 0x0000FF); 
		    }
		  }		  
		  properties = P->Next(properties);
		}
		while(properties); // fin de traitement des attributs
		
		Context c = Context(font_name,size,r,v,b);
		d->setContext(c);
		
		cell = new Cell;
		cell->new_context = true;
		cell->pt = P->Next(tmp_cell->pt);
		pile.push(cell);
	      }
	      else{
		cell = new Cell;
		cell->new_context = false;
		cell->pt = P->Next(tmp_cell->pt);
		pile.push(cell);
	      }

	      cell = new Cell;
	      cell->new_context = false;
	      cell->pt = P->Child(tmp_cell->pt);
	      pile.push(cell);
	    }	  
	    else if(!strcmp(tmp,"br")){

	      if(!frame) frame = new Paragraph(c, d->getAlign());
	      frame->addString("",d->getContext());

	      cell = new Cell;
	      cell->new_context = false;
	      cell->pt = P->Next(tmp_cell->pt);
	      pile.push(cell);
	    }
	    else{
	      if(frame){
		finalTextXMLManager(frame);
		d->addFrame(frame);
	      }

	      initTextXMLManager(P, tmp_cell->pt, d);

	      frame = 0;
	    }
	  }
	  else{
	    cerr<<" noeud nom traité ..."<<endl;
	  }
	}
	delete tmp_cell;
      }
      if(frame){
	finalTextXMLManager(frame);
	d->addFrame(frame);
      }
    }
    else if( P->getType(courant) == XML_ELEMENT_NODE ){
      if(!strcmp(tmp,"h1")||!strcmp(tmp,"h2")||!strcmp(tmp,"h3")||!strcmp(tmp,"h4")){
	//-------- H* --------
	// création d'un contexte
	unsigned char r, v, b;
	d->getContext().getColor(r, v, b);
	
	if(!strcmp(tmp,"h1")){
	  Context c = Context(d->getContext().getFontName(),d->getContext().getSize()+H1,r, v, b);
	  d->setContext(c);
	}
	if(!strcmp(tmp,"h2")){
	  Context c = Context(d->getContext().getFontName(),d->getContext().getSize()+H2,r, v, b);
	  d->setContext(c);
	}
	if(!strcmp(tmp,"h3")){
	  Context c = Context(d->getContext().getFontName(),d->getContext().getSize()+H3,r, v, b);
	  d->setContext(c);
	}
	if(!strcmp(tmp,"h4")){
	  Context c = Context(d->getContext().getFontName(),d->getContext().getSize()+H4,r, v, b);
	  d->setContext(c);
	}
	
	initTextXMLManager(P, P->Child(courant),d);
	
	d->removeContext();
      }
      else if(!strcmp(tmp,"right")){
	//-------- RIGHT --------
	d->setAlign(RIGHT);
	initTextXMLManager(P, P->Child(courant),d);
	d->removeAlign();
      }
      else if(!strcmp(tmp,"left")){
	//-------- LEFT --------
	d->setAlign(LEFT);
	initTextXMLManager(P, P->Child(courant),d);
	d->removeAlign();
      }
      else if(!strcmp(tmp,"center")){
	//-------- CENTER --------
	d->setAlign(CENTER);
	initTextXMLManager(P, P->Child(courant),d);
	d->removeAlign();
      }
      else if(!strcmp(tmp,"hline")){
	//-------- HLINE --------
	xmlAttr* properties;
	xmlNodePtr contents;

	Hline* f = new Hline(c.getRenderer());
	unsigned char r, v, b;
	f->getColor(r, v, b);

	properties = P->Attribut(courant);
	if(properties != 0){
	  do{
	    contents = P->Value(properties);
	    if(contents){
	      tmp = nameManager((char* )P->getName(properties));
	      
	      if(!strcmp(tmp,"color")){
		tmp = strtok((char*) P->getContent(contents)," ");
		char c;
		int val;
		sscanf(tmp,"%c%x",&c,&val);
		assert(c == '#');
		r = (unsigned char)((val & 0xFF0000)>>16);
		v = (unsigned char)((val & 0x00FF00)>>8);
		b = (unsigned char)(val & 0x0000FF); 
	      }
	    }		  
	    properties = P->Next(properties);
	  }
	  while(properties);
	  f->setColor(r, v, b);
	}

	d->addFrame(f);
      }
      else if(!strcmp(tmp,"table")){	
	//-------- TABLE --------
	
	int nbcols = 0,max_nbcols = 0, nblignes = 0;
	xmlNodePtr children = P->Child(courant), c_children;
	  
	// on recherche le nombre de ligne
	while(children){
	  if( P->getType(children) == XML_ELEMENT_NODE ){
	    char* tmp =  nameManager((char*)P->getName(children));
	    if(!strcmp(tmp,"tr")){
	      ++nblignes;
	      c_children = P->Child(children);

	      nbcols = 0;
	      while(c_children){
		if( P->getType(c_children) == XML_ELEMENT_NODE ){
		  char* tmp =  nameManager((char*)P->getName(c_children));
		  if(!strcmp(tmp,"td"))
		    ++nbcols;
		}
		c_children = P->Next(c_children);
	      }
	      if( max_nbcols < nbcols) max_nbcols = nbcols;
	    }
	  }// on ignore les tag de type texte
	  children = P->Next(children);
	}

	Table* f = new Table(c, nblignes, max_nbcols);

	// lecture des attributs
	xmlAttr* properties;
	xmlNodePtr contents;
	
	unsigned char r, v, b;
	f->getColor(r, v, b);
	
	properties = P->Attribut(courant);
	if(properties != 0){
	  do{
	    contents = P->Value(properties);
	    if(contents){
	      tmp = nameManager((char* )P->getName(properties));
	      
	      if(!strcmp(tmp,"color")){
		tmp = strtok((char*) P->getContent(contents)," ");
		char c;
		int val;
		scanf(tmp,"%c%x",&c,&val);
		assert(c == '#');
		r = (unsigned char)((val & 0xFF0000)>>16);
		v = (unsigned char)((val & 0x00FF00)>>8);
		b = (unsigned char)(val & 0x0000FF); 
	      }
	    }		  
	    properties = P->Next(properties);
	  }
	  while(properties);
	  f->setColor(r, v, b);
	}

	nblignes = 0;

	// on stocke les informations
	children =  P->Child(courant);
	while(children){
	  if( P->getType(children) == XML_ELEMENT_NODE ){
	    char* tmp =  nameManager((char*)P->getName(children));
	    if(!strcmp(tmp,"tr")){
	      c_children = P->Child(children);
	      nbcols = 0;
	      while(c_children){
		if( P->getType(c_children) == XML_ELEMENT_NODE ){
		  char* tmp =  nameManager((char*)P->getName(c_children));
		  if(!strcmp(tmp,"td")){
		    Document* doc = new Document(d->getContext(), d->getAlign());
		    f->addFrame(doc, nblignes, nbcols); 
		    initTextXMLManager(P,P->Child(c_children), doc);
		    ++nbcols;
		  }
		}
		c_children = P->Next(c_children);
	      }
	      ++nblignes;
	    }
	  }// on ignore les tag de type texte
	  children = P->Next(children);
	}
	
	d->addFrame(f);	
      }
      else{
	//cout<<"Main warning, le tag n'est pas gere : "<<tmp<<endl;
	initTextXMLManager(P, P->Child(courant), d);
      }
      initTextXMLManager(P, P->Next(courant),d);
    }
  }  
}

//---------------------------------------------------------------------------
void TextRenderer::getBoundingBox(float w_max, float& h, float& w) const{
  if(doc!=0){
    doc->getBoundingBox(w_max, h, w);
    //    if( w < w_max) w = w_max;
  }    
  else{
    cerr<<"TextRenderer warning : no document defined"<<endl;
    h = 0;
    w = w_max;
  }
}

//---------------------------------------------------------------------------
void TextRenderer::setString(const std::string &str, TextMode mode){
  if(doc != 0){
    delete doc;
    doc = 0;
  }

  if( str != ""){
    doc = new Document();
    doc->setContext(c); // set default context
    doc->setDefaultAlign();
    
    if(mode == XML) {
      string s = "<document>" + str + "</document>";
      Parser P(s.c_str()); // parse le texte
      initTextXMLManager(&P, P.getHead(), doc); 
    }
    else{ // mode verbatim
      initTextManager(str);
    }
  }
}
//---------------------------------------------------------------------------
void TextRenderer::setMode(FontMode m) {
  c.getRenderer().setMode(m);
}
//---------------------------------------------------------------------------
void TextRenderer::setContext(const std::string &font, int size, unsigned char r, unsigned char v, unsigned char b){
  c.setFontName(font);
  c.setSize(size);
  c.setColor(r, v, b);
}
//---------------------------------------------------------------------------
void TextRenderer::setColor(unsigned char r, unsigned char g, unsigned char b){
  c.setColor(r, g, b);
}
