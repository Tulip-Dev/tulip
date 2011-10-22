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

#include <tulip/Observable.h>
#include "ui_SimplePluginProgressWidget.h"

namespace tlp {

SimplePluginProgressWidget::SimplePluginProgressWidget(QWidget *parent, Qt::WindowFlags f)
  :QWidget(parent,f), _ui(new Ui::SimplePluginProgressWidgetData) {
  _ui->setupUi(this);
  _ui->cancelButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogCancelButton));
}

void SimplePluginProgressWidget::progress_handler(int step, int max_step) {
  _ui->progressBar->setValue(step);
  _ui->progressBar->setMaximum(max_step);
}

void SimplePluginProgressWidget::preview_handler(bool p) {
  if (p)
    Observable::unholdObservers();
  else
    Observable::holdObservers();
}

void SimplePluginProgressWidget::setComment(std::string s) {
  setComment(QString(s.c_str()));
}

void SimplePluginProgressWidget::setComment(const QString &s) {
  _ui->comment->setText("<b>" + s + "</b>");
}

void SimplePluginProgressWidget::setComment(const char *s) {
  setComment(std::string(s));
}

SimplePluginProgressDialog::SimplePluginProgressDialog(QWidget *parent): QDialog(parent) {
  setModal(true);
  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->setContentsMargins(0,0,0,0);
  _progress = new SimplePluginProgressWidget(this);
  mainLayout->addWidget(static_cast<SimplePluginProgressWidget *>(_progress));
  setLayout(mainLayout);
}

SimplePluginProgressDialog::~SimplePluginProgressDialog() {
  delete _progress;
}

}
