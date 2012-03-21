#include "GraphPerspectiveLogger.h"

#include "ui_GraphPerspectiveLogger.h"
#include <iostream>

GraphPerspectiveLogger::GraphPerspectiveLogger(QWidget* parent): QWidget(parent), _logSeverity(QtDebugMsg), _logCount(0), _ui(new Ui::GraphPerspectiveLogger) {
  _ui->setupUi(this);
  connect(_ui->clearButton,SIGNAL(clicked()),this,SLOT(clear()));
}

QString GraphPerspectiveLogger::iconForType(QtMsgType type) {
  QString pxUrl(":/tulip/graphperspective/icons/16/logger-");
  switch (type) {
  case QtDebugMsg:
    pxUrl+="info.png";
    break;
  case QtWarningMsg:
    pxUrl+="danger.png";
    break;
  case QtCriticalMsg:
    pxUrl+="error.png";
    break;
  default:
    break;
  }
  return pxUrl;
}

unsigned int GraphPerspectiveLogger::count() const {
  return _logCount;
}

void GraphPerspectiveLogger::log(QtMsgType type, const char* msg) {
  if (type == QtFatalMsg) {
    std::cerr<<msg<<std::endl;
    abort();
  }
  if (type > _logSeverity)
    _logSeverity = type;
  _logCount++;
  _ui->listWidget->addItem(new QListWidgetItem(QIcon(iconForType(type)),msg));
}

QPixmap GraphPerspectiveLogger::icon() {
  return QPixmap(iconForType(_logSeverity));
}


void GraphPerspectiveLogger::clear() {
  _ui->listWidget->clear();
  _logCount = 0;
  _logSeverity = QtDebugMsg;
  emit cleared();
}
