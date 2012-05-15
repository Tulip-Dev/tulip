#ifndef ORIENTEDPUSHNBUTTON_H
#define ORIENTEDPUSHNBUTTON_H

#include <QPushButton>
#include <QStyleOptionButton>

class OrientedPushButton : public QPushButton {
public:
  OrientedPushButton(QWidget* parent = NULL);
  OrientedPushButton(const QString& text, QWidget* parent = NULL);
  OrientedPushButton(const QIcon& icon, const QString& text, QWidget* parent = NULL);
  ~OrientedPushButton();

  Qt::Orientation orientation() const;
  void setOrientation(Qt::Orientation orientation);

  bool mirrored() const;
  void setMirrored(bool mirrored);

  QSize sizeHint() const;
  QSize minimumSizeHint() const;

protected:
  void paintEvent(QPaintEvent* event);

private:
  QStyleOptionButton getStyleOption() const;
  void init();

  Qt::Orientation _orientation;
  bool _mirrored;
};

#endif // ORIENTEDPUSHNBUTTON_H
