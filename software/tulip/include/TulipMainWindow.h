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
  void showPage(int);

protected:
  void closeEvent(QCloseEvent *);

protected slots:
  void systemTrayRequest(QSystemTrayIcon::ActivationReason);
  void pageChooserClicked();

private:
  Ui::TulipMainWindowData *_ui;
  QSystemTrayIcon *_systemTrayIcon;

};

#endif // TULIPMAINWINDOW_H
