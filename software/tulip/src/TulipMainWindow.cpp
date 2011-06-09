#include "TulipMainWindow.h"

#include <QtGui/QMenu>
#include <QtGui/QCloseEvent>
#include <QtCore/QFile>
#include <QtCore/QProcess>
#include <QtCore/QDir>

#include <tulip/TlpTools.h>
#include <tulip/TulipRelease.h>

#include "ui_TulipMainWindow.h"

#include <QtCore/QDebug>

TulipMainWindow::TulipMainWindow(QWidget *parent): QMainWindow(parent), _ui(new Ui::TulipMainWindowData()), _systemTrayIcon(0) {
  _ui->setupUi(this);
  _pageChoosers.clear();
  _pageChoosers.push_back(_ui->welcomePageChooser);
  _pageChoosers.push_back(_ui->pluginsPageChooser);
  _pageChoosers.push_back(_ui->aboutPageChooser);

  _ui->mainLogo->setPixmap(QPixmap((tlp::TulipBitmapDir + "/welcomelogo.bmp").c_str()));

  connect(_ui->welcomePageChooser,SIGNAL(clicked()),this,SLOT(pageChooserClicked()));
  connect(_ui->pluginsPageChooser,SIGNAL(clicked()),this,SLOT(pageChooserClicked()));
  connect(_ui->aboutPageChooser,SIGNAL(clicked()),this,SLOT(pageChooserClicked()));

  //System tray
  _systemTrayIcon = new QSystemTrayIcon(QIcon(":/tulip/gui/icons/logo32x32.png"),this);
  _systemTrayIcon->setToolTip(trUtf8("Tulip agent"));
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
  connect(_systemTrayIcon,SIGNAL(messageClicked()),this,SLOT(systemTrayMessageClicked()));
  connect(_ui->pages,SIGNAL(currentChanged(int)),this,SLOT(pageSwitched(int)));
  connect(_ui->welcomePage,SIGNAL(openPerspective(QString)),this,SLOT(CreatePerspective(QString)));
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
  QApplication::exit(0);
}

void TulipMainWindow::closeEvent(QCloseEvent *e) {
  e->ignore();
  hide();
}

void TulipMainWindow::pageChooserClicked() {
  if (!isVisible())
    setVisible(true);
  _ui->pages->setCurrentIndex(_pageChoosers.indexOf(sender()));
}

void TulipMainWindow::pageSwitched(int i) {
  if (i >= _pageChoosers.size() || i < 0)
    return;
  static_cast<QToolButton *>(_pageChoosers[i])->setChecked(true);
}

void TulipMainWindow::systemTrayRequest(QSystemTrayIcon::ActivationReason reason) {
  if (reason == QSystemTrayIcon::Trigger)
    setVisible(!isVisible());
}

void TulipMainWindow::systemTrayMessageClicked() {
  if (_currentTrayMessage == PluginErrorMessage)
    _ui->pluginsPage->showErrorsPage();
  _currentTrayMessage = NoMessage;
}

PluginsCenter *TulipMainWindow::pluginsCenter() const {
  return _ui->pluginsPage;
}


// Methods and properties coming from the D-Bus implementation
qlonglong TulipMainWindow::pid() const {
  return QApplication::applicationPid();
}

void TulipMainWindow::ShowAboutPage() {
  if (!isVisible())
    setVisible(true);
  raise();
  _ui->pages->setCurrentWidget(_ui->aboutPage);
}

void TulipMainWindow::ShowPluginsCenter() {
  if (!isVisible())
    setVisible(true);
  raise();
  _ui->pages->setCurrentWidget(_ui->pluginsPage);
}

void TulipMainWindow::ShowWelcomeScreen() {
  if (!isVisible())
    setVisible(true);
  raise();
  _ui->pages->setCurrentWidget(_ui->welcomePage);
}

void TulipMainWindow::AddPluginRepository(const QString &url) {

}

void TulipMainWindow::RemovePluginRepository(const QString &url) {

}

void TulipMainWindow::CreatePerspective(const QString &name) {
  runPerspectiveProcess(name,"");
}

void TulipMainWindow::ShowOpenProjectWindow() {

}

void TulipMainWindow::OpenProject(const QString &file) {

}

void TulipMainWindow::OpenProjectWith(const QString &file, const QString &perspective) {

}

QStringList TulipMainWindow::GetCompatiblePerspectives(const QString &file) {
  return QStringList();
}

void TulipMainWindow::runPerspectiveProcess(const QString &perspective, const QString &file) {
  QStringList args;
  if (!perspective.isEmpty())
    args << "--perspective=" + perspective;
  if (!file.isEmpty())
    args << file;
  QDir appDir(QApplication::applicationDirPath());
  QProcess::startDetached(appDir.absoluteFilePath("tulip_perspective"),args);
}
