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

#include "GraphPerspectiveLogger.h"

#include "ui_GraphPerspectiveLogger.h"
#include <iostream>

#include <QKeyEvent>
#include <QClipboard>
#include <QMenu>
#include <QPushButton>
#include <QShowEvent>
#include <QHideEvent>

#include <tulip/TlpQtTools.h>
#include <tulip/TulipSettings.h>

GraphPerspectiveLogger::GraphPerspectiveLogger(QWidget *parent)
    : QDialog(parent), _logType(QtDebugMsg), _ui(new Ui::GraphPerspectiveLogger),
      _pythonOutput(false) {
  _ui->setupUi(this);
  _ui->listWidget->installEventFilter(this);
  _ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
  QPushButton *copybutton =
      new QPushButton(QIcon(":/tulip/gui/icons/16/clipboard.png"), "&Copy selection", this);
  copybutton->setToolTip("Copy the selected lines into the clipboard");
  _ui->buttonBox->addButton(copybutton, QDialogButtonBox::ActionRole);
  QPushButton *clearbutton = new QPushButton("Clear", this);
  clearbutton->setToolTip("Remove all messages");
  _ui->buttonBox->addButton(clearbutton, QDialogButtonBox::ActionRole);
  connect(_ui->listWidget, SIGNAL(customContextMenuRequested(QPoint)), this,
          SLOT(showContextMenu(QPoint)));
  connect(copybutton, SIGNAL(clicked()), this, SLOT(copy()));
  connect(clearbutton, SIGNAL(clicked()), this, SLOT(clear()));
  _ui->buttonBox->button(QDialogButtonBox::Close)->setToolTip("Close this window");
  QPushButton *resetb = _ui->buttonBox->button(QDialogButtonBox::Reset);
  resetb->setToolTip("Remove all messages and close this window");
  connect(resetb, SIGNAL(clicked()), this, SLOT(clear()));
  connect(resetb, SIGNAL(clicked()), this, SLOT(hide()));
  connect(_ui->anchoredCB, SIGNAL(toggled(bool)), this, SLOT(setAnchored(bool)));
  _ui->anchoredCB->setChecked(tlp::TulipSettings::instance().loggerAnchored());
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
void GraphPerspectiveLogger::log(QtMsgType type, const char *msg) {
  logImpl(type, QString::fromUtf8(msg));
}
#endif

void GraphPerspectiveLogger::logImpl(QtMsgType type, const QString &msg) {

  // on some windows systems
  // "No errors." messages may be logged coming from QGLShader::link
  // we try to avoid them
  if (msg.indexOf("No errors.") != -1 || msg.isEmpty())
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
    msgClean = msg.mid(14).mid(2, msg.length() - 17);
#else
    msgClean = msg.mid(14).mid(2, msg.length() - 18);
#endif
    _pythonOutput = true;
  } else {
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
    pxUrl += "info";
    break;

  case Warning:
    pxUrl += "danger";
    break;

  case Error:
    pxUrl += "error";
    break;
  }

  pxUrl += ".png";

  return pxUrl;
}

void GraphPerspectiveLogger::clear() {
  _ui->listWidget->clear();
  _logType = QtDebugMsg;
  emit cleared();
  _logCounts[Info] = 0;
  _logCounts[Warning] = 0;
  _logCounts[Error] = 0;
  _logCounts[Python] = 0;
}

void GraphPerspectiveLogger::copy() {
  QStringList strings;

  foreach (QListWidgetItem *item, _ui->listWidget->selectedItems())
    strings << item->text();

  if (!strings.isEmpty())
    QApplication::clipboard()->setText(strings.join("\n"));
}

void GraphPerspectiveLogger::showContextMenu(const QPoint &pos) {
  QMenu m;
  QAction *clear = m.addAction("Clear content", this, SLOT(clear()));
  QAction *copy = m.addAction("Copy", this, SLOT(copy()), QKeySequence::Copy);
  m.addAction("Close", this, SLOT(close()), QKeySequence::Close);
  copy->setEnabled(_ui->listWidget->count() != 0);
  clear->setEnabled(_ui->listWidget->count() != 0);
  m.exec(_ui->listWidget->mapToGlobal(pos));
}

// catch the copy to clipboard event of the QListWidget and reimplement
// its behaviour in order to be able to copy the text of all the selected rows
// (only the text of the current item is copied otherwise)
bool GraphPerspectiveLogger::eventFilter(QObject *, QEvent *event) {
  QKeyEvent *ke = dynamic_cast<QKeyEvent *>(event);

  if (ke && ke->matches(QKeySequence::Copy)) {
    copy();
    return true;
  }

  return false;
}

void GraphPerspectiveLogger::showEvent(QShowEvent *evt) {
  QDialog::showEvent(evt);

  if (!_windowGeometry.isNull()) {
    restoreGeometry(_windowGeometry);
  }
}

void GraphPerspectiveLogger::hideEvent(QHideEvent *evt) {

  _windowGeometry = saveGeometry();
  QDialog::hideEvent(evt);
}

void GraphPerspectiveLogger::setGeometry(int x, int y, int w, int h) {
  setMinimumSize(QSize(0, 0));
  setMaximumSize(QSize(16777215, 16777215));
  QDialog::setGeometry(x, y, w, h);
  _windowGeometry = saveGeometry();

  if (_anchored) {
    setMinimumSize(size());
    setMaximumSize(size());
  }
}

void GraphPerspectiveLogger::setAnchored(bool anchored) {
  _anchored = anchored;
  bool visible = isVisible();

  if (_anchored) {
    setAttribute(Qt::WA_X11NetWmWindowTypeDialog, false);
    setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
    setMinimumSize(size());
    setMaximumSize(size());
    emit resetLoggerPosition();
  } else {
    setAttribute(Qt::WA_X11NetWmWindowTypeDialog, true);
    setWindowFlags(Qt::Dialog);
    setMinimumSize(QSize(0, 0));
    setMaximumSize(QSize(16777215, 16777215));
  }

  tlp::TulipSettings::instance().setLoggerAnchored(anchored);

  // force the update of the window after modifying its flags
  if (visible) {
    show();
  }
}
