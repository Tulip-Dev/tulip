#ifndef _Tulip_NODELINKDIAGRAMCOMPONENT_H
#define _Tulip_NODELINKDIAGRAMCOMPONENT_H

#include "GlMainView.h"


namespace tlp {

  class TLP_QT_SIMPLE_SCOPE NodeLinkDiagramComponentCreator : public ViewCreator {

  public :
  
    View* create(const std::string &name,QWidget *parent);
  };

  class TLP_QT_SIMPLE_SCOPE NodeLinkDiagramComponent : public GlMainView {
    
    Q_OBJECT;

  public:

    NodeLinkDiagramComponent(const std::string &pluginName,QWidget *parent=NULL, const char *name=NULL);
    ~NodeLinkDiagramComponent();
  
  };

}

#endif






