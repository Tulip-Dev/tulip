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

#ifndef TULIPITEMDELEGATE_H
#define TULIPITEMDELEGATE_H

#include <QStyledItemDelegate>

#include <tulip/TulipItemEditorCreators.h>
#include <tulip/TulipMetaTypes.h>

namespace tlp {

class TLP_QT_SCOPE TulipItemDelegate : public QStyledItemDelegate {
  Q_OBJECT

  QMap<int, TulipItemEditorCreator *> _creators;

  QObject *_currentMonitoredChild;
  QComboBox *_currentMonitoredCombo;

public:
  static QVariant showEditorDialog(tlp::ElementType, tlp::PropertyInterface *, tlp::Graph *,
                                   TulipItemDelegate *, QWidget *dialogParent = nullptr,
                                   unsigned int id = UINT_MAX);

  explicit TulipItemDelegate(QObject *parent = nullptr);
  ~TulipItemDelegate() override;

  template <typename T>
  void registerCreator(tlp::TulipItemEditorCreator *);

  void unregisterCreator(tlp::TulipItemEditorCreator *);

  template <typename T>
  void unregisterCreator();

  template <typename T>
  tlp::TulipItemEditorCreator *creator() const;

  tlp::TulipItemEditorCreator *creator(int) const;

  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                        const QModelIndex &index) const override;
  void paint(QPainter *painter, const QStyleOptionViewItem &option,
             const QModelIndex &index) const override;
  QString displayText(const QVariant &value, const QLocale &locale) const override;
  void setEditorData(QWidget *editor, const QModelIndex &index) const override;
  void setModelData(QWidget *editor, QAbstractItemModel *model,
                    const QModelIndex &index) const override;
  QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

  bool eventFilter(QObject *object, QEvent *event) override;

protected slots:
  void comboDataChanged();
};
}

#include "cxx/TulipItemDelegate.cxx"

#endif // TULIPITEMDELEGATE_H
///@endcond
