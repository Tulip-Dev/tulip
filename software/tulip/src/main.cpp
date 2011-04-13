/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <QtCore/QTimer>
#include <QtCore/QLocale>
#include <QtGui/QDesktopWidget>

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
