/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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

#ifndef NEED_CONFIG_MSG_BOX_H
#define NEED_CONFIG_MSG_BOX_H

#include <QAbstractButton>
#include <QApplication>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QSpacerItem>
#include <QVBoxLayout>
#include <tulip/Perspective.h>

QFrame *new_NeedConfigurationMsgBox(QString text, QPushButton **okButton) {
  QFrame *frame = new QFrame();
  frame->setFrameShape(QFrame::Box);
  frame->setFrameShadow(QFrame::Plain);
  QVBoxLayout *frameLayout = new QVBoxLayout(frame);
  QFormLayout *formLayout = new QFormLayout();
  QLabel *warningLabel = new QLabel(frame);
  warningLabel->setText(
      "<html><head/><body><p><span style=\" font-size:18pt; font-weight:600; color:#a51d2d;\">Warning !</span></p></body></html>");
  formLayout->setWidget(0, QFormLayout::LabelRole, warningLabel);

  QVBoxLayout *verticalLayout = new QVBoxLayout();
  QLabel *warningText = new QLabel(text, frame);
  verticalLayout->addWidget(warningText);

  QSpacerItem *verticalSpacer =
      new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);
  verticalLayout->addItem(verticalSpacer);

  QHBoxLayout *horizontalLayout = new QHBoxLayout();
  QDialogButtonBox *buttonBox = new QDialogButtonBox(frame);
  buttonBox->setStandardButtons(QDialogButtonBox::Ok);
  *okButton = buttonBox->button(QDialogButtonBox::Ok);

  horizontalLayout->addWidget(buttonBox);
  verticalLayout->addLayout(horizontalLayout);
  formLayout->setLayout(0, QFormLayout::FieldRole, verticalLayout);
  frameLayout->addLayout(formLayout);

  // set a specific name before applying style sheet
  frame->setObjectName("needConfigurationMessageBox");
  tlp::Perspective::setStyleSheet(frame);
  frame->resize(/*416, 158*/ 365, 151);

  return frame;
}

#endif // NEED_CONFIG_MSG_BOX_H
