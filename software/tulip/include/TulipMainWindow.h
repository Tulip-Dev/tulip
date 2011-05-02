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

public slots:
  void startApp();
  void closeApp();

protected:
  void closeEvent(QCloseEvent *);

protected slots:
  void systemTrayRequest(QSystemTrayIcon::ActivationReason);

private:
  Ui::TulipMainWindowData *_ui;
  QSystemTrayIcon *_systemTrayIcon;
};

#endif // TULIPMAINWINDOW_H
