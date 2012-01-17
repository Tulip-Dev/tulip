#ifndef WORKSPACEEXPOSEWIDGET_H
#define WORKSPACEEXPOSEWIDGET_H

#include <QtGui/QGraphicsView>
#include <QtCore/QMap>
#include <QtGui/QPixmap>
#include <tulip/tulipconf.h>

class PreviewItem;

namespace tlp {
class WorkspacePanel;

class TLP_QT_SCOPE WorkspaceExposeWidget : public QGraphicsView {
  Q_OBJECT

  QVector<PreviewItem*> _items;
  void updatePositions();

public:
  static QSize previewSize();

  explicit WorkspaceExposeWidget(QWidget *parent = 0);
  int currentPanelIndex() const;
  QVector<WorkspacePanel*> panels() const;

public slots:
  void setData(const QVector<WorkspacePanel*>& panels, int currentPanelIndex);

};

}

#endif // WORKSPACEEXPOSEWIDGET_H

