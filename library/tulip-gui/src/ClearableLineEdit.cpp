#include "tulip/ClearableLineEdit.h"
#include <QtGui/QPaintEvent>
#include <QtGui/QPainter>

QPixmap* ClearableLineEdit::CLEAR_PIXMAP = NULL;

void ClearableLineEdit::initPixmap() {
  if (CLEAR_PIXMAP == NULL) {
    CLEAR_PIXMAP = new QPixmap(":/tulip/gui/ui/clearbutton.png");
  }
}

ClearableLineEdit::ClearableLineEdit(QWidget *parent): QLineEdit(parent), _clearButtonHovered(false) {
  setMouseTracking(true);
}

void ClearableLineEdit::paintEvent(QPaintEvent* ev) {
  QLineEdit::paintEvent(ev);
  QPainter p(this);
  p.setOpacity(_clearButtonHovered ? 1 : 0.7);
  initPixmap();
  p.drawPixmap(pixmapRect(),*CLEAR_PIXMAP);
}

QRect ClearableLineEdit::pixmapRect() {
  initPixmap();
  QRect pixmapRect(width() - CLEAR_PIXMAP->width() - 5, height()/2-CLEAR_PIXMAP->height()/2,CLEAR_PIXMAP->width(),CLEAR_PIXMAP->height());
  return pixmapRect;
}

void ClearableLineEdit::mouseMoveEvent(QMouseEvent* ev) {
  QLineEdit::mouseMoveEvent(ev);
  bool oldValue = _clearButtonHovered;
  _clearButtonHovered = pixmapRect().contains(ev->pos());

  if (oldValue != _clearButtonHovered)
    repaint();
}

void ClearableLineEdit::mousePressEvent(QMouseEvent* ev) {
  QLineEdit::mousePressEvent(ev);

  if (pixmapRect().contains(ev->pos())) {
    clear();
    emit textEdited("");
    emit editingFinished();
  }
}


