/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
#include "tulip/InteractorChainOfResponsibility.h"

using namespace tlp;
using namespace std;

InteractorChainOfResponsibility::InteractorChainOfResponsibility(const QString &iconPath,const QString &text):view(NULL),interactorIconPath(iconPath),interactorText(text),action(NULL) {
}

InteractorChainOfResponsibility::~InteractorChainOfResponsibility() {
  for(vector<InteractorComponent *>::iterator it =interactorComponents.begin(); it != interactorComponents.end(); ++it) {
    delete(*it);
  }

  delete action;
}

void InteractorChainOfResponsibility::setView(View *view){
  construct();
  this->view=view;
}

void InteractorChainOfResponsibility::install(QWidget *widget) {
  int id=0;
  for(vector<InteractorComponent *>::iterator it =interactorComponents.begin(); it != interactorComponents.end(); ++it) {
    InteractorComponent *newInteractor=(*it)->clone();
    newInteractor->setView(view);
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
