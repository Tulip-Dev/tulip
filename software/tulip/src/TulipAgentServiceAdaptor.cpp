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

#include "TulipAgentServiceAdaptor.h"
#include <QtCore/QMetaObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

/*
 * Implementation of adaptor class TulipAgentServiceAdaptor
 */

TulipAgentServiceAdaptor::TulipAgentServiceAdaptor(QObject *parent)
  : QDBusAbstractAdaptor(parent) {
  // constructor
  setAutoRelaySignals(true);
}

TulipAgentServiceAdaptor::~TulipAgentServiceAdaptor() {
  // destructor
}

qlonglong TulipAgentServiceAdaptor::pid() const {
  // get the value of property pid
  return qvariant_cast< qlonglong >(parent()->property("pid"));
}

void TulipAgentServiceAdaptor::AddPluginRepository(const QString &url) {
  // handle method call org.labri.Tulip.AddPluginRepository
  QMetaObject::invokeMethod(parent(), "AddPluginRepository", Q_ARG(QString, url));
}

void TulipAgentServiceAdaptor::CreatePerspective(const QString &name) {
  // handle method call org.labri.Tulip.CreatePerspective
  QMetaObject::invokeMethod(parent(), "CreatePerspective", Q_ARG(QString, name));
}

void TulipAgentServiceAdaptor::CreatePerspective(const QString &name, const QVariantMap &parameters) {
  // handle method call org.labri.Tulip.CreatePerspective
  QMetaObject::invokeMethod(parent(), "CreatePerspective", Q_ARG(QString, name), Q_ARG(QVariantMap, parameters));
}

void TulipAgentServiceAdaptor::EnableCrashHandling(const QString &folder, qlonglong pid) {
  // handle method call org.labri.Tulip.EnableCrashHandling
  QMetaObject::invokeMethod(parent(), "EnableCrashHandling", Q_ARG(QString, folder), Q_ARG(qlonglong, pid));
}

QStringList TulipAgentServiceAdaptor::GetCompatiblePerspectives(const QString &file) {
  // handle method call org.labri.Tulip.GetCompatiblePerspectives
  QStringList result;
  QMetaObject::invokeMethod(parent(), "GetCompatiblePerspectives", Q_RETURN_ARG(QStringList, result), Q_ARG(QString, file));
  return result;
}

void TulipAgentServiceAdaptor::OpenProject(const QString &file) {
  // handle method call org.labri.Tulip.OpenProject
  QMetaObject::invokeMethod(parent(), "OpenProject", Q_ARG(QString, file));
}

void TulipAgentServiceAdaptor::OpenProjectWith(const QString &file, const QString &perspective, const QVariantMap &parameters) {
  // handle method call org.labri.Tulip.OpenProjectWith
  QMetaObject::invokeMethod(parent(), "OpenProjectWith", Q_ARG(QString, file), Q_ARG(QString, perspective), Q_ARG(QVariantMap, parameters));
}

void TulipAgentServiceAdaptor::RemovePluginRepository(const QString &url) {
  // handle method call org.labri.Tulip.RemovePluginRepository
  QMetaObject::invokeMethod(parent(), "RemovePluginRepository", Q_ARG(QString, url));
}

void TulipAgentServiceAdaptor::ShowAboutPage() {
  // handle method call org.labri.Tulip.ShowAboutPage
  QMetaObject::invokeMethod(parent(), "ShowAboutPage");
}

void TulipAgentServiceAdaptor::ShowOpenProjectWindow() {
  // handle method call org.labri.Tulip.ShowOpenProjectWindow
  QMetaObject::invokeMethod(parent(), "ShowOpenProjectWindow");
}

void TulipAgentServiceAdaptor::ShowPluginsCenter() {
  // handle method call org.labri.Tulip.ShowPluginsCenter
  QMetaObject::invokeMethod(parent(), "ShowPluginsCenter");
}

void TulipAgentServiceAdaptor::ShowTrayMessage(const QString &title, const QString &message, uint icon, uint duration) {
  // handle method call org.labri.Tulip.ShowTrayMessage
  QMetaObject::invokeMethod(parent(), "ShowTrayMessage", Q_ARG(QString, title), Q_ARG(QString, message), Q_ARG(uint, icon), Q_ARG(uint, duration));
}

void TulipAgentServiceAdaptor::ShowWelcomeScreen() {
  // handle method call org.labri.Tulip.ShowWelcomeScreen
  QMetaObject::invokeMethod(parent(), "ShowWelcomeScreen");
}

