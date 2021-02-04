/*
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
///@cond DOXYGEN_HIDDEN

#ifndef COLORBUTTON_H
#define COLORBUTTON_H

#include <QPushButton>

#include <tulip/tulipconf.h>
#include <tulip/Color.h>
#include <tulip/TulipMetaTypes.h>

namespace tlp {

class TLP_QT_SCOPE ChooseColorButton : public QPushButton {
  Q_OBJECT
  Q_PROPERTY(QColor color READ color WRITE setColor)
  Q_PROPERTY(Color tulipColor READ tulipColor WRITE setTulipColor)

public:
  explicit ChooseColorButton(QWidget *parent = nullptr);

  QColor color() const;
  QString text() const;
  Color tulipColor() const;

protected:
  QColor _color;
  QString _text;
  QString _dialogTitle;

signals:
  void colorChanged(QColor);
  void tulipColorChanged(Color);

public slots:
  void setColor(const QColor &);
  void setText(const QString &text);
  void setTulipColor(const Color &);
  void setDialogTitle(const QString &);

protected slots:
  void chooseColor();
};

class TLP_QT_SCOPE ColorButton : public ChooseColorButton {
  Q_OBJECT

public:
  explicit ColorButton(QWidget *parent = nullptr) : ChooseColorButton(parent) {}

protected:
  void paintEvent(QPaintEvent *) override;
};
} // namespace tlp

#endif // COLORBUTTON_H
///@endcond
