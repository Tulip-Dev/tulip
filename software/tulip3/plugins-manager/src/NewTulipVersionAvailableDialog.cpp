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
#include "NewTulipVersionAvailableDialog.h"

#include <iostream>

#include "QtCore/QSettings"

using namespace std;

namespace tlp {

  NewTulipVersionAvailableDialog::NewTulipVersionAvailableDialog(const QString &version,QWidget* parent) : QDialog(parent),version(version) {
    setupUi(this);

    tulipVersion->setText(QString("Tulip ")+version);
    connect(okButton,SIGNAL(clicked()),this,SLOT(okClicked()));
    connect(askMeAgain,SIGNAL(stateChanged(int)),this,SLOT(askMeAgainStateChanged(int)));
  }

  bool NewTulipVersionAvailableDialog::needDisplayDialog(const QString &version){
    bool result;
    QSettings settings("TulipSoftware","Tulip");
    settings.beginGroup("Preference");
    result=settings.value("AskAgainForVersion_"+version,true).toBool();
    settings.endGroup();
    return result;
  }

  void NewTulipVersionAvailableDialog::okClicked() {
    accept();
  }

  void NewTulipVersionAvailableDialog::askMeAgainStateChanged(int state){
    if(state==Qt::Unchecked){
      QSettings settings("TulipSoftware","Tulip");
      settings.beginGroup("Preference");
      settings.setValue("AskAgainForVersion_"+version,false);
      settings.endGroup();
    }else{
      QSettings settings("TulipSoftware","Tulip");
      settings.beginGroup("Preference");
      settings.setValue("AskAgainForVersion_"+version,true);
      settings.endGroup();
    }

  }

}
