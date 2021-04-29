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
#include <QShortcut>

#include <tulip/TlpQtTools.h>
#include <tulip/TulipSettings.h>

using namespace tlp;

GraphPerspectiveLogger::GraphPerspectiveLogger(QWidget *parent)
    : QDialog(parent), _logType(QtDebugMsg), _ui(new Ui::GraphPerspectiveLogger),
      _pythonOutput(false) {
  _ui->setupUi(this);
  _ui->listWidget->installEventFilter(this);
  _ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
  // ensure vertical scroll bar is displayed as soon as
  // a line is vertically truncated
  _ui->listWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

  QPushButton *button = new QPushButton("Clear", this);
  button->setToolTip("Remove all messages");
  connect(button, SIGNAL(clicked()), this, SLOT(clear()));
  _ui->buttonBox->addButton(button, QDialogButtonBox::ActionRole);
  button = new QPushButton(QIcon(":/tulip/gui/icons/16/clipboard.png"), "Copy selection", this);
  button->setToolTip(QString("Copy the selected messages into the clipboard [%1]")
                         .arg(QKeySequence(QKeySequence::Copy).toString()));
  connect(button, SIGNAL(clicked()), this, SLOT(copy()));
  _ui->buttonBox->addButton(button, QDialogButtonBox::ActionRole);
  button = new QPushButton("Remove selection", this);
  button->setToolTip(
      QString("Remove the selected messages [%1]").arg(QKeySequence(QKeySequence::Cut).toString()));
  connect(button, SIGNAL(clicked()), this, SLOT(remove()));
  _ui->buttonBox->addButton(button, QDialogButtonBox::ActionRole);
  connect(_ui->listWidget, SIGNAL(customContextMenuRequested(QPoint)), this,
          SLOT(showContextMenu(QPoint)));
  _ui->buttonBox->button(QDialogButtonBox::Close)->setToolTip("Close this window");
  button = _ui->buttonBox->button(QDialogButtonBox::Reset);
  button->setToolTip("Remove all messages and close this window");
  connect(button, SIGNAL(clicked()), this, SLOT(clear()));
  connect(button, SIGNAL(clicked()), this, SLOT(hide()));
  connect(_ui->anchoredCB, SIGNAL(toggled(bool)), this, SLOT(setAnchored(bool)));
  _ui->anchoredCB->setChecked(tlp::TulipSettings::loggerAnchored());
  connect(_ui->decreaseFontSizeButton, SIGNAL(clicked()), this, SLOT(decreaseFontSize()));
  connect(_ui->increaseFontSizeButton, SIGNAL(clicked()), this, SLOT(increaseFontSize()));
  auto shortCut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Minus), this);
  connect(shortCut, SIGNAL(activated()), this, SLOT(decreaseFontSize()));
  SET_TIPS_WITH_CTRL_SHORTCUT(_ui->decreaseFontSizeButton, "decrease font size", "-");
  shortCut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Plus), this);
  connect(shortCut, SIGNAL(activated()), this, SLOT(increaseFontSize()));
  SET_TIPS_WITH_CTRL_SHORTCUT(_ui->increaseFontSizeButton, "increase font size", "-");
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

void GraphPerspectiveLogger::log(QtMsgType type, const QMessageLogContext &, const QString &msg,
                                 bool pyOutput) {
  _logType = type;
  _pythonOutput = pyOutput;

  LogType lastLogType = getLastLogType();
  auto item = new QListWidgetItem(QIcon(icon(lastLogType)), msg, nullptr, _logType);
  _ui->listWidget->addItem(item);
  _ui->listWidget->scrollToItem(item);
  _logCounts[lastLogType] += 1;
}

QPixmap GraphPerspectiveLogger::icon(LogType logType) const {
  QString pxUrl(":/tulip/graphperspective/icons/16/logger-");

  switch (logType) {
  case Python:
    return QPixmap(":/tulip/gui/icons/16/python.png");

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

  for (auto item : _ui->listWidget->selectedItems())
    strings << item->text();

  if (!strings.isEmpty())
    QApplication::clipboard()->setText(strings.join("\n"));
}

void GraphPerspectiveLogger::remove() {
  for (auto item : _ui->listWidget->selectedItems()) {
    if (!(_pythonOutput = item->text().startsWith("[Python")))
      _logType = static_cast<QtMsgType>(item->type());
    _logCounts[getLastLogType()] -= 1;
    delete _ui->listWidget->takeItem(_ui->listWidget->row(item));
    emit itemRemoved();
  }
}

void GraphPerspectiveLogger::showContextMenu(const QPoint &pos) {
  QMenu m;
  if (_ui->listWidget->count() > 0) {
    m.addAction("Clear", this, SLOT(clear()));
    m.addAction("Copy selection", this, SLOT(copy()), QKeySequence::Copy);
    m.addAction("Remove selection", this, SLOT(remove()), QKeySequence::Cut);
    m.addSeparator();
  }
  m.addAction("Close", this, SLOT(close()));
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

  if (ke && ke->matches(QKeySequence::Cut)) {
    remove();
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

  tlp::TulipSettings::setLoggerAnchored(anchored);

  // force the update of the window after modifying its flags
  if (visible) {
    show();
  }
}

void GraphPerspectiveLogger::decreaseFontSize() {
  int fs = _ui->listWidget->font().pointSize();
  _ui->listWidget->setStyleSheet(QString("QListView { font-size: %1pt; }").arg(fs - 1));
}

void GraphPerspectiveLogger::increaseFontSize() {
  int fs = _ui->listWidget->font().pointSize();
  _ui->listWidget->setStyleSheet(QString("QListView { font-size: %1pt; }").arg(fs + 1));
}
