/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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

#include <QMenu>
#include <QFileDialog>
#include <QCloseEvent>
#include <QFile>
#include <QProcess>
#include <QDir>
#include <QScrollArea>

#include <tulip/TulipRelease.h>
#include <tulip/PythonVersionChecker.h>
#include <tulip/PluginLister.h>
#include <tulip/Perspective.h>
#include <tulip/TlpTools.h>
#include <tulip/TulipSettings.h>
#include <tulip/TulipProject.h>
#include <tulip/PluginModel.h>

#include "ui_TulipMainWindow.h"
#include "TulipPerspectiveProcessHandler.h"
#include "TulipWelcomePage.h"
#include "PerspectiveItemWidget.h"
#include "PerspectiveSelectionDialog.h"

using namespace tlp;

TulipMainWindow* TulipMainWindow::_instance = NULL;

TulipMainWindow::TulipMainWindow(QWidget *parent): QMainWindow(parent), _ui(new Ui::TulipMainWindowData()), _systemTrayIcon(0) {
  _ui->setupUi(this);

  _errorMessage = new QLabel();
  QLabel* errorIcon = new QLabel();
  errorIcon->setMaximumSize(16,16);
  errorIcon->setMinimumSize(16,16);
  errorIcon->setPixmap(QPixmap(":/tulip/app/icons/16/dialog-error.png"));
  _ui->statusBar->insertWidget(0,errorIcon);
  _ui->statusBar->insertWidget(1,_errorMessage);

  _ui->statusBar->setVisible(false);
  _pageChoosers.clear();
  _pageChoosers.push_back(_ui->welcomePageChooser);
  _pageChoosers.push_back(_ui->pluginsPageChooser);
  _pageChoosers.push_back(_ui->aboutPageChooser);

  _ui->mainLogo->setPixmap(QPixmap((tlp::TulipBitmapDir + "/welcomelogo.bmp").c_str()));
  // set title
  QString title("Tulip ");

  // show patch number only if needed
  if (TULIP_INT_VERSION % 10)
    title += TULIP_VERSION;
  else
    title += TULIP_MM_VERSION;

  setWindowTitle(title);
  _ui->mainTitle->setText(QString("<html><head/><body><p align=\"center\"><span style=\" font-family:'MS Shell Dlg 2'; font-size:18pt; font-weight:600;\">") +
                          title + "</span></p></body></html>");

  connect(_ui->welcomePageChooser,SIGNAL(clicked()),this,SLOT(pageChooserClicked()));
  connect(_ui->pluginsPageChooser,SIGNAL(clicked()),this,SLOT(pageChooserClicked()));
  connect(_ui->aboutPageChooser,SIGNAL(clicked()),this,SLOT(pageChooserClicked()));

  //System tray
  _systemTrayIcon = new QSystemTrayIcon(QIcon(":/tulip/gui/icons/tulip.ico"),this);
  _systemTrayIcon->setToolTip(trUtf8("Tulip agent"));
  QMenu *systemTrayMenu = new QMenu();
  systemTrayMenu->addAction(trUtf8("Show"),this,SLOT(showProjectsCenter()));
  systemTrayMenu->addAction(trUtf8("Hide"),this,SLOT(close()));
  systemTrayMenu->addSeparator();
  systemTrayMenu->addAction(trUtf8("Projects"),this,SLOT(showProjectsCenter()));
  systemTrayMenu->addAction(trUtf8("Plugins center"),this,SLOT(showPluginsCenter()));
  systemTrayMenu->addAction(trUtf8("About us"),this,SLOT(showAboutCenter()));
  systemTrayMenu->addSeparator();
  connect(systemTrayMenu->addAction(trUtf8("Exit")), SIGNAL(triggered()),this, SLOT(closeApp()));
  connect(_ui->exitButton, SIGNAL(released()), this, SLOT(closeApp()));
  _systemTrayIcon->setContextMenu(systemTrayMenu);
  connect(_systemTrayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(systemTrayRequest(QSystemTrayIcon::ActivationReason)));
  connect(_systemTrayIcon,SIGNAL(messageClicked()),this,SLOT(systemTrayMessageClicked()));
  connect(_ui->pages,SIGNAL(currentChanged(int)),this,SLOT(pageSwitched(int)));
  connect(_ui->welcomePage,SIGNAL(openPerspective(QString)),this,SLOT(createPerspective(QString)));
  connect(_ui->welcomePage,SIGNAL(openProject()),this,SLOT(showOpenProjectWindow()));
  connect(_ui->welcomePage,SIGNAL(openFile(QString)),this,SLOT(openProject(QString)));
  _systemTrayIcon->show();

  connect(TulipPerspectiveProcessHandler::instance(),SIGNAL(showPluginsAgent()),this,SLOT(showPluginsCenter()));
  connect(TulipPerspectiveProcessHandler::instance(),SIGNAL(showProjectsAgent()),this,SLOT(showProjectsCenter()));
  connect(TulipPerspectiveProcessHandler::instance(),SIGNAL(showAboutAgent()),this,SLOT(showAboutCenter()));
  connect(TulipPerspectiveProcessHandler::instance(),SIGNAL(showTrayMessage(QString)),this,SLOT(showTrayMessage(QString)));
  connect(TulipPerspectiveProcessHandler::instance(),SIGNAL(showErrorMessage(QString,QString)),this,SLOT(showErrorMessage(QString,QString)));
  connect(TulipPerspectiveProcessHandler::instance(),SIGNAL(openProject(QString)),this,SLOT(openProject(QString)));
  connect(TulipPerspectiveProcessHandler::instance(),SIGNAL(openProjectWith(QString,QString)),this,SLOT(openProjectWith(QString,QString)));
  connect(TulipPerspectiveProcessHandler::instance(),SIGNAL(openPerspective(QString)),this,SLOT(createPerspective(QString)));

#ifdef BUILD_PYTHON_COMPONENTS
  checkPython();
#endif
}

