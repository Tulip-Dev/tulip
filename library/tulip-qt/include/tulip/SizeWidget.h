#ifndef SIZEWIDGET_H
#define SIZEWIDGET_H

#include <QtGui/QWidget>
#include <tulip/Size.h>
namespace Ui {
    class SizeWidget;
}
namespace tlp{
    /**
  * @brief Simple Size edition widget.
  **/
    class SizeWidget : public QWidget
    {
        Q_OBJECT
    public:
        explicit SizeWidget(QWidget *parent = 0);
        ~SizeWidget();
        tlp::Size size()const;
    public slots:
        void setSize(const tlp::Size& size);
    signals:
        void sizeChanged(tlp::Size size);

    private slots:
        void sizeUpdated();

    private:
        Ui::SizeWidget *ui;
    };
}

#endif // SIZEWIDGET_H
