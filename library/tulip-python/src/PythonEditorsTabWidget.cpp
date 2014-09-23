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

#include <QFileInfo>

#include "tulip/PythonEditorsTabWidget.h"
#include "tulip/PythonCodeEditor.h"

using namespace tlp;

PythonEditorsTabWidget::PythonEditorsTabWidget(QWidget *parent) : QTabWidget(parent), _fontZoom(0), _dontTreatFocusIn(false) {
  connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTabRequested(int)));
}

int PythonEditorsTabWidget::addEditor(const QString &fileName) {
  PythonCodeEditor *codeEditor = new PythonCodeEditor();

  QFileInfo fileInfo(fileName);
  codeEditor->loadCodeFromFile(fileName);
  codeEditor->analyseScriptCode(true);
  codeEditor->setFocus(Qt::ActiveWindowFocusReason);
  codeEditor->installEventFilter(this);
  connect(codeEditor, SIGNAL(textChanged()), this, SLOT(scriptTextChanged()));
  int idx = addTab(codeEditor, fileInfo.fileName());
  setTabToolTip(idx, fileInfo.absoluteFilePath());
  setCurrentIndex(idx);

  if (_fontZoom < 0) {
    for (int i = _fontZoom ; i < 0 ; ++i) {
      codeEditor->zoomOut();
    }
  }

  if (_fontZoom > 0) {
    for (int i = _fontZoom ; i > 0 ; --i) {
      codeEditor->zoomIn();
    }
  }

  return idx;
}

PythonCodeEditor *PythonEditorsTabWidget::getCurrentEditor() const {
  return getEditor(currentIndex());
}

PythonCodeEditor *PythonEditorsTabWidget::getEditor(int editorIdx) const {
  if (editorIdx >= 0 && editorIdx < count()) {
    return static_cast<PythonCodeEditor*>(widget(editorIdx));
  }
  else {
    return NULL;
  }
}

void PythonEditorsTabWidget::scriptTextChanged() {

  if (QObject::sender() != widget(currentIndex()))
    return;

  QString curTabText = tabText(currentIndex());

  if (curTabText == "")
    return;

  if (!curTabText.contains("no file") && curTabText[curTabText.size() -1] != '*') {
    curTabText += "*";
    setTabText(currentIndex(), curTabText);
  }
}

void PythonEditorsTabWidget::indicateErrors(const QMap<QString, QVector<int> > &errorLines) {
  for (int i = 0 ; i < count() ; ++i) {
    QString moduleFile = getEditor(i)->getFileName();

    if (errorLines.find(moduleFile) != errorLines.end()) {
      const QVector<int> &linesErrorNumbers = errorLines[moduleFile];
      PythonCodeEditor *codeEditor = getEditor(i);

      for (int i = 0 ; i < linesErrorNumbers.size() ; ++i) {
        codeEditor->indicateScriptCurrentError(linesErrorNumbers[i]-1);
      }
    }
  }
}

void PythonEditorsTabWidget::clearErrorIndicators() {
  for (int i = 0 ; i < count() ; ++i) {
    getEditor(i)->clearErrorIndicator();
  }
}

bool PythonEditorsTabWidget::eventFilter(QObject *obj, QEvent *event) {
#ifdef __APPLE__
  Qt::KeyboardModifiers modifier = Qt::MetaModifier;
#else
  Qt::KeyboardModifiers modifier = Qt::ControlModifier;
#endif

  if (event->type() == QEvent::KeyPress) {
    QKeyEvent *keyEvt = static_cast<QKeyEvent *>(event);

    if (keyEvt->modifiers() == modifier && keyEvt->key() == Qt::Key_S) {
      if (obj == getCurrentEditor()) {
        saveCurrentEditorContentToFile();
        return true;
      }
    }
  }
  else if (event->type() == QEvent::FocusIn && !_dontTreatFocusIn) {

    _dontTreatFocusIn = true;
    reloadCodeInEditorsIfNeeded();
    _dontTreatFocusIn = false;

    return true;
  }

  return false;
}

void PythonEditorsTabWidget::reloadCodeInEditorsIfNeeded() {
  bool emitSignal = false;

  for (int i = 0 ; i < count() ; ++i) {
    emitSignal = reloadCodeInEditorIfNeeded(i) || emitSignal;
  }

  if (emitSignal)
    emit filesReloaded();
}

bool PythonEditorsTabWidget::reloadCodeInEditorIfNeeded(int index) {
  PythonCodeEditor *codeEditor = getEditor(index);
  QString fileName = codeEditor->getFileName();

  if (fileName != "") {
    QFileInfo fileInfo(fileName);

    if (fileInfo.exists() && fileInfo.lastModified() != codeEditor->getLastSavedTime()) {
      if (codeEditor->loadCodeFromFile(fileName)) {
        setTabText(index, fileInfo.fileName());
      }

      return true;
    }
  }

  return false;
}

void PythonEditorsTabWidget::saveCurrentEditorContentToFile() {
  saveEditorContentToFile(currentIndex());
}

void PythonEditorsTabWidget::saveEditorContentToFile(int editorIdx) {
  if (editorIdx >= 0 && editorIdx < count()) {
    QString moduleNameExt = tabText(editorIdx);
    QString moduleName;

    if (!moduleNameExt.contains("no file")) {

      if (moduleNameExt[moduleNameExt.size() - 1] == '*')
        moduleName = moduleNameExt.mid(0, moduleNameExt.size() - 4);
      else
        moduleName = moduleNameExt.mid(0, moduleNameExt.size() - 3);

      setTabText(editorIdx, moduleName+".py");
      QFile file(getEditor(editorIdx)->getFileName());
      QFileInfo fileInfo(file);

      if (getEditor(editorIdx)->saveCodeToFile()) {
        setTabToolTip(editorIdx, fileInfo.absoluteFilePath());
      }

      emit fileSaved(editorIdx);
    }
  }
}

void PythonEditorsTabWidget::closeTabRequested(int tab) {
  emit tabAboutToBeDeleted(tab);
  QWidget *editorWidget = widget(tab);
  _dontTreatFocusIn = true;
  delete editorWidget;
  _dontTreatFocusIn = false;
}

void PythonEditorsTabWidget::decreaseFontSize() {
  for (int i = 0 ; i < count() ; ++i) {
    getEditor(i)->zoomOut();
  }

  --_fontZoom;
}

void PythonEditorsTabWidget::increaseFontSize() {
  for (int i = 0 ; i < count() ; ++i) {
    getEditor(i)->zoomIn();
  }

  ++_fontZoom;
}
