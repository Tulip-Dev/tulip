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
#include "tulip/QtOpenGlErrorViewer.h"

#include <QtGui/QMessageBox>
#include <QtGui/QDialog>
#include <QtCore/QSettings>
#include "ui_QtOpenGlErrorViewerWithAskAgainDialog.h"

namespace tlp {

QtOpenGlErrorViewerWithAskAgainDialog::QtOpenGlErrorViewerWithAskAgainDialog(QWidget *parent):QDialog(parent),_ui(new Ui::QtOpenGlErrorViewerWithAskAgainDialogData()){
  _ui->setupUi(this);
  connect(_ui->okButton,SIGNAL(clicked()),this,SLOT(accept()));
}

void QtOpenGlErrorViewer::displayError(const std::string &title,const std::string &errorMsg){
  QMessageBox::warning(parent, title.c_str(), errorMsg.c_str());
}

void QtOpenGlErrorViewer::displayErrorWithAskAgain(const std::string &title,const std::string &errorMsg){
  QSettings settings("TulipSoftware","Tulip");
  settings.beginGroup("QtOpenGlErrorViewer");
  bool askAgain=settings.value(title.c_str(),true).toBool();

  if(askAgain){
    QtOpenGlErrorViewerWithAskAgainDialog dialog;
    dialog.setWindowTitle(title.c_str());
    dialog._ui->textLabel->setText(errorMsg.c_str());
    dialog.exec();
    settings.setValue(title.c_str(),dialog._ui->askAgainCheckBox->isChecked());
  }

  settings.endGroup();

}

}
