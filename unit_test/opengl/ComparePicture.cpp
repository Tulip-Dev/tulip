#include <QtGui/QApplication>

#include "ComparePictureMainWindow.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  ComparePictureMainWindow mainWin;
  mainWin.show();
  return app.exec();
}


