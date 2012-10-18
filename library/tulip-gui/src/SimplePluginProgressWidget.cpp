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
#include "tulip/SimplePluginProgressWidget.h"
#include <QtGui/QStylePainter>
#include <QtCore/QDebug>
#include <QtCore/QTime>
#include <iostream>

#include <tulip/Perspective.h>
#include <tulip/Observable.h>
#include "ui_SimplePluginProgressWidget.h"

using namespace tlp;

SimplePluginProgressWidget::SimplePluginProgressWidget(QWidget *parent, Qt::WindowFlags f)
  :QWidget(parent,f), _ui(new Ui::SimplePluginProgressWidgetData), _state(tlp::TLP_CONTINUE) {
  _ui->setupUi(this);
  _ui->cancelButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogDiscardButton));
  _ui->stopButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogCancelButton));
  connect(_ui->cancelButton,SIGNAL(clicked()),this,SLOT(cancelClicked()));
  connect(_ui->stopButton,SIGNAL(clicked()),this,SLOT(stopClicked()));
  connect(_ui->previewBox,SIGNAL(toggled(bool)),this,SLOT(setPreview(bool)));
}

SimplePluginProgressWidget::~SimplePluginProgressWidget() {
  delete _ui;
}

void SimplePluginProgressWidget::checkLastUpdate() {
  if (_lastUpdate.msecsTo(QTime::currentTime()) > 1000) {
    QApplication::processEvents(QEventLoop::AllEvents,1000);
    _lastUpdate = QTime::currentTime();
  }
}

void SimplePluginProgressWidget::setComment(const std::string& s) {
  setComment(QString(s.c_str()));
}

void SimplePluginProgressWidget::setComment(const QString &s) {
  _ui->comment->setText("<b>" + s + "</b>");
  checkLastUpdate();
}

void SimplePluginProgressWidget::setComment(const char *s) {
  setComment(std::string(s));
}

ProgressState SimplePluginProgressWidget::progress(int step, int max_step) {
  PluginProgress::progress(step,max_step);
  _ui->progressBar->setValue(step);
  _ui->progressBar->setMaximum(max_step);
  checkLastUpdate();
  return _state;
}

void SimplePluginProgressWidget::cancel() {
  _state = tlp::TLP_CANCEL;
}

void SimplePluginProgressWidget::stop() {
  _state = tlp::TLP_STOP;
}

bool SimplePluginProgressWidget::isPreviewMode() const {
  return _ui->previewBox->isChecked();
}

void SimplePluginProgressWidget::setPreviewMode(bool drawPreview) {
  PluginProgress::setPreviewMode(drawPreview);
  _ui->previewBox->setChecked(drawPreview);
}

void SimplePluginProgressWidget::showPreview(bool showPreview) {
  _ui->previewBox->setVisible(showPreview);
}

ProgressState SimplePluginProgressWidget::state() const {
  return _state;
}

std::string SimplePluginProgressWidget::getError() {
  return _error;
}

void SimplePluginProgressWidget::setError(const std::string &error) {
  _error = error;
}

void SimplePluginProgressWidget::setCancelButtonVisible(bool v) {
  _ui->cancelButton->setVisible(v);
}

void SimplePluginProgressWidget::setStopButtonVisible(bool v) {
  _ui->stopButton->setVisible(v);
}

void SimplePluginProgressWidget::setPreviewButtonVisible(bool v) {
  _ui->previewBox->setVisible(v);
}

void SimplePluginProgressWidget::cancelClicked() {
  cancel();
}

void SimplePluginProgressWidget::stopClicked() {
  stop();
}

void SimplePluginProgressWidget::setPreview(bool f) {
  if (f) {
    while (Observable::observersHoldCounter() > 0)
      Observable::unholdObservers();
  }
  else
    Observable::holdObservers();
}

// ===================
// DIALOG
// ===================

SimplePluginProgressDialog::SimplePluginProgressDialog(QWidget *parent): QDialog(parent) {
  setModal(true);
  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->setContentsMargins(0,0,0,0);
  _progress = new SimplePluginProgressWidget(this);
  mainLayout->addWidget(static_cast<SimplePluginProgressWidget *>(_progress));
  setLayout(mainLayout);
  resize(500,height());
}

SimplePluginProgressDialog::~SimplePluginProgressDialog() {
  delete _progress;
}

void SimplePluginProgressDialog::setComment(const std::string& s) {
  _progress->setComment(s);
}

void SimplePluginProgressDialog::setComment(const QString& s) {
  _progress->setComment(s);
}

void SimplePluginProgressDialog::setComment(const char* s) {
  _progress->setComment(s);
}

ProgressState SimplePluginProgressDialog::progress(int step, int max_step) {
  PluginProgress::progress(step,max_step);
  return _progress->progress(step,max_step);
}

void SimplePluginProgressDialog::cancel() {
  _progress->cancel();
}

void SimplePluginProgressDialog::stop() {
  _progress->stop();
}

bool SimplePluginProgressDialog::isPreviewMode() const {
  return _progress->isPreviewMode();
}

void SimplePluginProgressDialog::setPreviewMode(bool drawPreview) {
  PluginProgress::setPreviewMode(drawPreview);
  _progress->setPreviewMode(drawPreview);
}

void SimplePluginProgressDialog::showPreview(bool showPreview) {
  _progress->showPreview(showPreview);
}

ProgressState SimplePluginProgressDialog::state() const {
  return _progress->state();
}

std::string SimplePluginProgressDialog::getError() {
  return _progress->getError();
}

void SimplePluginProgressDialog::setError(const std::string &error) {
  _progress->setError(error);
}

void SimplePluginProgressDialog::setCancelButtonVisible(bool v) {
  _progress->setCancelButtonVisible(v);
}

void SimplePluginProgressDialog::setStopButtonVisible(bool v) {
  _progress->setStopButtonVisible(v);
}

void SimplePluginProgressDialog::setPreviewButtonVisible(bool v) {
  _progress->setPreviewButtonVisible(v);
}
