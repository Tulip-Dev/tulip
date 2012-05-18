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
#ifndef SIZEEDITOR_H
#define SIZEEDITOR_H

#include <QtGui/QWidget>
#include <tulip/Size.h>
#include <tulip/tulipconf.h>

namespace Ui {
class SizeEditorData;
}

namespace tlp {

class TLP_QT_SCOPE SizeEditor: public QWidget {
  Q_OBJECT
  Q_PROPERTY(Size size READ size WRITE setSize)

  Ui::SizeEditorData* _ui;

public:
  explicit SizeEditor(QWidget *parent=NULL);
  virtual ~SizeEditor();
  tlp::Size size() const;

public slots:
  void setSize(const tlp::Size& s);
};

}

#endif // SIZEEDITOR_H
