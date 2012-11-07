#include <QtGui/QPainter>
#include <QtGui/QTabBar>

#include "tulip/PythonTabWidget.h"
#include "tulip/PythonInterpreter.h"

using namespace tlp;

PythonTabWidget::PythonTabWidget(QWidget *parent) : QTabWidget(parent) {}

void PythonTabWidget::paintEvent(QPaintEvent * event) {
    QTabWidget::paintEvent(event);
    static QImage pythonLogoImg(":/tulip/python/python-logo-small.png");
    static QString pythonVersion(PythonInterpreter::getInstance()->getPythonVersionStr());
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
    painter.setPen(QColor(100, 100, 100));
    painter.setFont(QFont("Arial", static_cast<int>(12 * tabBar()->height() / 27.0), QFont::DemiBold));
    int firstLabelWidth = static_cast<int>(120 * tabBar()->height() / 27.0);
    int imageWidth = static_cast<int>(110 * tabBar()->height() / 27.0);
    int secondLabelWidth = static_cast<int>(40 * tabBar()->height() / 27.0);
    QRectF rect(tabBar()->pos().x()+width()-(firstLabelWidth+imageWidth+secondLabelWidth), tabBar()->pos().y(), firstLabelWidth, tabBar()->height());
    QRectF rect2(tabBar()->pos().x()+width()-(imageWidth+secondLabelWidth), tabBar()->pos().y(), imageWidth, tabBar()->height());
    QRectF rect3(tabBar()->pos().x()+width()-secondLabelWidth, tabBar()->pos().y(), secondLabelWidth, tabBar()->height());
    painter.drawText(rect,  Qt::AlignCenter, "Powered by ");
    painter.drawImage(rect2, pythonLogoImg);
    painter.drawText(rect3,  Qt::AlignCenter, pythonVersion);
}
