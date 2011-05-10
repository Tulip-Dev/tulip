#include "tulip/SizeWidget.h"
#include "SizeWidgetData.h"
#include <cfloat>
using namespace tlp;

SizeWidget::SizeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SizeWidget)
{
    ui->setupUi(this);
    ui->widthDoubleSpinBox->setMinimum(0);
    ui->widthDoubleSpinBox->setMaximum(FLT_MAX);
    ui->heightDoubleSpinBox->setMinimum(0);
    ui->heightDoubleSpinBox->setMaximum(FLT_MAX);
    ui->depthDoubleSpinBox->setMinimum(0);
    ui->depthDoubleSpinBox->setMaximum(FLT_MAX);
    connect(ui->widthDoubleSpinBox,SIGNAL(valueChanged(double)),this,SLOT(sizeUpdated()));
    connect(ui->heightDoubleSpinBox,SIGNAL(valueChanged(double)),this,SLOT(sizeUpdated()));
    connect(ui->depthDoubleSpinBox,SIGNAL(valueChanged(double)),this,SLOT(sizeUpdated()));
}

SizeWidget::~SizeWidget()
{
    delete ui;
}

Size SizeWidget::size()const{
    return Size(ui->widthDoubleSpinBox->value(),ui->heightDoubleSpinBox->value(),ui->depthDoubleSpinBox->value());
}

void SizeWidget::setSize(const Size& size){
    blockSignals(true);
    ui->widthDoubleSpinBox->setValue(size[0]);
    ui->heightDoubleSpinBox->setValue(size[1]);
    ui->depthDoubleSpinBox->setValue(size[2]);
    blockSignals(false);
    sizeUpdated();
}

void SizeWidget::sizeUpdated(){
    emit sizeChanged(size());
}
