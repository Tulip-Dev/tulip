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

class QMainWindow;

namespace tlp {

class PythonCodeEditor;
class LineNumberArea;

class TLP_PYTHON_SCOPE AutoCompletionList : public QListWidget {

  tlp::PythonCodeEditor *_codeEditor;
  bool _activated;
  bool _wasActivated;

public:
  explicit AutoCompletionList();

  void setCodeEditor(tlp::PythonCodeEditor *parent);

protected:
  void insertSelectedItem();
  void keyPressEvent(QKeyEvent *e) override;
  void showEvent(QShowEvent *event) override;
  void hideEvent(QHideEvent *event) override;
  void mouseDoubleClickEvent(QMouseEvent *event) override;
  bool eventFilter(QObject *obj, QEvent *event) override;
};

class TLP_PYTHON_SCOPE FindReplaceDialog : public QDialog {

  Q_OBJECT

  Ui::FindReplaceDialogData *_ui;
  QPlainTextEdit *_editor;
  QPushButton *_findButton, *_replaceButton, *_replaceFindButton;
  QPushButton *_replaceAllButton;
  QString _lastSearch;
  bool _resetSearch;

  void setSearchResult(const bool result);

public:
  FindReplaceDialog(QPlainTextEdit *_editor, QWidget *parent = nullptr);
  ~FindReplaceDialog() override;

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
};

class TLP_PYTHON_SCOPE PythonCodeEditor : public QPlainTextEdit {

  Q_OBJECT

  friend class LineNumberArea;
  friend class AutoCompletionList;

public:
  explicit PythonCodeEditor(QWidget *parent = nullptr);
  ~PythonCodeEditor() override;

  QString getCleanCode() const;

  int lineNumberAreaWidth() const;

  inline qreal tabWidth() const {
#if (QT_VERSION < QT_VERSION_CHECK(5, 10, 0))
    return tabStopWidth();
#else
    return tabStopDistance();
#endif
  }

  inline void setTabWidth(qreal width) {
#if (QT_VERSION < QT_VERSION_CHECK(5, 10, 0))
    setTabStopWidth(width);
#else
    setTabStopDistance(width);
#endif
  }

  inline int charWidth(char c) const {
    if (c == '\t') {
      return textWidth("    ");
    }
#if (QT_VERSION < QT_VERSION_CHECK(5, 11, 0))
    return fontMetrics().width(QLatin1Char(c));
#else
    return fontMetrics().horizontalAdvance(QLatin1Char(c));
#endif
  }

  inline int textWidth(const QString &text) const {
#if (QT_VERSION < QT_VERSION_CHECK(5, 11, 0))
    return fontMetrics().width(QString(text).replace('\t', "    "));
#else
    return fontMetrics().horizontalAdvance(QString(text).replace('\t', "    "));
#endif
  }
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
  bool selectedLinesCommented() const;

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

  const QString &getFileName() const {
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

  void analyseScriptCode(const bool wholeText = false);

  AutoCompletionDataBase *getAutoCompletionDb() const {
    return _autoCompletionDb;
  }

  void setPlainText(const QString &text);

  QMainWindow *mainWindow() const {
    return _mainWindow;
  }

  static void deleteStaticResources() {
    delete _autoCompletionDb;
    _autoCompletionDb = nullptr;
    delete _autoCompletionList;
    _autoCompletionList = nullptr;
  }

protected:
  void resizeEvent(QResizeEvent *event) override;
  void showEvent(QShowEvent *) override;
  void paintEvent(QPaintEvent *event) override;
  void keyPressEvent(QKeyEvent *e) override;
  void wheelEvent(QWheelEvent *event) override;
  void mouseDoubleClickEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void lineNumberAreaPaintEvent(QPaintEvent *event);
  void insertFromMimeData(const QMimeData *source) override;

protected slots:

  void updateLineNumberAreaWidth();
  void updateLineNumberArea(const QRect &, int);
  void resetExtraSelections();
  void matchParens();
  virtual void highlightCurrentLine();
  void highlightErrors();
  virtual void showAutoCompletionList(bool dotContext = false);
  virtual void updateAutoCompletionList(bool dotContext = false);
  void highlightSelection();

protected:
  virtual void updateAutoCompletionListPosition();

  void createParenSelection(int pos);
  void updateTabWidth();
  void findIndentPattern(const QString &pythonCode);

  QString getEditedFunctionName() const;

  void showTooltip(int line, int col, const QString &text);
  void hideTooltip();
  bool isTooltipActive() const;

  QFontMetrics fontMetrics() const;
  void showFindDialog(QString selection, bool findMode);

  QWidget *_lineNumberArea;
  PythonCodeHighlighter *_highlighter;
  ParenMatcherHighlighter *_parenHighlighter;
  QFont _currentFont;
  QVector<int> _currentErrorLines;

  static AutoCompletionList *_autoCompletionList;
  static AutoCompletionDataBase *_autoCompletionDb;

  FindReplaceDialog *_findReplaceDialog;

  bool _autoIndent;
  bool _indentGuides;
  bool _highlightCurLine;
  bool _tooltipActive;
  bool _findReplaceActivate;
  bool _commentShortcutsActivate;
  bool _indentShortcutsActivate;
  bool _darkBackground;

  QPoint _toolTipPos;
  QString _toolTipText;
  QString _toolTipFunc;

  QString _pythonFileName;
  QDateTime _lastSavedTime;

  bool _shellWidget;
  bool _moduleEditor;

  QMainWindow *_mainWindow;

  QString _indentPattern;
};
} // namespace tlp

#endif /* PYTHONCODEEDITOR2_H_ */
