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

#ifndef TREEVIEWCOMBOBOX_H
#define TREEVIEWCOMBOBOX_H

#include <QComboBox>
#include <QTreeView>

#include <tulip/tulipconf.h>

class TLP_QT_SCOPE TreeViewComboBox : public QComboBox {

  Q_OBJECT
  QTreeView *_treeView;
  bool _skipNextHide;
  bool _popupVisible;
  QModelIndex _lastIndex;

public:
  explicit TreeViewComboBox(QWidget *parent = nullptr);

  void setModel(QAbstractItemModel *model);

  virtual void showPopup();
  virtual void hidePopup();

  QModelIndex selectedIndex() const;

  bool eventFilter(QObject *, QEvent *);

public slots:

  void selectIndex(const QModelIndex &);
  void rowsRemoved(const QModelIndex &, int, int);
  void currentIndexChanged();

signals:

  void currentItemChanged();
};

#endif // TREEVIEWCOMBOBOX_H
///@endcond
