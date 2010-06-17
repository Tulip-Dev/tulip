/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
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
// +-------------------------------------------------------------------------+
// | Tulip Software                                                          |
// | Author:  Morgan Mathiaut                                                |
// +-------------------------------------------------------------------------+
// | License:                                                                |
// |                                                                         |
// | Tulip is free software; you can redistribute it and/or modify           |
// | it under the terms of the GNU General Public License as published by    |
// | the Free Software Foundation; either version 2 of the License, or       |
// | (at your option) any later version.                                     |
// |                                                                         |
// | Tulip is distributed in the hope that it will be useful,                |
// | but WITHOUT ANY WARRANTY; without even the implied warranty of          |
// | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the            |
// | GNU General Public License for more details.                            |
// |                                                                         |
// | You should have received a copy of the GNU General Public License       |
// | along with QSimpleSheet; if not, write to the Free Software             |
// | Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA|
// |                                                                         |
// +-------------------------------------------------------------------------+

// Based on

// +-------------------------------------------------------------------------+
// | QSimpleSheet simple spreadsheet                                         |
// | Copyright (C) 2008 Shahab <shahab.sh.70@gmail.com>                      |
// +-------------------------------------------------------------------------+
// | License:                                                                |
// |                                                                         |
// | QSimpleSheet is free software; you can redistribute it and/or modify    |
// | it under the terms of the GNU General Public License as published by    |
// | the Free Software Foundation; either version 2 of the License, or       |
// | (at your option) any later version.                                     |
// |                                                                         |
// | QSimpleSheet is distributed in the hope that it will be useful,         |
// | but WITHOUT ANY WARRANTY; without even the implied warranty of          |
// | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the            |
// | GNU General Public License for more details.                            |
// |                                                                         |
// | You should have received a copy of the GNU General Public License       |
// | along with QSimpleSheet; if not, write to the Free Software             |
// | Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA|
// |                                                                         |
// +-------------------------------------------------------------------------+
// | Author:  Shahab Shirazi <shahab.sh.70@gmail.com>                        |
// +-------------------------------------------------------------------------+

#ifndef Tulip_SPREADITEMDELEGATE_H
#define Tulip_SPREADITEMDELEGATE_H

#include <QtGui/QItemDelegate>

class QWidget;
class QStyleOptionViewItem;
class QModelIndex;
class QAbstractItemModel;

#include <tulip/tulipconf.h>

namespace tlp {

  class SpreadItemDelegate : public QItemDelegate
  {
    Q_OBJECT
   
      public:
    SpreadItemDelegate(QAbstractItemDelegate*, QObject* = 0);
    virtual ~SpreadItemDelegate() {}
    QWidget* createEditor(QWidget*, const QStyleOptionViewItem&, const QModelIndex&) const;
    void setEditorData(QWidget*, const QModelIndex&) const;
    void setModelData(QWidget*, QAbstractItemModel*, const QModelIndex&) const;
      
  signals:
    void dataChangedSignal(int, int, QString&, QString&) const;
      
  private:
    QAbstractItemDelegate *lastItemDelegate;
  };

}

#endif
