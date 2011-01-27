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
#include "tulip/QtProgress.h"
#include "tulip/PluginProgressWidget.h"
#include "tulip/TlpQtTools.h"

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <QtGui/QVBoxLayout>
using namespace std;

namespace tlp {

    //=====================================
    QtProgress::QtProgress(QWidget* parent,string text,View *view,int updateInterval):
            QDialog(parent),progressWidget(new PluginProgressWidget(view,updateInterval,parent)),firstCall(true){
        resize(QSize(417,92));
        setWindowTitle(tlpStringToQString(text));
        setModal(true);
        QVBoxLayout *vblayout = new QVBoxLayout(this);
        vblayout->setMargin(0);
        setLayout(vblayout);
        vblayout->addWidget(progressWidget);
    }
    //=====================================
    QtProgress::~QtProgress() {
    }
    ProgressState QtProgress::progress(int step, int max_step){
        if (firstCall){
            show();
        }
        firstCall=false;
        return progressWidget->progress(step,max_step);
    }

    //=====================================

    void QtProgress::cancel(){
        progressWidget->cancel();
    }
    //=====================================
    void QtProgress::stop(){
        progressWidget->stop();
    }
    //=====================================
    bool QtProgress::isPreviewMode() const {
        return progressWidget->isPreviewMode();
    }
    //=====================================
    void QtProgress::setPreviewMode(bool mode){
        progressWidget->setPreviewMode(mode);
    }

    //=====================================
    void QtProgress::showPreview(bool show){
        progressWidget->showPreview(show);
    }
    //=====================================
    ProgressState QtProgress::state() const{
        return progressWidget->state();
    }
    //=====================================
    string QtProgress::getError(){
        return progressWidget->getError();
    }
    //=====================================
    void QtProgress::setError(string error){
        progressWidget->setError(error);
    }
    //=====================================
    void QtProgress::setComment(string msg) {
        if (firstCall) {
            show();
        }
        firstCall=false;
        progressWidget->setComment(msg);
    }
}
