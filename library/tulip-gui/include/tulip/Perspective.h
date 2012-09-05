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
#ifndef _PERSPECTIVE_H
#define _PERSPECTIVE_H

#include <tulip/PluginLister.h>
#include <tulip/TulipProject.h>
#include <tulip/PluginContext.h>
#include <tulip/Plugin.h>
#include <QtCore/QObject>
#include <QtCore/QVariant>

class QMainWindow;

namespace tlp {

class PluginProgress;

static const std::string PERSPECTIVE_CATEGORY = QObject::trUtf8("Perspective").toStdString();

class TLP_QT_SCOPE PerspectiveContext : public tlp::PluginContext {
public:
  PerspectiveContext(): mainWindow(0), project(0), tulipPort(0) {}
  QMainWindow *mainWindow;
  TulipProject *project;
  QString externalFile;
  QVariantMap parameters;
  quint64 tulipPort;
};

class TLP_QT_SCOPE Perspective : public QObject, public tlp::Plugin {
  Q_OBJECT

  static tlp::Perspective* _instance;
  QSet<QString> _reservedProperties;

protected:
  TulipProject *_project;
  QMainWindow *_mainWindow;
  QString _externalFile;
  QVariantMap _parameters;
public:

  virtual std::string category() const {
    return PERSPECTIVE_CATEGORY;
  }
  std::string icon() const {
    return ":/tulip/gui/icons/32/plugin_controller.png";
  }

  static void setInstance(tlp::Perspective*);
  static tlp::Perspective* instance();

  template<typename T>
  static T* typedInstance() {
    return dynamic_cast<T*>(instance());
  }

  Perspective(const tlp::PluginContext* c);

  virtual ~Perspective();

  virtual bool isCompatible(tlp::TulipProject *);
  virtual void start(tlp::PluginProgress *)=0;
  virtual tlp::PluginProgress* progress();
  QMainWindow* mainWindow() const;

  bool isReservedPropertyName(QString);
  void registerReservedProperty(QString);

  virtual void redrawPanels(bool center=false)=0;

public slots:

  virtual bool terminated() {
    return true;
  }

signals:
  void showTulipWelcomeScreen();
  void showTulipPluginsCenter();
  void showTulipAboutPage();

  void createPerspective(QString name, const QVariantMap &parameters = QVariantMap());

  void openProject(QString);
  void openProjectWith(QString,QString,const QVariantMap &parameters = QVariantMap());

  void showOpenProjectWindow();

  void addPluginRepository(QString);
  void removePluginRepository(QString);

  void showTrayMessage(QString title,QString message,uint icon=0,uint duration=10000);
};

}

#endif //_PERSPECTIVE_H
