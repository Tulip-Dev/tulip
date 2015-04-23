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

#ifndef CONSOLE_OUTPUT_HANDLER_H
#define CONSOLE_OUTPUT_HANDLER_H

#include <QPlainTextEdit>
#include <QTextBrowser>
#include <QTextBlock>
#include <QApplication>
#include <QTime>
#include <QScrollBar>

#include <iostream>

class ConsoleOutputHandler : public QObject {

  Q_OBJECT

public:

  ConsoleOutputHandler() {
    timer.start();
  }


public slots :

  void writeToConsole(QAbstractScrollArea *consoleWidget, const QString &output, bool errorOutput) {

    if (!consoleWidget)
      return;

    QTextBrowser *textBrowser = dynamic_cast<QTextBrowser*>(consoleWidget);
    QPlainTextEdit *textEdit = dynamic_cast<QPlainTextEdit*>(consoleWidget);

    QBrush brush(Qt::SolidPattern);

    if (errorOutput) {
      brush.setColor(Qt::red);
    }
    else {
      brush.setColor(Qt::black);
    }

    QTextCursor cursor;
    QTextCharFormat formt;

    if (textEdit) {
      formt = textEdit->textCursor().charFormat();
      formt.setForeground(brush);
      textEdit->moveCursor(QTextCursor::End);
      cursor = textEdit->textCursor();
    }
    else {
      formt = textBrowser->textCursor().charFormat();
      formt.setForeground(brush);
      formt.setAnchor(false);
      formt.setUnderlineStyle(QTextCharFormat::NoUnderline);
      formt.setAnchorHref("");
      textBrowser->moveCursor(QTextCursor::End);
      cursor = textBrowser->textCursor();
    }

    cursor.insertText(output, formt);

    if (textBrowser) {
      QRegExp rx("^.*File.*\"(.*)\".*line.*(\\d+).*$");
      QRegExp rx2("^.*File.*\"(.*)\".*line.*(\\d+).*in (.*)$");
      cursor = textBrowser->document()->find(rx, QTextCursor(textBrowser->document()->begin()));

      while (!cursor.isNull()) {
        rx.indexIn(cursor.selectedText());
        rx2.indexIn(cursor.selectedText());

        if (rx.cap(1) != "<string>" && rx2.cap(3) != "tlpimporthook") {
          formt = cursor.charFormat();
          formt.setAnchor(true);
          formt.setUnderlineStyle(QTextCharFormat::SingleUnderline);
          formt.setAnchorHref(QUrl::toPercentEncoding(rx.cap(1) + ":" + rx.cap(2)));
          cursor.setCharFormat(formt);
        }

        cursor = textBrowser->document()->find(rx, cursor);
      }

      if (timer.elapsed() >= 50) {
        QApplication::processEvents();
        timer.start();
      }
    }
  }

private:

  QTime timer;

};

class ConsoleOutputEmitter : public QObject {

  Q_OBJECT

public:

  ConsoleOutputEmitter() : _consoleWidget(NULL), _outputActivated(true) {}

  void sendOutputToConsole(const QString &output, bool errorOutput) {
    if (_outputActivated) {
      emit consoleOutput(_consoleWidget, output, errorOutput);
    }
  }

  void setConsoleWidget(QAbstractScrollArea *consoleWidget) {
    _consoleWidget = consoleWidget;
  }

  QAbstractScrollArea *consoleWidget() const {
    return _consoleWidget;
  }

  void setOutputActivated(bool outputActivated) {
    _outputActivated = outputActivated;
  }

signals:

  void consoleOutput(QAbstractScrollArea *consoleWidget, const QString &output, bool errorOutput);

private :

  QAbstractScrollArea *_consoleWidget;
  bool _outputActivated;
};

class ConsoleInputHandler : public QObject {

  Q_OBJECT

public:

  ConsoleInputHandler() : _startReadCol(-1), _consoleWidget(NULL), _lineRead(false), _wasReadOnly(false) {}

  void setConsoleWidget(QAbstractScrollArea *consoleWidget) {
    _consoleWidget = consoleWidget;
  }

