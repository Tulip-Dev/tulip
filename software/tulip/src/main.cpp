#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <qtimer.h>
#include <QtGui/qdesktopwidget.h>

#include "AppStartUp.h"
#include "Application.h"
#include "TulipApp.h"


using namespace std;
int main( int argc, char **argv ) {

  QLocale::setDefault(QLocale(QLocale::English));

  Application tulip( argc, argv );
  TulipApp *mainWindow = new TulipApp();
  QDesktopWidget desktop;
  QRect screenRect = desktop.availableGeometry();
  if (screenRect.height() > 890) {
    QRect wRect = mainWindow->geometry();
    int delta = (870 - wRect.height())/2;
    wRect.setTop(wRect.top() - delta);
    wRect.setBottom(wRect.bottom() + delta);
    mainWindow->setGeometry(wRect);
  }
  QTimer::singleShot(0, mainWindow, SLOT(startTulip()) );

  return tulip.exec();
}
