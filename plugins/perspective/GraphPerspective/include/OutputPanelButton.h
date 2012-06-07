#ifndef OUTPUTPANELBUTTON_H
#define OUTPUTPANELBUTTON_H

#include <QtGui/QToolButton>

class OutputPanelButton : public QToolButton {
  Q_OBJECT

  Q_PROPERTY(int number READ number WRITE setNumber)
  int _number;

  Q_PROPERTY(QString title READ title WRITE setTitle)
  QString _title;

public:
  explicit OutputPanelButton(QWidget *parent = 0);
  int number() const { return _number; }
  QString title() const { return _title; }

  QSize sizeHint() const;
  
protected:
  void paintEvent(QPaintEvent *);

public slots:
  void setNumber(int number) {
    _number = number;
  }
  void setTitle(QString title) {
    _title = title;
  }
  
};

#endif // OUTPUTPANELBUTTON_H
