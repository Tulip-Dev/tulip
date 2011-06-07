#include "tulip/SimplePluginProgressWidget.h"
#include <QtGui/QStylePainter>
#include <QtCore/QDebug>

#include "ui_SimplePluginProgressWidget.h"

CircleProgressBar::CircleProgressBar(QWidget *parent)
  : QWidget(parent), _value(0), _max(100) {
}

void CircleProgressBar::setValue(int value, int max) {
  _value= value;
  _max = max;
  repaint();
}

void CircleProgressBar::paintEvent(QPaintEvent *e) {
  QWidget::paintEvent(e);
  QStylePainter p(this);
  p.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing | QPainter::Antialiasing);

  QPen pen(QApplication::palette().color(QPalette::Mid));
  pen.setWidth(5);
  p.setPen(pen);
  p.setBrush(Qt::transparent);
  QRect outRect(rect().x() + 5, rect().y() + 5, rect().width() - 10, rect().height() - 10);
  int angle = _value * 5840 / _max;
  p.drawPie(outRect, 0, angle);

  p.setBrush(QApplication::palette().color(QPalette::Window));
  p.setPen(QApplication::palette().color(QPalette::Window));
  p.drawEllipse(outRect.x()+3,outRect.y()+3,outRect.width()-6,outRect.height()-6);

  p.setBrush(QApplication::palette().text());
  p.setPen(QApplication::palette().color(QPalette::Text));
  p.drawText(outRect,Qt::AlignCenter | Qt::AlignVCenter, QString::number(_value * 100 / _max) + "%");
}

int CircleProgressBar::heightForWidth(int w) const  {
  return w;
}
namespace tlp {

SimplePluginProgressWidget::SimplePluginProgressWidget(QWidget *parent, Qt::WindowFlags f)
  :QWidget(parent,f), _ui(new Ui::SimplePluginProgressWidgetData) {
  _ui->setupUi(this);
}

void SimplePluginProgressWidget::progress_handler(int step, int max_step) {
  _ui->circleProgress->setValue(step,max_step);
}

void SimplePluginProgressWidget::preview_handler(bool p) {

}

void SimplePluginProgressWidget::setComment(std::string s) {
  _ui->commentLabel->setText("<html><head/><body><b>" + QString(s.c_str()) + "</b></body></html>");
}

}
