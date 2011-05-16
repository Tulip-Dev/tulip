#include "TulipSplashScreen.h"

#include <QtGui/QPainter>
#include <QtGui/QApplication>
#include <QtCore/QPropertyAnimation>

#include <tulip/TlpTools.h>
#include <tulip/AbstractPluginInfo.h>

using namespace tlp;

TulipSplashScreen::TulipSplashScreen(): PluginLoader(), QSplashScreen(), _fileCounter(0) {
  setPixmap(QPixmap((tlp::TulipBitmapDir + "/logo.bmp").c_str()));

  setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
  QPropertyAnimation *fadeInAnimation = new QPropertyAnimation(this,"windowOpacity");
  fadeInAnimation->setStartValue(0);
  fadeInAnimation->setEndValue(1);
  fadeInAnimation->setDuration(200);
  fadeInAnimation->start(QAbstractAnimation::DeleteWhenStopped);
  _abortedPlugins.clear();
}

void TulipSplashScreen::start(const std::string &, const std::string &type) {
  _title = trUtf8("Loading ") + type.c_str() + "...";
  _message = QString();
  repaint();
}

void TulipSplashScreen::loading(const std::string &filename) {
  QApplication::processEvents();
  _fileCounter++;
  _message = filename.c_str();
  repaint();
}

void TulipSplashScreen::loaded(const AbstractPluginInfo* infos, const std::list <Dependency>& ) {
  _message = infos->getName().c_str() + trUtf8(" loaded.");
}

void TulipSplashScreen::aborted(const std::string &filename, const std::string &erreurmsg) {
  _message = trUtf8("Error loading ") + filename.c_str() + ": " + erreurmsg.c_str();

  _abortedPlugins[filename.c_str()] = erreurmsg.c_str();
}

void TulipSplashScreen::finished(bool state, const std::string &msg) {
  _title = trUtf8("Plugins loaded.");
  if (!state)
    _message = trUtf8("Errors have been reported, see details on the startup screen.");
  repaint();
}

void TulipSplashScreen::drawContents(QPainter *painter) {
  QSize size(pixmap().size());
  painter->drawPixmap(0,0,pixmap());
  QRectF messageRect(0,size.height()*2/3,size.width(),size.height()/3);

  painter->setPen(Qt::transparent);
  QLinearGradient grad(messageRect.x(),messageRect.y(),messageRect.x(),messageRect.y()+messageRect.height());
  grad.setColorAt(0,Qt::transparent);
  grad.setColorAt(0.4,QColor(255,255,255,170));
  painter->setBrush(grad);
  painter->drawRect(messageRect);

  painter->setBrush(Qt::black);
  painter->setPen(Qt::black);
  QFont font = painter->font();
  font.setBold(true);
  painter->setFont(font);
  painter->drawText(messageRect.x(),messageRect.y()+messageRect.height()/3,messageRect.width(),messageRect.height()/3,Qt::AlignHCenter,_title);
  font.setBold(false);
  painter->setFont(font);
  painter->drawText(messageRect.x(),messageRect.y()+messageRect.height()*2/3,messageRect.width(),messageRect.height()/3,Qt::AlignHCenter,_message);

  QRectF progressRect(messageRect.x() + 10, messageRect.y()+messageRect.height() - 10, messageRect.width()-20, 5);
  painter->setBrush(QColor(0,0,0,50));
  painter->setPen(QColor(0,0,0,50));
  painter->drawRect(progressRect);
  qreal w = progressRect.width();
  if (_numberOfFiles>0)
    w = 1. * _fileCounter*progressRect.width()/_numberOfFiles;

  painter->setBrush(QColor(0,0,0,200));
  painter->drawRect(progressRect.x(),progressRect.y(),w,progressRect.height());
}
