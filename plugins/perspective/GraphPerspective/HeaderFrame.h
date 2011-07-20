#ifndef HEADERFRAME_H
#define HEADERFRAME_H

#include <QtGui/QWidget>

namespace Ui {
class HeaderFrameData;
}

class HeaderFrame: public QWidget {
  Q_OBJECT

  Ui::HeaderFrameData *_ui;
  Q_PROPERTY(QString title READ title WRITE setTitle)
  QString _title;
public:
  explicit HeaderFrame(QWidget *parent=0);

  QString title() const;
  QStringList menus() const;

public slots:
  void setTitle(const QString &title);
  void setMenus(const QStringList &menus);

signals:
  void menuChanged(const QString &);
};

#endif // HEADERFRAME_H
