#include "AboutTulipPage.h"

#include "ui_AboutTulipPage.h"

AboutTulipPage::AboutTulipPage(QWidget *parent) :
    QWidget(parent), _ui(new Ui::AboutTulipPageData()) {
  _ui->setupUi(this);
}
