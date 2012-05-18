#ifndef TULIPFONTWIDGET_H
#define TULIPFONTWIDGET_H

#include <QtGui/QWidget>
#include <tulip/TulipFont.h>
namespace Ui {
class TulipFontWidget;
}
namespace tlp {
class TLP_QT_SCOPE TulipFontWidget : public QWidget {
  Q_OBJECT
  Q_PROPERTY(TulipFont font READ font WRITE setFont)

public:
  explicit TulipFontWidget(QWidget *parent = NULL);
  ~TulipFontWidget();
  void setFont(const TulipFont& font);
  TulipFont font()const;

private:
  Ui::TulipFontWidget *ui;

private slots:
  void editFont();
};
}
#endif // TULIPFONTWIDGET_H
