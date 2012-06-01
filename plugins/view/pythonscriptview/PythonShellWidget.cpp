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

#include "PythonInterpreter.h"

#include <iostream>

#include <QtCore/QRegExp>
#include <QtGui/QKeyEvent>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QDir>

#include <tulip/TlpTools.h>
#include <tulip/Observable.h>

#include "PythonShellWidget.h"


using namespace std;

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
const string setCurrentGraphFunction =
  "graph = None\n"
  "def setCurrentGraph(g):\n"
  "	global graph\n"
  "	graph = g\n"
  ;

PythonShellWidget::PythonShellWidget(QWidget *parent) : PythonCodeEditor(parent) {
  setAutoIndentation(false);
  setIndentationGuides(false);
  setHighlightEditedLine(false);
  setFindReplaceActivated(false);
  setCommentShortcutsActivated(false);
  setIndentShortcutsActivated(false);
  insert(QString(PythonInterpreter::getInstance()->getPythonShellBanner().c_str())+ QString("\n"));
  insert("# The current graph is bound to the \"graph\" variable\n");
  insert("# Use Ctrl + Space to show dynamic auto-completion dialog\n");
  insert(ps1);
  currentPs = ps1;
  currentHistoryPos = -1;
  highlighter->setShellMode(true);
  PythonInterpreter::getInstance()->runString(setCurrentGraphFunction);
  shellWidget = true;
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

  QString lineNotTrimmed = textCursor().block().text().mid(currentPs.length());
  QString line = rtrim(textCursor().block().text()).mid(currentPs.length());

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

        if (col > currentPs.length()) {
          PythonCodeEditor::keyPressEvent(e);
        }

        if (key == Qt::Key_Right && col == currentPs.length()) {
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
      if (currentHistoryPos >= 0) {

        setSelection(lines()-1, 0, lines()-1, lineLength(lines()-1));
        removeSelectedText();
        insert(currentPs + history[currentHistoryPos]);

        if (currentHistoryPos != 0)
          --currentHistoryPos;
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

      if (currentHistoryPos < static_cast<int>(history.size() -  1) && history.size() > 0) {
        ++currentHistoryPos;
        insert(currentPs + history[currentHistoryPos]);
      }
      else {
        insert(currentPs);
      }
    }
    else {
      setCursorPosition(lines() - 1, lineLength(lines() - 1));
    }
  }
  else if (key ==  Qt::Key_Home) {
    if (isCursorOnLastLine()) {

      if (e->modifiers() == Qt::ShiftModifier) {
        setSelection(lines()-1, currentPs.length(), lines()-1, col);
      }
      else {
        setCursorPosition(lines()-1, currentPs.length());
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
      setSelection(lines()-1, currentPs.length(), lines()-1, lineLength(lines()-1));
    }
  }
  else if ((key ==  Qt::Key_Enter || key ==  Qt::Key_Return) && e->modifiers() == Qt::NoModifier) {
    if  (isCursorOnLastLine()) {
      if (line != "") {
        currentCodeLines += (line + "\n");
        history.push_back(line);
        currentHistoryPos = history.size() - 1;
      }

      setCursorPosition(lines() - 1, lineLength(lines() - 1));
      PythonCodeEditor::keyPressEvent(e);

      if (line != "") {
        if(currentPs == ps2 || line.endsWith(":")) {
          currentPs = ps2;
          insert(ps2+"\t");
        }
        else {
          executeCurrentLines();
          currentPs = ps1;
          insert(ps1);
        }
      }
      else {
        executeCurrentLines();
        currentPs = ps1;
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
  PythonInterpreter::getInstance()->runString(currentCodeLines.toUtf8().data());
  currentCodeLines = "";
  PythonInterpreter::getInstance()->setProcessQtEventsDuringScriptExecution(false);
  PythonInterpreter::getInstance()->setDefaultConsoleWidget();
  PythonInterpreter::getInstance()->setDefaultSIGINTHandler();
  tlp::Observable::unholdObservers();
  insert("", true);
}

void PythonShellWidget::setGraph(tlp::Graph *graph) {
  PythonInterpreter::getInstance()->runGraphScript("__main__", "setCurrentGraph", graph);
  PythonCodeEditor::setGraph(graph);
}

void PythonShellWidget::showEvent(QShowEvent *) {
  setFocus();
}

void PythonShellWidget::updateAutoCompletionList(bool) {


  if (!autoCompletionList->isVisible())
    return;

  PythonCodeEditor::updateAutoCompletionList();

  QString lineNotTrimmed = textCursor().block().text().mid(currentPs.length());
  QString line = rtrim(textCursor().block().text()).mid(currentPs.length());

  if (lineNotTrimmed[lineNotTrimmed.length()-1] != ' ' && lineNotTrimmed[lineNotTrimmed.length()-1] != '\t') {
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
      vector<string> dynamicAutoCompletionList = PythonInterpreter::getInstance()->getObjectDictEntries(context[0].toStdString(), context[1].toStdString());

      for (size_t i = 0 ; i < dynamicAutoCompletionList.size() ; ++i) {
        QString entry = dynamicAutoCompletionList[i].c_str();

        if (autoCompletionList->findItems(entry, Qt::MatchExactly).empty())
          autoCompletionList->addItem(entry);
      }
    }

    if (autoCompletionResult.count() == 0) {
      if (context.size() == 1) {
        vector<string> dynamicAutoCompletionList = PythonInterpreter::getInstance()->getGlobalDictEntries(context[0].toStdString());

        for (size_t i = 0 ; i < dynamicAutoCompletionList.size() ; ++i) {
          QString entry = dynamicAutoCompletionList[i].c_str();

          if (autoCompletionList->findItems(entry, Qt::MatchExactly).empty())
            autoCompletionList->addItem(entry);
        }
      }
    }
  }

  autoCompletionList->sortItems();

  if (autoCompletionList->count() == 0)
    autoCompletionList->hide();
  else
    autoCompletionList->setCurrentRow(0);
}
