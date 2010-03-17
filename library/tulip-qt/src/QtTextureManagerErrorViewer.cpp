#include "QtTextureManagerErrorViewer.h"

#include <QtGui/QMessageBox>

namespace tlp {

void QtTextureManagerErrorViewer::displayError(const std::string &fileName,const std::string &errorMsg){
  if(lastErrorForFile.count(fileName)!=0){
    if(lastErrorForFile[fileName]==errorMsg)
      return;
  }
  QMessageBox::warning(parent, "Texture manager warning", errorMsg.c_str());
  lastErrorForFile[fileName]=errorMsg;
}

}
