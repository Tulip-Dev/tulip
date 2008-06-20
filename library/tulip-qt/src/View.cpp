#include "tulip/View.h"

using namespace std;

namespace tlp {

  TemplateFactory<ViewCreatorFactory,ViewCreator, ViewCreatorContext *> *ViewCreatorFactory::factory;  

  View::View(QWidget *parent) :QMainWindow(parent) 
  {}

}
