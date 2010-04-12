#ifndef Tulip_PREFERENCEDIALOG_H
#define Tulip_PREFERENCEDIALOG_H

#include "PreferenceDialogData.h"

class PreferenceDialog : public QDialog, public Ui::PreferenceDialogData {
    Q_OBJECT

public:
  PreferenceDialog( QWidget* parent = 0);
  ~PreferenceDialog();

  static void loadPreference();
  void savePreference();
  bool preferencesAreModified() {return modified;}

protected slots:

  virtual void accept();
  void selectionColorButtonSlot();

protected :

  void setSelectionButtonColor(QColor tmp);

  bool modified;

};

#endif
