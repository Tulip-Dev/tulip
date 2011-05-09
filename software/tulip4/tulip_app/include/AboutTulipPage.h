#ifndef ABOUTTULIPPAGE_H
#define ABOUTTULIPPAGE_H

#include <QtGui/QWidget>

namespace Ui {
class AboutTulipPageData;
}

class AboutTulipPage : public QWidget {
public:
  explicit AboutTulipPage(QWidget *parent = 0);
private:
  Ui::AboutTulipPageData *_ui;
};

#endif // ABOUTTULIPPAGE_H
