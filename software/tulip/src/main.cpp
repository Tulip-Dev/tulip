#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <qfileinfo.h>
#include <qtimer.h>

#include <tulip/TlpTools.h>
#include "AppStartUp.h"
#include "Application.h"
#include "viewGl.h"

using namespace std;
int main( int argc, char **argv ) {
  Application tulip( argc, argv );
  qApp=&tulip;
  /*
    AppStartUp *appStart=new AppStartUp();
    tulip.setMainWidget(appStart);
    appStart->initTulip();
  */
  viewGl *mainWindow;
  mainWindow = new viewGl();
  tulip.setMainWidget(mainWindow);
  QTimer::singleShot(0, mainWindow, SLOT(startTulip()) );

  //  mainWindow->show();
  /*  if (argc>1) {
    for (int i=1;i<argc;++i) {
      QFileInfo info(argv[i]);
      QString s = info.absFilePath();
      mainWindow->fileOpen(0, s);
    }
    }*/
  //  delete appStart;

  return tulip.exec();
}
