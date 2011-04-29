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

#include <QtGui/QMessageBox>
#include <QtGui/QImageWriter>
#include <QtGui/QFileDialog>

using namespace std;

namespace tlp {

class LockLabel : public QLabel {
public :
  LockLabel():QLabel(),locked(true){
    installEventFilter(this);
    setPixmap(QPixmap(":/i_locked.png"));
  }

  bool isLocked(){
    return locked;
  }

protected :
  bool eventFilter(QObject *, QEvent *event){
    if(event->type() == QEvent::MouseButtonRelease){
      if(locked){
        setPixmap(QPixmap(":/i_unlocked.png"));
        locked=false;
      }else{
        setPixmap(QPixmap(":/i_locked.png"));
        locked=true;
      }
      return true;
    }
    return false;
  }

  bool locked;

};

SnapshotDialog::SnapshotDialog(View &v,QWidget *parent):QDialog(parent),view(&v),pixmapItem(NULL),inSizeSpinBoxValueChanged(false) {
  setupUi(this);
  scene=new QGraphicsScene();
  scene->setBackgroundBrush(QApplication::palette().color(QPalette::Midlight));
  graphicsView->setScene(scene);

  widthSpinBox->setValue(view->getWidget()->width());
  heightSpinBox->setValue(view->getWidget()->height());

  pixmapItem=scene->addPixmap(QPixmap(10,10));

  sizeSpinBoxValueChanged();

  connect(widthSpinBox,SIGNAL(valueChanged(int)),this,SLOT(widthSpinBoxValueChanged(int)));
  connect(heightSpinBox,SIGNAL(valueChanged(int)),this,SLOT(heightSpinBoxValueChanged(int)));
  connect(fileName,SIGNAL(textChanged(QString)),this,SLOT(fileNameTextChanged(QString)));
  connect(browseButton,SIGNAL(clicked()),this,SLOT(browseClicked()));

  lockLabel=new LockLabel();
  lockLayout->addWidget(lockLabel);
}

SnapshotDialog::~SnapshotDialog(){
}

void SnapshotDialog::resizeEvent(QResizeEvent *){
  sizeSpinBoxValueChanged();
}

void SnapshotDialog::accept(){
  QImage image=view->createPicture(widthSpinBox->value(),heightSpinBox->value(),false);
  if(!image.save(fileName->text(),0,qualitySpinBox->value())){
    QMessageBox::critical(this,"Snapshot can't be saved","Snapshot can't be saved in file : "+fileName->text());
  }else{
    QDialog::accept();
  }
}

void SnapshotDialog::widthSpinBoxValueChanged(int value){
  if(inSizeSpinBoxValueChanged)
    return;

  inSizeSpinBoxValueChanged=true;

  if(lockLabel->isLocked()){
    heightSpinBox->setValue(value/ratio);
  }else{
    sizeSpinBoxValueChanged();
  }

  inSizeSpinBoxValueChanged=false;
}

void SnapshotDialog::heightSpinBoxValueChanged(int value){
  if(inSizeSpinBoxValueChanged)
    return;

  inSizeSpinBoxValueChanged=true;

  if(lockLabel->isLocked()){
    widthSpinBox->setValue(value*ratio);
  }else{
    sizeSpinBoxValueChanged();
  }

  inSizeSpinBoxValueChanged=false;
}

void SnapshotDialog::browseClicked(){
  QList<QByteArray> formatList=QImageWriter::supportedImageFormats();
  QString formatedFormatList;
  for(QList<QByteArray>::iterator it=formatList.begin();it!=formatList.end();++it){
    formatedFormatList+=QString(*it).toLower()+" (*."+QString(*it).toLower()+");;";
  }
  QString newFileName=QFileDialog::getSaveFileName(this,tr("Save Image As"), QDir::homePath(), tr(QString(formatedFormatList).toStdString().c_str()));
  fileName->setText(newFileName);
}

void SnapshotDialog::fileNameTextChanged(QString text){
  if(text!="")
    okButton->setEnabled(true);
  else
    okButton->setEnabled(false);
}

void SnapshotDialog::sizeSpinBoxValueChanged(){

  float viewRatio=((float)graphicsView->width())/((float)graphicsView->height());
  float imageRatio=((float)widthSpinBox->value())/((float)heightSpinBox->value());

  QImage image;
  if(viewRatio>imageRatio){
    image=view->createPicture((graphicsView->height()-2)*imageRatio,graphicsView->height()-2,false);

  }else{
    image=view->createPicture(graphicsView->width()-2,(graphicsView->width()-2)/imageRatio,false);
  }

  ratio=((float)widthSpinBox->value())/((float)heightSpinBox->value());

  delete pixmapItem;
  delete scene;
  scene=new QGraphicsScene();
  scene->setBackgroundBrush(QApplication::palette().color(QPalette::Midlight));
  graphicsView->setScene(scene);
  pixmapItem=scene->addPixmap(QPixmap::fromImage(image));
  pixmapItem->setPos(graphicsView->sceneRect().center()-pixmapItem->boundingRect().center());
}



}
