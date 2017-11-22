#ifndef TULIPFONTICONDIALOG_H
#define TULIPFONTICONDIALOG_H

#include <tulip/tulipconf.h>

#include <QDialog>
#include <QString>
#include <QIcon>

namespace Ui {
class TulipFontIconDialog;
}

namespace tlp {

class TLP_QT_SCOPE TulipFontIconDialog : public QDialog {

  Q_OBJECT

  Ui::TulipFontIconDialog *_ui;
  QString _selectedIconName;

public:
  TulipFontIconDialog(QWidget *parent = nullptr);

  QString getSelectedIconName() const;

  void setSelectedIconName(const QString &iconName);

  void accept();

  void showEvent(QShowEvent *);

  static QIcon getFontAwesomeIcon(const QString &iconName);

  static QIcon getMaterialDesignIcon(const QString &iconName);

protected slots:

  void updateIconList();

  void openUrlInBrowser(const QString &url);
};
}

#endif
