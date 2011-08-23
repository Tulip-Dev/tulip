#ifndef HEADERFRAME_H
#define HEADERFRAME_H

#include <QtGui/QWidget>

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

public:
  explicit HeaderFrame(QWidget *parent=0);
  virtual ~HeaderFrame();

  QString title() const;
  QStringList menus() const;

  bool isExpandable() const;
  bool isExpanded() const;

  QWidget *expandControl() const;

public slots:
  void setTitle(const QString &title);
  void setMenus(const QStringList &menus);
  void setExpandable(bool f);

  void expand(bool e);

signals:
  void menuChanged(const QString &);
  void expanded(bool);

protected slots:
  void setExpanded(bool e);
};

#endif // HEADERFRAME_H
