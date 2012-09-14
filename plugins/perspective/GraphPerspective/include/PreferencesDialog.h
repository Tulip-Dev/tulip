#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QtGui/QDialog>

namespace Ui {
class PreferencesDialog;
}

class PreferencesDialog : public QDialog {
  Q_OBJECT

  Ui::PreferencesDialog* _ui;
public:
  explicit PreferencesDialog(QWidget *parent = 0);

signals:

public slots:
  void writeSettings();
  void readSettings();

};

#endif // PREFERENCESDIALOG_H
