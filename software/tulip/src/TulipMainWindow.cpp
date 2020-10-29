/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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
#include <QGuiApplication>
#include <QScreen>

#include <tulip/TulipRelease.h>
#include <tulip/PythonVersionChecker.h>
#include <tulip/PluginLister.h>
#include <tulip/Perspective.h>
#include <tulip/TlpTools.h>
#include <tulip/TulipSettings.h>
#include <tulip/TulipProject.h>
#include <tulip/PluginModel.h>
#include <tulip/TlpQtTools.h>

#include "ui_TulipMainWindow.h"
#include "TulipPerspectiveProcessHandler.h"
#include "TulipWelcomePage.h"
#include "PerspectiveItemWidget.h"
#include "PerspectiveSelectionDialog.h"

using namespace tlp;

TulipMainWindow *TulipMainWindow::_instance = nullptr;

TulipMainWindow::TulipMainWindow(QWidget *parent)
    : QMainWindow(parent), _errorMessage(new QLabel()), _ui(new Ui::TulipMainWindowData),
      _systemTrayIcon(nullptr) {
  _ui->setupUi(this);

  QLabel *errorIcon = new QLabel();
  errorIcon->setMaximumSize(16, 16);
  errorIcon->setMinimumSize(16, 16);
  errorIcon->setPixmap(QPixmap(":/tulip/app/icons/16/dialog-error.png"));
  _ui->statusBar->insertWidget(0, errorIcon);
  _ui->statusBar->insertWidget(1, _errorMessage);

  _ui->statusBar->setVisible(false);
  _pageChoosers.clear();
  _pageChoosers.push_back(_ui->welcomePageChooser);
  _pageChoosers.push_back(_ui->pluginsPageChooser);
  _pageChoosers.push_back(_ui->aboutPageChooser);

  QPixmap logo(tlpStringToQString(TulipBitmapDir + "/welcomelogo.bmp"));
  // take care of the devicePixelRatio
  // before setting the logo
  logo.setDevicePixelRatio(QGuiApplication::primaryScreen()->devicePixelRatio());
  _ui->mainLogo->setPixmap(logo);
  // set title
  QString title("Tulip ");

  // show patch number only if needed
  if (TULIP_INT_VERSION % 10)
    title += TULIP_VERSION;
  else
    title += TULIP_MM_VERSION;

  setWindowTitle(title);
  _ui->mainTitle->setText(
      QString("<html><head/><body><p align=\"center\"><span style=\" font-family:'MS Shell Dlg 2'; "
              "font-size:18pt; font-weight:600;\">") +
      title + "</span></p></body></html>");

  connect(_ui->welcomePageChooser, SIGNAL(clicked()), this, SLOT(pageChooserClicked()));
  connect(_ui->pluginsPageChooser, SIGNAL(clicked()), this, SLOT(pageChooserClicked()));
  connect(_ui->aboutPageChooser, SIGNAL(clicked()), this, SLOT(pageChooserClicked()));

  // System tray
  _systemTrayIcon = new QSystemTrayIcon(QIcon(":/tulip/gui/icons/tulip.ico"), this);
  _systemTrayIcon->setToolTip("Tulip agent");
  QMenu *systemTrayMenu = new QMenu();
  systemTrayMenu->addAction("Show", this, SLOT(showProjectsCenter()));
  systemTrayMenu->addAction("Hide", this, SLOT(hide()));
  systemTrayMenu->addSeparator();
  systemTrayMenu->addAction("Projects", this, SLOT(showProjectsCenter()));
  systemTrayMenu->addAction("Plugins center", this, SLOT(showPluginsCenter()));
  systemTrayMenu->addAction("About us", this, SLOT(showAboutCenter()));
  systemTrayMenu->addSeparator();
  connect(systemTrayMenu->addAction("Exit"), SIGNAL(triggered()), this, SLOT(close()));
  connect(_ui->exitButton, SIGNAL(clicked()), this, SLOT(close()));
  _systemTrayIcon->setContextMenu(systemTrayMenu);
  connect(_systemTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this,
          SLOT(systemTrayRequest(QSystemTrayIcon::ActivationReason)));
  connect(_systemTrayIcon, SIGNAL(messageClicked()), this, SLOT(systemTrayMessageClicked()));
  connect(_ui->pages, SIGNAL(currentChanged(int)), this, SLOT(pageSwitched(int)));
  connect(_ui->welcomePage, SIGNAL(openPerspective(QString)), this,
          SLOT(createPerspective(QString)));
  connect(_ui->welcomePage, SIGNAL(openProject()), this, SLOT(showOpenProjectWindow()));
  connect(_ui->welcomePage, SIGNAL(openFile(QString)), this, SLOT(openProject(QString)));
  _systemTrayIcon->show();

  connect(TulipPerspectiveProcessHandler::instance(), SIGNAL(showPluginsAgent()), this,
          SLOT(showPluginsCenter()));
  connect(TulipPerspectiveProcessHandler::instance(), SIGNAL(showProjectsAgent()), this,
          SLOT(showProjectsCenter()));
  connect(TulipPerspectiveProcessHandler::instance(), SIGNAL(showAboutAgent()), this,
          SLOT(showAboutCenter()));
  connect(TulipPerspectiveProcessHandler::instance(), SIGNAL(showTrayMessage(QString)), this,
          SLOT(showTrayMessage(QString)));
  connect(TulipPerspectiveProcessHandler::instance(), SIGNAL(showErrorMessage(QString, QString)),
          this, SLOT(showErrorMessage(QString, QString)));
  connect(TulipPerspectiveProcessHandler::instance(), SIGNAL(openProject(QString)), this,
          SLOT(openProject(QString)));
  connect(TulipPerspectiveProcessHandler::instance(), SIGNAL(openProjectWith(QString, QString)),
          this, SLOT(openProjectWith(QString, QString)));
  connect(TulipPerspectiveProcessHandler::instance(), SIGNAL(openPerspective(QString)), this,
          SLOT(createPerspective(QString)));

#ifdef TULIP_BUILD_PYTHON_COMPONENTS
  checkPython();
#endif
}

