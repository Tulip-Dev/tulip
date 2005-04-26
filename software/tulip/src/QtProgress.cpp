#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <qapplication.h>
#include "QtProgress.h"
#include <tulip/GlGraphWidget.h>
#include <qprogressbar.h>
using namespace std;

MyProgress::MyProgress(QWidget* parent,string text,GlGraphWidget *glGraphWidget):
  QtProgressData( parent, text.c_str(), Qt::WType_Popup ),
  firstCall(true),label(text),parent(parent),glGraphWidget(glGraphWidget) {
}

MyProgress::~MyProgress() {
}

void MyProgress::progress_handler(int i,int j) {
  //  cerr << __PRETTY_FUNCTION__ << endl;
  progressBar->setTotalSteps(j);
  progressBar->setProgress (i);
  if (state()!=TLP_CONTINUE) { 
    return;
  }
  qApp->processEvents();
  if (firstCall) show();
  firstCall=false;
  if (glGraphWidget!=0) {
    glGraphWidget->getGlGraph()->centerScene();
    glGraphWidget->UpdateGL();
  }
}
void MyProgress::stopCompute(){
  //  cerr << __PRETTY_FUNCTION__ << endl;
  PluginProgress::stop();
}
void MyProgress::cancelCompute(){
  //  cerr << __PRETTY_FUNCTION__ << endl;
  PluginProgress::cancel();
}
