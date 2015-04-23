/**
 *
 * This file is part of Tulip (www.tulip-software.org)
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

#include <tulip/StringEditor.h>
#include <tulip/TlpQtTools.h>
#include <QVBoxLayout>
#include <QDialogButtonBox>

using namespace tlp;

StringEditor::StringEditor(QWidget *parent) :
  QDialog(parent) {
  QVBoxLayout* layout = new QVBoxLayout;
  setLayout(layout);
  edit = new QTextEdit(this);
  layout->addWidget(edit);
  QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Cancel|QDialogButtonBox::Ok,Qt::Horizontal);
  layout->addWidget(buttonBox);
  QWidget::setTabOrder(edit, buttonBox);
  QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
  QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
  setString(QString());
  setModal(true);
  setWindowTitle("Set string value");
}

QString StringEditor::getString()const {
  return currentString;
}

void StringEditor::setString(const QString& qstr) {
  currentString = qstr;
  blockSignals(true);
  edit->setPlainText(qstr);
  blockSignals(false);
}

void StringEditor::done(int r) {
  if (r == QDialog::Accepted)
    currentString = edit->toPlainText();

  QDialog::done(r);
}

// to ensure it is shown in the center of its parent
void StringEditor::showEvent(QShowEvent* ev) {
  QDialog::showEvent(ev);

  if (parentWidget())
    move(parentWidget()->window()->frameGeometry().topLeft() +
         parentWidget()->window()->rect().center() -
         rect().center());
}

