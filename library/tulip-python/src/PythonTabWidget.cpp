#include <QtGui/QPainter>
#include <QtGui/QTabBar>
#include <QtGui/QLinearGradient>

#include "tulip/PythonTabWidget.h"
#include "tulip/PythonInterpreter.h"

using namespace tlp;

PythonTabWidget::PythonTabWidget(QWidget *parent) : QTabWidget(parent), _drawGradient(false), _textColor(QColor(100, 100, 100)) {}

void PythonTabWidget::paintEvent(QPaintEvent * event) {
  QTabWidget::paintEvent(event);
  static QImage pythonLogoImg(":/tulip/python/python-logo-small.png");
  static QString pythonVersion(PythonInterpreter::getInstance()->getPythonVersionStr());
  QPainter painter(this);
  painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);

  if (_drawGradient) {
    QLinearGradient gradient(tabBar()->pos().x()+width()/2,tabBar()->pos().y(),tabBar()->pos().x()+width()/2,tabBar()->pos().y()+tabBar()->height());
    gradient.setColorAt(0.0, "#838383");
    gradient.setColorAt(0.4, "#707070");
    gradient.setColorAt(0.401, "#636363");
    gradient.setColorAt(1.0, "#4a4a4a");
    painter.fillRect(tabBar()->pos().x(), tabBar()->pos().y(), width(), tabBar()->height(), gradient);
  }

  painter.setPen(_textColor);
  painter.setFont(QFont("Arial", static_cast<int>(12 * tabBar()->height() / 27.0), QFont::DemiBold));
  int firstLabelWidth = static_cast<int>(120 * tabBar()->height() / 27.0);
  int imageWidth = static_cast<int>(30 * tabBar()->height() / 27.0);
  int secondLabelWidth = static_cast<int>(120 * tabBar()->height() / 27.0);
  QRectF rect(tabBar()->pos().x()+width()-(firstLabelWidth+imageWidth+secondLabelWidth), tabBar()->pos().y(), firstLabelWidth, tabBar()->height());
  QRectF rect2(tabBar()->pos().x()+width()-(imageWidth+secondLabelWidth), tabBar()->pos().y(), imageWidth, tabBar()->height());
  QRectF rect3(tabBar()->pos().x()+width()-secondLabelWidth, tabBar()->pos().y(), secondLabelWidth, tabBar()->height());
  painter.drawText(rect,  Qt::AlignCenter, "Powered by ");
  painter.drawImage(rect2, pythonLogoImg);
  painter.drawText(rect3,  Qt::AlignCenter, QString("Python ") + pythonVersion);
}
