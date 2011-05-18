#include "tulip/SizeWidget.h"
#include "SizeWidgetData.h"
#include <cfloat>
using namespace tlp;

SizeWidget::SizeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SizeWidget)
{
    ui->setupUi(this);
    QDoubleValidator *validator = new QDoubleValidator(this);
    validator->setRange(-FLT_MAX,FLT_MAX,1000);
    ui->widthLineEdit->setValidator(validator);
    ui->heightLineEdit->setValidator(validator);
    ui->depthLineEdit->setValidator(validator);
    setSize(Size());
    connect(ui->widthLineEdit,SIGNAL(valueChanged(double)),this,SLOT(sizeUpdated()));
    connect(ui->heightLineEdit,SIGNAL(valueChanged(double)),this,SLOT(sizeUpdated()));
    connect(ui->depthLineEdit,SIGNAL(valueChanged(double)),this,SLOT(sizeUpdated()));
}

SizeWidget::~SizeWidget()
{
    delete ui;
}

Size SizeWidget::size()const{
    return Size(ui->widthLineEdit->text().toFloat(),ui->heightLineEdit->text().toFloat(),ui->depthLineEdit->text().toFloat());
}

void SizeWidget::setSize(const Size& size){
    blockSignals(true);
    ui->widthLineEdit->setText(QString::number(size[0]));
    ui->heightLineEdit->setText(QString::number(size[1]));
    ui->depthLineEdit->setText(QString::number(size[2]));
    blockSignals(false);
    sizeUpdated();
}

void SizeWidget::sizeUpdated(){
    emit sizeChanged(size());
}
