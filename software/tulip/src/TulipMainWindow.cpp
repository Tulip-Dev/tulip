/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */

#include "TulipMainWindow.h"

#include <QtGui/QMenu>
#include <QtGui/QFileDialog>
#include <QtGui/QCloseEvent>
#include <QtCore/QFile>
#include <QtCore/QProcess>
#include <QtCore/QDir>

#include <tulip/TlpTools.h>
#include <tulip/TulipRelease.h>
#include <tulip/TulipSettings.h>
#include <tulip/TulipProject.h>
#include <QtGui/QScrollArea>

#include "ui_PerspectiveSelectionDialog.h"
#include "ui_TulipMainWindow.h"
#include "TulipPerspectiveProcessHandler.h"
#include "TulipWelcomePage.h"
#include "PerspectiveItemWidget.h"

// Helper classes
PerspectiveSelectionDialog::PerspectiveSelectionDialog(QWidget *parent): QDialog(parent) {
  Ui::PerspectiveSelectionDialogData *ui = new Ui::PerspectiveSelectionDialogData;
  ui->setupUi(this);
  ui->perspectiveDialogScrollContents->setLayout(TulipWelcomePage::buildPerspectiveListLayout(this,SLOT(perspectiveSelected())));
}

void PerspectiveSelectionDialog::perspectiveSelected() {
  PerspectiveItemWidget *item = static_cast<PerspectiveItemWidget *>(sender());
  selectedPerspectiveName = item->perspectiveId();
  close();
}

TulipMainWindow* TulipMainWindow::_instance = NULL;

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
  connect(_ui->welcomePage,SIGNAL(openPerspective(QString)),this,SLOT(createPerspective(QString)));
  connect(_ui->welcomePage,SIGNAL(openProject()),this,SLOT(showOpenProjectWindow()));
  connect(_ui->welcomePage,SIGNAL(openFile(QString)),this,SLOT(openProject(QString)));
  _systemTrayIcon->show();
}

TulipMainWindow::~TulipMainWindow() {
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
  else
    _ui->welcomePageChooser->click();

  show();

  _currentTrayMessage = NoMessage;
}

PluginsCenter *TulipMainWindow::pluginsCenter() const {
  return _ui->pluginsPage;
}

void TulipMainWindow::createPerspective(const QString &name) {
  createPerspective(name,QVariantMap());
}
void TulipMainWindow::showOpenProjectWindow() {
  setVisible(true);
  QString filePath =
    QFileDialog::getOpenFileName(this,
                                 trUtf8("Choose a Tulip project to open with its associated perspective. Or select a external file format to import."),
                                 QDir::homePath(),
                                 "Tulip Files(*.tlp *.tlp.gz *.tlpx);;Tulip Project (*.tlpx);;Tulip Graph (*.tlp *.tlp.gz)");

  if (filePath.isEmpty())
    return;

  openProject(filePath);
}
void TulipMainWindow::openProject(const QString &file) {
  tlp::TulipProject *project = tlp::TulipProject::openProject(file);

  if (!project->isValid()) {
    setVisible(true);
    PerspectiveSelectionDialog dlg(this);
    dlg.exec();

    if (!dlg.selectedPerspectiveName.isNull())
      openProjectWith(file,dlg.selectedPerspectiveName,QVariantMap());
  }
  else
    openProjectWith(file, project->perspective(),QVariantMap());

  delete project;
}

void TulipMainWindow::createPerspective(const QString &name,const QVariantMap &parameters) {
  TulipPerspectiveProcessHandler::instance().createPerspective(name,"",parameters);
}
void TulipMainWindow::openProjectWith(const QString &file, const QString &perspective, const QVariantMap &parameters) {
  TulipSettings::instance().addToRecentDocuments(file);
  TulipPerspectiveProcessHandler::instance().createPerspective(perspective,file,parameters);
}

// Methods and properties coming from the D-Bus implementation
//qlonglong TulipMainWindow::pid() const {
//  return QApplication::applicationPid();
//}

//void TulipMainWindow::ShowAboutPage() {
//  if (!isVisible())
//    setVisible(true);

//  raise();
//  _ui->pages->setCurrentWidget(_ui->aboutPage);
//}

//void TulipMainWindow::ShowPluginsCenter() {
//  if (!isVisible())
//    setVisible(true);

//  raise();
//  _ui->pages->setCurrentWidget(_ui->pluginsPage);
//}

//void TulipMainWindow::ShowWelcomeScreen() {
//  if (!isVisible())
//    setVisible(true);

//  raise();
//  _ui->pages->setCurrentWidget(_ui->welcomePage);
//}

//void TulipMainWindow::AddPluginRepository(const QString &url) {
//  _ui->pluginsPage->addRemoteLocation(url);
//}

//void TulipMainWindow::RemovePluginRepository(const QString &url) {
//  _ui->pluginsPage->removeRemoteLocation(url);
//}


//QStringList TulipMainWindow::GetCompatiblePerspectives(const QString &/*file*/) {
//  return QStringList();
//}

//void TulipMainWindow::EnableCrashHandling(const QString &folder, qlonglong pid) {
//  TulipPerspectiveProcessHandler::instance().enableCrashHandling(pid,folder);
//}

void TulipMainWindow::showTrayMessage(const QString &title, const QString &message, uint icon, uint duration) {
  if (!_systemTrayIcon)
    return;

  _systemTrayIcon->showMessage(title,message,(QSystemTrayIcon::MessageIcon)icon,duration);
}

void TulipMainWindow::pluginErrorMessage(const QString &message) {
  _currentTrayMessage = PluginErrorMessage;
  showTrayMessage(trUtf8("Error while loading plugins"),message + "\n\n" + trUtf8("Click on this message to see detailed informations"),(uint)QSystemTrayIcon::Critical,10000);
}
