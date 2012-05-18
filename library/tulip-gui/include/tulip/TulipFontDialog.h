#ifndef TULIPFONTDIALOG_H
#define TULIPFONTDIALOG_H

#include <QtGui/QDialog>
#include <tulip/tulipconf.h>
#include <tulip/TulipFont.h>

namespace Ui {
class TulipFontDialog;
}

namespace tlp {

class TLP_QT_SCOPE TulipFontDialog : public QDialog {
  Q_OBJECT

  Ui::TulipFontDialog* _ui;

public:
  explicit TulipFontDialog(QWidget *parent = NULL);
  TulipFont font() const;
  int fontSize() const;

  static TulipFont getFont(QWidget* parent = NULL,const TulipFont& selectedFont=TulipFont());

public slots:
  void selectFont(const TulipFont&);

protected slots:
  void fontChanged();
};

}

#endif // TULIPFONTDIALOG_H
