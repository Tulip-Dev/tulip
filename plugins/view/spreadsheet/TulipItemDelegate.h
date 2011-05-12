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

#ifndef TULIPTABLEWIDGETITEMDELEGATE_H_
#define TULIPTABLEWIDGETITEMDELEGATE_H_
#include <QtGui/QStyledItemDelegate>
#include <tulip/tulipconf.h>

namespace tlp{
    /**
    * @brief QItemDelegate to display and edit Tulip data in the Qt model/view architecture.
    **/
    class TulipItemDelegate: public QStyledItemDelegate {
        Q_OBJECT
    public:
        TulipItemDelegate(QObject* parent=NULL) :
                QStyledItemDelegate(parent) {
        }
        virtual ~TulipItemDelegate() {
        }
        QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem & option,
                              const QModelIndex& index) const;
        void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;

    protected:
        QWidget* createFileNameEditor(QWidget* parent , const QString& defaultFileName,const QString& filenameFilter=QString()) const;
    };
}

#endif /* TULIPTABLEWIDGETITEMDELEGATE_H_ */
