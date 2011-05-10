/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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
#ifndef COLORBUTTON_H
#define COLORBUTTON_H

#include <QtGui/QPushButton>
#include <tulip/tulipconf.h>
/**
  * @brief The ColorButton widget provides a push button to display and allow user selection of a color.
  *
  * Display a QColorDialog when clicking on it to let user select a new color.
  **/

namespace tlp{
    class TLP_QT_SCOPE ColorButton: public QPushButton {
        Q_OBJECT
        Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    public:
                ColorButton(QWidget *parent=0);
        ~ColorButton();
        QColor color() const;

    public slots:
        void setColor(const QColor &);

    protected slots:
        void askColor();

    signals:
        void colorChanged(const QColor &);

    protected:
        void paintEvent(QPaintEvent* event);

    private:
        QColor _color;
    };
}

#endif // COLORBUTTON_H
