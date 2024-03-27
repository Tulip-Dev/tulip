/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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
#include <tulip/Perspective.h>
#include <tulip/TulipProject.h>

#include <QApplication>
#include <QProcess>
#include <QAction>
#include <QStatusBar>

#include <tulip/SimplePluginProgressWidget.h>

using namespace tlp;

tlp::Perspective *Perspective::_instance = nullptr;

Perspective *Perspective::instance() {
  return _instance;
}
void Perspective::setInstance(Perspective *p) {
  _instance = p;
}

Perspective::Perspective(const tlp::PluginContext *c)
    : _maximised(false), _project(nullptr), _mainWindow(nullptr), _externalFile(QString()),
      _parameters(QVariantMap()), _restartNeeded(false) {
  if (c != nullptr) {
    const PerspectiveContext *perspectiveContext = static_cast<const PerspectiveContext *>(c);
    _mainWindow = perspectiveContext->mainWindow;
    _project = perspectiveContext->project;
    _externalFile = perspectiveContext->externalFile;
    _parameters = perspectiveContext->parameters;
    _perspectiveId = perspectiveContext->id;
  }
}

Perspective::~Perspective() {
  delete _project;

  if (this == _instance)
    _instance = nullptr;
}

PluginProgress *Perspective::progress(ProgressOptions options) {
  SimplePluginProgressDialog *dlg = new SimplePluginProgressDialog(_mainWindow);
  dlg->setWindowIcon(_mainWindow->windowIcon());
  dlg->showPreview(options.testFlag(IsPreviewable));
  dlg->setCancelButtonVisible(options.testFlag(IsCancellable));
  dlg->setStopButtonVisible(options.testFlag(IsStoppable));
  dlg->show();
  QApplication::processEvents();
  return dlg;
}

QMainWindow *Perspective::mainWindow() const {
  return _mainWindow;
}

void Perspective::showFullScreen(bool f) {
  if (f) {
    _maximised = _mainWindow->isMaximized();
    _mainWindow->showFullScreen();
  } else {
    _mainWindow->showNormal();

    if (_maximised)
      _mainWindow->showMaximized();
  }
}

void Perspective::registerReservedProperty(QString s) {
  _reservedProperties.insert(s);
}

void Perspective::centerPanelsForGraph(Graph *) {}

bool Perspective::isReservedPropertyName(QString s) {
  return _reservedProperties.contains(s);
}

void Perspective::openProjectFile(const QString &path) {
  QProcess::startDetached(QApplication::applicationFilePath(), QStringList() << path);
}

void Perspective::createPerspective(const QString &name) {
  QProcess::startDetached(QApplication::applicationFilePath(), QStringList()
                                                                   << "--perspective=" + name);
}

void Perspective::redirectStatusTipOfMenu(QMenu *menu) {
  if (Perspective::instance()) {
    menu->setToolTipsVisible(true);
    connect(menu, SIGNAL(hovered(QAction *)), instance(), SLOT(showStatusTipOf(QAction *)));
    connect(menu, SIGNAL(aboutToHide()), instance(), SLOT(clearStatusMessage()));
  }
}

void Perspective::showStatusTipOf(QAction *action) {
  QString tip = action->statusTip();

  if (tip.isEmpty())
    tip = action->toolTip();

  action->setStatusTip(tip);
  showStatusMessage(tip);
}

void Perspective::displayStatusMessage(const QString &msg) {
  _mainWindow->statusBar()->showMessage(msg);
}

void Perspective::clearStatusMessage() {
  mainWindow()->statusBar()->clearMessage();
}

void Perspective::setStyleSheet(QWidget *w) {
  if (instance())
    w->setStyleSheet(instance()->mainWindow()->styleSheet());
}

QString Perspective::styleSheet() {
  return instance() ? instance()->mainWindow()->styleSheet() : QString();
}
