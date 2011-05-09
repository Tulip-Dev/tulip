#ifndef TULIPWELCOMEPAGE_H
#define TULIPWELCOMEPAGE_H

#include <QtGui/QWidget>

namespace Ui {
class TulipWelcomePageData;
}

class QNetworkReply;

class TulipWelcomePage: public QWidget {
  Q_OBJECT
public:
  explicit TulipWelcomePage(QWidget *parent=0);
  virtual ~TulipWelcomePage();

protected slots:
  void rssReply(QNetworkReply *);
  void openLink(const QString &link);

private:
  Ui::TulipWelcomePageData *_ui;
};

#endif // TULIPWELCOMEPAGE_H
