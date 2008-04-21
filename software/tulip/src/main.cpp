#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <qfileinfo.h>
#include <qtimer.h>

#include "AppStartUp.h"
#include "Application.h"
#include "viewGl.h"

using namespace std;
int main( int argc, char **argv ) {
  Application tulip( argc, argv );
  viewGl *mainWindow = new viewGl();

  //tulip.setMainWidget(mainWindow);
  QTimer::singleShot(0, mainWindow, SLOT(startTulip()) );

  return tulip.exec();
}
