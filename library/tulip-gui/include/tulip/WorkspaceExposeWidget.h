#ifndef WORKSPACEEXPOSEWIDGET_H
#define WORKSPACEEXPOSEWIDGET_H

#include <QtGui/QGraphicsView>
#include <QtCore/QMap>
#include <QtGui/QPixmap>
#include <tulip/tulipconf.h>

namespace tlp {

class WorkspacePanel;

class TLP_QT_SCOPE WorkspaceExposeWidget : public QGraphicsView {
  Q_OBJECT

  QVector<WorkspacePanel*> _panels;
  QMap<WorkspacePanel*,QPixmap> _previewPixmaps;
  QMap<WorkspacePanel*,QGraphicsItem*> _previewItems;
  int _currentPanelIndex;

public:
  explicit WorkspaceExposeWidget(QWidget *parent = 0);
  int currentPanelIndex() const;
  QVector<WorkspacePanel*> panels() const;

public slots:
  void setData(const QVector<WorkspacePanel*>& panels, const QMap<WorkspacePanel*,QPixmap>& previews, int currentPanelIndex);

protected:
  void resizeEvent(QResizeEvent *event);

};

}

#endif // WORKSPACEEXPOSEWIDGET_H

