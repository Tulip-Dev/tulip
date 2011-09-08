#ifndef COLORBUTTON_H
#define COLORBUTTON_H

#include <QtGui/QPushButton>
#include <tulip/tulipconf.h>
#include <tulip/Color.h>
#include <tulip/TulipMetaTypes.h>

namespace tlp {

class TLP_QT_SCOPE ColorButton : public QPushButton {
  Q_OBJECT
  Q_PROPERTY(QColor color READ color WRITE setColor)
  Q_PROPERTY(Color tulipColor READ tulipColor WRITE setTulipColor)

  QColor _color;
public:
  explicit ColorButton(QWidget *parent = 0);

  QColor color() const;
  Color tulipColor() const;

 protected:
  void paintEvent(QPaintEvent *);

signals:
  void colorChanged(QColor);
  void tulipColorChanged(Color);

public slots:
  void setColor(const QColor&);
  void setTulipColor(const Color&);

protected slots:
  void chooseColor();
};

}

#endif // COLORBUTTON_H
