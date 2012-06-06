/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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

#include "tulip/SnapshotDialog.h"

#include <tulip/GlMainView.h>
#include <tulip/GlMainWidget.h>

#include <QtGui/QMessageBox>
#include <QtGui/QImageWriter>
#include <QtGui/QFileDialog>
#include <QtGui/QGraphicsPixmapItem>
#include <QtGui/QClipboard>

using namespace std;

namespace tlp {

class LockLabel : public QLabel {
public :
  LockLabel():QLabel(),locked(true) {
    installEventFilter(this);
    setPixmap(QPixmap(":/tulip/gui/icons/i_locked.png"));
  }

  bool isLocked() {
    return locked;
  }

protected :
  bool eventFilter(QObject *, QEvent *event) {
    if(event->type() == QEvent::MouseButtonRelease) {
      if(locked) {
        setPixmap(QPixmap(":/tulip/gui/icons/i_unlocked.png"));
        locked=false;
      }
      else {
        setPixmap(QPixmap(":/tulip/gui/icons/i_locked.png"));
        locked=true;
      }

      return true;
    }

    return false;
  }

  bool locked;

};

SnapshotDialog::SnapshotDialog(GlMainView &v,QWidget *parent):QDialog(parent),view(&v),scene(NULL),pixmapItem(NULL),inSizeSpinBoxValueChanged(false) {
  setupUi(this);

  widthSpinBox->setValue(view->getGlMainWidget()->rect().width());
  heightSpinBox->setValue(view->getGlMainWidget()->rect().height());

  sizeSpinBoxValueChanged();

  connect(widthSpinBox,SIGNAL(valueChanged(int)),this,SLOT(widthSpinBoxValueChanged(int)));
  connect(heightSpinBox,SIGNAL(valueChanged(int)),this,SLOT(heightSpinBoxValueChanged(int)));
  connect(fileName,SIGNAL(textChanged(QString)),this,SLOT(fileNameTextChanged(const QString &)));
  connect(browseButton,SIGNAL(clicked()),this,SLOT(browseClicked()));
  connect(copybutton, SIGNAL(clicked()), this, SLOT(copyClicked()));

  lockLabel=new LockLabel();
  lockLayout->addWidget(lockLabel);

  fileName->setPlaceholderText(QApplication::translate("SnapshotDialogData", "Enter the filename or use the browse button", 0, QApplication::UnicodeUTF8));
}

SnapshotDialog::~SnapshotDialog() {
}

void SnapshotDialog::resizeEvent(QResizeEvent *) {
  sizeSpinBoxValueChanged();
}

void SnapshotDialog::accept() {
  QPixmap pixmap=view->snapshot(QSize(widthSpinBox->value(),heightSpinBox->value()));

  QImage image(pixmap.toImage());

  if(!image.save(fileName->text(),0,qualitySpinBox->value())) {
    QMessageBox::critical(this,"Snapshot cannot be saved","Snapshot cannot be saved in file : "+fileName->text());
  }
  else {
    QDialog::accept();
  }
}

void SnapshotDialog::widthSpinBoxValueChanged(int value) {
  if(inSizeSpinBoxValueChanged)
    return;

  inSizeSpinBoxValueChanged=true;

  if(lockLabel->isLocked()) {
    heightSpinBox->setValue(value/ratio);
  }
  else {
    sizeSpinBoxValueChanged();
  }

  inSizeSpinBoxValueChanged=false;
}

void SnapshotDialog::heightSpinBoxValueChanged(int value) {
  if(inSizeSpinBoxValueChanged)
    return;

  inSizeSpinBoxValueChanged=true;

  if(lockLabel->isLocked()) {
    widthSpinBox->setValue(value*ratio);
  }
  else {
    sizeSpinBoxValueChanged();
  }

  inSizeSpinBoxValueChanged=false;
}

void SnapshotDialog::browseClicked() {
  QList<QByteArray> formatList=QImageWriter::supportedImageFormats();
  QString formatedFormatList;

  for(QList<QByteArray>::iterator it=formatList.begin(); it!=formatList.end(); ++it) {
    formatedFormatList+=QString(*it).toLower()+" (*."+QString(*it).toLower()+");;";
  }

  QString newFileName=QFileDialog::getSaveFileName(this,tr("Save image as..."), QDir::homePath(), tr(QString(formatedFormatList).toStdString().c_str()));
  fileName->setText(newFileName);
}

void SnapshotDialog::fileNameTextChanged(const QString &text) {
  okButton->setEnabled(text.isEmpty()?false:true);
}

void SnapshotDialog::sizeSpinBoxValueChanged() {

  float viewRatio=((float)graphicsView->width())/((float)graphicsView->height());
  float imageRatio=((float)widthSpinBox->value())/((float)heightSpinBox->value());

  QPixmap pixmap;

  if(viewRatio>imageRatio) {
    pixmap=view->snapshot(QSize((graphicsView->height()-2)*imageRatio,graphicsView->height()-7));
  }
  else {
    pixmap=view->snapshot(QSize(graphicsView->width()-2,(graphicsView->width()-2)/imageRatio));
  }

  ratio=((float)widthSpinBox->value())/((float)heightSpinBox->value());

  if(pixmapItem==NULL) {
    delete pixmapItem;
    delete scene;
  }

  scene=new QGraphicsScene();
  scene->setBackgroundBrush(QApplication::palette().color(QPalette::Midlight));
  graphicsView->setScene(scene);
  QPixmap pixmap2;
  QImage img(pixmap.toImage());
  img = QImage(img.bits(),img.width(),img.height(),QImage::Format_ARGB32);
  pixmap2.convertFromImage(img);
  pixmapItem=scene->addPixmap(pixmap2);
  pixmapItem->setPos(graphicsView->sceneRect().center()-pixmapItem->boundingRect().center());
}

void SnapshotDialog::copyClicked() {
  QPixmap pixmap=view->snapshot(QSize(widthSpinBox->value(),heightSpinBox->value()));

  QClipboard *clipboard = QApplication::clipboard();
  clipboard->setPixmap(pixmap);
}

}
