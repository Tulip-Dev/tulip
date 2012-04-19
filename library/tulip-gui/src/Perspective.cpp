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

using namespace tlp;

tlp::Perspective* Perspective::_instance = NULL;

Perspective* Perspective::instance() {
  return _instance;
}
void Perspective::setInstance(Perspective* p) {
  _instance = p;
}

Perspective::Perspective(const tlp::PluginContext* c) : _project(NULL), _mainWindow(NULL), _externalFile(QString()), _parameters(QVariantMap()) {
  if(c != NULL) {
    const PerspectiveContext* perspectiveContext = dynamic_cast<const PerspectiveContext*>(c);
    _mainWindow = perspectiveContext->mainWindow;
    _project = perspectiveContext->project;
    _externalFile = perspectiveContext->externalFile;
    _parameters = perspectiveContext->parameters;
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
