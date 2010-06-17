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
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef  _WIN32
// compilation pb workaround
#include <windows.h>
#endif
#include <QtCore/qstring.h>
#include <QtCore/qstringlist.h>
#include <QtGui/qlistview.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qmessagebox.h>
#include <QtGui/qinputdialog.h>
#include <QtGui/qlabel.h>
#include <QtGui/qtabwidget.h>

#include "CopyPropertyDialog.h"
#include "tulip/ForEach.h"

using namespace std;
using namespace tlp;

//=============================================================================
CopyPropertyDialog::CopyPropertyDialog(QWidget* parent)
  : QDialog(parent) {
  setupUi(this);
  connect((QObject *) buttonOK , SIGNAL(clicked()), this, SLOT(accept()) );
  connect((QObject *) buttonCancel , SIGNAL(clicked()), this, SLOT(reject()) );
}

void CopyPropertyDialog::setProperties(std::string& srcProp,
				       std::vector<std::string>& localProps,
				       std::vector<std::string>& inheritedProps) {
  setWindowTitle((std::string("Copy property ") + srcProp).c_str());
  unsigned int i = 0;
  if (localProps.size() == 0)
    localPropertyButton->setEnabled(false);
  else
    for (; i < localProps.size(); ++i)
      localProperties->addItem(QString(localProps[i].c_str()));
  if (inheritedProps.size() == 0)
    inheritedPropertyButton->setEnabled(false);
  else
    for (i = 0; i < inheritedProps.size(); ++i)
      inheritedProperties->addItem(QString(inheritedProps[i].c_str()));
}

std::string CopyPropertyDialog::getDestinationProperty(CopyPropertyDialog::destType& type) {
  type = CopyPropertyDialog::NEW;
  if (exec() == QDialog::Accepted) {
    if (newPropertyText->isEnabled())
      return newPropertyText->text().toAscii().data();
    type = CopyPropertyDialog::LOCAL;
    if (localProperties->isEnabled())
      return localProperties->currentText().toAscii().data();
    type = CopyPropertyDialog::INHERITED;
      return inheritedProperties->currentText().toAscii().data();
  }
  return std::string();
}

void CopyPropertyDialog::setNew(bool flag) {
  newPropertyText->setEnabled(flag);
}

void CopyPropertyDialog::setLocal(bool flag) {
  localProperties->setEnabled(flag);
}

void CopyPropertyDialog::setInherited(bool flag) {
  inheritedProperties->setEnabled(flag);
}
