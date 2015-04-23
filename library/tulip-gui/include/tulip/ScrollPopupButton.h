/*
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
///@cond DOXYGEN_HIDDEN


#ifndef SCROLLPOPUPBUTTON_H
#define SCROLLPOPUPBUTTON_H

#include <QPushButton>


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
///@endcond
