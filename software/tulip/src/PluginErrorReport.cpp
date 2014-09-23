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

#include "PluginErrorReport.h"

#include <QFileInfo>
#include <QDebug>

#include "ui_PluginErrorReport.h"

PluginErrorReport::PluginErrorReport(const QString &fileName, const QString &errorMsg, QWidget *parent): QWidget(parent), _ui(new Ui::PluginErrorReportData()) {
  setStyleSheet("#PluginErrorReportData { background-color: white; }");
  _ui->setupUi(this);

  QFileInfo fileInfo(fileName);
  _ui->fileName->setText("<html><head/><body><b>" + fileInfo.fileName() + "</b></body></head>");

  _ui->errorMsg->setText("<p><span style=\"font-size:small;\">" +
                         errorMsg +
                         "</span></p>");

}

void PluginErrorReport::focusInEvent(QFocusEvent *) {
  setStyleSheet("#PluginErrorReportData { background-color: rgb(232, 238, 244); border: 1px solid #C9C9C9; }");
  repaint();
}

void PluginErrorReport::focusOutEvent(QFocusEvent *) {
  setStyleSheet("#PluginErrorReportData { background-color: white; }");
  repaint();
}
