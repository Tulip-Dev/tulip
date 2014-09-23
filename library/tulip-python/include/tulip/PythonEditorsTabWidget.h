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

#ifndef PYTHONEDITORSTABWIDGET_H
#define PYTHONEDITORSTABWIDGET_H

#include <QMap>
#include <QTabWidget>

#include <tulip/tulipconf.h>

namespace tlp {

class PythonCodeEditor;

class TLP_PYTHON_SCOPE PythonEditorsTabWidget : public QTabWidget {

  Q_OBJECT

  int _fontZoom;
  bool _dontTreatFocusIn;
  bool reloadCodeInEditorIfNeeded(int index);

public:

  explicit PythonEditorsTabWidget(QWidget *parent = 0);

  int addEditor(const QString &fileName = "");

  PythonCodeEditor *getCurrentEditor() const;

  PythonCodeEditor *getEditor(int) const;

  void indicateErrors(const QMap<QString, QVector<int> > &errorLines);

  void clearErrorIndicators();

  bool eventFilter(QObject *, QEvent *);

  void saveCurrentEditorContentToFile();

  void saveEditorContentToFile(int);

  void increaseFontSize();

  void decreaseFontSize();

signals:

  void tabAboutToBeDeleted(int);

  void fileSaved(int);

  void filesReloaded();

public slots:

  void scriptTextChanged();

  void reloadCodeInEditorsIfNeeded();

  void closeTabRequested(int tab);

};

}

#endif // PYTHONEDITORSTABWIDGET_H
