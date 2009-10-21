#ifndef Tulip_PREFERENCEDIALOG_H
#define Tulip_PREFERENCEDIALOG_H

#include "PreferenceDialogData.h"

class PreferenceDialog : public QDialog, public Ui::PreferenceDialogData {
    Q_OBJECT

public:
  PreferenceDialog( QWidget* parent = 0);
  ~PreferenceDialog();

  static void loadPreference();
  static void savePreference();

protected slots:

  virtual void accept();
  void selectionColorButtonSlot();

private :
  void setSelectionButtonColor(QColor tmp);

};

#endif
