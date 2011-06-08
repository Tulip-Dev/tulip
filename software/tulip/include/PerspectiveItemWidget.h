#ifndef PERSPECTIVEITEMWIDGET_H
#define PERSPECTIVEITEMWIDGET_H

#include <QtGui/QWidget>

namespace tlp {
class PluginInformations;
}

namespace Ui {
class PerspectiveItemData;
}

class PerspectiveItemWidget : public QWidget {
  Q_OBJECT

  QString _perspectiveId;
  Ui::PerspectiveItemData *_ui;
public:
  explicit PerspectiveItemWidget(tlp::PluginInformations *infos,QWidget *parent = 0);
  QString perspectiveId() const { return _perspectiveId; }

signals:
  void selected();

protected:
  void mouseDoubleClickEvent(QMouseEvent *);
  void focusInEvent(QFocusEvent *);
  void focusOutEvent(QFocusEvent *);
};

#endif // PERSPECTIVEITEMWIDGET_H
