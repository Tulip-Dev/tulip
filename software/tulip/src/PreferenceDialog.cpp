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
  bool autoLoadController=preferenceManager.getAutoLoadController();
  Color selectionColor=preferenceManager.getSelectionColor();

  setSelectionButtonColor(QColor(selectionColor[0],selectionColor[1],selectionColor[2],selectionColor[3]));
  networkRadioButton->setChecked(networkConnection);
  autoLoadControllerButton->setChecked(autoLoadController);

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
  bool autoLoadController = settings.value("autoLoadConnection",false).toBool();

  settings.endGroup();

  PreferenceManager &preferenceManager=PreferenceManager::getInst();
  preferenceManager.setNetworkConnection(networkConnection);
  preferenceManager.setSelectionColor(selectionColor);
  preferenceManager.setAutoLoadController(autoLoadController);
}
void PreferenceDialog::savePreference(){
  QColor selectionColor = selectionColorButton->palette().color(QPalette::Button);
  
  QSettings settings("TulipSoftware","Tulip");
  settings.beginGroup("Preference");

  settings.setValue("selectionColorR",selectionColor.red());
  settings.setValue("selectionColorG",selectionColor.green());
  settings.setValue("selectionColorB",selectionColor.blue());
  settings.setValue("selectionColorA",selectionColor.alpha());

  settings.setValue("networkConnection",networkRadioButton->isChecked());
  settings.setValue("autoLoadController",autoLoadControllerButton->isChecked());

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
  savePreference();

  QDialog::accept();
}

