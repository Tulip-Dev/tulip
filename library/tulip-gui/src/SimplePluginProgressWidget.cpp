#include "tulip/SimplePluginProgressWidget.h"
#include <QtGui/QStylePainter>
#include <QtCore/QDebug>

#include "ui_SimplePluginProgressWidget.h"

CircleProgressBar::CircleProgressBar(QWidget *parent)
  : QWidget(parent), _value(0), _max(100) {
  qWarning("build");
}

void CircleProgressBar::setValue(int value, int max) {
  _value= value;
  _max = max;
  repaint();
}

void CircleProgressBar::paintEvent(QPaintEvent *e) {
  QWidget::paintEvent(e);
  QStylePainter p(this);
  p.setPen(Qt::transparent);
  p.setRenderHints(QPainter::HighQualityAntialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing | QPainter::NonCosmeticDefaultPen | QPainter::Antialiasing);

  p.setBrush(Qt::red);
  p.drawPie(rect().x(),rect().y(), rect().width()-10,rect().height()-10,0,360*16);
}

namespace tlp {

SimplePluginProgressWidget::SimplePluginProgressWidget(QWidget *parent, Qt::WindowFlags f)
  :QWidget(parent,f), _ui(new Ui::SimplePluginProgressWidgetData) {
  _ui->setupUi(this);
}

void SimplePluginProgressWidget::progress_handler(int step, int max_step) {

}

void SimplePluginProgressWidget::preview_handler(bool p) {

}

}
