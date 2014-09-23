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


#ifndef TREEVIEWCOMBOBOX_H
#define TREEVIEWCOMBOBOX_H

#include <QComboBox>
#include <QTreeView>
#include <QMap>
#include <QPair>

#include <tulip/tulipconf.h>

class QTimer;

class TLP_QT_SCOPE DeferredUpdateTreeView : public QTreeView {

  Q_OBJECT

public:

  DeferredUpdateTreeView(QWidget *parent = 0);

  ~DeferredUpdateTreeView();

protected slots:

  void dataChanged(const QModelIndex & topLeft, const QModelIndex & bottomRight);

  void callDataChanged();

  void rowsAboutToBeRemoved(const QModelIndex & parent, int start, int end);

  void rowsInserted(const QModelIndex & parent, int start, int end);

private:

  void cancelUpdates(const QModelIndex &parent);

  QMap<QPair<QModelIndex, QModelIndex>, QTimer *> _updateTimers;

};

class TLP_QT_SCOPE TreeViewComboBox : public QComboBox {

  Q_OBJECT
  QTreeView* _treeView;
  bool _skipNextHide;
  bool _popupVisible;
  QModelIndex _lastIndex;

public:
  explicit TreeViewComboBox(QWidget *parent = NULL);

  void setModel(QAbstractItemModel * model);

  virtual void showPopup();
  virtual void hidePopup();

  QModelIndex selectedIndex() const;

  bool eventFilter(QObject*, QEvent*);

public slots:

  void selectIndex(const QModelIndex&);
  void rowsRemoved(const QModelIndex&, int, int);
  void currentIndexChanged();

signals:

  void currentItemChanged();

};

#endif // TREEVIEWCOMBOBOX_H
///@endcond
