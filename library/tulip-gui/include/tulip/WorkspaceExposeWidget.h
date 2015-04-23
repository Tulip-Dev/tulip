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


#ifndef WORKSPACEEXPOSEWIDGET_H
#define WORKSPACEEXPOSEWIDGET_H

#include <QGraphicsObject>
#include <QGraphicsView>

#include <tulip/tulipconf.h>

class QAbstractAnimation;

namespace tlp {
class WorkspacePanel;

class PreviewItem: public QGraphicsObject {
  Q_OBJECT

  static QPixmap* _closeButtonPixmap;
  static QRect _closePixmapRect;

  QPixmap _pixmap;
  WorkspacePanel* _panel;
  bool _hovered;
  bool _closeButtonHovered;

  int textHeight() const;
public:
  explicit PreviewItem(const QPixmap& pixmap, WorkspacePanel* panel, QGraphicsItem* parent = NULL);
  QRectF boundingRect() const;
  void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
  WorkspacePanel* panel() const;
  bool shouldClose(const QPointF&);
protected:
  void hoverEnterEvent(QGraphicsSceneHoverEvent*);
  void hoverLeaveEvent(QGraphicsSceneHoverEvent*);
  void hoverMoveEvent(QGraphicsSceneHoverEvent*);
  void mouseDoubleClickEvent(QGraphicsSceneMouseEvent*);
signals:
  void opened();
};


class TLP_QT_SCOPE WorkspaceExposeWidget : public QGraphicsView {
  Q_OBJECT

  QAbstractAnimation* _positionAnimation;
  QList<PreviewItem*> _items;
  PreviewItem* _selectedItem;
  QGraphicsRectItem* _placeholderItem;
  bool _switchToSingleMode;

  int _currentPanelIndex;

  void updatePositions(bool resetScenePos=true);
  static const int MARGIN;

  void finish();

public:
  static QSize previewSize();

  explicit WorkspaceExposeWidget(QWidget *parent = NULL);
  int currentPanelIndex() const;
  QVector<WorkspacePanel*> panels() const;

  bool isSwitchToSingleMode() const;

public slots:
  void setData(const QVector<WorkspacePanel*>& panels, int currentPanelIndex);

signals:
  void exposeFinished();

protected:
  void resizeEvent(QResizeEvent *event);
  bool eventFilter(QObject *, QEvent *);
  bool event(QEvent*);

protected slots:
  void updatePositionsAnimationFinished();
  void resetSceneRect();
  void itemOpened();
};

}

#endif // WORKSPACEEXPOSEWIDGET_H

///@endcond
