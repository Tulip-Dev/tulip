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

#ifndef TULIPAGENTSERVICEADAPTOR_H_1314624429
#define TULIPAGENTSERVICEADAPTOR_H_1314624429

#include <QtCore/QObject>
#include <QtDBus/QtDBus>
#include <QtDBus/QDBusAbstractAdaptor>
class QByteArray;
template<class T> class QList;
template<class Key, class Value> class QMap;
class QString;
class QStringList;
class QVariant;

/*
 * Adaptor class for interface org.labri.Tulip
 */
class TulipAgentServiceAdaptor: public QDBusAbstractAdaptor {
  Q_OBJECT
  Q_CLASSINFO("D-Bus Interface", "org.labri.Tulip")
  Q_CLASSINFO("D-Bus Introspection", ""
              "  <interface name=\"org.labri.Tulip\">\n"
              "    <property access=\"read\" type=\"x\" name=\"pid\"/>\n"
              "    <method name=\"ShowWelcomeScreen\"/>\n"
              "    <method name=\"ShowPluginsCenter\"/>\n"
              "    <method name=\"ShowAboutPage\"/>\n"
              "    <method name=\"ShowOpenProjectWindow\"/>\n"
              "    <method name=\"OpenProject\">\n"
              "      <arg direction=\"in\" type=\"s\" name=\"file\"/>\n"
              "    </method>\n"
              "    <method name=\"OpenProjectWith\">\n"
              "      <annotation value=\"QVariantMap\" name=\"com.trolltech.QtDBus.QtTypeName.In2\"/>\n"
              "      <arg direction=\"in\" type=\"s\" name=\"file\"/>\n"
              "      <arg direction=\"in\" type=\"s\" name=\"perspective\"/>\n"
              "      <arg direction=\"in\" type=\"a{sv}\" name=\"parameters\"/>\n"
              "    </method>-->\n"
              "    <method name=\"CreatePerspective\">\n"
              "      <annotation value=\"QVariantMap\" name=\"com.trolltech.QtDBus.QtTypeName.In1\"/>\n"
              "      <arg direction=\"in\" type=\"s\" name=\"name\"/>\n"
              "      <arg direction=\"in\" type=\"a{sv}\" name=\"parameters\"/>\n"
              "    </method>\n"
              "    <method name=\"CreatePerspective\">\n"
              "      <arg direction=\"in\" type=\"s\" name=\"name\"/>\n"
              "    </method>\n"
              "    <method name=\"AddPluginRepository\">\n"
              "      <arg direction=\"in\" type=\"s\" name=\"url\"/>\n"
              "    </method>\n"
              "    <method name=\"RemovePluginRepository\">\n"
              "      <arg direction=\"in\" type=\"s\" name=\"url\"/>\n"
              "    </method>\n"
              "    <method name=\"GetCompatiblePerspectives\">\n"
              "      <arg direction=\"in\" type=\"s\" name=\"file\"/>\n"
              "      <arg direction=\"out\" type=\"as\" name=\"result\"/>\n"
              "    </method>\n"
              "    <method name=\"EnableCrashHandling\">\n"
              "      <arg direction=\"in\" type=\"s\" name=\"folder\"/>\n"
              "      <arg direction=\"in\" type=\"x\" name=\"pid\"/>\n"
              "    </method>\n"
              "    <method name=\"ShowTrayMessage\">\n"
              "      <arg direction=\"in\" type=\"s\" name=\"title\"/>\n"
              "      <arg direction=\"in\" type=\"s\" name=\"message\"/>\n"
              "      <arg direction=\"in\" type=\"u\" name=\"icon\"/>\n"
              "      <arg direction=\"in\" type=\"u\" name=\"duration\"/>\n"
              "    </method>\n"
              "    <signal name=\"Terminate\"/>\n"
              "  </interface>\n"
              "")
public:
  TulipAgentServiceAdaptor(QObject *parent);
  virtual ~TulipAgentServiceAdaptor();

public: // PROPERTIES
  Q_PROPERTY(qlonglong pid READ pid)
  qlonglong pid() const;

public Q_SLOTS: // METHODS
  void AddPluginRepository(const QString &url);
  void CreatePerspective(const QString &name);
  void CreatePerspective(const QString &name, const QVariantMap &parameters);
  void EnableCrashHandling(const QString &folder, qlonglong pid);
  QStringList GetCompatiblePerspectives(const QString &file);
  void OpenProject(const QString &file);
  void OpenProjectWith(const QString &file, const QString &perspective, const QVariantMap &parameters);
  void RemovePluginRepository(const QString &url);
  void ShowAboutPage();
  void ShowOpenProjectWindow();
  void ShowPluginsCenter();
  void ShowTrayMessage(const QString &title, const QString &message, uint icon, uint duration);
  void ShowWelcomeScreen();
Q_SIGNALS: // SIGNALS
  void Terminate();
};

#endif
