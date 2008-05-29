#include "PluginsManagerDialog.h"
#include "UpdatePlugin.h"

#include <QtGui/QVBoxLayout>
#include <QtGui/QMessageBox>

#include "PluginsManagerMainWindow.h"

using namespace std;

namespace tlp {

  void PluginsManagerDialog::createWidget(PluginsManagerMainWindow *pluginsManager) {
    connect(pluginsManager, SIGNAL(closeSignal()), this, SLOT(close()));
    QVBoxLayout *layout=new QVBoxLayout(this);
    layout->setMargin(0);
    layout->addWidget(pluginsManager);
    QDialog::setLayout(layout);
  }

  PluginsManagerDialog::PluginsManagerDialog(std::vector<LocalPluginInfo> &plugins,QWidget *parent):QDialog(parent){
    PluginsManagerMainWindow *pluginsManager=new PluginsManagerMainWindow(plugins,parent);
    createWidget(pluginsManager);
  }

  PluginsManagerDialog::PluginsManagerDialog(MultiServerManager *msm,QWidget *parent):QDialog(parent){
    PluginsManagerMainWindow *pluginsManager=new PluginsManagerMainWindow(msm,parent);
    createWidget(pluginsManager);
  }

  bool PluginsManagerDialog::pluginUpdatesPending() {
    return UpdatePlugin::pluginUpdatesPending();
  }
}
