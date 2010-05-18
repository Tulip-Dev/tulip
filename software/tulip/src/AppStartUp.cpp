#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string>
#include <QtGui/qprogressbar.h>
#include <QtGui/qdialog.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qtextedit.h>
#include <QtGui/qlabel.h>
#include <QtGui/qapplication.h>
#include <QtCore/QSettings>
#include <tulip/TlpTools.h>
#include <tulip/PluginLoaderTxt.h>
#include <tulip/GlMainWidget.h>
#include <tulip/Glyph.h>
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

static const int tipsNumber=2;
static const std::string tips[tipsNumber] = {
  "If you want to use the new plugins (algorithms, interactors, views), look at those available in the plugin manager.<br>Menu : <b>Help -> Plugins</b>",
  "If you find a bug in tulip, please make a bug report on project page of sourceforge : http://sourceforge.net/tracker/?group_id=61223"
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

  if(dontClose){
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
  string dir = TulipLibDir;
  dir += "tlp/bitmaps/";
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
