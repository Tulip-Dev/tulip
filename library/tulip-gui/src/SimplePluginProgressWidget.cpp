/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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

#include <tulip/SimplePluginProgressWidget.h>
#include "ui_SimplePluginProgressWidget.h"

#include <QCloseEvent>
#include <QStyle>
#include <QVBoxLayout>

#include <iostream>

#include <tulip/TlpQtTools.h>
#include <tulip/TulipSettings.h>

using namespace tlp;

SimplePluginProgressWidget::SimplePluginProgressWidget(QWidget *parent, Qt::WindowFlags f)
    : QWidget(parent, f), _ui(new Ui::SimplePluginProgressWidgetData),
      _lastUpdate(QTime::currentTime()), _state(tlp::TLP_CONTINUE) {

  _ui->setupUi(this);
  _ui->cancelButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogCancelButton));
  _ui->stopButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_MediaStop));
  connect(_ui->cancelButton, SIGNAL(clicked()), this, SLOT(cancelClicked()));
  connect(_ui->stopButton, SIGNAL(clicked()), this, SLOT(stopClicked()));
}

SimplePluginProgressWidget::~SimplePluginProgressWidget() {
  delete _ui;
}

void SimplePluginProgressWidget::checkLastUpdate() {
  if (_lastUpdate.msecsTo(QTime::currentTime()) > 50) {
    QApplication::processEvents();
    _lastUpdate = QTime::currentTime();
  }
}

void SimplePluginProgressWidget::setComment(const std::string &s) {
  setComment(tlpStringToQString(s));
}

void SimplePluginProgressWidget::setComment(const QString &s) {
  _ui->comment->setText("<b>" + s + "</b>");
  checkLastUpdate();
}

void SimplePluginProgressWidget::setComment(const char *s) {
  setComment(tlpStringToQString(s));
}

ProgressState SimplePluginProgressWidget::progress(int step, int max_step) {
  PluginProgress::progress(step, max_step);
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
  _ui->previewBox->setChecked(drawPreview);
}

void SimplePluginProgressWidget::showPreview(bool showPreview) {
  _ui->previewBox->setVisible(showPreview);
  checkLastUpdate();
}

void SimplePluginProgressWidget::showStops(bool showButtons) {
  _ui->cancelButton->setVisible(showButtons);
  _ui->stopButton->setVisible(showButtons);
  if (showButtons)
    setComment("");
  else
    setComment("Processing in progress...");
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

void SimplePluginProgressWidget::closeEvent(QCloseEvent *ev) {
  ev->ignore();
}

void SimplePluginProgressWidget::setCancelButtonVisible(bool v) {
  _ui->cancelButton->setVisible(v);
}

void SimplePluginProgressWidget::setStopButtonVisible(bool v) {
  _ui->stopButton->setVisible(v);
}

void SimplePluginProgressWidget::cancelClicked() {
  cancel();
}

void SimplePluginProgressWidget::stopClicked() {
  stop();
}

// ===================
// DIALOG
// ===================

SimplePluginProgressDialog::SimplePluginProgressDialog(QWidget *parent)
    : QDialog(parent, Qt::WindowTitleHint | Qt::CustomizeWindowHint), _painted(false) {
  setModal(true);
  // configure style sheet
  QString s_sheet(R"(
QDialog, QLabel, #SimplePluginProgressWidgetData { background-color: %BG_COLOR%; }
QLabel { color: %FG_COLOR%; }
)");

  if (TulipSettings::isDisplayInDarkMode())
    s_sheet.replace("%BG_COLOR%", "#323232").replace("%FG_COLOR%", "white");
  else
    s_sheet.replace("%BG_COLOR%", "white").replace("%FG_COLOR%", "black");
  setStyleSheet(s_sheet);
  _progress = new SimplePluginProgressWidget(this);
  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->setContentsMargins(0, 0, 0, 0);
  mainLayout->addWidget(_progress);
  setWindowIcon(QIcon(":/tulip/gui/icons/tulip.ico"));
  resize(500, height());
}

SimplePluginProgressDialog::~SimplePluginProgressDialog() {
  delete _progress;
}

void SimplePluginProgressDialog::setComment(const std::string &s) {
  _progress->setComment(s);
}

void SimplePluginProgressDialog::setComment(const QString &s) {
  _progress->setComment(s);
}

void SimplePluginProgressDialog::setComment(const char *s) {
  _progress->setComment(s);
}

void SimplePluginProgressDialog::setTitle(const std::string &title) {
  setWindowTitle(tlpStringToQString(title));
  QApplication::processEvents();
}

ProgressState SimplePluginProgressDialog::progress(int step, int max_step) {
  PluginProgress::progress(step, max_step);
  return _progress->progress(step, max_step);
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
  _progress->setPreviewMode(drawPreview);
}

void SimplePluginProgressDialog::showPreview(bool showPreview) {
  _progress->showPreview(showPreview);
  if (_painted) {
    _painted = false;
    update();
    while (!_painted)
      QApplication::processEvents();
    QApplication::processEvents();
  }
}

void SimplePluginProgressDialog::showStops(bool showButtons) {
  _progress->showStops(showButtons);
  _painted = false;
  update();
  while (!_painted)
    QApplication::processEvents();
  QApplication::processEvents();
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

void SimplePluginProgressDialog::paintEvent(QPaintEvent *ev) {
  QWidget::paintEvent(ev);
  _painted = true;
}

void SimplePluginProgressDialog::closeEvent(QCloseEvent *ev) {
  ev->ignore();
}

void SimplePluginProgressDialog::setCancelButtonVisible(bool v) {
  _progress->setCancelButtonVisible(v);
}

void SimplePluginProgressDialog::setStopButtonVisible(bool v) {
  _progress->setStopButtonVisible(v);
}
