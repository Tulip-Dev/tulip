#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef  _WIN32
// compilation pb workaround
#include <windows.h>
#endif
#include <QtCore/qstring.h>
#include <QtCore/qstringlist.h>
#include <QtGui/qlistview.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qmessagebox.h>
#include <QtGui/qinputdialog.h>
#include <QtGui/qlabel.h>
#include <QtGui/qcolordialog.h>
#include <QtGui/qtabwidget.h>

#include "CopyPropertyDialog.h"
#include "tulip/ForEach.h"

using namespace std;
using namespace tlp;

//=============================================================================
CopyPropertyDialog::CopyPropertyDialog(QWidget* parent, const char* name, bool modal, Qt::WFlags fl)
  : CopyPropertyDialogData(parent, name, (Qt::WFlags) (fl | Qt::Widget)) {
  connect((QObject *) buttonOK , SIGNAL(clicked()), this, SLOT(accept()) );
  connect((QObject *) buttonCancel , SIGNAL(clicked()), this, SLOT(reject()) );
}

void CopyPropertyDialog::setProperties(std::string& srcProp,
				       std::vector<std::string>& localProps,
				       std::vector<std::string>& inheritedProps) {
  setCaption((std::string("Copy property ") + srcProp).c_str());
  unsigned int i = 0;
  if (localProps.size() == 0)
    localPropertyButton->setEnabled(false);
  else
    for (; i < localProps.size(); ++i)
      localProperties->insertItem(QString(localProps[i].c_str()));
  if (inheritedProps.size() == 0)
    inheritedPropertyButton->setEnabled(false);
  else
    for (i = 0; i < inheritedProps.size(); ++i)
      inheritedProperties->insertItem(QString(inheritedProps[i].c_str()));
}

std::string CopyPropertyDialog::getDestinationProperty(CopyPropertyDialog::destType& type) {
  type = CopyPropertyDialog::NEW;
  if (exec() == QDialog::Accepted) {
    if (newPropertyText->isEnabled())
      return newPropertyText->text().ascii();
    type = CopyPropertyDialog::LOCAL;
    if (localProperties->isEnabled())
      return localProperties->currentText().ascii();
    type = CopyPropertyDialog::INHERITED;
      return inheritedProperties->currentText().ascii();
  }
  return std::string();
}

void CopyPropertyDialog::setNew(bool flag) {
  newPropertyText->setEnabled(flag);
}

void CopyPropertyDialog::setLocal(bool flag) {
  localProperties->setEnabled(flag);
}

void CopyPropertyDialog::setInherited(bool flag) {
  inheritedProperties->setEnabled(flag);
}
