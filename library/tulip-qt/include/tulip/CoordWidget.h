#ifndef COORDWIDGET_H
#define COORDWIDGET_H
#include <QtGui/QWidget>
#include <tulip/Coord.h>
#include <tulip/TlpQtTools.h>

namespace Ui {
    class CoordWidget;
}

namespace tlp{

  /**
  * @brief Simple Coord edition widget.
  **/
    class CoordWidget : public QWidget
    {
        Q_OBJECT
    public:
        CoordWidget(QWidget* parent = NULL);
        ~CoordWidget();
        tlp::Coord coord()const;
    public slots:
        void setCoord(const tlp::Coord& coord);

    signals:
        void coordChanged(tlp::Coord coord);

    private slots:
        void coordUpdated();

    private:
        Ui::CoordWidget *ui;
    };
}

#endif // COORDWIDGET_H
