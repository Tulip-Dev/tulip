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

#ifndef PYTHONCODEEDITOR2_H_
#define PYTHONCODEEDITOR2_H_

#include "PythonCodeHighlighter.h"
#include "ParenMatcherHighlighter.h"
#include "APIDataBase.h"
#include "AutoCompletionDataBase.h"
#include "FindReplaceDialogData.h"

#include <QtGui/QListWidget>
#include <QtGui/QPlainTextEdit>

class GragKeyboardFocusEventFilter : public QObject {
public :
  bool eventFilter(QObject *, QEvent *event) {
    if (event->type() == QEvent::ShortcutOverride) {
      event->accept();
      return true;
    }

    return false;
  }
};

class PythonCodeEditor;

class AutoCompletionList : public QListWidget {

public :

  AutoCompletionList(PythonCodeEditor *parent=NULL);

protected :

  void keyPressEvent(QKeyEvent *e);
  void showEvent(QShowEvent * event);
  void hideEvent(QHideEvent * event);
  bool eventFilter(QObject *obj, QEvent * event);

private :

  PythonCodeEditor *codeEditor;

  bool activated;
  bool wasActivated;
};

class FindReplaceDialog : public QDialog, public Ui::FindReplaceDialogData {

  Q_OBJECT

public :

  FindReplaceDialog(QPlainTextEdit *editor);

  void setFindMode(const bool findMode);

  void setTextToFind(const QString &text);


public slots:

  void textToFindChanged();
  bool doFind();
  bool doReplace();
  void doReplaceFind();
  void doReplaceAll();
  void setResetSearch() {
    this->resetSearch = true;
  }
  void regexpToggled(bool toggled);


protected:

  void hideEvent(QHideEvent * event);

private :

  void setSearchResult(const bool result);

  QPlainTextEdit *editor;
  QString lastSearch;
  bool resetSearch;
};

class PythonCodeEditor : public QPlainTextEdit {

  Q_OBJECT

  friend class LineNumberArea;
  friend class AutoCompletionList;

public :

  PythonCodeEditor(QWidget *parent=NULL);
  ~PythonCodeEditor();

  QString getCleanCode() const;

  int lineNumberAreaWidth() const;
  void indicateScriptCurrentError(int lineNumber);
  void clearErrorIndicator();

  void zoomIn();
  void zoomOut();

  void getCursorPosition(int &line, int &col) const;
  void setCursorPosition(int line, int col);
  void getSelection(int &lineFrom, int &indexFrom, int &lineTo, int &indexTo) const;
  void setSelection(int startLine, int startCol, int endLine, int endCol);
  void removeSelectedText();
  bool hasSelectedText() const;
  int lines() const;
  int lineLength(int lineNumber) const;
  QString selectedText() const;
  void insertAt(QString text, int line, int col);

  void commentSelectedCode();
  void uncommentSelectedCode();


  void indentSelectedCode();
  void unindentSelectedCode();

  void setAutoIndentation(const bool autoIndent) {
    this->autoIndent = autoIndent;
  }

  bool autoIndentation() const {
    return autoIndent;
  }

  void setIndentationGuides(const bool indentGuides) {
    this->indentGuides = indentGuides;
  }

  bool indentationGuides() const {
    return indentGuides;
  }

  void setHighlightEditedLine(const bool highlightCurLine) {
    this->highlightCurLine = highlightCurLine;
  }

  bool highlightEditedLine() const {
    return highlightCurLine;
  }

  void setFindReplaceActivated(const bool activateFindReplace) {
    findReplaceActivate = activateFindReplace;
  }

  bool findReplaceActivated() const {
    return findReplaceActivate;
  }

  void setCommentShortcutsActivated(const bool activateCommentShortcuts) {
    commentShortcutsActivate = activateCommentShortcuts;
  }

  bool commentShortcutsActivated() const {
    return commentShortcutsActivate;
  }

  void setIndentShortcutsActivated(const bool activateIndentShortcuts) {
    indentShortcutsActivate = activateIndentShortcuts;
  }

  bool indentShortcutsActivated() const {
    return indentShortcutsActivate;
  }

  void setGraph(tlp::Graph *graph);

  void setFileName(const QString &fileName) {
    pythonFileName = fileName;
  }

  QString getFileName() const {
    return pythonFileName;
  }

  void setModuleEditor(const bool moduleEditor) {
    this->moduleEditor = moduleEditor;
  }

  void analyseScriptCode(const bool wholeText=false);

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

  QWidget *lineNumberArea;
  PythonCodeHighlighter *highlighter;
  ParenMatcherHighlighter *parenHighlighter;
  QFont currentFont;
  QVector<int> currentErrorLines;

  static APIDataBase *apiDb;
  AutoCompletionList *autoCompletionList;
  AutoCompletionDataBase *autoCompletionDb;

  FindReplaceDialog *findReplaceDialog;

  bool autoIndent;
  bool indentGuides;
  bool highlightCurLine;
  bool tooltipActive;
  bool findReplaceActivate;
  bool commentShortcutsActivate;
  bool indentShortcutsActivate;

  QPoint toolTipPos;
  QString toolTipText;
  QString toolTipFunc;

  QString pythonFileName;

  bool shellWidget;
  bool moduleEditor;

  static GragKeyboardFocusEventFilter keyboardFocusEventFilter;

};

class LineNumberArea : public QWidget {
public:
  LineNumberArea(PythonCodeEditor *editor) : QWidget(editor) {
    codeEditor = editor;
  }

  QSize sizeHint() const {
    return QSize(codeEditor->lineNumberAreaWidth(), 0);
  }

protected:
  void paintEvent(QPaintEvent *event) {
    codeEditor->lineNumberAreaPaintEvent(event);
  }

private:
  PythonCodeEditor *codeEditor;
};

#endif /* PYTHONCODEEDITOR2_H_ */
