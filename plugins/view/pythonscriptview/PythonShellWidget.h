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

#ifndef PYTHONSHELLWIDGET_H_
#define PYTHONSHELLWIDGET_H_

#include <vector>

#include <QtGui/QPlainTextEdit>

#include "PythonCodeEditor.h"

#include <tulip/Graph.h>

class QKeyEvent;

class PythonShellWidget : public PythonCodeEditor {

  Q_OBJECT

public :

  PythonShellWidget(QWidget *parent = NULL);

  void keyPressEvent(QKeyEvent * e);

  bool isCursorOnLastLine();

  void executeCurrentLines();

  void setGraph(tlp::Graph *graph);

protected:

  void showEvent(QShowEvent * event);

public slots:

  void insert(const QString &txt, const bool atEnd = false);

protected slots:

  void updateAutoCompletionList(bool dotContext=false);

private :

  QString currentPs;
  QString currentCodeLines;

  std::vector<QString> history;
  int currentHistoryPos;


};

#endif /* PYTHONSHELLWIDGET_H_ */
