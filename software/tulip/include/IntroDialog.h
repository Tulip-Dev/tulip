#ifndef INTRODIALOG_H
#define INTRODIALOG_H
#include "InfoDialogData.h"

class IntroDialog : public InfoDialogForm
{ 
    Q_OBJECT

public:
    IntroDialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~IntroDialog();

};

#endif // INTRODIALOG_H
