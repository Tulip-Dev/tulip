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

GraphPerspectiveLogger::GraphPerspectiveLogger(QWidget* parent): QFrame(parent), _logSeverity(QtDebugMsg), _logCount(0), _ui(new Ui::GraphPerspectiveLogger) {
  _ui->setupUi(this);
  setWindowFlags(Qt::Popup);
  connect(_ui->clearButton,SIGNAL(clicked()),this,SLOT(clear()));
}

GraphPerspectiveLogger::~GraphPerspectiveLogger() {
  delete _ui;
}

static QString iconForType(QtMsgType type) {
  QString pxUrl(":/tulip/graphperspective/icons/16/logger-");

  switch (type) {
  case QtDebugMsg:
    pxUrl+="info";
    break;

  case QtWarningMsg:
    pxUrl+="danger";
    break;

  case QtCriticalMsg:
  case QtFatalMsg:
    pxUrl+="error";
    break;


  }

  pxUrl += ".png";

  return pxUrl;
}

unsigned int GraphPerspectiveLogger::count() const {
  return _logCount;
}

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
void GraphPerspectiveLogger::log(QtMsgType type, const QMessageLogContext &, const QString &msg) {
  if (type == QtFatalMsg) {
    std::cerr << msg.toUtf8().data() << std::endl;
    abort();
  }

  if (type > _logSeverity)
    _logSeverity = type;

  _logCount++;
  _ui->listWidget->addItem(new QListWidgetItem(QIcon(iconForType(type)), msg));
}
#else
void GraphPerspectiveLogger::log(QtMsgType type, const char* msg) {
  // on some windows systems
  // "No errors." messages may be logged coming from QGLShader::link
  // we try to avoid them
  if (QString(msg).indexOf("No errors.") != -1)
    return;

  if (type == QtFatalMsg) {
    std::cerr<<msg<<std::endl;
    abort();
  }

  if (type > _logSeverity)
    _logSeverity = type;

  _logCount++;
  _ui->listWidget->addItem(new QListWidgetItem(QIcon(iconForType(type)),
                           QString::fromUtf8(msg)));
}
#endif

QPixmap GraphPerspectiveLogger::icon() {
  return QPixmap(iconForType(_logSeverity));
}


void GraphPerspectiveLogger::clear() {
  _ui->listWidget->clear();
  _logCount = 0;
  _logSeverity = QtDebugMsg;
  emit cleared();
  close();
}
