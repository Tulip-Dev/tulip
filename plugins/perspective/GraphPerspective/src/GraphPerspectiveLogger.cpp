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

#include "tulip/FontIconManager.h"

#include <iostream>

#include <QKeyEvent>
#include <QClipboard>

#include <tulip/TlpQtTools.h>

GraphPerspectiveLogger::GraphPerspectiveLogger(QWidget *parent)
    : QFrame(parent), _logSeverity(QtDebugMsg), _logCount(0), _ui(new Ui::GraphPerspectiveLogger), _pythonOutput(false) {
  _ui->setupUi(this);
  _ui->clearButton->setIcon(tlp::FontIconManager::instance()->getMaterialDesignIcon(tlp::md::broom, Qt::yellow));
  // we want to be able to select multiple rows in the logger list for copy/paste operations
  _ui->listWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
  _ui->listWidget->installEventFilter(this);
  setWindowFlags(Qt::Popup);
  connect(_ui->clearButton, SIGNAL(clicked()), this, SLOT(clear()));
}

GraphPerspectiveLogger::~GraphPerspectiveLogger() {
  delete _ui;
}

static QIcon iconForType(QtMsgType type) {
  QIcon icon;

  switch (type) {
  case QtDebugMsg:
#if (QT_VERSION >= QT_VERSION_CHECK(5, 5, 0))
  case QtInfoMsg:
#endif
    icon = tlp::FontIconManager::instance()->getMaterialDesignIcon(tlp::md::information, QColor("#407FB2"), 0.9);
    break;

  case QtWarningMsg:
    icon = tlp::FontIconManager::instance()->getMaterialDesignIcon(tlp::md::alert, QColor("#E18D2B"), 0.9);
    break;

  case QtCriticalMsg:
  case QtFatalMsg:
    icon = tlp::FontIconManager::instance()->getMaterialDesignIcon(tlp::md::minuscircle, QColor("#C42730"), 0.9);
    break;
  }
  return icon;
}

unsigned int GraphPerspectiveLogger::count() const {
  return _logCount;
}

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
void GraphPerspectiveLogger::log(QtMsgType type, const QMessageLogContext &, const QString &msg) {
  if (type == QtFatalMsg) {
    std::cerr << tlp::QStringToTlpString(msg) << std::endl;
    abort();
  }

  if (type > _logSeverity)
    _logSeverity = type;

  _logCount++;

  if (msg.startsWith("[Python")) {
    // remove quotes around message added by Qt
    QString msgClean = msg.mid(14).mid(2, msg.length() - 17);
    _ui->listWidget->addItem(
        new QListWidgetItem(tlp::FontIconManager::instance()->getMaterialDesignIcon(tlp::md::languagepython, Qt::gray, 0.9), msgClean));
    _pythonOutput = true;
  } else {
    _ui->listWidget->addItem(new QListWidgetItem(iconForType(type), msg));
    _pythonOutput = false;
  }
}
#else
void GraphPerspectiveLogger::log(QtMsgType type, const char *msg) {

  QString qmsg(QString::fromUtf8(msg));

  // on some windows systems
  // "No errors." messages may be logged coming from QGLShader::link
  // we try to avoid them
  if (qmsg.indexOf("No errors.") != -1)
    return;

  if (type == QtFatalMsg) {
    std::cerr << tlp::QStringToTlpString(qmsg) << std::endl;
    abort();
  }

  if (type > _logSeverity)
    _logSeverity = type;

  _logCount++;

  if (qmsg.startsWith("[Python")) {
    // remove quotes around message added by Qt
    QString msgClean = qmsg.mid(14).mid(2, qmsg.length() - 18);
    _ui->listWidget->addItem(
        new QListWidgetItem(tlp::FontIconManager::instance()->getMaterialDesignIcon(tlp::md::languagepython, Qt::gray, 0.9), msgClean));
    _pythonOutput = true;
  } else {
    _ui->listWidget->addItem(new QListWidgetItem(iconForType(type), qmsg));
    _pythonOutput = false;
  }
}
#endif

QPixmap GraphPerspectiveLogger::icon() {
  if (!_pythonOutput) {
    return QPixmap(iconForType(_logSeverity).pixmap(QSize(16, 16)));
  } else {
    return tlp::FontIconManager::instance()->getMaterialDesignIcon(tlp::md::languagepython, Qt::gray).pixmap(QSize(16, 16));
  }
}

void GraphPerspectiveLogger::clear() {
  _ui->listWidget->clear();
  _logCount = 0;
  _logSeverity = QtDebugMsg;
  emit cleared();
  close();
}

// catch the copy to cliboard event of the QListWidget and reimplement
// its behaviour in order to be able to copy the text of all the selected rows
// (only the text of the current item is copied otherwise)
bool GraphPerspectiveLogger::eventFilter(QObject *, QEvent *event) {
  QKeyEvent *ke = dynamic_cast<QKeyEvent *>(event);

  if (ke && ke->matches(QKeySequence::Copy)) {
    QStringList strings;
    foreach (QListWidgetItem *item, _ui->listWidget->selectedItems())
      strings << item->text();

    QApplication::clipboard()->setText(strings.join("\n"));
    return true;
  }

  return false;
}