TulipMainWindow::~TulipMainWindow() {
  delete _ui;
}

#ifdef TULIP_BUILD_PYTHON_COMPONENTS
void TulipMainWindow::checkPython() {
  if (!PythonVersionChecker::isPythonVersionMatching()) {

    QStringList installedPythons = PythonVersionChecker::installedVersions();

    QString requiredPython = "Python " + PythonVersionChecker::compiledVersion();
#ifdef X86_64
    requiredPython += " (64 bit)";
#else
    requiredPython += " (32 bit)";
#endif

    QString errorMessage;

    if (installedPythons.isEmpty()) {
      errorMessage = requiredPython + " does not seem to be installed on your system.\nPlease "
                                      "install it in order to use Tulip.";
    } else {
      errorMessage =
          "Python version mismatch. Please install " + requiredPython + " in order to use Tulip.\n";

      if (installedPythons.size() == 1) {
        errorMessage += "Detected version is " + installedPythons.at(0) + ".";
      } else {
        errorMessage += "Detected versions are ";

        for (int i = 0; i < installedPythons.size(); ++i) {
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

void TulipMainWindow::closeEvent(QCloseEvent *e) {
  _systemTrayIcon->deleteLater();
  e->accept();
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
  createPerspective(name, QVariantMap());
}
void TulipMainWindow::showOpenProjectWindow() {
  setVisible(true);
  QString filePath = QFileDialog::getOpenFileName(
      this,
      "Choose a Tulip project to open with its associated perspective. Or select a "
      "external file format to import.",
      QDir::homePath(),
      "Tulip Files(*.tlp *.tlp.gz *.tlpb *.tlpb.gz *.tlpx *.tlpz *.tlpbz);;Tulip "
      "Project (*.tlpx);;Tulip Graph (*.tlp *.tlp.gz *.tlpb *.tlpb.gz *.tlpz "
      "*.tlpbz)");

  if (filePath.isEmpty())
    return;

  openProject(filePath);
}

void TulipMainWindow::bringWindowToFront() {
  hide();
  show();
  raise();
  activateWindow();
}

void TulipMainWindow::showPluginsCenter() {
  bringWindowToFront();
  _ui->pages->setCurrentWidget(_ui->pluginsPage);
}

void TulipMainWindow::showProjectsCenter() {
  bringWindowToFront();
  _ui->pages->setCurrentWidget(_ui->welcomePage);
}

void TulipMainWindow::showAboutCenter() {
  bringWindowToFront();
  _ui->pages->setCurrentWidget(_ui->aboutPage);
}

void TulipMainWindow::showTrayMessage(const QString &message) {
  showTrayMessage("Perspective", "\n" + message + "\n\n" + "Click to dismiss",
                  uint(QSystemTrayIcon::Information), 3000);
}

void TulipMainWindow::openProject(const QString &file) {

  tlp::TulipProject *project = tlp::TulipProject::openProject(file);

  if (project != nullptr) {
    openProjectWith(file, project->perspective(), QVariantMap());
  } else {
    QString perspectiveName;

    if (PluginLister::availablePlugins<tlp::Perspective>().size() > 1) {
      PerspectiveSelectionDialog dlg;

      if (dlg.exec() == QDialog::Accepted) {
        perspectiveName = dlg.perspectiveName();
      }
    } else {
      std::string stdName = *(PluginLister::availablePlugins<tlp::Perspective>().begin());
      perspectiveName = stdName.c_str();
    }

    if (!perspectiveName.isEmpty())
      TulipPerspectiveProcessHandler::instance()->createPerspective(perspectiveName, file,
                                                                    QVariantMap());
  }

  delete project;
}

void TulipMainWindow::createPerspective(const QString &name, const QVariantMap &parameters) {
  TulipPerspectiveProcessHandler::instance()->createPerspective(name, "", parameters);
}
void TulipMainWindow::openProjectWith(const QString &file, const QString &perspective,
                                      const QVariantMap &parameters) {
  TulipSettings::addToRecentDocuments(file);
  TulipPerspectiveProcessHandler::instance()->createPerspective(perspective, file, parameters);
}

void TulipMainWindow::showTrayMessage(const QString &title, const QString &message, uint icon,
                                      uint duration) {
  if (_systemTrayIcon == nullptr)
    return;

  _systemTrayIcon->showMessage(title, message, static_cast<QSystemTrayIcon::MessageIcon>(icon),
                               duration);
}

void TulipMainWindow::showErrorMessage(const QString &title, const QString &message) {
  _ui->statusBar->setVisible(true);
  showTrayMessage(title, message, QSystemTrayIcon::Critical, 10000);
  _errorMessage->setText(message);
}

void TulipMainWindow::pluginErrorMessage(const QString &message) {
  _currentTrayMessage = PluginErrorMessage;
  showTrayMessage("Error while loading plugins",
                  message + "\n\nClick on this message to see detailed information",
                  uint(QSystemTrayIcon::Critical), 10000);
}
