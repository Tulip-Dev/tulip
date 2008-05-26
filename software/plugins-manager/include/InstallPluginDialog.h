//-*-c++-*-
/**
 Author: Morgan Mathiaut and Samuel Carruesco, Mickael Melas, Laurent Peyronnet, Michael Roche, Sabrina Wons
 Email : mathiaut@labri.fr
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/

#ifndef _Tulip_INSTALLPLUGINDIALOG_H_
#define _Tulip_INSTALLPLUGINDIALOG_H_

#include <map>

#include <QtGui/QDialog>
#include <QtGui/QPushButton>
#include <QtGui/QLabel>
#include <QtGui/QBoxLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QProgressBar>

#include "InstallExitDialog.h"

namespace tlp {

  class TLP_PLUGINSMANAGER_SCOPE InstallPluginDialog : public QDialog{
    Q_OBJECT
    
  public:
    InstallPluginDialog(const std::vector<std::string> &namePluginsInstall,const std::vector<std::string> &namePluginsRemove,QWidget *parent=0);
    
    void installStart(const std::string &name);
    void installFinished(const std::string &name, bool loadCheckOK);

    void stopInstall();
    void addPlugin(bool install,const std::string &name);

  private:

    QVBoxLayout *mainBox;
    QVBoxLayout *installBox;
    QVBoxLayout *removeBox;
    QVBoxLayout *installLabelBox;
    QVBoxLayout *installBarBox;
    std::vector<QProgressBar *> installPluginBars;
    QVBoxLayout *removeLabelBox;
    QVBoxLayout *removeBarBox;
    std::vector<QProgressBar *> removePluginBars;
    QHBoxLayout *buttonBox;

    std::map<std::string, unsigned int> pluginsInstallIndex;
    std::map<std::string, unsigned int> pluginsRemoveIndex;
    
    QLabel* labelTitle;
    QPushButton* ok;
    QPushButton* cancel;
     
    InstallExitDialog *exitDialog;

    unsigned int processNumber;
    std::string installErrors;
    
    void closeEvent(QCloseEvent *e);
    void terminated();
		
  public slots:

    void installPart(const std::string&, float);
			    
  private slots:
    void cancelInstall();
   
  };
  
}

#endif
