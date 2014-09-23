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

#ifndef TULIPTABLEWIDGETITEMDELEGATE_H_
#define TULIPTABLEWIDGETITEMDELEGATE_H_

#include <tulip/tulipconf.h>
#include <tulip/TulipItemDelegate.h>

class QStyleOptionViewItem;

namespace tlp {

/**
* @brief QItemDelegate to display and edit data from a GraphTableModel in the Qt model/view architecture.
**/
class TLP_QT_SCOPE GraphTableItemDelegate: public TulipItemDelegate {
  Q_OBJECT
public:
  GraphTableItemDelegate(QObject* parent=NULL) ;
  void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

};
}
#endif /* TULIPTABLEWIDGETITEMDELEGATE_H_ */
///@endcond
