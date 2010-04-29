#include "tulip/QtOpenGlErrorViewer.h"

#include <QtGui/QMessageBox>

namespace tlp {

void QtOpenGlErrorViewer::displayError(const std::string &fileName,const std::string &errorMsg){
  QMessageBox::warning(parent, "Texture manager warning", errorMsg.c_str());
}

}
