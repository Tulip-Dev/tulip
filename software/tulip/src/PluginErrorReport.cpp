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

#include "PluginErrorReport.h"

#include <QFileInfo>

PluginErrorReport::PluginErrorReport(const QString &fileName, const QString &errorMsg,
                                     QWidget *parent)
    : QLabel(parent) {
  setStyleSheet("#PluginErrorReportData { background-color: white; }");

  QFileInfo fileInfo(fileName);
  setText("<html><head/><body><p><b>" + fileInfo.fileName() + "</b></p><p><span style=\"font-size:small;\">" + errorMsg + "</span></p></body></head>");

}

void PluginErrorReport::focusInEvent(QFocusEvent *) {
  setStyleSheet("#PluginErrorReportData { background-color: rgb(232, 238, 244); border: 1px solid "
                "#C9C9C9; }");
  repaint();
}

void PluginErrorReport::focusOutEvent(QFocusEvent *) {
  setStyleSheet("#PluginErrorReportData { background-color: white; }");
  repaint();
}
