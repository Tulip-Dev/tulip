#ifndef ABOUTTULIPPAGE_H
#define ABOUTTULIPPAGE_H

#include <QtGui/QWidget>

namespace Ui {
class AboutTulipPageData;
}

/**
  @brief The "About us/Getting help" screen displayed in the Tulip app startup window.
  This widget is mainly meant to provide links to the user and developer documentations, direct user to Tulip forums
  and provide basic offline support.
  */
class AboutTulipPage : public QWidget {
public:
  explicit AboutTulipPage(QWidget *parent = 0);
private:
  Ui::AboutTulipPageData *_ui;
};

#endif // ABOUTTULIPPAGE_H
