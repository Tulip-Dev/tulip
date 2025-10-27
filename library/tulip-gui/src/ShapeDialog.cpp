/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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
#include <QBuffer>
#include <QHelpEvent>
#include <QToolTip>

#include <tulip/GlyphRenderer.h>
#include <tulip/GlyphManager.h>
#include <tulip/EdgeExtremityGlyph.h>
#include <tulip/PluginLister.h>
#include <tulip/ShapeDialog.h>
#include <tulip/TlpQtTools.h>

#include "ui_ShapeDialog.h"

using namespace tlp;

ShapeDialog::ShapeDialog(bool forNodes, QWidget *parent)
    : QDialog(parent), _forNodes(forNodes), _ui(new Ui::ShapeDialog) {

  _ui->setupUi(this);
  _ui->shapeListWidget->installEventFilter(this);

  updateShapeList();
  if (!_forNodes)
    setWindowTitle("Select an edge extremity shape");
}

ShapeDialog::~ShapeDialog() {
  delete _ui;
}

void ShapeDialog::updateShapeList() {
  _ui->shapeListWidget->clear();
  if (_forNodes) {
    for (const auto &glyphName : PluginLister::availablePlugins<Glyph>()) {
      QString name = tlpStringToQString(glyphName);
      _ui->shapeListWidget->addItem(new QListWidgetItem(GlyphRenderer::render(name), name));
    }
  } else {
    _ui->shapeListWidget->addItem(new QListWidgetItem(QPixmap(), "NONE"));
    for (const auto &glyphName : PluginLister::availablePlugins<EdgeExtremityGlyph>()) {
      QString name = tlpStringToQString(glyphName);
      _ui->shapeListWidget->addItem(
          new QListWidgetItem(EdgeExtremityGlyphRenderer::render(name), name));
    }
  }

  if (_ui->shapeListWidget->count() > 0)
    _ui->shapeListWidget->setCurrentRow(0);
}

QString ShapeDialog::getSelectedShapeName() const {
  return _selectedShapeName;
}

void ShapeDialog::setSelectedShapeName(const QString &shapeName) {
  QList<QListWidgetItem *> items = _ui->shapeListWidget->findItems(shapeName, Qt::MatchExactly);

  if (!items.isEmpty()) {
    _ui->shapeListWidget->setCurrentItem(items.at(0));
    _selectedShapeName = shapeName;
  }
}

void ShapeDialog::accept() {
  if (_ui->shapeListWidget->count() > 0) {
    _selectedShapeName = _ui->shapeListWidget->currentItem()->text();
  }

  QDialog::accept();
}

void ShapeDialog::showEvent(QShowEvent *ev) {
  QDialog::showEvent(ev);

  _selectedShapeName = _ui->shapeListWidget->currentItem()->text();

  if (parentWidget())
    move(parentWidget()->window()->frameGeometry().topLeft() +
         parentWidget()->window()->rect().center() - rect().center());
}

bool ShapeDialog::eventFilter(QObject *, QEvent *event) {
  if (event->type() == QEvent::ToolTip) {
    QHelpEvent *he = static_cast<QHelpEvent *>(event);
    auto lwi = _ui->shapeListWidget->itemAt(he->pos().x(), he->pos().y());
    if (lwi) {
      // show a 48 pixel height icon
      auto qimg = _forNodes ? GlyphRenderer::render(lwi->text(), 48)
                            : EdgeExtremityGlyphRenderer::render(lwi->text(), 48);
      QByteArray bytes;
      QBuffer buf(&bytes);
      qimg.save(&buf, "png", 100);
      QString ttip;
      ttip = QString("<center><img src='data:image/png;base64, %0'/></center><br/>")
                 .arg(QString(bytes.toBase64()))
                 .append(lwi->text());
      QToolTip::showText(he->globalPos(), ttip);
      return true;
    }
  }
  return false;
}
