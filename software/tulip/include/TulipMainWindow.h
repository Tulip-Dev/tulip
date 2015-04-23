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

#ifndef TULIPMAINWINDOW_H
#define TULIPMAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QSystemTrayIcon>
#include <QVariant>

namespace Ui {
class TulipMainWindowData;
}
namespace tlp {
struct PluginInformation;
}
class PluginsCenter;
class QLabel;

/**
  @brief The window used in the tulip_app process to display the Welcome, plugins center and getting help screens.
  This class is basically a controller for the 3 underleying screens that manage their own logic.
  */
class TulipMainWindow: public QMainWindow {
  Q_OBJECT

  static TulipMainWindow* _instance;
  TulipMainWindow(QWidget *parent=NULL);

  QLabel* _errorMessage;

public:
  static TulipMainWindow* instance() {
    if (!_instance)
      _instance = new TulipMainWindow;

    return _instance;
  }

  virtual ~TulipMainWindow();

  PluginsCenter *pluginsCenter() const;

public slots:
  void closeApp();
  void pluginErrorMessage(const QString &message);
  void showTrayMessage(const QString &title, const QString &message, uint icon, uint duration);
  void showErrorMessage(const QString& title, const QString& message);

  void createPerspective(const QString &name);
  void createPerspective(const QString &name, const QVariantMap &parameters);

  void openProject(const QString &file);
  void openProjectWith(const QString &file, const QString &perspective, const QVariantMap &parameters = QVariantMap());

  void showOpenProjectWindow();

  void showPluginsCenter();
  void showProjectsCenter();
  void showAboutCenter();
  void showTrayMessage(const QString& message);

protected:
  void closeEvent(QCloseEvent *);

protected slots:
  void systemTrayRequest(QSystemTrayIcon::ActivationReason);
  void systemTrayMessageClicked();
  void pageChooserClicked();
  void pageSwitched(int);

private:

#ifdef BUILD_PYTHON_COMPONENTS
  void checkPython();
#endif

  enum SystemTrayMessage {
    NoMessage,
    PluginErrorMessage
  };

  Ui::TulipMainWindowData *_ui;
  QSystemTrayIcon *_systemTrayIcon;
  SystemTrayMessage _currentTrayMessage;

  QVector<QObject *> _pageChoosers;
};

#endif // TULIPMAINWINDOW_H
