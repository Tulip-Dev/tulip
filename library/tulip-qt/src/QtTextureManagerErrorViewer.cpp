#include "tulip/QtTextureManagerErrorViewer.h"

#include <QtGui/QMessageBox>

namespace tlp {

void QtTextureManagerErrorViewer::displayError(const std::string &fileName,const std::string &errorMsg){
  QMessageBox::warning(parent, "Texture manager warning", errorMsg.c_str());
}

}
