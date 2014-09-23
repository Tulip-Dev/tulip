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

#ifndef PYTHONCODEEDITOR2_H_
#define PYTHONCODEEDITOR2_H_

#include <tulip/AutoCompletionDataBase.h>

#include <QDateTime>
#include <QListWidget>
#include <QPlainTextEdit>
#include <QDialog>

namespace Ui {
class FindReplaceDialogData;
}

class PythonCodeHighlighter;
class ParenMatcherHighlighter;

namespace tlp {

class PythonCodeEditor;
class LineNumberArea;

class TLP_PYTHON_SCOPE AutoCompletionList : public QListWidget {

  tlp::PythonCodeEditor *_codeEditor;
  bool _activated;
  bool _wasActivated;

public :

  explicit AutoCompletionList(tlp::PythonCodeEditor *parent=0);

protected :

  void insertSelectedItem();
  void keyPressEvent(QKeyEvent *e);
  void showEvent(QShowEvent * event);
  void hideEvent(QHideEvent * event);
  void mouseDoubleClickEvent(QMouseEvent * event);
  bool eventFilter(QObject *obj, QEvent * event);

};

class TLP_PYTHON_SCOPE FindReplaceDialog : public QDialog  {

  Q_OBJECT

  Ui::FindReplaceDialogData *_ui;
  QPlainTextEdit *_editor;
  QString _lastSearch;
  bool _resetSearch;

  void setSearchResult(const bool result);

public :

  FindReplaceDialog(QPlainTextEdit *_editor, QWidget *parent=NULL);
  ~FindReplaceDialog();

  void setFindMode(const bool findMode);

  void setTextToFind(const QString &text);


public slots:

  void textToFindChanged();
  bool doFind();
  bool doReplace();
  void doReplaceFind();
  void doReplaceAll();
  void setResetSearch() {
    _resetSearch = true;
  }
  void regexpToggled(bool toggled);


protected:

  void hideEvent(QHideEvent * event);

};

class TLP_PYTHON_SCOPE PythonCodeEditor : public QPlainTextEdit {

  Q_OBJECT

  friend class LineNumberArea;
  friend class AutoCompletionList;

public :

  explicit PythonCodeEditor(QWidget *parent=0);
  ~PythonCodeEditor();

  QString getCleanCode() const;

  int lineNumberAreaWidth() const;

  void indicateScriptCurrentError(int lineNumber);
  void clearErrorIndicator();

  void zoomIn();
  void zoomOut();

  void getCursorPosition(int &line, int &col) const;
  void setCursorPosition(int line, int col);
  void scrollToLine(int line);

  void getSelection(int &lineFrom, int &indexFrom, int &lineTo, int &indexTo) const;
  void setSelection(int startLine, int startCol, int endLine, int endCol);
  void removeSelectedText();
  bool hasSelectedText() const;
  QString selectedText() const;

  int lines() const;
  int lineLength(int lineNumber) const;

  void insertAt(QString text, int line, int col);

  void commentSelectedCode();
  void uncommentSelectedCode();

  void indentSelectedCode();
  void unindentSelectedCode();

  void setAutoIndentation(const bool autoIndent) {
    this->_autoIndent = autoIndent;
  }

  bool autoIndentation() const {
    return _autoIndent;
  }

  void setIndentationGuides(const bool indentGuides) {
    this->_indentGuides = indentGuides;
  }

  bool indentationGuides() const {
    return _indentGuides;
  }

  void setHighlightEditedLine(const bool highlightCurLine) {
    this->_highlightCurLine = highlightCurLine;
  }

  bool highlightEditedLine() const {
    return _highlightCurLine;
  }

  void setFindReplaceActivated(const bool activateFindReplace) {
    _findReplaceActivate = activateFindReplace;
  }

  bool findReplaceActivated() const {
    return _findReplaceActivate;
  }

  void setCommentShortcutsActivated(const bool activateCommentShortcuts) {
    _commentShortcutsActivate = activateCommentShortcuts;
  }

  bool commentShortcutsActivated() const {
    return _commentShortcutsActivate;
  }

  void setIndentShortcutsActivated(const bool activateIndentShortcuts) {
    _indentShortcutsActivate = activateIndentShortcuts;
  }

  bool indentShortcutsActivated() const {
    return _indentShortcutsActivate;
  }

  void setFileName(const QString &fileName) {
    _pythonFileName = fileName;
  }

  QString getFileName() const {
    return _pythonFileName;
  }

  bool loadCodeFromFile(const QString &filePath);

  bool saveCodeToFile();

  QDateTime getLastSavedTime() const {
    return _lastSavedTime;
  }

  void setModuleEditor(const bool moduleEditor) {
    this->_moduleEditor = moduleEditor;
  }

  void analyseScriptCode(const bool wholeText=false);

  AutoCompletionDataBase *getAutoCompletionDb() const {
    return _autoCompletionDb;
  }

  void setPlainText(const QString &text);

protected:

  void resizeEvent(QResizeEvent *event);
  void showEvent(QShowEvent *);
  void paintEvent(QPaintEvent *event);
  void keyPressEvent (QKeyEvent * e);
  void wheelEvent(QWheelEvent * event);
  void mouseDoubleClickEvent(QMouseEvent * event);
  void mouseMoveEvent(QMouseEvent * event);
  void mousePressEvent(QMouseEvent * event);
  void mouseReleaseEvent(QMouseEvent * event);
  void lineNumberAreaPaintEvent(QPaintEvent *event);
  void insertFromMimeData(const QMimeData * source);


protected slots:

  void updateLineNumberAreaWidth();
  void updateLineNumberArea(const QRect &, int);
  void resetExtraSelections();
  void matchParens();
  virtual void highlightCurrentLine();
  void highlightErrors();
  virtual void showAutoCompletionList(bool dotContext=false);
  virtual void updateAutoCompletionList(bool dotContext=false);
  void highlightSelection();

protected:

  virtual void updateAutoCompletionListPosition();

  void createParenSelection(int pos);
  void updateTabStopWidth();

  QString getEditedFunctionName() const;

  void showTooltip(int line, int col, const QString &text);
  void hideTooltip();
  bool isTooltipActive() const;

  QFontMetrics fontMetrics() const;

  QWidget *_lineNumberArea;
  PythonCodeHighlighter *_highlighter;
  ParenMatcherHighlighter *_parenHighlighter;
  QFont _currentFont;
  QVector<int> _currentErrorLines;

  AutoCompletionList *_autoCompletionList;
  AutoCompletionDataBase *_autoCompletionDb;

  FindReplaceDialog *_findReplaceDialog;

  bool _autoIndent;
  bool _indentGuides;
  bool _highlightCurLine;
  bool _tooltipActive;
  bool _findReplaceActivate;
  bool _commentShortcutsActivate;
  bool _indentShortcutsActivate;

  QPoint _toolTipPos;
  QString _toolTipText;
  QString _toolTipFunc;

  QString _pythonFileName;
  QDateTime _lastSavedTime;

  bool _shellWidget;
  bool _moduleEditor;

};

}

#endif /* PYTHONCODEEDITOR2_H_ */
