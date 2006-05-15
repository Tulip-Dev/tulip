#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string>
#include <qprogressbar.h>
#include <qlabel.h>
#include <tulip/TlpTools.h>
#include <tulip/PluginLoaderTxt.h>
#include <tulip/GlGraphWidget.h>
#include <tulip/Glyph.h>
#include <qapplication.h>
#include "AppStartUp.h"

using namespace std;
using namespace tlp;

//==============================================================
struct PluginLoaderQt:public PluginLoader {
  AppStartUp *appStartUp;
  int progress;
  virtual void start(const string &path,const string &type) {
    appStartUp->setProgress(0);
    progress=0;
  }
  virtual void numberOfFile(int nbFile) {
    appStartUp->setTotalSteps(nbFile);
    qApp->processEvents();
  }
  virtual void loading(const string &filename){}
  virtual void loaded(const string &name,
		      const string &author,
		      const string &date, 
		      const string &info,
		      const string &release,
		      const string &version)
  {
    progress++;
    appStartUp->setLabel(name);
    appStartUp->setProgress(progress);
    qApp->processEvents();
  }
  virtual void aborted(const string &filename,const  string &erreurmsg) {
    progress++;
    //    cerr << "Loading error : " << erreurmsg << endl;
    appStartUp->setLabel("Error");
    appStartUp->setProgress(progress);
    qApp->processEvents();
  }
  virtual void finished(bool state,const string &msg){}
};


void AppStartUp::initTulip() {
  setTotalSteps(0);
  setProgress(0);
  setLabel("Tulip");

  #ifndef NDEBUG
  PluginLoaderTxt plug;
  #else
  PluginLoaderQt plug;
  plug.appStartUp = this;
  #endif
  
  //tlp::initTulipLib(); already done in Application.cpp
  tlp::loadPlugins(&plug);   // library side plugins
  GlGraph::loadPlugins(&plug);   // software side plugins, i.e. glyphs
}

/* 
 *  Constructs a AppStartUp which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
AppStartUp::AppStartUp( QWidget* parent,  const char* name, bool modal, WFlags fl )
    : AppStartUpForm( parent, name, modal, fl ) {
  string tmp="Tulip V. ";
  tmp+=VERSION;
  tulipVersion->setText(tmp.c_str());
}

/*  
 *  Destroys the object and frees any allocated resources
 */
AppStartUp::~AppStartUp() {
    // no need to delete child widgets, Qt does it all for us
}

void AppStartUp::setProgress(int progress) {
  progressBar->setProgress(progress);
}
 
void AppStartUp::setTotalSteps(int totalSteps) {
  progressBar->setTotalSteps(totalSteps);
}
 
void AppStartUp::setLabel(string str) {
  QString tmpQString=str.c_str();
  textLabel->setText(tmpQString);
}
