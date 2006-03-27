#ifndef INFODIALOG_H
#define INFODIALOG_H
#include "InfoDialogData.h"

class InfoDialog : public InfoDialogForm
{ 
    Q_OBJECT

public:
  InfoDialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WFlags fl = 0 );
    ~InfoDialog();

};

#endif // INFODIALOG_H
