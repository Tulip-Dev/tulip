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

#include "tulip/PythonInterpreter.h"

#include <QRegExp>
#include <QKeyEvent>

#include <tulip/TlpTools.h>
#include <tulip/Observable.h>

#include "tulip/PythonShellWidget.h"
#include "tulip/PythonCodeHighlighter.h"
#include "tulip/ParenMatcherHighlighter.h"

using namespace tlp;

static QRegExp linesepRegExp = QRegExp("\r\n|\n|\r");

static QString ps1 = ">>> ";
static QString ps2 = "... ";

static QString rtrim(const QString &s) {
  int lastNonSpaceIdx = s.length() -1;

  while (s.at(lastNonSpaceIdx).isSpace()) {
    --lastNonSpaceIdx;
  }

  return s.mid(0, lastNonSpaceIdx+1);
}

static QString ltrim(const QString &s) {
  int firstNonSpaceIdx = 0;

  while (s.at(firstNonSpaceIdx).isSpace()) {
    ++firstNonSpaceIdx;
  }

  return s.mid(firstNonSpaceIdx);
}


PythonShellWidget::PythonShellWidget(QWidget *parent) : PythonCodeEditor(parent) {
  setAutoIndentation(false);
  setIndentationGuides(false);
  setHighlightEditedLine(false);
  setFindReplaceActivated(false);
  setCommentShortcutsActivated(false);
  setIndentShortcutsActivated(false);
  insert(PythonInterpreter::getInstance()->getPythonShellBanner()+ "\n");
  insert("# Use Ctrl + Space to show dynamic auto-completion dialog\n");
  insert(ps1);
  _currentPs = ps1;
  _currentHistoryPos = -1;
  _highlighter->setShellMode(true);
  _shellWidget = true;
  setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
}

bool PythonShellWidget::isCursorOnLastLine() {
  int line = textCursor().blockNumber() + 1;
  return line == document()->blockCount();
}

void PythonShellWidget::insert(const QString &txt, const bool atEnd) {
  if (atEnd) {
    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::End);
    setTextCursor(cursor);
  }

  QTextCharFormat formt = textCursor().charFormat();
  formt.setForeground(Qt::black);
  textCursor().insertText(txt, formt);
}

void PythonShellWidget::keyPressEvent(QKeyEvent * e) {

  int key = e->key();
  QString txt = e->text();

  QString lineNotTrimmed = textCursor().block().text().mid(_currentPs.length());
  QString line = rtrim(textCursor().block().text()).mid(_currentPs.length());

#if (QT_VERSION >= QT_VERSION_CHECK(4, 7, 0))
  int col = textCursor().positionInBlock();
#else
  int col = textCursor().position() - textCursor().block().position();
#endif

  if (key ==  Qt::Key_Backspace || key ==  Qt::Key_Left || key == Qt::Key_Right) {
    if (isCursorOnLastLine()) {
      if (key ==  Qt::Key_Backspace && textCursor().selectedText() != "") {
        textCursor().removeSelectedText();
      }
      else {

        if (col > _currentPs.length()) {
          PythonCodeEditor::keyPressEvent(e);
        }

        if (key == Qt::Key_Right && col == _currentPs.length()) {
          PythonCodeEditor::keyPressEvent(e);
        }

      }
    }
    else {
      setCursorPosition(lines() - 1, lineLength(lines() - 1));
    }
  }
  else if (key ==  Qt::Key_Up) {
    if (isCursorOnLastLine()) {
      if (_currentHistoryPos >= 0) {

        setSelection(lines()-1, 0, lines()-1, lineLength(lines()-1));
        removeSelectedText();
        insert(_currentPs + _history[_currentHistoryPos]);

        if (_currentHistoryPos != 0)
          --_currentHistoryPos;
      }
    }
    else {
      setCursorPosition(lines() - 1, lineLength(lines() - 1));
    }
  }
  else if (key ==  Qt::Key_Down) {
    if (isCursorOnLastLine()) {
      setSelection(lines()-1, 0, lines()-1, lineLength(lines()-1));
      removeSelectedText();

      if (_currentHistoryPos < static_cast<int>(_history.size() -  1) && _history.size() > 0) {
        ++_currentHistoryPos;
        insert(_currentPs + _history[_currentHistoryPos]);
      }
      else {
        insert(_currentPs);
      }
    }
    else {
      setCursorPosition(lines() - 1, lineLength(lines() - 1));
    }
  }
  else if (key ==  Qt::Key_Home) {
    if (isCursorOnLastLine()) {

      if (e->modifiers() == Qt::ShiftModifier) {
        setSelection(lines()-1, _currentPs.length(), lines()-1, col);
      }
      else {
        setCursorPosition(lines()-1, _currentPs.length());
      }
    }
    else {
      setCursorPosition(lines() - 1, lineLength(lines() - 1));
    }
  }
  else if (key ==  Qt::Key_End) {
    if (isCursorOnLastLine()) {

      if (e->modifiers() == Qt::ShiftModifier) {
        setSelection(lines()-1, col, lines()-1, lineLength(lines()-1));
      }
      else {
        setCursorPosition(lines()-1, lineLength(lines() - 1));
      }
    }
    else {
      setCursorPosition(lines() - 1, lineLength(lines() - 1));
    }
  }
  else if (key ==  Qt::Key_A && e->modifiers() == Qt::ControlModifier) {
    if (isCursorOnLastLine()) {
      setSelection(lines()-1, _currentPs.length(), lines()-1, lineLength(lines()-1));
    }
  }
  else if ((key ==  Qt::Key_Enter || key ==  Qt::Key_Return) && e->modifiers() == Qt::NoModifier) {
    if  (isCursorOnLastLine()) {
      if (line != "") {
        _currentCodeLines += (line + "\n");
        _history.push_back(line);
        _currentHistoryPos = _history.size() - 1;
      }

      setCursorPosition(lines() - 1, lineLength(lines() - 1));
      PythonCodeEditor::keyPressEvent(e);

      if (line != "") {
        if(_currentPs == ps2 || line.endsWith(":")) {
          _currentPs = ps2;
          insert(ps2+"\t");
        }
        else {
          executeCurrentLines();
          _currentPs = ps1;
          insert(ps1);
        }
      }
      else {
        executeCurrentLines();
        _currentPs = ps1;
        insert(ps1);
      }
    }
  }
  else if (txt.length() > 0 && txt >= " " && e->modifiers() == Qt::NoModifier && e->text() != "(" && e->text() != ")") {
    if (isCursorOnLastLine()) {
      insert(txt);
    }
    else {
      insert(txt, true);
    }
  }
  else {
    PythonCodeEditor::keyPressEvent(e);
  }
}

