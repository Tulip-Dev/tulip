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

    return new QTextEdit(configurationWidgetText);
  }

}
