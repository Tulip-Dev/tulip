#ifndef TULIPWELCOMEPAGE_H
#define TULIPWELCOMEPAGE_H

#include <QtGui/QWidget>
#include "ui_TulipWelcomePage.h"

class TulipWelcomePage: public QWidget {
public:
  explicit TulipWelcomePage(QWidget *parent=0);
  virtual ~TulipWelcomePage();

private:
  Ui::TulipWelcomePageData *_ui;
};

#endif // TULIPWELCOMEPAGE_H