TulipMainWindow::~TulipMainWindow() {
}

#ifdef BUILD_PYTHON_COMPONENTS
void TulipMainWindow::checkPython() {
  if (!PythonVersionChecker::isPythonVersionMatching()) {

    QStringList installedPythons = PythonVersionChecker::installedVersions();

    QString requiredPython = "Python " + PythonVersionChecker::compiledVersion();
#ifdef I64
    requiredPython += " (64 bit)";
#else
    requiredPython += " (32 bit)";
#endif

    QString errorMessage;

    if (installedPythons.isEmpty()) {
      errorMessage = requiredPython + " does not seem installed on your system.\nPlease install it in order to use Tulip.";
    }
    else {
      errorMessage = "Python version mismatch. Please install " + requiredPython + " in order to use Tulip.\n";

      if (installedPythons.size() == 1) {
        errorMessage += "Detected version is " + installedPythons.at(0) + ".";
      }
      else {
        errorMessage += "Detected versions are ";

        for (int i = 0 ; i < installedPythons.size() ; ++i) {
          errorMessage += installedPythons.at(i);

          if (i < installedPythons.size() - 1) {
            errorMessage += ", ";
          }
        }

        errorMessage += " .";
      }
    }

    showErrorMessage("Python", errorMessage);
  }
}
#endif

void TulipMainWindow::closeApp() {
  _systemTrayIcon->hide();
  delete _systemTrayIcon;
  QApplication::exit(0);
}

void TulipMainWindow::closeEvent(QCloseEvent *e) {
  e->ignore();

  if (TulipSettings::instance().value("app/showsystraynotif",true).toBool()) {
    showTrayMessage(trUtf8("Tulip"),trUtf8("Tulip is still running.\nTo show the Tulip window again, click Tulip icon located into your notification area.\n\nNote: This message will be displayed only once."),QSystemTrayIcon::Information,3000);
    TulipSettings::instance().setValue("app/showsystraynotif",false);
  }

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

void TulipMainWindow::showPluginsCenter() {
  if (!isVisible())
    setVisible(true);

  raise();
#if defined(__APPLE__) || defined(_WIN32)
  _ui->pages->setCurrentWidget(_ui->pluginsPage);
#endif
}

void TulipMainWindow::showProjectsCenter() {
  if (!isVisible())
    setVisible(true);

  raise();
  _ui->pages->setCurrentWidget(_ui->welcomePage);
}

void TulipMainWindow::showAboutCenter() {
  if (!isVisible())
    setVisible(true);

  raise();
  _ui->pages->setCurrentWidget(_ui->aboutPage);
}

void TulipMainWindow::showTrayMessage(const QString &message) {
  showTrayMessage(trUtf8("Perspective"),"\n" + message + "\n\n" + trUtf8("Click to dismiss"),(uint)QSystemTrayIcon::Information,3000);
}

void TulipMainWindow::openProject(const QString &file) {
  raise();
  tlp::TulipProject *project = tlp::TulipProject::openProject(file);

  if (project->isValid()) {
    openProjectWith(file, project->perspective(),QVariantMap());
  }
  else {
    QString perspectiveName = QString::null;

    if (PluginLister::instance()->availablePlugins<tlp::Perspective>().size() > 1) {
      PerspectiveSelectionDialog dlg;

      if (dlg.exec() == QDialog::Accepted) {
        perspectiveName = dlg.perspectiveName();
      }
    }
    else {
      std::string stdName = *(PluginLister::instance()->availablePlugins<tlp::Perspective>().begin());
      perspectiveName = stdName.c_str();
    }

    if (!perspectiveName.isNull())
      TulipPerspectiveProcessHandler::instance()->createPerspective(perspectiveName,file,QVariantMap());
  }

  delete project;
}

void TulipMainWindow::createPerspective(const QString &name,const QVariantMap &parameters) {
  TulipPerspectiveProcessHandler::instance()->createPerspective(name,"",parameters);
}
void TulipMainWindow::openProjectWith(const QString &file, const QString &perspective, const QVariantMap &parameters) {
  TulipSettings::instance().addToRecentDocuments(file);
  TulipPerspectiveProcessHandler::instance()->createPerspective(perspective,file,parameters);
}

void TulipMainWindow::showTrayMessage(const QString &title, const QString &message, uint icon, uint duration) {
  if (!_systemTrayIcon)
    return;

  _systemTrayIcon->showMessage(title,message,(QSystemTrayIcon::MessageIcon)icon,duration);
}

void TulipMainWindow::showErrorMessage(const QString &title, const QString &message) {
  _ui->statusBar->setVisible(true);
  showTrayMessage(title,message,QSystemTrayIcon::Critical,10000);
  _errorMessage->setText(message);
}

void TulipMainWindow::pluginErrorMessage(const QString &message) {
  _currentTrayMessage = PluginErrorMessage;
  showTrayMessage(trUtf8("Error while loading plugins"),message + "\n\n" + trUtf8("Click on this message to see detailed information"),(uint)QSystemTrayIcon::Critical,10000);
}
