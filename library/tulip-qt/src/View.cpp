#include "tulip/View.h"

using namespace std;

namespace tlp {

  TemplateFactory<ViewCreatorFactory,ViewCreator, ViewCreatorContext *> *ViewCreatorFactory::factory;  

  View::View(const std::string &pluginName,QWidget *parent) :pluginName(pluginName),QMainWindow(parent) 
  {}

}
