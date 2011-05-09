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
#include "tulip/PluginProgressWidget.h"
#include "ui_PluginProgressWidget.h"

using namespace tlp;
using namespace std;

PluginProgressWidget::PluginProgressWidget(QWidget *parent):QWidget(parent),SimplePluginProgress(),view(NULL),updateIterval(200),time(QTime::currentTime()),
ui(new Ui::PluginProgressWidget){
ui->setupUi(this);
connect(ui->stopPushButton,SIGNAL(clicked(bool)),this,SLOT(stopCompute()));
connect(ui->cancelPushButton,SIGNAL(clicked(bool)),this,SLOT(cancelCompute()));
connect(ui->preview,SIGNAL(toggled(bool)),this,SLOT(changePreview(bool)));
}

PluginProgressWidget::PluginProgressWidget(View *view,int updateInterval,QWidget *parent) :
        QWidget(parent),SimplePluginProgress(),view(view),updateIterval(updateInterval),time(QTime::currentTime()),
    ui(new Ui::PluginProgressWidget)
{
    ui->setupUi(this);
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(0);
    connect(ui->stopPushButton,SIGNAL(clicked(bool)),this,SLOT(stopCompute()));
    connect(ui->cancelPushButton,SIGNAL(clicked(bool)),this,SLOT(cancelCompute()));
    connect(ui->preview,SIGNAL(toggled(bool)),this,SLOT(changePreview(bool)));
}

PluginProgressWidget::~PluginProgressWidget()
{
    delete ui;
}

ProgressState PluginProgressWidget::progress(int step, int max_step) {
  progress_handler(step, max_step);
  return state();
}

//=====================================
void PluginProgressWidget::preview_handler(bool b) {
    if (ui->preview->isChecked() == b) return;
    ui->preview->setChecked(b);
}
//=====================================
void PluginProgressWidget::progress_handler(int i,int j) {
  if (state()!=TLP_CONTINUE) {
    return;
}
  if(time.msecsTo(QTime::currentTime())> updateIterval){
      ui->progressBar->setMaximum(j);
      ui->progressBar->setValue(i);
      qApp->processEvents();
      time = QTime::currentTime();
  }  
  if (view!=0 && isPreviewMode()) {
    view->init();
  }
}
//=====================================
void PluginProgressWidget::setComment(string msg) {
  ui->comment->setText(QString::fromUtf8(msg.c_str()));  
  qApp->processEvents();
}
//=====================================
void PluginProgressWidget::showPreview(bool flag) {
  if (flag)
    ui->preview->show();
  else
    ui->preview->hide();
}
//=====================================
void PluginProgressWidget::stopCompute(){
  SimplePluginProgress::stop();
}
//=====================================
void PluginProgressWidget::cancelCompute(){
  SimplePluginProgress::cancel();
}
//=====================================
void PluginProgressWidget::changePreview(bool b){
  SimplePluginProgress::setPreviewMode(b);
}
