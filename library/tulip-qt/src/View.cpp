#include "tulip/View.h"

using namespace std;

namespace tlp {

  TemplateFactory<ViewFactory,View, ViewContext *> *ViewFactory::factory;

}
