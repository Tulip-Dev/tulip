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

#include <time.h>
#include <string>
#include <QtGui/QDialog>
#include <QtCore/QSettings>
#include <tulip/TlpTools.h>
#include <tulip/GlyphManager.h>
#include <tulip/EdgeExtremityGlyphManager.h>
#include <tulip/InteractorManager.h>
#include <tulip/ViewPluginsManager.h>
#include <tulip/ControllerPluginsManager.h>
#include <tulip/EdgeExtremityGlyphManager.h>
#include "AppStartUp.h"

#include "TulipPluginLoader.h"

using namespace std;
using namespace tlp;

static std::string errorMsgs;

static unsigned int tipsNumber = 0;
static const char* tips[] = {
  "A lot of Tulip plugins (algorithms, interactors, views) are available for download, take a look at the plugin manager: <br/>Menu : <b>Help -> Plugins</b>",
  "If you find a bug in Tulip, please make a bug report on our sourceforge project page at: http://sourceforge.net/tracker/?group_id=61223",
  "Pajek file import is now available in Tulip using<br/>Menu : <b>File -> Import -> File -> Pajek (.net)</b>",
  "UCINET dl file import is now available in Tulip using<br/>Menu : <b>File -> Import -> File -> UCINET dl</b>",
  "GEXF file import is now available in Tulip using<br/>Menu : <b>File -> Import -> File -> GEXF</b>",
  "A Python interpreter enables to dynamically load, save, write, run and test your code. It is available using<br/>Menu : <b>View -> Python Script view</b>",
  "OGDF layout algorithms are available using<br/>Menu : <b>Algorithms -> Layout -> ...</b>",
  "Take a snapshot of the visualization of your data using<br/>Toolbar : <b>snapshot button</b>",
  /* last one needs to be NULL */
  NULL
};

void AppStartUp::initTulip(TulipPluginLoader *loader, std::string &errors) {
  setTotalSteps(0);
  setProgress(0);
  setLabel("Tulip");
  setAttribute(Qt::WA_DeleteOnClose);

  loader->appStartUp = this;

  tlp::loadPlugins(loader); // library side plugins
  GlyphManager::getInst().loadPlugins(loader); // software side plugins, i.e. glyphs
  EdgeExtremityGlyphManager::getInst().loadPlugins(loader);
  InteractorManager::getInst().loadPlugins(loader); // interactors plugins
  ViewPluginsManager::getInst().loadPlugins(loader); // view plugins
  ControllerPluginsManager::getInst().loadPlugins(loader); // controller plugins

  loadPluginsCheckDependencies(loader);

  errors = errorMsgs;
  // free memory
  errorMsgs.resize(0);

  if(dontClose) {
    closeButton->setEnabled(true);
    setTotalSteps(100);
    setProgress(100);
  }
  else {
    close();
  }
}

// accumulate error messages
void AppStartUp::addErrorMsg(const std::string& errMsg) {
  errorMsgs += errMsg + '\n';
}

/*
 *  Constructs a AppStartUp which is a child of 'parent'
 */
AppStartUp::AppStartUp(QWidget* parent) :
  QDialog(parent), movie(NULL), currentFrame(0) {

  setupUi(this);
  setModal(true);

// fix a strange bug when linking to the qt debug libraries for mingw
// when visibility of the text in the progress bar is enabled, tulip crashes at startup
#if defined(__MINGW32__) && !defined(NDEBUG)
  progressBar->setTextVisible(false);
#endif

  string tmp = "Tulip ";
  tmp+=TULIP_RELEASE;
  setWindowTitle(tmp.c_str());
  movie = new QMovie(string(":/startUpPicture.png").c_str());
  movie->setCacheMode(QMovie::CacheAll);
  PixmapLabel1->setMovie(movie);
  movie->start();
  movie->stop();

  // compute tipsNumber
  while(tips[tipsNumber]) ++tipsNumber;

  srand ( time(NULL) );
  currentTipNumber=rand() % tipsNumber;
  textBrowser->setHtml(tips[currentTipNumber]);

  QSettings settings("TulipSoftware","Tulip");
  settings.beginGroup("Preference");
  dontClose=settings.value("dontCloseStartUp",true).toBool();
  settings.endGroup();

  if(!dontClose)
    showAgain->setCheckState(Qt::Unchecked);

  closeButton->setEnabled(false);

  connect(closeButton,SIGNAL(clicked()),this,SLOT(close()));
  connect(nextButton,SIGNAL(clicked()),this,SLOT(nextTip()));
}

/*
 *  Destroys the object and frees any allocated resources
 */
AppStartUp::~AppStartUp() {
  QSettings settings("TulipSoftware","Tulip");
  settings.beginGroup("Preference");
  settings.setValue("dontCloseStartUp",showAgain->checkState()==Qt::Checked);
  settings.endGroup();
  delete movie;
}

void AppStartUp::setProgress(int progress) {
  progressBar->setValue(progress);

  if (progress != 0 && currentFrame != 99) {
    currentFrame = (int) (((float) progress / ((float) totalSteps * 0.75))
                          * 100);

    if (currentFrame > 99)
      currentFrame = 99;

    movie->jumpToFrame(currentFrame);
  }
}

void AppStartUp::setTotalSteps(int totalSteps) {
  this->totalSteps = totalSteps;
  progressBar->setMaximum(totalSteps);
}

void AppStartUp::setLabel(string str) {
  QString tmpQString = str.c_str();
  //textLabel->setText(tmpQString);
}

void AppStartUp::nextTip() {
  currentTipNumber = (currentTipNumber + 1) % tipsNumber;
  textBrowser->setHtml(tips[currentTipNumber]);
}
