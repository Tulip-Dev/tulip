#ifndef SCROLLPOPUPBUTTON_H
#define SCROLLPOPUPBUTTON_H

#include <QtGui/QPushButton>
#include <tulip/tulipconf.h>
class QSlider;

class TLP_QT_SCOPE ScrollPopupButton : public QPushButton {
  Q_OBJECT
  Q_PROPERTY(int value READ value WRITE setValue)
  Q_PROPERTY(int minimum READ minimum WRITE setMinimum)
  Q_PROPERTY(int maximum READ maximum WRITE setMaximum)

  QSlider* _slider;

public:
  explicit ScrollPopupButton(QWidget *parent = 0);
  int value() const;
  int maximum() const;
  int minimum() const;

public slots:
  virtual void showPopup();
  virtual void hidePopup();
  void setPopupVisible(bool);
  void setValue(int);
  void setMinimum(int);
  void setMaximum(int);

signals:
  void valueChanged(int);

protected:
  bool eventFilter(QObject *, QEvent *);
};

#endif // SCROLLPOPUPBUTTON_H
