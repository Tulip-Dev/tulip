#include "PreferenceDialog.h"

#include <QtCore/QSettings>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string>

#include <tulip/PreferenceManager.h>

using namespace std;
using namespace tlp;

PreferenceDialog::PreferenceDialog( QWidget* parent)
    : QDialog( parent),modified(false) {
  setupUi(this);

  PreferenceManager &preferenceManager=PreferenceManager::getInst();
  bool networkConnection=preferenceManager.getNetworkConnection();
  bool autoLoadController=preferenceManager.getAutoLoadController();
  Color selectionColor=preferenceManager.getSelectionColor();

  networkRadioButton->setChecked(networkConnection);
  autoLoadControllerButton->setChecked(autoLoadController);
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
  bool autoLoadController = settings.value("autoLoadController",false).toBool();

  settings.endGroup();

  PreferenceManager &preferenceManager=PreferenceManager::getInst();
  preferenceManager.setNetworkConnection(networkConnection);
  preferenceManager.setSelectionColor(selectionColor);
  preferenceManager.setAutoLoadController(autoLoadController);
}
void PreferenceDialog::savePreference(){
  QSettings settings("TulipSoftware","Tulip");
  settings.beginGroup("Preference");

  // Check modification
  modified = modified || (settings.value("networkConnection",true).toBool()!=networkRadioButton->isChecked());
  modified = modified || (settings.value("autoLoadController",true).toBool()!=autoLoadControllerButton->isChecked());

  // Store data
  settings.setValue("networkConnection",networkRadioButton->isChecked());
  settings.setValue("autoLoadController",autoLoadControllerButton->isChecked());

  settings.endGroup();
}

void PreferenceDialog::accept() {
  savePreference();

  QDialog::accept();
}