void PythonShellWidget::executeCurrentLines() {
  tlp::Observable::holdObservers();
  PythonInterpreter::getInstance()->setConsoleWidget(this);
  PythonInterpreter::getInstance()->setProcessQtEventsDuringScriptExecution(true);
  PythonInterpreter::getInstance()->runString(_currentCodeLines);
  _currentCodeLines = "";
  PythonInterpreter::getInstance()->setProcessQtEventsDuringScriptExecution(false);
  PythonInterpreter::getInstance()->resetConsoleWidget();
  PythonInterpreter::getInstance()->setDefaultSIGINTHandler();
  tlp::Observable::unholdObservers();
  insert("", true);
}

void PythonShellWidget::showEvent(QShowEvent *) {
  setFocus();
}

void PythonShellWidget::updateAutoCompletionList(bool) {
  if (!_autoCompletionList->isVisible())
    return;

  _autoCompletionList->clear();

  QString lineNotTrimmed = textCursor().block().text().mid(_currentPs.length());
  QString line = rtrim(textCursor().block().text()).mid(_currentPs.length());

  if (!lineNotTrimmed.isEmpty() && lineNotTrimmed[lineNotTrimmed.length()-1] != ' ' && lineNotTrimmed[lineNotTrimmed.length()-1] != '\t') {
    QStringList context = ltrim(line).split(".");

    for (int i = 0 ; i < context.count() ; ++i) {
      context[i].replace("=", " ");
      context[i].replace("+", " ");
      context[i].replace("-", " ");
      context[i].replace("*", " ");
      context[i].replace("%", " ");
      context[i].replace("/", " ");
      context[i].replace("^", " ");
      context[i].replace("<", " ");
      context[i].replace(">", " ");
      context[i].replace("|", " ");
      context[i].replace("!", " ");
      QStringList words = context[i].split(" ");
      context[i] = words.back();
    }

    QStringList autoCompletionResult;

    if (context.size() == 2) {
      QVector<QString> dynamicAutoCompletionList = PythonInterpreter::getInstance()->getObjectDictEntries(context[0], context[1]);

      for (int i = 0 ; i < dynamicAutoCompletionList.size() ; ++i) {
        QString entry = dynamicAutoCompletionList[i];

        if (_autoCompletionList->findItems(entry, Qt::MatchExactly).empty())
          _autoCompletionList->addItem(entry);
      }
    }

    if (autoCompletionResult.count() == 0) {
      if (context.size() == 1) {
        QVector<QString> dynamicAutoCompletionList = PythonInterpreter::getInstance()->getGlobalDictEntries(context[0]);

        for (int i = 0 ; i < dynamicAutoCompletionList.size() ; ++i) {
          QString entry = dynamicAutoCompletionList[i];

          if (_autoCompletionList->findItems(entry, Qt::MatchExactly).empty())
            _autoCompletionList->addItem(entry);
        }
      }
    }
  }

  if (_autoCompletionList->count() == 0) {
    PythonCodeEditor::updateAutoCompletionList();
  }
  else {
    _autoCompletionList->sortItems();
  }

  if (_autoCompletionList->count() == 0)
    _autoCompletionList->hide();
  else
    _autoCompletionList->setCurrentRow(0);
}
