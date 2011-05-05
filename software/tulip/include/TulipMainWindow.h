#ifndef TULIPMAINWINDOW_H
#define TULIPMAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtGui/QSystemTrayIcon>

#include "ui_TulipMainWindow.h"

class TulipMainWindow: public QMainWindow {
  Q_OBJECT

public:
  TulipMainWindow(QWidget *parent=0);
  virtual ~TulipMainWindow();

  void setPluginsErrors(const QMap<QString,QString> &);

public slots:
  void startApp();
  void closeApp();
  void showPage(int);

protected:
  void closeEvent(QCloseEvent *);

protected slots:
  void systemTrayRequest(QSystemTrayIcon::ActivationReason);
  void systemTrayMessageClicked();
  void pageChooserClicked();

private:
  enum SystemTrayMessage {
    NoMessage,
    PluginErrorMessage
  };

  Ui::TulipMainWindowData *_ui;
  QSystemTrayIcon *_systemTrayIcon;
  SystemTrayMessage _currentTrayMessage;
};

#endif // TULIPMAINWINDOW_H
