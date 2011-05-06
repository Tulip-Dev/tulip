#ifndef ABOUTTULIPPAGE_H
#define ABOUTTULIPPAGE_H

#include <QtGui/QWidget>

#include "ui_AboutTulipPage.h"

class AboutTulipPage : public QWidget {
public:
  explicit AboutTulipPage(QWidget *parent = 0);
private:
  Ui::AboutTulipPageData *_ui;
};

#endif // ABOUTTULIPPAGE_H
