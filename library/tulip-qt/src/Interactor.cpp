#include "tulip/Interactor.h"

using namespace tlp;
using namespace std;

TemplateFactory<InteractorFactory,Interactor, InteractorContext *> *InteractorFactory::factory;

InteractorAction::InteractorAction(Interactor *interactor,const QIcon &icon, const QString &text):QAction(icon,text,NULL),interactor(interactor){

}
