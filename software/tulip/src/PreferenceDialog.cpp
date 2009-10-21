#include "PreferenceDialog.h"

#include <QtCore/QSettings>
#include <QtGui/QColorDialog>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string>

#include <tulip/PreferenceManager.h>

using namespace std;
using namespace tlp;

PreferenceDialog::PreferenceDialog( QWidget* parent)
    : QDialog( parent) {
  setupUi(this);

  PreferenceManager &preferenceManager=PreferenceManager::getInst();
  bool networkConnection=preferenceManager.getNetworkConnection();
  Color selectionColor=preferenceManager.getSelectionColor();

  setSelectionButtonColor(QColor(selectionColor[0],selectionColor[1],selectionColor[2],selectionColor[3]));
  networkRadioButton->setChecked(networkConnection);

  connect(selectionColorButton,SIGNAL(clicked()),this,SLOT(selectionColorButtonSlot()));
}


PreferenceDialog::~PreferenceDialog()
{

}

void PreferenceDialog::loadPreference(){
  QSettings settings("TulipSoftware","Tulip");
  settings.beginGroup("Preference");

  Color selectionColor=Color(255,0,255,255);
  selectionColor[0] = settings.value("selectionColorR",255).toUInt();
  selectionColor[1] = settings.value("selectionColorG",0).toUInt();
  selectionColor[2] = settings.value("selectionColorB",255).toUInt();
  selectionColor[3] = settings.value("selectionColorA",255).toUInt();

  bool networkConnection= settings.value("networkConnection",true).toBool();

  settings.endGroup();

  PreferenceManager &preferenceManager=PreferenceManager::getInst();
  preferenceManager.setNetworkConnection(networkConnection);
  preferenceManager.setSelectionColor(selectionColor);
}
void PreferenceDialog::savePreference(){
  PreferenceManager &preferenceManager=PreferenceManager::getInst();

  QSettings settings("TulipSoftware","Tulip");
  settings.beginGroup("Preference");

  settings.setValue("selectionColorR",preferenceManager.getSelectionColor()[0]);
  settings.setValue("selectionColorG",preferenceManager.getSelectionColor()[1]);
  settings.setValue("selectionColorB",preferenceManager.getSelectionColor()[2]);
  settings.setValue("selectionColorA",preferenceManager.getSelectionColor()[3]);

  settings.setValue("networkConnection",preferenceManager.getNetworkConnection());

  settings.endGroup();
}

void PreferenceDialog::setSelectionButtonColor(QColor tmp) {
   if (tmp.isValid()) {
     QString colorStr;
     QString str;

     str.setNum(tmp.red(),16);
     if(str.size()!=2)
       str.insert(0,"0");
     colorStr.append(str);

     str.setNum(tmp.green(),16);
     if(str.size()!=2)
       str.insert(0,"0");
     colorStr.append(str);

     str.setNum(tmp.blue(),16);
     if(str.size()!=2)
       str.insert(0,"0");
     colorStr.append(str);

     QString textColor;
     int h,s,v;
     tmp.getHsv(&h, &s, &v);
     if (v < 128)
       textColor="ffffff";
     else
       textColor="000000";

     selectionColorButton->setStyleSheet("QPushButton { background-color: #"+colorStr+"; color: #"+textColor+" }");
   }
 }

void PreferenceDialog::selectionColorButtonSlot(){
  setSelectionButtonColor(QColorDialog::getColor(selectionColorButton->palette().color(QPalette::Button), this));
}

void PreferenceDialog::accept() {
  cout << "accept" << endl;

  PreferenceManager &preferenceManager=PreferenceManager::getInst();
  cout << "down : " << networkRadioButton->isChecked() << endl;
  preferenceManager.setNetworkConnection(networkRadioButton->isChecked());
  QColor selectionColor = selectionColorButton->palette().color(QPalette::Button);
  preferenceManager.setSelectionColor(Color(selectionColor.red(),selectionColor.green(),selectionColor.blue(),selectionColor.alpha()));

  PreferenceDialog::savePreference();

  QDialog::accept();
}

