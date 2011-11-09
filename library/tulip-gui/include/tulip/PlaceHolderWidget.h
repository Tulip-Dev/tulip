#ifndef PLACEHOLDERWIDGET_H
#define PLACEHOLDERWIDGET_H

#include <QtGui/QWidget>

class PlaceHolderWidget: public QWidget {
  QWidget* _widget;

public:
  explicit PlaceHolderWidget(QWidget *parent = 0);
  QWidget* widget() const;
  QWidget* takeWidget();
  void setWidget(QWidget* widget);
};

#endif // PLACEHOLDERWIDGET_H
