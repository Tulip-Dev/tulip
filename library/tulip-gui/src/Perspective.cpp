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
#include <tulip/Perspective.h>
#include <tulip/TulipProject.h>

#include <QApplication>
#include <QMainWindow>
#include <QProcess>
#include <QTcpSocket>
#include <QHostAddress>

#include <tulip/SimplePluginProgressWidget.h>

using namespace tlp;

tlp::Perspective* Perspective::_instance = NULL;

Perspective* Perspective::instance() {
  return _instance;
}
void Perspective::setInstance(Perspective* p) {
  _instance = p;
}

Perspective::Perspective(const tlp::PluginContext* c) : _agentSocket(NULL), _maximised(false), _project(NULL), _mainWindow(NULL), _externalFile(QString()), _parameters(QVariantMap()) {
  if(c != NULL) {
    const PerspectiveContext* perspectiveContext = dynamic_cast<const PerspectiveContext*>(c);
    _mainWindow = perspectiveContext->mainWindow;
    _project = perspectiveContext->project;
    _externalFile = perspectiveContext->externalFile;
    _parameters = perspectiveContext->parameters;
    _perspectiveId = perspectiveContext->id;

    if (perspectiveContext->tulipPort != 0) {
      _agentSocket = new QTcpSocket(this);
      _agentSocket->connectToHost(QHostAddress::LocalHost,perspectiveContext->tulipPort);

      if (!_agentSocket->waitForConnected(2000)) {
        _agentSocket->deleteLater();
        _agentSocket = NULL;
      }

      if (_project != NULL) {
        notifyProjectLocation(_project->absoluteRootPath());
      }
    }
    else {
      qWarning("Perspective running in standalone mode");
    }
  }
}

Perspective::~Perspective() {
  delete _project;
}

PluginProgress* Perspective::progress(ProgressOptions options)  {
  SimplePluginProgressDialog* dlg = new SimplePluginProgressDialog(_mainWindow);
  dlg->setWindowIcon(_mainWindow->windowIcon());
  dlg->showPreview(options.testFlag(IsPreviewable));
  dlg->setCancelButtonVisible(options.testFlag(IsCancellable));
  dlg->setStopButtonVisible(options.testFlag(IsStoppable));
  dlg->show();
  QApplication::processEvents();
  return dlg;
}

QMainWindow* Perspective::mainWindow() const {
  return _mainWindow;
}

void Perspective::showFullScreen(bool f) {
  if (f) {
    _maximised = _mainWindow->isMaximized();
    _mainWindow->showFullScreen();
  }
  else {
    _mainWindow->showNormal();

    if (_maximised)
      _mainWindow->showMaximized();
  }
}

void Perspective::registerReservedProperty(QString s) {
  _reservedProperties.insert(s);
}

void Perspective::centerPanelsForGraph(Graph *) {
}

bool Perspective::isReservedPropertyName(QString s) {
  return _reservedProperties.contains(s);
}

void Perspective::sendAgentMessage(const QString& msg) {
  if (_agentSocket == NULL)
    return;

  _agentSocket->write(msg.toUtf8());
  _agentSocket->flush();
}

void Perspective::showPluginsCenter() {
  sendAgentMessage("SHOW_AGENT\tPLUGINS");
}

void Perspective::showProjectsPage() {
  sendAgentMessage("SHOW_AGENT\tPROJECTS");
}

void Perspective::showAboutPage() {
  sendAgentMessage("SHOW_AGENT\tABOUT");
}

void Perspective::showTrayMessage(const QString &s) {
  sendAgentMessage("TRAY_MESSAGE\t" + s);
}

void Perspective::showErrorMessage(const QString &title, const QString &s) {
  sendAgentMessage("ERROR_MESSAGE\t" + title + " " + s);
}

void Perspective::openProjectFile(const QString &path) {
  if (_agentSocket != NULL) {
    sendAgentMessage("OPEN_PROJECT\t" + path);
  }
  else { // on standalone mode, spawn a new standalone perspective
    QProcess::startDetached(QApplication::applicationFilePath(),QStringList() << path);
  }
}

void Perspective::createPerspective(const QString &name) {
  if (_agentSocket != NULL) {
    sendAgentMessage("CREATE_PERSPECTIVE\t" + name);
  }
  else { // on standalone mode, spawn a new standalone perspective
    QProcess::startDetached(QApplication::applicationFilePath(),QStringList() << "--perspective=" + name);
  }
}

void Perspective::notifyProjectLocation(const QString &path) {
  sendAgentMessage("PROJECT_LOCATION\t" + QString::number(_perspectiveId) + " " + path);
}
