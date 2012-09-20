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
#include "tulip/Perspective.h"

#include <QtGui/QApplication>
#include <QtGui/QMainWindow>
#include <tulip/SimplePluginProgressWidget.h>

#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>

using namespace tlp;

tlp::Perspective* Perspective::_instance = NULL;

Perspective* Perspective::instance() {
  return _instance;
}
void Perspective::setInstance(Perspective* p) {
  _instance = p;
}

Perspective::Perspective(const tlp::PluginContext* c) : _project(NULL), _mainWindow(NULL), _externalFile(QString()), _parameters(QVariantMap()), _agentSocket(NULL) {
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

bool Perspective::isCompatible(tlp::TulipProject *) {
  return false;
}

PluginProgress* Perspective::progress() {
  SimplePluginProgressDialog* dlg = new SimplePluginProgressDialog(_mainWindow);
  dlg->show();
  QApplication::processEvents();
  return dlg;
}

QMainWindow* Perspective::mainWindow() const {
  return _mainWindow;
}

void Perspective::registerReservedProperty(QString s) {
  _reservedProperties.insert(s);
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
  sendAgentMessage("SHOW_AGENT PLUGINS");
}

void Perspective::showProjectsPage() {
  sendAgentMessage("SHOW_AGENT PROJECTS");
}

void Perspective::showAboutPage() {
  sendAgentMessage("SHOW_AGENT ABOUT");
}

void Perspective::showTrayMessage(const QString &s) {
  sendAgentMessage("TRAY_MESSAGE " + s);
}

void Perspective::openProjectFile(const QString &path) {
  sendAgentMessage("OPEN_PROJECT " + path);
}

void Perspective::createPerspective(const QString &name) {
  sendAgentMessage("CREATE_PERSPECTIVE " + name);
}

void Perspective::notifyProjectLocation(const QString &path) {
  sendAgentMessage("PROJECT_LOCATION " + QString::number(_perspectiveId) + " " + path);
}
