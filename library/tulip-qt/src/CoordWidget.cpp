#include "tulip/CoordWidget.h"
#include "CoordWidgetData.h"
#include <QtGui/QDoubleValidator>
#include <cfloat>
using namespace tlp;
CoordWidget::CoordWidget(QWidget* parent) : QWidget(parent),ui(new Ui::CoordWidget())
{
    ui->setupUi(this);
    QDoubleValidator *validator = new QDoubleValidator(this);
    validator->setRange(-FLT_MAX,FLT_MAX,1000);
    ui->xLineEdit->setValidator(validator);
    ui->yLineEdit->setValidator(validator);
    ui->zLineEdit->setValidator(validator);
    setCoord(Coord());
    connect(ui->xLineEdit,SIGNAL(textChanged(QString)),this,SLOT(coordUpdated()));
    connect(ui->yLineEdit,SIGNAL(textChanged(QString)),this,SLOT(coordUpdated()));
    connect(ui->zLineEdit,SIGNAL(textChanged(QString)),this,SLOT(coordUpdated()));
}

CoordWidget::~CoordWidget(){
    delete ui;
}

Coord CoordWidget::coord()const{
    return Coord(ui->xLineEdit->text().toFloat(),ui->yLineEdit->text().toFloat(),ui->zLineEdit->text().toFloat());
}
void CoordWidget::setCoord(const Coord& coord){
    blockSignals(true);
    ui->xLineEdit->setText(QString::number(coord[0]));
    ui->yLineEdit->setText(QString::number(coord[1]));
    ui->zLineEdit->setText(QString::number(coord[2]));
    blockSignals(false);
    coordUpdated();
}

void CoordWidget::coordUpdated(){
 emit(coordChanged(coord()));
}
