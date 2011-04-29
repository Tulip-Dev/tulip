#include "TulipMainWindow.h"

#include <QtGui/QSystemTrayIcon>
#include <QtGui/QMenu>

#include <tulip/TlpTools.h>
#include <tulip/TulipRelease.h>

TulipMainWindow::TulipMainWindow(QWidget *parent): QMainWindow(parent), _ui(new Ui::TulipMainWindowData()) {
  _ui->setupUi(this);
  _ui->mainLogo->setPixmap(QPixmap((tlp::TulipBitmapDir + "/welcomelogo.bmp").c_str()));

  //System tray
  _systemTrayIcon = new QSystemTrayIcon(QIcon(":/tulip/app/icons/logo32x32.png"),this);
  _systemTrayIcon->setToolTip(QString("Tulip"));
  QMenu *systemTrayMenu = new QMenu();
  systemTrayMenu->addAction(trUtf8("Show"));
  systemTrayMenu->addAction(trUtf8("Hide"));
  systemTrayMenu->addSeparator();
  systemTrayMenu->addAction(trUtf8("Welcome"));
  systemTrayMenu->addAction(trUtf8("Plugins center"));
  systemTrayMenu->addAction(trUtf8("About us"));
  systemTrayMenu->addSeparator();
  systemTrayMenu->addAction(trUtf8("Exit"));
  _systemTrayIcon->setContextMenu(systemTrayMenu);
}

TulipMainWindow::~TulipMainWindow() {
}

void TulipMainWindow::startApp() {
  show();
  _systemTrayIcon->show();
}
