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
#include <QtGui/QApplication>


#include <iostream>

class ConsoleOutputHandler : public QObject {

  Q_OBJECT

public:

  ConsoleOutputHandler() {}

public slots :

  void writeToConsole(QPlainTextEdit *consoleWidget, const QString &output, bool errorOutput) {

    if (!consoleWidget)
      return;

    QBrush brush(Qt::SolidPattern);

    if (errorOutput) {
      brush.setColor(Qt::red);
    }
    else {
      brush.setColor(Qt::black);
    }

    QTextCharFormat formt = consoleWidget->textCursor().charFormat();
    formt.setForeground(brush);
    consoleWidget->moveCursor(QTextCursor::End);
    QTextCursor cursor = consoleWidget->textCursor();
    cursor.insertText(output, formt);
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

  void setConsoleWidget(QPlainTextEdit *consoleWidget) {
    this->consoleWidget = consoleWidget;
  }

  void setOutputActivated(bool outputActivated) {
    this->outputActivated = outputActivated;
  }

signals:

  void consoleOutput(QPlainTextEdit *consoleWidget, const QString &output, bool errorOutput);

private :

  QPlainTextEdit *consoleWidget;
  bool outputActivated;
};

#endif /* CONSOLEOUTPUTHANDLER_H_ */
