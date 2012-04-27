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

#ifndef CONSOLEOUTPUTHANDLER_H_
#define CONSOLEOUTPUTHANDLER_H_

#include <QtGui/QPlainTextEdit>
#include <QtGui/QTextBrowser>
#include <QtGui/QTextBlock>
#include <QtGui/QApplication>


#include <iostream>

class ConsoleOutputHandler : public QObject {

  Q_OBJECT

public:

  ConsoleOutputHandler() {}

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
          formt.setAnchorHref(rx.cap(1) + ":" + rx.cap(2));
          cursor.setCharFormat(formt);
        }

        cursor = textBrowser->document()->find(rx, cursor);
      }
    }

    QApplication::processEvents();
  }

};

class ConsoleOutputEmitter : public QObject {

  Q_OBJECT

public:

  ConsoleOutputEmitter() : consoleWidget(NULL), outputActivated(true) {}

  void sendOutputToConsole(const QString &output, bool errorOutput) {
    if (outputActivated) {
      emit consoleOutput(consoleWidget, output, errorOutput);
    }
  }

  void setConsoleWidget(QAbstractScrollArea *consoleWidget) {
    this->consoleWidget = consoleWidget;
  }

  void setOutputActivated(bool outputActivated) {
    this->outputActivated = outputActivated;
  }

signals:

  void consoleOutput(QAbstractScrollArea *consoleWidget, const QString &output, bool errorOutput);

private :

  QAbstractScrollArea *consoleWidget;
  bool outputActivated;
};

#endif /* CONSOLEOUTPUTHANDLER_H_ */
