#include "tulip/InteractorChainOfResponsibility.h"

using namespace tlp;
using namespace std;

InteractorChainOfResponsibility::InteractorChainOfResponsibility(const QString &iconPath,const QString &text):view(NULL),interactorIconPath(iconPath),interactorText(text),action(NULL) {
}

InteractorChainOfResponsibility::~InteractorChainOfResponsibility() {
  for(vector<InteractorComponent *>::iterator it =interactorComponents.begin(); it != interactorComponents.end(); ++it) {
    delete(*it);
  }
  if(action)
    delete action;
}

void InteractorChainOfResponsibility::setView(View *view){
  construct();
  this->view=view;
  for(vector<InteractorComponent *>::iterator it=interactorComponents.begin();it!=interactorComponents.end();++it)
    (*it)->setView(view);
}

void InteractorChainOfResponsibility::install(QWidget *widget) {
  int id=0;
  for(vector<InteractorComponent *>::iterator it =interactorComponents.begin(); it != interactorComponents.end(); ++it) {
    InteractorComponent *newInteractor=(*it)->clone();
    newInteractor->setView((*it)->getView());
    installedInteractorComponents.push_back(newInteractor);
    newInteractor->setID(++id);
    widget->installEventFilter(newInteractor);
  }
}

void InteractorChainOfResponsibility::remove(){
  for(vector<InteractorComponent *>::iterator it =installedInteractorComponents.begin(); it != installedInteractorComponents.end(); ++it) {
    removeEventFilter(*it);
    delete (*it);
  }
  installedInteractorComponents.clear();
}

void InteractorChainOfResponsibility::compute(GlMainWidget *glMainWidget) {
  for(vector<InteractorComponent *>::iterator it=installedInteractorComponents.begin();it!=installedInteractorComponents.end();++it)
    (*it)->compute(glMainWidget);
}

void InteractorChainOfResponsibility::draw(GlMainWidget *glMainWidget) {
  for(vector<InteractorComponent *>::iterator it=installedInteractorComponents.begin();it!=installedInteractorComponents.end();++it)
      (*it)->draw(glMainWidget);
}

void InteractorChainOfResponsibility::pushInteractorComponent(InteractorComponent *component) {
  interactorComponents.push_back(component);
}

InteractorAction* InteractorChainOfResponsibility::getAction(){
  if(!action)
    action = new InteractorAction(this,QIcon(interactorIconPath),interactorText);
  return action;
}
