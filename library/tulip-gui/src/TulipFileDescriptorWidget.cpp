#include "tulip/TulipFileDescriptorWidget.h"

#include <QtGui/QKeyEvent>
#include <QtGui/QFileDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QPushButton>
#include <QtGui/QLineEdit>
#include <QtGui/QApplication>

TulipFileDescriptorWidget::TulipFileDescriptorWidget(QWidget* parent): QWidget(parent), _data(TulipFileDescriptor()) {
  _data.absolutePath = "";
  _data.isDir = false;

  setLayout(new QHBoxLayout);
  layout()->setSpacing(0);
  layout()->setMargin(0);
  _display = new QLineEdit;
  static_cast<QLineEdit*>(_display)->setReadOnly(true);
  layout()->addWidget(_display);
  QPushButton* browseBtn = new QPushButton(trUtf8("..."));
  layout()->addWidget(browseBtn);
  connect(browseBtn,SIGNAL(clicked()),this,SLOT(browse()));
}

void TulipFileDescriptorWidget::browse() {
  QString result;
  if (_data.isDir)
    result = QFileDialog::getExistingDirectory(parentWidget(),trUtf8("Choose directory"),_data.absolutePath);
  else
    result = QFileDialog::getOpenFileName(parentWidget(),trUtf8("Choose file"),_data.absolutePath);
  if (!result.isEmpty())
    _data.absolutePath = result;
  setData(_data);
  QKeyEvent* ev1 = new QKeyEvent(QEvent::KeyPress,Qt::Key_Enter,Qt::NoModifier);
  QKeyEvent* ev2 = new QKeyEvent(QEvent::KeyRelease,Qt::Key_Enter,Qt::NoModifier);
  QApplication::sendEvent(_display,ev1);
  QApplication::sendEvent(_display,ev2);
}

void TulipFileDescriptorWidget::setData(TulipFileDescriptor desc) {
  _data = desc;
  static_cast<QLineEdit*>(_display)->setText(_data.absolutePath);
}

TulipFileDescriptor TulipFileDescriptorWidget::data() const {
  return _data;
}
