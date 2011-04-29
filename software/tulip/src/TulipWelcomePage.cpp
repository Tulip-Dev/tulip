#include "TulipWelcomePage.h"

TulipWelcomePage::TulipWelcomePage(QWidget *parent): QWidget(parent), _ui(new Ui::TulipWelcomePageData) {
  _ui->setupUi(this);
}

TulipWelcomePage::~TulipWelcomePage() {
}
