#ifndef TULIPMAINWINDOW_H
#define TULIPMAINWINDOW_H

#include <QtGui/QMainWindow>

/*
  This class can be used to add pre(-defined behavior to the main window before passing it to the perspective.
  Note that no GUI modification should be done since the Perspective must be free to do anything on it
  */
class TulipPerspectiveMainWindow : public QMainWindow {
  Q_OBJECT
public:
  explicit TulipPerspectiveMainWindow(QWidget *parent = 0);

signals:

public slots:
  void clearMemoryChecker();
  void printMemoryChecker();
};

#endif // TULIPMAINWINDOW_H
