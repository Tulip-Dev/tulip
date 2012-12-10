#ifndef CONSOLE_OUTPUT_HANDLER_H
#define CONSOLE_OUTPUT_HANDLER_H

#include <QtGui/QPlainTextEdit>
#include <QtGui/QTextBrowser>
#include <QtGui/QTextBlock>
#include <QtGui/QApplication>
#include <QtCore/QTime>

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
          formt.setAnchorHref(rx.cap(1) + ":" + rx.cap(2));
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

#endif
