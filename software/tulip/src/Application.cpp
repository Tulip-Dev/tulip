#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tulip/TlpTools.h>
#include "Application.h"

#include <qdir.h>

using namespace std;
//**********************************************************************
Application::Application(int argc, char ** argv): QApplication(argc,argv) 
{
  tlp::initTulipLib((char *) QApplication::applicationDirPath().ascii());
  string::const_iterator begin=tlp::TulipPluginsPath.begin();
  string::const_iterator end=begin;
  while (end!=tlp::TulipPluginsPath.end())
    if ((*end)==tlp::PATH_DELIMITER) {
      if (begin!=end) {
	string path = string(begin,end) + "/../bitmaps/";
	QDir *bitmapsDir= new QDir(path.c_str());
	if(bitmapsDir->exists()) {
	  ((Application *)qApp)->bitmapPath = path.c_str();
	  delete bitmapsDir;
	  return;
	}
	delete bitmapsDir;
      }
      ++end;
      begin=end;
    }
    else
      ++end;
  if (begin!=end) {
    string path = string(begin,end) + "/../bitmaps/";
    QDir *bitmapsDir= new QDir(path.c_str());
    if(bitmapsDir->exists()) {
      ((Application *)qApp)->bitmapPath = path.c_str();
      delete bitmapsDir;
      return;
    }
    delete bitmapsDir;
  }
} 

//**********************************************************************
Application::~Application() {
}
//**********************************************************************










