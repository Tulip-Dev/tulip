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

// original code is written in Python and is taken from
// https://jdreaver.com/posts/2014-07-28-scientific-notation-spin-box-pyside.html
// see also https://gist.github.com/jdreaver/0be2e44981159d0854f5

#ifndef SCIENTIFICDOUBLESPINBOX_H
#define SCIENTIFICDOUBLESPINBOX_H

#include <tulip/tulipconf.h>

#include <QDoubleSpinBox>
#include <QValidator>

namespace tlp {

/**
 * @brief Utility class used to validate that a QString contains a valid representation of a
 * floating point number
 * including scientific notation
 *
 * @since Tulip 5.0
 */
class TLP_QT_SCOPE FloatValidator : public QValidator {

public:
  State validate(QString &input, int &pos) const;

  void fixup(QString &text) const;
};

/**
 * @brief Utility class implementing a QDoubleSpinBox supporting scientific notation
 *
 * @since Tulip 5.0
 */
class TLP_QT_SCOPE ScientificDoubleSpinBox : public QDoubleSpinBox {

public:
  ScientificDoubleSpinBox(QWidget *parent = nullptr);

  QValidator::State validate(QString &input, int &pos) const;

  void fixup(QString &input) const;

  double valueFromText(const QString &text) const;

  QString textFromValue(double value) const;

  void stepBy(int steps);
};
}

#endif // SCIENTIFICDOUBLESPINBOX_H
