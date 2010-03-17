#ifndef Tulip_QTTEXTUREMANAGERERRORVIEWER_H
#define Tulip_QTTEXTUREMANAGERERRORVIEWER_H

#include <QtGui/QWidget>

#include <tulip/GlTextureManager.h>

namespace tlp {

class TLP_QT_SCOPE QtTextureManagerErrorViewer : public GlTextureManagerErrorViewer {

public :

  QtTextureManagerErrorViewer(QWidget *parent):parent(parent){}

  virtual void displayError(const std::string &fileName,const std::string &errorMsg);

protected :

  QWidget *parent;
  std::map<std::string,std::string> lastErrorForFile;

};

}

#endif
