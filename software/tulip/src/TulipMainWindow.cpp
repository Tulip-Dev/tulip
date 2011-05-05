#include "TulipMainWindow.h"

#include <QtGui/QMenu>
#include <QtGui/QCloseEvent>
#include <QtCore/QFile>

#include <tulip/TlpTools.h>
#include <tulip/TulipRelease.h>

TulipMainWindow::TulipMainWindow(QWidget *parent): QMainWindow(parent), _ui(new Ui::TulipMainWindowData()), _systemTrayIcon(0) {
  _ui->setupUi(this);
  _ui->mainLogo->setPixmap(QPixmap((tlp::TulipBitmapDir + "/welcomelogo.bmp").c_str()));
  setFixedSize(800,577);

  connect(_ui->welcomePageChooser,SIGNAL(clicked()),this,SLOT(pageChooserClicked()));
  connect(_ui->pluginsPageChooser,SIGNAL(clicked()),this,SLOT(pageChooserClicked()));
  connect(_ui->aboutPageChooser,SIGNAL(clicked()),this,SLOT(pageChooserClicked()));

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
  connect(systemTrayMenu->addAction(trUtf8("Exit")), SIGNAL(triggered()),this, SLOT(closeApp()));
  _systemTrayIcon->setContextMenu(systemTrayMenu);
  connect(_systemTrayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(systemTrayRequest(QSystemTrayIcon::ActivationReason)));
}

TulipMainWindow::~TulipMainWindow() {
}

void TulipMainWindow::startApp() {
  show();
  _systemTrayIcon->show();
}

void TulipMainWindow::closeApp() {
  _systemTrayIcon->hide();
  delete _systemTrayIcon;
  QApplication::exit();
}

void TulipMainWindow::closeEvent(QCloseEvent *e) {
  e->ignore();
  hide();
}

void TulipMainWindow::systemTrayRequest(QSystemTrayIcon::ActivationReason reason) {
  if (reason == QSystemTrayIcon::Trigger)
    setVisible(!isVisible());
}

void TulipMainWindow::showPage(int i) {
  if (!isVisible())
    setVisible(true);
  _ui->pages->setCurrentIndex(i);
}

void TulipMainWindow::pageChooserClicked() {
  if (sender() == _ui->welcomePageChooser)
    showPage(0);
  else if (sender() == _ui->pluginsPageChooser)
    showPage(1);
  else
    showPage(2);
}
