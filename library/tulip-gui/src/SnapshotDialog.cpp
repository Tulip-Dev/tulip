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

#include <GL/glew.h>

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
#include <QPushButton>

#include <tulip/View.h>

using namespace std;

namespace tlp {

class LockLabel : public QLabel {
public :
  LockLabel():QLabel(),locked(true),alwaysLocked(false) {
    installEventFilter(this);
    setPixmap(QPixmap(":/tulip/gui/icons/i_locked.png"));
  }

  bool isLocked() const {
    return locked || alwaysLocked;
  }

  void setAlwaysLocked(bool alwaysLocked) {
    this->alwaysLocked = alwaysLocked;

    if (alwaysLocked) {
      setPixmap(QPixmap(":/tulip/gui/icons/i_locked.png"));
    }
  }

protected :
  bool eventFilter(QObject *, QEvent *event) {
    if(event->type() == QEvent::MouseButtonRelease && !alwaysLocked) {
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
  bool alwaysLocked;

};


SnapshotDialog::SnapshotDialog(const View *v, QWidget *parent):QDialog(parent),ui(new Ui::SnapshotDialogData()),view(v),scene(NULL),pixmapItem(NULL),ratio(-1),inSizeSpinBoxValueChanged(false) {
  ui->setupUi(this);

  int maxTextureSize = 0;
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);

  // restrict snapshot width and height to the half of the GL_MAX_TEXTURE_SIZE value
  ui->widthSpinBox->setMaximum(maxTextureSize/2);
  ui->heightSpinBox->setMaximum(maxTextureSize/2);

  ui->widthSpinBox->setValue(view->centralItem()->scene()->sceneRect().width());
  ui->heightSpinBox->setValue(view->centralItem()->scene()->sceneRect().height());

  connect(ui->widthSpinBox,SIGNAL(valueChanged(int)),this,SLOT(widthSpinBoxValueChanged(int)));
  connect(ui->heightSpinBox,SIGNAL(valueChanged(int)),this,SLOT(heightSpinBoxValueChanged(int)));

  QPushButton *copybutton = new QPushButton(QIcon(":/tulip/gui/icons/16/clipboard.png"), tr("&Copy to clipboard"), this);
  ui->buttonBox->addButton(copybutton, QDialogButtonBox::ActionRole);
  connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(clicked(QAbstractButton*)));

  lockLabel=new LockLabel();
  ui->horizontalLayout_2->addWidget(lockLabel);
  ui->horizontalLayout_2->setAlignment(lockLabel,Qt::AlignLeft | Qt::AlignVCenter);
}

void SnapshotDialog::clicked(QAbstractButton* b) {
  if(ui->buttonBox->buttonRole(b)==QDialogButtonBox::ResetRole) {
    ui->widthSpinBox->setValue(view->centralItem()->scene()->sceneRect().width());
    ui->heightSpinBox->setValue(view->centralItem()->scene()->sceneRect().height());
    ui->qualitySpinBox->setValue(100);
  }

  if(ui->buttonBox->buttonRole(b)==QDialogButtonBox::ActionRole) {
    QPixmap pixmap=view->snapshot(QSize(ui->widthSpinBox->value(),ui->heightSpinBox->value()));
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setPixmap(pixmap);
  }
}

SnapshotDialog::~SnapshotDialog() {
  delete ui;
  delete scene;
}

void SnapshotDialog::resizeEvent(QResizeEvent *) {
  sizeSpinBoxValueChanged();
}

static const QString default_filter("png");

void SnapshotDialog::accept() {
  QString formatedFormatList;

  //Put the default save format as the first choice (selectedFilter not supported under MacOSX and some Linux window managers)
  foreach(QString ext, QImageWriter::supportedImageFormats()) {
    ext = ext.toLower();

    if ((formatedFormatList.indexOf(ext) == -1)&&(ext!=default_filter)) {
      formatedFormatList+=ext+" (*."+ext+");;";
    }
  }

  QString selectedFilter(default_filter+" (*." + default_filter + ")");
  formatedFormatList = selectedFilter+";;"+formatedFormatList;
  // remove last ;;
  formatedFormatList.resize(formatedFormatList.size() - 2);

  QString fileName=
    QFileDialog::getSaveFileName(this,tr("Save image as..."),
                                 QDir::homePath(),
                                 formatedFormatList,
                                 &selectedFilter
                                 // on MacOSX selectedFilter is ignored by the
                                 // native dialog
#ifdef __APPLE__
                                 , QFileDialog::DontUseNativeDialog
#endif
                                );

  if(fileName.isEmpty())
    return;

  // force file extension
  QString selectedExtension = QString('.') + selectedFilter.section(' ', 0, 0);

  if (!fileName.endsWith(selectedExtension))
    fileName += selectedExtension;

  this->setEnabled(false);

  QPixmap pixmap=view->snapshot(QSize(ui->widthSpinBox->value(),ui->heightSpinBox->value()));

  QImage image(pixmap.toImage());

  if(!image.save(fileName,0,ui->qualitySpinBox->value())) {
    QMessageBox::critical(this,"Snapshot cannot be saved","Snapshot cannot be saved in file: "+fileName);
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

void SnapshotDialog::sizeSpinBoxValueChanged() {

  if (ui->widthSpinBox->value() < 10 || ui->heightSpinBox->value() < 10) {
    return;
  }

  float viewRatio = float(ui->graphicsView->width()) / float(ui->graphicsView->height());
  float imageRatio = float(ui->widthSpinBox->value()) / float(ui->heightSpinBox->value());

  // regenerate preview pixmap only if the aspect ratio changed
  if (imageRatio != ratio) {

    QPixmap pixmap;

    if(viewRatio>imageRatio) {
      pixmap = view->snapshot(QSize((view->centralItem()->scene()->sceneRect().height()-2)*imageRatio,view->centralItem()->scene()->sceneRect().height()-2));
      pixmap = pixmap.scaled((ui->graphicsView->height()-2)*imageRatio, ui->graphicsView->height()-2, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    }
    else {
      pixmap = view->snapshot(QSize(view->centralItem()->scene()->sceneRect().width()-2,(view->centralItem()->scene()->sceneRect().width()-2)/imageRatio));
      pixmap = pixmap.scaled(ui->graphicsView->width()-2, (ui->graphicsView->width()-2)/imageRatio, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    }

    ratio = float(ui->widthSpinBox->value())/ float(ui->heightSpinBox->value());

    if (pixmapItem != NULL) {
      delete scene;
    }

    scene=new QGraphicsScene();
    scene->setBackgroundBrush(QApplication::palette().color(QPalette::Midlight));
    ui->graphicsView->setScene(scene);
    pixmapItem=scene->addPixmap(pixmap);
    pixmapItem->setPos(ui->graphicsView->sceneRect().center()-pixmapItem->boundingRect().center());

  }
}

void SnapshotDialog::setSnapshotHasViewSizeRatio(bool snapshotHasViewSizeRatio) {
  lockLabel->setAlwaysLocked(snapshotHasViewSizeRatio);
}

}
