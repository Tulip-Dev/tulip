#ifndef WORKSPACEEXPOSEWIDGET_H
#define WORKSPACEEXPOSEWIDGET_H

#include <QtGui/QGraphicsView>
#include <QtCore/QMap>
#include <QtGui/QPixmap>
#include <tulip/tulipconf.h>

class PreviewItem;
class QAbstractAnimation;

namespace tlp {
class WorkspacePanel;

class TLP_QT_SCOPE WorkspaceExposeWidget : public QGraphicsView {
  Q_OBJECT

  QAbstractAnimation* _positionAnimation;
  QList<PreviewItem*> _items;
  PreviewItem* _selectedItem;
  QGraphicsRectItem* _placeholderItem;
  void updatePositions(bool resetScenePos=true);

  static const int MARGIN;

public:
  static QSize previewSize();

  explicit WorkspaceExposeWidget(QWidget *parent = 0);
  int currentPanelIndex() const;
  QVector<WorkspacePanel*> panels() const;

public slots:
  void setData(const QVector<WorkspacePanel*>& panels, int currentPanelIndex);

protected:
  void resizeEvent(QResizeEvent *event);
  bool eventFilter(QObject *, QEvent *);

protected slots:
  void updatePositionsAnimationFinished();
  void resetSceneRect();

};

}

#endif // WORKSPACEEXPOSEWIDGET_H

