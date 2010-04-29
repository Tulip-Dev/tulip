#ifndef Tulip_QTOPENGLERRORVIEWER_H
#define Tulip_QTOPENGLERRORVIEWER_H

#include <QtGui/QWidget>

#include <tulip/OpenGlErrorViewer.h>

namespace tlp {

class TLP_QT_SCOPE QtOpenGlErrorViewer : public OpenGlErrorViewer {

public :

  QtOpenGlErrorViewer(QWidget *parent):parent(parent){}

  virtual void displayError(const std::string &fileName,const std::string &errorMsg);

protected :

  QWidget *parent;

};

}

#endif
