#include "ProxyConfigurationDialog.h"

#include <iostream>

#include <QtCore/QSettings>
#include <QtGui/QIntValidator>

using namespace std;

namespace tlp {

  ProxyConfigurationDialog::ProxyConfigurationDialog(QWidget* parent) : QDialog(parent) {
    setupUi(this);
    connect(enableProxy,SIGNAL(stateChanged(int)),this,SLOT(proxyStateChange(int)));
    connect(useUsernamePassword,SIGNAL(stateChanged(int)),this,SLOT(useUsernamePasswordStateChange(int)));

    QSettings settings("TulipSoftware","Tulip");
    settings.beginGroup("PluginsManager");
    bool proxyEnable=settings.value("proxyEnable",false).toBool();
    bool proxyUsernamePasswordEnable=settings.value("proxyUsernamePasswordEnable",false).toBool();
    QString proxyAddress=settings.value("proxyAddress","").toString();
    quint16 proxyPort=settings.value("proxyPort",0).toUInt();
    QString proxyUsername=settings.value("proxyUsername","").toString();
    QString proxyPassword=settings.value("proxyPassword","").toString();
    settings.endGroup();

    if(proxyEnable)
          enableProxy->setCheckState(Qt::Checked);
    if(proxyUsernamePasswordEnable)
      useUsernamePassword->setCheckState(Qt::Checked);
    hostEdit->setText(proxyAddress);
    portEdit->setText(QString::number(proxyPort));
    portEdit->setValidator(new QIntValidator(0,65535,portEdit));
    usernameEdit->setText(proxyUsername);
    passwordEdit->setText(proxyPassword);
  }

  void ProxyConfigurationDialog::saveProxy() {
    QSettings settings("TulipSoftware","Tulip");
    settings.beginGroup("PluginsManager");
    settings.setValue("proxyEnable",enableProxy->isChecked());
    settings.setValue("proxyAddress",hostEdit->text());
    settings.setValue("proxyPort",portEdit->text().toUInt());
    settings.setValue("proxyUsernamePasswordEnable",useUsernamePassword->isChecked());
    settings.setValue("proxyUsername",usernameEdit->text());
    settings.setValue("proxyPassword",passwordEdit->text());
    settings.endGroup();
  }

  void ProxyConfigurationDialog::proxyStateChange(int state){
    bool enable = (state == Qt::Checked);

    hostEdit->setEnabled(enable);
    portEdit->setEnabled(enable);
    useUsernamePassword->setEnabled(enable);
  }

  void ProxyConfigurationDialog::useUsernamePasswordStateChange(int state){
    bool enable = (state == Qt::Checked);

    usernameEdit->setEnabled(enable);
    passwordEdit->setEnabled(enable);
  }

}
