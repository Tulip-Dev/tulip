/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
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
#include "tulip/QtProgress.h"

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <QtGui/qapplication.h>
#include <QtGui/qcheckbox.h>
#include <QtGui/qlabel.h>
#include <tulip/GlMainWidget.h>
#include <QtGui/qprogressbar.h>

using namespace std;

namespace tlp {

  //=====================================
  QtProgress::QtProgress(QWidget* parent,string text,View *view):
    QDialog(parent),
    firstCall(true),label(text), parent(parent),view(view) {
    setupUi(this);
    setModal(true);
  }
  //=====================================
  QtProgress::~QtProgress() {
  }
  //=====================================
  void QtProgress::progress_handler(int i,int j) {
    //  cerr << __PRETTY_FUNCTION__ << endl;
    progressBar->setMaximum(j);
    progressBar->setValue(i);
    if (state()!=TLP_CONTINUE) { 
      return;
    }
    qApp->processEvents();
    if (firstCall) show();
    firstCall=false;
    if (view!=0 && preview->isChecked()) {
      view->init();
    }
  }
  //=====================================
  void QtProgress::setComment(string msg) {
    comment->setText(QString::fromUtf8(msg.c_str()));
    if (firstCall) show();
    firstCall=false;
    qApp->processEvents();
  }
  //=====================================
  void QtProgress::showPreview(bool flag) {
    if (flag)
      preview->show();
    else
      preview->hide();
  }
  //=====================================
  void QtProgress::stopCompute(){
    //  cerr << __PRETTY_FUNCTION__ << endl;
    PluginProgress::stop();
  }
  //=====================================
  void QtProgress::cancelCompute(){
    //  cerr << __PRETTY_FUNCTION__ << endl;
    PluginProgress::cancel();
  }
  //=====================================
  void QtProgress::changePreview(bool b){
    // cerr << __PRETTY_FUNCTION__ << endl;
    PluginProgress::setPreviewMode(b);
  }
  
}
