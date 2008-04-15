#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string>
#include <qlabel.h>
#include "InfoDialog.h"

using namespace std;

/* 
 *  Constructs a InfoDialog which is a child of 'parent'
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
InfoDialog::InfoDialog( QWidget* parent)
    : QDialog( parent) {
  setupUi(this);
  string tmp="Tulip ";
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