  QAbstractScrollArea *consoleWidget() const {
    return _consoleWidget;
  }

  void startReadLine() {
    if (_consoleWidget) {
      _consoleWidget->installEventFilter(this);
      qApp->installEventFilter(this);
      _consoleWidget->setFocus();
    }
    else {
      _lineRead = true;
      return;
    }

    _lineRead = false;
    QTextBrowser *textBrowser = dynamic_cast<QTextBrowser*>(_consoleWidget);
    QPlainTextEdit *textEdit = dynamic_cast<QPlainTextEdit*>(_consoleWidget);
    QColor lineColor = QColor(Qt::green).lighter(160);

    if (textBrowser) {
      _readPos = textBrowser->textCursor();
      _wasReadOnly = textBrowser->isReadOnly();
      textBrowser->setReadOnly(false);
      textBrowser->verticalScrollBar()->setValue(textBrowser->verticalScrollBar()->maximum());
    }
    else if (textEdit) {
      _readPos = textEdit->textCursor();
      _wasReadOnly = textEdit->isReadOnly();
      textEdit->setReadOnly(false);
    }

    _startReadCol = _readPos.columnNumber();
    QTextBlockFormat format = _blockFormat = _readPos.blockFormat();
    format.setBackground(lineColor);
    format.setProperty(QTextFormat::FullWidthSelection, true);
    _readPos.setBlockFormat(format);
  }

  bool lineRead() const {
    return _lineRead;
  }

  QString line() const {
    return _line;
  }

  bool eventFilter(QObject *, QEvent *event) {
    QTextBrowser *textBrowser = dynamic_cast<QTextBrowser*>(_consoleWidget);
    QPlainTextEdit *textEdit = dynamic_cast<QPlainTextEdit*>(_consoleWidget);
    QTextCursor curCursor;

    if (textBrowser) {
      curCursor = textBrowser->textCursor();
    }
    else {
      curCursor = textEdit->textCursor();
    }

    if (event->type() == QEvent::KeyPress) {
      QKeyEvent *kev = static_cast<QKeyEvent *>(event);
      int key = kev->key();

      if ((key ==  Qt::Key_Enter || key ==  Qt::Key_Return) && kev->modifiers() == Qt::NoModifier) {
        _lineRead = true;
        _line = _readPos.block().text().mid(_startReadCol);
        _line.append("\n");
        _readPos.insertText("\n");
        _readPos.setBlockFormat(_blockFormat);

        if (textBrowser) {
          textBrowser->setReadOnly(_wasReadOnly);
        }
        else {
          textEdit->setReadOnly(_wasReadOnly);
        }

        _consoleWidget->removeEventFilter(this);
        qApp->removeEventFilter(this);
        return true;
      }
      else if (key ==  Qt::Key_Up || key ==  Qt::Key_Down) {
        return true;
      }
      else if (key == Qt::Key_Left) {
        if (curCursor.columnNumber() > _startReadCol) {
          if (textEdit) {
            textEdit->moveCursor(QTextCursor::Left);
          }
          else {
            textBrowser->moveCursor(QTextCursor::Left);
          }
        }

        return true;
      }
      else if (key == Qt::Key_Right) {
        if (textEdit) {
          textEdit->moveCursor(QTextCursor::Right);
        }
        else {
          textBrowser->moveCursor(QTextCursor::Right);
        }
      }
      else if (key == Qt::Key_Backspace) {
        if (curCursor.columnNumber() > _startReadCol) {
          curCursor.deletePreviousChar();
        }

        return true;
      }
    }
    else if (event->type() == QEvent::MouseButtonDblClick || event->type() == QEvent::MouseButtonPress ||
             event->type() == QEvent::MouseButtonRelease) {
      return true;
    }

    return false;
  }

private:

  QTextCursor _readPos;
  int _startReadCol;
  QAbstractScrollArea *_consoleWidget;
  bool _lineRead;
  QString _line;
  bool _wasReadOnly;
  QTextBlockFormat _blockFormat;

};



#endif
