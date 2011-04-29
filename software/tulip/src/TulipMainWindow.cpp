#include "TulipMainWindow.h"

#include <tulip/TlpTools.h>

TulipMainWindow::TulipMainWindow(QWidget *parent): QMainWindow(parent), _ui(new Ui::TulipMainWindowData()) {
  _ui->setupUi(this);
  _ui->mainLogo->setPixmap(QPixmap((tlp::TulipBitmapDir + "/welcomelogo.bmp").c_str()));
}

TulipMainWindow::~TulipMainWindow() {
}

void TulipMainWindow::startApp() {

}
