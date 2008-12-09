#include "tulip/GWInteractor.h"

using namespace tlp;
using namespace std;

TemplateFactory<InteractorFactory,Interactor, InteractorContext *> *InteractorFactory::factory;
