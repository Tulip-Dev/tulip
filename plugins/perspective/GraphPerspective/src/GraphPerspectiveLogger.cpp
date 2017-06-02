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

#include "GraphPerspectiveLogger.h"

#include "ui_GraphPerspectiveLogger.h"
#include <iostream>

#include <QKeyEvent>
#include <QClipboard>
#include <QMenu>

#include <tulip/TlpQtTools.h>

GraphPerspectiveLogger::GraphPerspectiveLogger(QWidget* parent):
  QFrame(parent), _logType(QtDebugMsg), _ui(new Ui::GraphPerspectiveLogger), _pythonOutput(false) {
  _ui->setupUi(this);
  // we want to be able to select multiple rows in the logger list for copy/paste operations
  _ui->listWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
  _ui->listWidget->installEventFilter(this);
  _ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(_ui->listWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));
  setWindowFlags(Qt::Popup);
  connect(_ui->clearButton,SIGNAL(clicked()),this,SLOT(clear()));
}

GraphPerspectiveLogger::~GraphPerspectiveLogger() {
  delete _ui;
}

GraphPerspectiveLogger::LogType GraphPerspectiveLogger::getLastLogType() const {
  if (_pythonOutput) {
    return Python;
  }

  switch (_logType) {
  case QtDebugMsg:
#if (QT_VERSION >= QT_VERSION_CHECK(5, 5, 0))
  case QtInfoMsg:
#endif
    return Info;

  case QtWarningMsg:
    return Warning;

  case QtCriticalMsg:
  case QtFatalMsg:
    return Error;

  }

  return Info;
}

int GraphPerspectiveLogger::count() const {
  return _ui->listWidget->count();
}

int GraphPerspectiveLogger::countByType(LogType logType) const {
  return _logCounts[logType];
}

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
void GraphPerspectiveLogger::log(QtMsgType type, const QMessageLogContext &, const QString &msg) {
  logImpl(type, msg);
}
#else
void GraphPerspectiveLogger::log(QtMsgType type, const char* msg) {
  logImpl(type, QString::fromUtf8(msg));
}
#endif

void GraphPerspectiveLogger::logImpl(QtMsgType type, const QString &msg) {

  // on some windows systems
  // "No errors." messages may be logged coming from QGLShader::link
  // we try to avoid them
  if (msg.indexOf("No errors.") != -1)
    return;

  if (type == QtFatalMsg) {
    std::cerr << tlp::QStringToTlpString(msg) << std::endl;
    abort();
  }

  _logType = type;
  QString msgClean = msg;


  if (msg.startsWith("[Python")) {
    // remove quotes around message added by Qt
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    msgClean = msg.mid(14).mid(2, msg.length()-17);
#else
    msgClean = msg.mid(14).mid(2, msg.length()-18);
#endif
    _pythonOutput = true;
  }
  else {
    _pythonOutput = false;
  }

  LogType lastLogType = getLastLogType();
  _ui->listWidget->addItem(new QListWidgetItem(QIcon(icon(lastLogType)), msgClean));
  _logCounts[lastLogType] += 1;
}

QPixmap GraphPerspectiveLogger::icon(LogType logType) const {
  QString pxUrl(":/tulip/graphperspective/icons/16/logger-");

  switch (logType) {
  case Python:
    return QPixmap(":/tulip/graphperspective/icons/16/python.png");

  case Info:
    pxUrl+="info";
    break;

  case Warning:
    pxUrl+="danger";
    break;

  case Error:
    pxUrl+="error";
    break;
  }

  pxUrl += ".png";

  return pxUrl;
}

void GraphPerspectiveLogger::clear() {
  _ui->listWidget->clear();
  _logType = QtDebugMsg;
  emit cleared();
  close();
  _logCounts[Info] = 0;
  _logCounts[Warning] = 0;
  _logCounts[Error] = 0;
  _logCounts[Python] = 0;
}

void GraphPerspectiveLogger::copy() {
    QStringList strings;
    foreach(QListWidgetItem *item, _ui->listWidget->selectedItems())
      strings << item->text();
    QApplication::clipboard()->setText(strings.join("\n"));
}

void GraphPerspectiveLogger::showContextMenu(const QPoint &pos) {
    QMenu m;
    m.addAction("Clear content and close", this, SLOT(clear()));
    m.addAction("Copy", this, SLOT(copy()), QKeySequence::Copy);
    m.exec(_ui->listWidget->mapToGlobal(pos));
}

// catch the copy to clipboard event of the QListWidget and reimplement
// its behaviour in order to be able to copy the text of all the selected rows
// (only the text of the current item is copied otherwise)
bool GraphPerspectiveLogger::eventFilter(QObject *, QEvent *event) {
  QKeyEvent *ke = dynamic_cast<QKeyEvent*>(event);

  if (ke && ke->matches(QKeySequence::Copy)) {
    copy();
    return true;
  }

  return false;
}
