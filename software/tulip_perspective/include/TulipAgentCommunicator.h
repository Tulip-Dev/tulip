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

#ifndef TULIPAGENTCOMMUNICATOR_H_1314624228
#define TULIPAGENTCOMMUNICATOR_H_1314624228

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>

/*
 * Proxy class for interface org.labri.Tulip
 */
class TulipAgentCommunicator: public QDBusAbstractInterface {
  Q_OBJECT
public:
  static inline const char *staticInterfaceName() {
    return "org.labri.Tulip";
  }

public:
  TulipAgentCommunicator(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = 0);

  ~TulipAgentCommunicator();

  Q_PROPERTY(qlonglong pid READ pid)
  inline qlonglong pid() const {
    return qvariant_cast< qlonglong >(property("pid"));
  }

public Q_SLOTS: // METHODS
  inline QDBusPendingReply<> AddPluginRepository(const QString &url) {
    QList<QVariant> argumentList;
    argumentList << qVariantFromValue(url);
    return asyncCallWithArgumentList(QLatin1String("AddPluginRepository"), argumentList);
  }

  inline QDBusPendingReply<> CreatePerspective(const QString &name) {
    QList<QVariant> argumentList;
    argumentList << qVariantFromValue(name);
    return asyncCallWithArgumentList(QLatin1String("CreatePerspective"), argumentList);
  }

  inline QDBusPendingReply<> CreatePerspective(const QString &name, const QVariantMap &parameters) {
    QList<QVariant> argumentList;
    argumentList << qVariantFromValue(name) << qVariantFromValue(parameters);
    return asyncCallWithArgumentList(QLatin1String("CreatePerspective"), argumentList);
  }

  inline QDBusPendingReply<> EnableCrashHandling(const QString &folder, qlonglong pid) {
    QList<QVariant> argumentList;
    argumentList << qVariantFromValue(folder) << qVariantFromValue(pid);
    return asyncCallWithArgumentList(QLatin1String("EnableCrashHandling"), argumentList);
  }

  inline QDBusPendingReply<QStringList> GetCompatiblePerspectives(const QString &file) {
    QList<QVariant> argumentList;
    argumentList << qVariantFromValue(file);
    return asyncCallWithArgumentList(QLatin1String("GetCompatiblePerspectives"), argumentList);
  }

  inline QDBusPendingReply<> OpenProject(const QString &file) {
    QList<QVariant> argumentList;
    argumentList << qVariantFromValue(file);
    return asyncCallWithArgumentList(QLatin1String("OpenProject"), argumentList);
  }

  inline QDBusPendingReply<> OpenProjectWith(const QString &file, const QString &perspective, const QVariantMap &parameters) {
    QList<QVariant> argumentList;
    argumentList << qVariantFromValue(file) << qVariantFromValue(perspective) << qVariantFromValue(parameters);
    return asyncCallWithArgumentList(QLatin1String("OpenProjectWith"), argumentList);
  }

  inline QDBusPendingReply<> RemovePluginRepository(const QString &url) {
    QList<QVariant> argumentList;
    argumentList << qVariantFromValue(url);
    return asyncCallWithArgumentList(QLatin1String("RemovePluginRepository"), argumentList);
  }

  inline QDBusPendingReply<> ShowAboutPage() {
    QList<QVariant> argumentList;
    return asyncCallWithArgumentList(QLatin1String("ShowAboutPage"), argumentList);
  }

  inline QDBusPendingReply<> ShowOpenProjectWindow() {
    QList<QVariant> argumentList;
    return asyncCallWithArgumentList(QLatin1String("ShowOpenProjectWindow"), argumentList);
  }

  inline QDBusPendingReply<> ShowPluginsCenter() {
    QList<QVariant> argumentList;
    return asyncCallWithArgumentList(QLatin1String("ShowPluginsCenter"), argumentList);
  }

  inline QDBusPendingReply<> ShowTrayMessage(const QString &title, const QString &message, uint icon, uint duration) {
    QList<QVariant> argumentList;
    argumentList << qVariantFromValue(title) << qVariantFromValue(message) << qVariantFromValue(icon) << qVariantFromValue(duration);
    return asyncCallWithArgumentList(QLatin1String("ShowTrayMessage"), argumentList);
  }

  inline QDBusPendingReply<> ShowWelcomeScreen() {
    QList<QVariant> argumentList;
    return asyncCallWithArgumentList(QLatin1String("ShowWelcomeScreen"), argumentList);
  }

Q_SIGNALS: // SIGNALS
  void Terminate();
};

namespace org {
namespace labri {
typedef ::TulipAgentCommunicator Tulip;
}
}
#endif
