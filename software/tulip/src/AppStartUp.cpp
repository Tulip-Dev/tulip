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

#include <time.h>
#include <string>
#include <QtGui/QDialog>
#include <QtCore/QSettings>
#include <tulip/TlpTools.h>
#include <tulip/GlyphManager.h>
#include <tulip/EdgeExtremityGlyphManager.h>
#include <tulip/InteractorManager.h>
#include <tulip/EdgeExtremityGlyphManager.h>
#include "AppStartUp.h"

#include "TulipPluginLoader.h"

using namespace std;
using namespace tlp;

static std::string errorMsgs;

static const int tipsNumber=3;
static const std::string tips[tipsNumber] = {
  "If you want to add new plugins to Tulip(algorithms, interactors, views), take a look at the plugin manager: <br>Menu : <b>Help -> Plugins</b>",
  "If you find a bug in tulip, please make a bug report on our sourceforge project page at: http://sourceforge.net/tracker/?group_id=61223",
  "Tulip 3.5.0 increased performances on edge rendering by over 200%"
};

void AppStartUp::initTulip(TulipPluginLoader *loader, std::string &errors) {
  setTotalSteps(0);
  setProgress(0);
  setLabel("Tulip");
  setAttribute(Qt::WA_DeleteOnClose);

  loader->appStartUp = this;

  tlp::loadPlugins(loader); // library side plugins

  tlp::loadPlugins(loader, "/interactors"); // interactors plugins
  tlp::loadPlugins(loader, "/glyphs"); // glyphs plugins
  tlp::loadPlugins(loader, "/view"); // view plugins
  tlp::loadPlugins(loader, "/controller"); // controller plugins

  //initialization of utility members
  GlyphManager::getInst().loadGlyphPlugins();
  EdgeExtremityGlyphManager::getInst().loadGlyphPlugins();
  InteractorManager::getInst().loadInteractorPlugins();
  
  PluginManagerInterface::checkLoadedPluginsDependencies(loader);

  errors = errorMsgs;
  // free memory
  errorMsgs.resize(0);

  if(dontClose){
    closeButton->setEnabled(true);
    setTotalSteps(100);
    setProgress(100);
  }else{
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
	QDialog(parent), currentFrame(0) {

  setupUi(this);
  setModal(true);

  string tmp = "Tulip ";
  tmp+=TULIP_RELEASE;
  setWindowTitle(tmp.c_str());
  movie = new QMovie(string(":/startUpPicture.png").c_str());
  movie->setCacheMode(QMovie::CacheAll);
  PixmapLabel1->setMovie(movie);
  movie->start();
  movie->stop();

  srand ( time(NULL) );
  currentTipNumber=rand() % tipsNumber;
  textBrowser->setHtml(tips[currentTipNumber].c_str());

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
  textBrowser->setHtml(tips[currentTipNumber].c_str());
}
