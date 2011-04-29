#ifndef TULIPMAINWINDOW_H
#define TULIPMAINWINDOW_H

#include <QtGui/QMainWindow>
#include "ui_TulipMainWindow.h"

class TulipMainWindow: public QMainWindow {
  Q_OBJECT

public:
  TulipMainWindow(QWidget *parent=0);
  virtual ~TulipMainWindow();

public slots:
  void startApp();

private:
  Ui::TulipMainWindowData *_ui;
};

#endif // TULIPMAINWINDOW_H
