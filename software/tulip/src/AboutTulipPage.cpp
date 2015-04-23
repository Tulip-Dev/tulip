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
#include "AboutTulipPage.h"

#include "ui_AboutTulipPage.h"

#include <QAbstractItemModel>
#include <QDir>
#include <QUrl>

#include <QDesktopServices>

#include <tulip/TlpTools.h>

class PictureModel: public QAbstractItemModel {
  QDir _rootPath;

public:
  PictureModel(const QString& picturesPath,QObject* parent=0): QAbstractItemModel(parent), _rootPath(picturesPath) {
  }

  int rowCount(const QModelIndex & = QModelIndex()) const {
    return _rootPath.entryList(QDir::Files).size();
  }

  int columnCount(const QModelIndex & = QModelIndex()) const {
    return 1;
  }

  QModelIndex parent(const QModelIndex &) const {
    return QModelIndex();
  }

  QModelIndex index(int row, int column,const QModelIndex &parent = QModelIndex()) const {
    if (!hasIndex(row,column,parent)) {
      return QModelIndex();
    }

    return createIndex(row,column);
  }

  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const {
    if (role == Qt::DecorationRole) {
      return QImage(_rootPath.entryInfoList(QDir::Files)[index.row()].absoluteFilePath());
    }
    else if (role == Qt::DisplayRole) {
      return _rootPath.entryInfoList(QDir::Files)[index.row()].baseName();
    }

    return QVariant();
  }
};

AboutTulipPage::AboutTulipPage(QWidget *parent) :
  QWidget(parent), _ui(new Ui::AboutTulipPageData()) {
  _ui->setupUi(this);
  _ui->flowView->setBackgroundColor(Qt::white);
  _ui->flowView->setModel(new PictureModel(QDir(tlp::TulipBitmapDir.c_str()).filePath("samplePictures"),this));
  _ui->flowView->setSlideSize(QSize(250,250));
  _ui->flowView->setCurrentIndex(_ui->flowView->model()->index(_ui->flowView->model()->rowCount()/2,0));
  _ui->flowView->installEventFilter(this);
  _ui->flowView->setReflectionEffect(QxtFlowView::BlurredReflection);
}

bool AboutTulipPage::eventFilter(QObject* obj, QEvent* ev) {
  if (obj == _ui->flowView && ev->type() == QEvent::MouseButtonPress) {
    QDesktopServices::openUrl( QUrl("http://tulip.labri.fr/TulipDrupal/?q=node/" + _ui->flowView->model()->data(_ui->flowView->currentIndex()).toString() ));
    return true;
  }

  return false;
}
