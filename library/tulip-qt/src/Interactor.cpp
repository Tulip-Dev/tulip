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
#include "tulip/Interactor.h"

#include <QtGui/QTextEdit>

using namespace std;

namespace tlp {

  TemplateFactory<InteractorFactory,Interactor, InteractorContext *> *InteractorFactory::factory;

  InteractorAction::InteractorAction(Interactor *interactor,const QIcon &icon, const QString &text):QAction(icon,text,NULL),interactor(interactor){
  }

  void Interactor::setConfigurationWidgetText(const QString &text){
    configurationWidgetText=text;
  }

  QWidget *Interactor::getConfigurationWidget(){
    if(configurationWidgetText=="")
      return NULL;

    QTextEdit *textEdit=new QTextEdit(configurationWidgetText);
    textEdit->setReadOnly(true);
    return textEdit;
  }

}
