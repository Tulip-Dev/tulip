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

#include "TulipSplashScreen.h"

#include <QPainter>
#include <QApplication>
#include <QPropertyAnimation>
#include <QDir>

#include <tulip/TlpTools.h>
#include <tulip/Plugin.h>

using namespace tlp;

TulipSplashScreen::TulipSplashScreen(): PluginLoader(), QSplashScreen(), _fileCounter(0) {
  setPixmap(QPixmap(QDir(QApplication::applicationDirPath()).absoluteFilePath("../share/tulip/bitmaps/logo.bmp")));
  setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
  QPropertyAnimation *fadeInAnimation = new QPropertyAnimation(this,"windowOpacity");
  fadeInAnimation->setStartValue(0);
  fadeInAnimation->setEndValue(1);
  fadeInAnimation->setDuration(200);
  show();
  fadeInAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void TulipSplashScreen::start(const std::string &) {
  _title = trUtf8("Loading ") + "...";
  _message = QString();
  repaint();
}

void TulipSplashScreen::loading(const std::string &filename) {
  QApplication::processEvents();
  _fileCounter++;
  _message = filename.c_str();
  repaint();
}

void TulipSplashScreen::loaded(const Plugin* infos, const std::list <Dependency>& ) {
  _message = infos->name().c_str() + trUtf8(" loaded.");
}

void TulipSplashScreen::aborted(const std::string &filename, const std::string &erreurmsg) {
  _message = trUtf8("Error loading ") + filename.c_str() + ": " + erreurmsg.c_str();
}

void TulipSplashScreen::finished(bool state, const std::string&) {
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
