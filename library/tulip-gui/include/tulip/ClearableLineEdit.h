#ifndef CLEARABLELINEEDIT_H
#define CLEARABLELINEEDIT_H

#include <tulip/tulipconf.h>
#include <QtGui/QLineEdit>

class TLP_QT_SCOPE ClearableLineEdit : public QLineEdit {
  static QPixmap* CLEAR_PIXMAP;

  bool _clearButtonHovered;
  static void initPixmap();
  QRect pixmapRect();
public:
  explicit ClearableLineEdit(QWidget *parent = NULL);

protected:
  void paintEvent(QPaintEvent *);
  void mouseMoveEvent(QMouseEvent *);
  void mousePressEvent(QMouseEvent *);

};


#endif // CLEARABLELINEEDIT_H
