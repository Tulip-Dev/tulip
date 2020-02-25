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
#include <QGraphicsItem>
#include <QClipboard>
#include <QGraphicsScene>
#include <QPixmap>
#include <QPushButton>

#include <tulip/View.h>
#include <tulip/GlOffscreenRenderer.h>

using namespace std;

namespace tlp {

class LockLabel : public QLabel {
public:
  LockLabel() : QLabel(), locked(true), alwaysLocked(false) {
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

protected:
  bool eventFilter(QObject *, QEvent *event) override {
    if (event->type() == QEvent::MouseButtonRelease && !alwaysLocked) {
      if (locked) {
        setPixmap(QPixmap(":/tulip/gui/icons/i_unlocked.png"));
        locked = false;
      } else {
        setPixmap(QPixmap(":/tulip/gui/icons/i_locked.png"));
        locked = true;
      }

      return true;
    }

    return false;
  }

  bool locked;
  bool alwaysLocked;
};

SnapshotDialog::SnapshotDialog(const View *v, QWidget *parent)
    : QDialog(parent), ui(new Ui::SnapshotDialogData()), view(v), ratio(-1),
      inSizeSpinBoxValueChanged(false) {
  ui->setupUi(this);

  GlOffscreenRenderer::getInstance()->makeOpenGLContextCurrent();
  int maxTextureSize = 0;
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
  GlOffscreenRenderer::getInstance()->doneOpenGLContextCurrent();

  // restrict snapshot width and height to the half of the GL_MAX_TEXTURE_SIZE value
  ui->widthSpinBox->setMaximum(maxTextureSize / 2);
  ui->heightSpinBox->setMaximum(maxTextureSize / 2);

  ui->widthSpinBox->setValue(view->centralItem()->scene()->sceneRect().width());
  ui->heightSpinBox->setValue(view->centralItem()->scene()->sceneRect().height());

  connect(ui->widthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(widthSpinBoxValueChanged(int)));
  connect(ui->heightSpinBox, SIGNAL(valueChanged(int)), this, SLOT(heightSpinBoxValueChanged(int)));

  QPushButton *copyButton = ui->buttonBox->button(QDialogButtonBox::Apply);
  copyButton->setText("&Copy to clipboard");
  ui->buttonBox->addButton(copyButton, QDialogButtonBox::ActionRole);
  connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton *)), this,
          SLOT(clicked(QAbstractButton *)));

  lockLabel = new LockLabel();
  ui->horizontalLayout_5->insertWidget(2, lockLabel);
  ui->horizontalLayout_5->setAlignment(lockLabel, Qt::AlignLeft | Qt::AlignVCenter);
}

void SnapshotDialog::clicked(QAbstractButton *b) {
  if (ui->buttonBox->buttonRole(b) == QDialogButtonBox::ResetRole) {
    ui->widthSpinBox->setValue(view->centralItem()->scene()->sceneRect().width());
    ui->heightSpinBox->setValue(view->centralItem()->scene()->sceneRect().height());
    ui->qualitySpinBox->setValue(100);
  }

  if (ui->buttonBox->buttonRole(b) == QDialogButtonBox::ActionRole) {
    QPixmap pixmap = view->snapshot(QSize(ui->widthSpinBox->value(), ui->heightSpinBox->value()));
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setPixmap(pixmap);
  }
}

SnapshotDialog::~SnapshotDialog() {
  delete ui;
}

void SnapshotDialog::resizeEvent(QResizeEvent *) {
  sizeSpinBoxValueChanged();
}

static const QString default_filter("png");

void SnapshotDialog::accept() {
  QString formatedFormatList;

  // Put the default save format as the first choice (selectedFilter not supported under MacOSX and
  // some Linux window managers)
  for (QString ext : QImageWriter::supportedImageFormats()) {
    ext = ext.toLower();

    if ((formatedFormatList.indexOf(ext) == -1) && (ext != default_filter)) {
      formatedFormatList += ext + " (*." + ext + ");;";
    }
  }

  QString selectedFilter(default_filter + " (*." + default_filter + ")");
  formatedFormatList = selectedFilter + ";;" + formatedFormatList;
  // remove last ;;
  formatedFormatList.resize(formatedFormatList.size() - 2);

  QString fileName = QFileDialog::getSaveFileName(this, tr("Save image as..."), QString(),
                                                  formatedFormatList, &selectedFilter
// on MacOSX selectedFilter is ignored by the
// native dialog
#ifdef __APPLE__
                                                  ,
                                                  QFileDialog::DontUseNativeDialog
#endif
  );

  if (fileName.isEmpty())
    return;

  // force file extension
  QString selectedExtension = QString('.') + selectedFilter.section(' ', 0, 0);

  if (!fileName.endsWith(selectedExtension))
    fileName += selectedExtension;

  this->setEnabled(false);

  QPixmap pixmap = view->snapshot(QSize(ui->widthSpinBox->value(), ui->heightSpinBox->value()));

  if (!pixmap.save(fileName, nullptr, ui->qualitySpinBox->value())) {
    QMessageBox::critical(this, "Snapshot cannot be saved",
                          "Snapshot cannot be saved in file: " + fileName);
    this->setEnabled(true);
  } else {
    QDialog::accept();
  }
}

void SnapshotDialog::widthSpinBoxValueChanged(int value) {
  if (inSizeSpinBoxValueChanged)
    return;

  inSizeSpinBoxValueChanged = true;

  if (lockLabel->isLocked()) {
    ui->heightSpinBox->setValue(value / ratio);
  } else {
    sizeSpinBoxValueChanged();
  }

  inSizeSpinBoxValueChanged = false;
}

void SnapshotDialog::heightSpinBoxValueChanged(int value) {
  if (inSizeSpinBoxValueChanged)
    return;

  inSizeSpinBoxValueChanged = true;

  if (lockLabel->isLocked()) {
    ui->widthSpinBox->setValue(value * ratio);
  } else {
    sizeSpinBoxValueChanged();
  }

  inSizeSpinBoxValueChanged = false;
}

void SnapshotDialog::sizeSpinBoxValueChanged() {

  if (ui->widthSpinBox->value() < 10 || ui->heightSpinBox->value() < 10) {
    return;
  }

  float imageRatio = float(ui->widthSpinBox->value()) / float(ui->heightSpinBox->value());

  if (imageRatio != ratio) {
    // regenerate preview pixmap only if the aspect ratio changed
    QPixmap pixmap;

    pixmap =
        view->snapshot(QSize((view->centralItem()->scene()->sceneRect().height() - 2) * imageRatio,
                             view->centralItem()->scene()->sceneRect().height() - 2));
    ratio = float(ui->widthSpinBox->value()) / float(ui->heightSpinBox->value());
    ui->snapshotLabel->setPixmap(pixmap);
  }
  // resize snapshotLabel
  QSize sSize = ui->snapshotWidget->size();
  sSize -= QSize(2, 2);
  QSize psize = ui->snapshotLabel->pixmap()->size();
  psize.scale(sSize, Qt::KeepAspectRatio);
  ui->snapshotLabel->resize(psize);
  sSize -= psize;
  ui->snapshotLabel->move(sSize.width() / 2, sSize.height() / 2);
}

void SnapshotDialog::setSnapshotHasViewSizeRatio(bool snapshotHasViewSizeRatio) {
  lockLabel->setAlwaysLocked(snapshotHasViewSizeRatio);
}
} // namespace tlp
