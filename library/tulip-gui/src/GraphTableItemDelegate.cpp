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
#include <tulip/GraphTableItemDelegate.h>

#include <tulip/Color.h>

#include "tulip/ColorButton.h"
#include "tulip/TlpQtTools.h"
#include "tulip/SizeEditor.h"
#include "tulip/ColorButton.h"
#include <tulip/CoordEditor.h>
#include <tulip/TulipFileDescriptorWidget.h>
#include <tulip/TulipMetaTypes.h>


#include <tulip/VectorEditionWidget.h>
#include <tulip/GraphTableModel.h>


#include <QtGui/QLineEdit>
#include <QtGui/QComboBox>
#include <QtGui/QPainter>
#include <QtGui/QApplication>
#include <QtGui/QTextLayout>

using namespace tlp;
using namespace std;





GraphTableItemDelegate::GraphTableItemDelegate(QObject* parent) :
  TulipItemDelegate(parent) {

}

QWidget* GraphTableItemDelegate::createEditor(QWidget* p, const QStyleOptionViewItem& option,
    const QModelIndex& index) const {
  return TulipItemDelegate::createEditor(p, option, index);
}

void GraphTableItemDelegate::setModelData(QWidget* editor, QAbstractItemModel* model,
    const QModelIndex& index) const {
  TulipItemDelegate::setModelData(editor, model, index);
}

