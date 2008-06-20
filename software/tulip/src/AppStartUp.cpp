#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string>
#include <QtGui/qprogressbar.h>
#include <QtGui/qdialog.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qtextedit.h>
#include <QtGui/qlabel.h>
#include <tulip/TlpTools.h>
#include <tulip/PluginLoaderTxt.h>
#include <tulip/GlMainWidget.h>
#include <tulip/Glyph.h>
#include <tulip/GlyphManager.h>
#include <tulip/InteractorManager.h>
#include <tulip/ViewPluginsManager.h>
#include <QtGui/qapplication.h>
#include "AppStartUp.h"

#include "TulipPluginLoader.h"

using namespace std;
using namespace tlp;

static std::string errorMsgs;



void AppStartUp::initTulip(TulipPluginLoader *loader,std::string &errors) {
  setTotalSteps(0);
  setProgress(0);
  setLabel("Tulip");

  loader->appStartUp = this;

  /*#ifndef NDEBUG
  PluginLoaderTxt plug;
  #else
  PluginLoaderQt plug;
  plug.appStartUp = this;
  #endif*/
  
  //tlp::initTulipLib(); already done in Application.cpp
  tlp::loadPlugins(loader);   // library side plugins
  GlyphManager::getInst().loadPlugins(loader);   // software side plugins, i.e. glyphs
  InteractorManager::getInst().loadPlugins(loader); // interactors plugins
  ViewPluginsManager::getInst().loadPlugins(loader); // view plugins

  errors = errorMsgs;
  // free memory
  errorMsgs.resize(0);
}

// accumulate error messages
void AppStartUp::addErrorMsg(const std::string& errMsg)  {
  errorMsgs += errMsg + '\n';
}

/* 
 *  Constructs a AppStartUp which is a child of 'parent'
 */
AppStartUp::AppStartUp( QWidget* parent)
  : QDialog(parent),
    currentFrame(0){
  setupUi(this);
  string tmp="Tulip ";
  tmp+=VERSION;
  tulipVersion->setText(tmp.c_str());
  string dir=TulipLibDir;
  dir += "tlp/bitmaps/";
  movie=new QMovie(string(dir+"startup.gif").c_str());
  movie->setCacheMode(QMovie::CacheAll);
  PixmapLabel1->setMovie(movie);
  movie->start();
  movie->stop();
}

/*  
 *  Destroys the object and frees any allocated resources
 */
AppStartUp::~AppStartUp() {
    // no need to delete child widgets, Qt does it all for us
}

void AppStartUp::setProgress(int progress) {
  progressBar->setValue(progress);
  if(progress!=0 && currentFrame!=99) {
    currentFrame=(int)(((float)progress/((float)totalSteps*0.75))*100);
    if(currentFrame>99)
      currentFrame=99;
    movie->jumpToFrame(currentFrame);
  }
}
 
void AppStartUp::setTotalSteps(int totalSteps) {
  this->totalSteps=totalSteps;
  progressBar->setMaximum(totalSteps);
}
 
void AppStartUp::setLabel(string str) {
  QString tmpQString=str.c_str();
  textLabel->setText(tmpQString);
}
