#include <tulip/TulipFontWidget.h>
#include "ui_TulipFontWidget.h"
#include <tulip/TulipFontDialog.h>

using namespace tlp;

TulipFontWidget::TulipFontWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TulipFontWidget)
{
    ui->setupUi(this);
    connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(editFont()));
}

TulipFontWidget::~TulipFontWidget()
{
    delete ui;
}

void TulipFontWidget::setFont(const TulipFont& font){
    ui->lineEdit->setText(font.fontFile());
}

TulipFont TulipFontWidget::font()const{
    return TulipFont::fromFile(ui->lineEdit->text());
}

void TulipFontWidget::editFont(){
    TulipFont newFont = TulipFontDialog::getFont(this,font());
    if(newFont.exists()){
        setFont(newFont);
    }
}
