#include <iostream>

#include <stdlib.h>
#include <QtCore/QSettings>
#include "TulipApp.h"

using namespace std;

int main( int argc, char **argv ) {
  if (TulipApp::needRestart())
    cout << "TRUE" << endl;
  else
    cout << "FALSE" << endl;

  return 0;
}
