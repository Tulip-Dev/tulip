#ifndef TULIPMAINWINDOW_H
#define TULIPMAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtGui/QSystemTrayIcon>

namespace Ui {
class TulipMainWindowData;
}

/**
  @brief The window used in the tulip_app process to display the Welcome, plugins center and getting help screens.
  This class is basically a controller for the 3 underleying screens that manage their own logic.
  */
class TulipMainWindow: public QMainWindow {
  Q_OBJECT

public:
  TulipMainWindow(QWidget *parent=0);
  virtual ~TulipMainWindow();

  void setPluginsErrors(const QMap<QString,QString> &);

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
};

#endif // TULIPMAINWINDOW_H
