/**
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
#ifndef HEADERFRAME_H
#define HEADERFRAME_H

#include <QWidget>

namespace Ui {
class HeaderFrameData;
}

class HeaderFrame: public QWidget {
  Q_OBJECT

  QPair<int,int> _oldHeightInfos;

  Ui::HeaderFrameData *_ui;
  Q_PROPERTY(QString title READ title WRITE setTitle)
  QString _title;

  Q_PROPERTY(bool expandable READ isExpandable WRITE setExpandable)

  Q_PROPERTY(bool expanded READ isExpanded WRITE expand)
  bool _expanded;

  Q_PROPERTY(QStringList menus READ menus WRITE setMenus)

public:
  explicit HeaderFrame(QWidget *parent=0);
  virtual ~HeaderFrame();

  QString title() const;
  QStringList menus() const;
  QString currentMenu()const;
  int currentMenuIndex()const;

  bool isExpandable() const;
  bool isExpanded() const;

  QWidget *expandControl() const;
  QWidget* mainFrame() const;

public slots:
  void setTitle(const QString &title);
  void setMenus(const QStringList &menus);
  void setExpandable(bool f);

  void expand(bool e);
  void insertWidget(QWidget*);

signals:
  void menuChanged(const QString &);
  void expanded(bool);

protected slots:
  void setExpanded(bool e);
};

#endif // HEADERFRAME_H
