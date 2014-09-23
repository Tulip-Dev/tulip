/*
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

#ifndef PYTHONSHELLWIDGET_H_
#define PYTHONSHELLWIDGET_H_

#include <QVector>

#include <tulip/PythonCodeEditor.h>

class QKeyEvent;

namespace tlp {

class TLP_PYTHON_SCOPE PythonShellWidget : public PythonCodeEditor {

  Q_OBJECT

  QString _currentPs;
  QString _currentCodeLines;

  QVector<QString> _history;
  int _currentHistoryPos;

public :

  explicit PythonShellWidget(QWidget *parent = 0);

protected:

  void keyPressEvent(QKeyEvent * e);

  bool isCursorOnLastLine();

  void executeCurrentLines();

  void showEvent(QShowEvent * event);

public slots:

  void insert(const QString &txt, const bool atEnd = false);

protected slots:

  void updateAutoCompletionList(bool dotContext=false);

};

}

#endif /* PYTHONSHELLWIDGET_H_ */
