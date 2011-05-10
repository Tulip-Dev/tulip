#include "tulip/CoordWidget.h"
#include "CoordWidgetData.h"
#include <cfloat>
using namespace tlp;
CoordWidget::CoordWidget(QWidget* parent) : QWidget(parent),ui(new Ui::CoordWidget())
{
    ui->setupUi(this);
    ui->xDoubleSpinBox->setMinimum(FLT_MIN);
    ui->xDoubleSpinBox->setMaximum(FLT_MAX);
    ui->yDoubleSpinBox->setMinimum(FLT_MIN);
    ui->yDoubleSpinBox->setMaximum(FLT_MAX);
    ui->zDoubleSpinBox->setMinimum(FLT_MIN);
    ui->zDoubleSpinBox->setMaximum(FLT_MAX);
    connect(ui->xDoubleSpinBox,SIGNAL(valueChanged(double)),this,SLOT(coordUpdated()));
    connect(ui->yDoubleSpinBox,SIGNAL(valueChanged(double)),this,SLOT(coordUpdated()));
    connect(ui->zDoubleSpinBox,SIGNAL(valueChanged(double)),this,SLOT(coordUpdated()));
}

CoordWidget::~CoordWidget(){
    delete ui;
}

Coord CoordWidget::coord()const{
    return Coord(ui->xDoubleSpinBox->value(),ui->yDoubleSpinBox->value(),ui->zDoubleSpinBox->value());
}
void CoordWidget::setCoord(const Coord& coord){
    blockSignals(true);
    ui->xDoubleSpinBox->setValue(coord[0]);
    ui->yDoubleSpinBox->setValue(coord[1]);
    ui->zDoubleSpinBox->setValue(coord[2]);
    blockSignals(false);
    coordUpdated();
}

void CoordWidget::coordUpdated(){
 emit(coordChanged(coord()));
}
