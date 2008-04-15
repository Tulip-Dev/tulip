#ifndef INFODIALOG_H
#define INFODIALOG_H
#include "InfoDialogData.h"

class InfoDialog : public QDialog, public Ui::InfoDialogData { 
    Q_OBJECT

public:
  InfoDialog( QWidget* parent = 0);
    ~InfoDialog();

};

#endif // INFODIALOG_H
