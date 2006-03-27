#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string>
#include <qlabel.h>
#include "InfoDialog.h"

using namespace std;

/* 
 *  Constructs a InfoDialog which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
InfoDialog::InfoDialog( QWidget* parent,  const char* name, bool modal, Qt::WFlags fl )
    : InfoDialogForm( parent, name, modal, fl )
{
  string tmp="Tulip v.";
  tmp+=VERSION;
  TextLabel2->setText(tmp.c_str());
}

/*  
 *  Destroys the object and frees any allocated resources
 */
InfoDialog::~InfoDialog()
{
    // no need to delete child widgets, Qt does it all for us
}

