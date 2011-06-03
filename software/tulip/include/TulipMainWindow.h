#ifndef TULIPMAINWINDOW_H
#define TULIPMAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtGui/QSystemTrayIcon>

namespace Ui {
class TulipMainWindowData;
}
class PluginsCenter;

/**
  @brief The window used in the tulip_app process to display the Welcome, plugins center and getting help screens.
  This class is basically a controller for the 3 underleying screens that manage their own logic.
  */
class TulipMainWindow: public QMainWindow {
  Q_OBJECT

public:
  TulipMainWindow(QWidget *parent=0);
  virtual ~TulipMainWindow();

  PluginsCenter *pluginsCenter() const;

public slots:
  void startApp();
  void closeApp();

protected:
  void closeEvent(QCloseEvent *);

protected slots:
  void systemTrayRequest(QSystemTrayIcon::ActivationReason);
  void systemTrayMessageClicked();
  void pageChooserClicked();
  void pageSwitched(int);

private:
  enum SystemTrayMessage {
    NoMessage,
    PluginErrorMessage
  };

  Ui::TulipMainWindowData *_ui;
  QSystemTrayIcon *_systemTrayIcon;
  SystemTrayMessage _currentTrayMessage;

  QVector<QObject *> _pageChoosers;



// Methods and properties coming from the D-Bus service implementation
public:
  Q_PROPERTY(qlonglong pid READ pid)
  qlonglong pid() const;
public Q_SLOTS:
  void ShowAboutPage();
  void ShowPluginsCenter();
  void ShowWelcomeScreen();
};

#endif // TULIPMAINWINDOW_H
