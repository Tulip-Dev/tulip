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

#include "tulip/SnapshotDialog.h"
#include "ui_SnapshotDialog.h"

#include <QLabel>
#include <QEvent>
#include <QMessageBox>
#include <QImageWriter>
#include <QFileDialog>
#include <QGraphicsPixmapItem>
#include <QClipboard>
#include <QGraphicsScene>
#include <QPixmap>

#include <tulip/View.h>

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


SnapshotDialog::SnapshotDialog(const View *v, QWidget *parent):QDialog(parent),ui(new Ui::SnapshotDialogData()),view(v),scene(NULL),pixmapItem(NULL),inSizeSpinBoxValueChanged(false) {
  ui->setupUi(this);

  ui->widthSpinBox->setValue(view->centralItem()->scene()->sceneRect().width());
  ui->heightSpinBox->setValue(view->centralItem()->scene()->sceneRect().height());

  sizeSpinBoxValueChanged();

  connect(ui->widthSpinBox,SIGNAL(valueChanged(int)),this,SLOT(widthSpinBoxValueChanged(int)));
  connect(ui->heightSpinBox,SIGNAL(valueChanged(int)),this,SLOT(heightSpinBoxValueChanged(int)));
  connect(ui->copybutton, SIGNAL(clicked()), this, SLOT(copyClicked()));

  lockLabel=new LockLabel();
  ui->horizontalLayout_2->addWidget(lockLabel);
  lockLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);

  ui->okButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogOkButton));
  ui->cancelButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogCancelButton));
}

SnapshotDialog::~SnapshotDialog() {
  delete ui;
}

void SnapshotDialog::resizeEvent(QResizeEvent *) {
  sizeSpinBoxValueChanged();
}

void SnapshotDialog::accept() {
  QList<QByteArray> formatList=QImageWriter::supportedImageFormats();
  QString formatedFormatList;

  for(QList<QByteArray>::iterator it=formatList.begin(); it!=formatList.end(); ++it) {
    if (formatedFormatList.indexOf(QString(*it).toLower()) == -1)
      formatedFormatList+=QString(*it).toLower()+" (*."+QString(*it).toLower()+");;";
  }

  // remove last ;;
  formatedFormatList.resize(formatedFormatList.size() - 2);

  QString selectedFilter("jpeg (*.jpeg)");
  QString fileName=
    QFileDialog::getSaveFileName(this,tr("Save image as..."),
                                 QDir::homePath(),
                                 tr(QString(formatedFormatList).toStdString().c_str()),
                                 &selectedFilter
                                 // on MacOSX selectedFilter is ignored by the
                                 // native dialog
#ifdef __APPLE__
                                 , QFileDialog::DontUseNativeDialog
#endif
                                );

  if(fileName=="")
    return;

  // force file extension
  QString selectedExtension = QString('.') + selectedFilter.section(' ', 0, 0);

  if (!fileName.endsWith(selectedExtension))
    fileName += selectedExtension;

  this->setEnabled(false);

  QPixmap pixmap=view->snapshot(QSize(ui->widthSpinBox->value(),ui->heightSpinBox->value()));

  QImage image(pixmap.toImage());

  if(!image.save(fileName,0,ui->qualitySpinBox->value())) {
    QMessageBox::critical(this,"Snapshot cannot be saved","Snapshot cannot be saved in file : "+fileName);
    this->setEnabled(true);
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
    ui->heightSpinBox->setValue(value/ratio);
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
    ui->widthSpinBox->setValue(value*ratio);
  }
  else {
    sizeSpinBoxValueChanged();
  }

  inSizeSpinBoxValueChanged=false;
}

void SnapshotDialog::fileNameTextChanged(const QString &text) {
  ui->okButton->setEnabled(text.isEmpty()?false:true);
}

void SnapshotDialog::sizeSpinBoxValueChanged() {

  if (ui->widthSpinBox->value() < 10 || ui->heightSpinBox->value() < 10) {
    return;
  }

  float viewRatio=((float)ui->graphicsView->width())/((float)ui->graphicsView->height());
  float imageRatio=((float)ui->widthSpinBox->value())/((float)ui->heightSpinBox->value());

  QPixmap pixmap;

  if(viewRatio>imageRatio) {
    pixmap=view->snapshot(QSize((view->centralItem()->scene()->sceneRect().height()-2)*imageRatio,view->centralItem()->scene()->sceneRect().height()-2));
    pixmap = pixmap.scaled((ui->graphicsView->height()-2)*imageRatio, ui->graphicsView->height()-2, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
  }
  else {
    pixmap=view->snapshot(QSize(view->centralItem()->scene()->sceneRect().width()-2,(view->centralItem()->scene()->sceneRect().width()-2)/imageRatio));
    pixmap = pixmap.scaled(ui->graphicsView->width()-2, (ui->graphicsView->width()-2)/imageRatio, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
  }

  ratio=((float)ui->widthSpinBox->value())/((float)ui->heightSpinBox->value());

  if(pixmapItem==NULL) {
    delete pixmapItem;
    delete scene;
  }

  scene=new QGraphicsScene();
  scene->setBackgroundBrush(QApplication::palette().color(QPalette::Midlight));
  ui->graphicsView->setScene(scene);
  pixmapItem=scene->addPixmap(pixmap);
  pixmapItem->setPos(ui->graphicsView->sceneRect().center()-pixmapItem->boundingRect().center());
}

void SnapshotDialog::copyClicked() {
  QPixmap pixmap=view->snapshot(QSize(ui->widthSpinBox->value(),ui->heightSpinBox->value()));

  QClipboard *clipboard = QApplication::clipboard();
  clipboard->setPixmap(pixmap);
}

}
