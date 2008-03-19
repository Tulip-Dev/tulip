#include <iostream>
#include <cassert>
#include <QtGui/QCloseEvent>

#include "InstallPluginDialog.h"

using namespace std;

namespace tlp {

  InstallPluginDialog::InstallPluginDialog(const std::vector<string> &namePluginsInstall,const std::vector<string> &namePluginsRemove,QWidget *parent)
    :QDialog(parent),processNumber(0){

   
    exitDialog = new InstallExitDialog(this);

    setWindowTitle("Plugins install/Remove");
    setModal(true);
    mainBox = new QVBoxLayout(this);
    mainBox->setMargin(50);
    mainBox->setSpacing(25);
  
    labelTitle = new QLabel("Requested operations",this);
    labelTitle->setAlignment(Qt::AlignHCenter);
    QPalette pal;
    pal.setColor(labelTitle->foregroundRole(), Qt::magenta);
    labelTitle->setAutoFillBackground(true);
    labelTitle->setPalette(pal);
    QFont titleFont;
    titleFont.setBold(true);
    titleFont.setPixelSize(18); 
    labelTitle->setFont(titleFont);
    QFont blodFont;
    blodFont.setBold(true);

    mainBox->addWidget(labelTitle);

    installBox = new QVBoxLayout(mainBox);
    removeBox = new QVBoxLayout(mainBox);

    QLabel *installLabel=new QLabel("Install : ",this);
    installLabel->setFont(blodFont);
    installBox->addWidget(installLabel);
    QLabel *removeLabel=new QLabel("Remove : ",this);
    removeLabel->setFont(blodFont);
    removeBox->addWidget(removeLabel);

    QHBoxLayout *subInstallBox= new QHBoxLayout(installBox);
    QHBoxLayout *subRemoveBox= new QHBoxLayout(removeBox);
    installLabelBox = new QVBoxLayout(subInstallBox);
    installBarBox = new QVBoxLayout(subInstallBox);
    removeLabelBox = new QVBoxLayout(subRemoveBox);
    removeBarBox = new QVBoxLayout(subRemoveBox);

    buttonBox = new QHBoxLayout(mainBox);
    cancel = new QPushButton("Cancel",this);
    ok = new QPushButton("Ok",this);
    buttonBox->addWidget(ok);
    buttonBox->addWidget(cancel);
    ok->setEnabled(false);
    connect(cancel,SIGNAL(clicked()),this,SLOT(cancelInstall()));
    connect(ok,SIGNAL(clicked()),this,SLOT(reject()));

    setLayout(mainBox);

    // install
    unsigned int index=0;
    if(namePluginsInstall.size()==0) {
      QLabel *newLabel=new QLabel("None",this);
      installLabelBox->addWidget(newLabel);
    }else{
      for(vector<string>::const_iterator it=namePluginsInstall.begin();it!=namePluginsInstall.end();++it) {
	pluginsInstallIndex[*it]=index;
	index++;
	QLabel *newLabel=new QLabel(it->c_str(),this);
	QProgressBar *newBar=new QProgressBar(this);
	newBar->setMinimum(0);
	newBar->setMaximum(100);
	installLabelBox->addWidget(newLabel);
	installBarBox->addWidget(newBar);
	installPluginBars.push_back(newBar);
	processNumber++;
      }
    }

    // remove
    index=0;
    if(namePluginsRemove.size()==0) {
      QLabel *newLabel=new QLabel("None",this);
      removeLabelBox->addWidget(newLabel);
    }else{
      for(vector<string>::const_iterator it=namePluginsRemove.begin();it!=namePluginsRemove.end();++it) {
	pluginsRemoveIndex[*it]=index;
	index++;
	QLabel *newLabel=new QLabel(it->c_str(),this);
	QProgressBar *newBar=new QProgressBar(this);
	newBar->setMinimum(0);
	newBar->setMaximum(100);
	removeLabelBox->addWidget(newLabel);
	removeBarBox->addWidget(newBar);
	removePluginBars.push_back(newBar);
	processNumber++;
      }
    }
  
  }

  void InstallPluginDialog::installStart(const string &name){
    /*for(unsigned int i=0;i<labelsInstall.size();i++){
      if( name.compare(labelsInstall[i]->text().toStdString())==0 ){
	statusInstall[i]->setText(inProgress.c_str());
	break;
      }
      }  */
  }
  
  void InstallPluginDialog::installFinished(const string &name){
    /*std::vector<string>::iterator it;
    for(it=install.begin(); it!=install.end(); it++){
      if( (*(it)).compare(name)==0 ){
	install.erase(it);
	break;
      }
      }*/
    /*if(install.size()==0 && remove.size()==0 ){
      terminated();
      }*/
    installPart(name,1.);
    processNumber--;
    if(processNumber==0)
      terminated();
  }

  void InstallPluginDialog::stopInstall(){
    //std::vector<string> nameStopInstall;
    
    /*for(unsigned int i=0;i<statusInstall.size();i++){
      if( inProgress.compare(statusInstall[i]->text().toStdString())==0 ){
	nameStopInstall.push_back(labelsInstall[i]->text().toStdString());
      }
      }*/
  }

  void InstallPluginDialog::addPlugin(bool install, const string &name){
    unsigned int index;
    if(install){
      installBox->setEnabled(true);
      index=installPluginBars.size();
      pluginsInstallIndex[name]=index;
      QLabel *newLabel=new QLabel(name.c_str(),this);
      QProgressBar *newBar=new QProgressBar(this);
      newBar->setMinimum(0);
      newBar->setMaximum(100);
      installLabelBox->addWidget(newLabel);
      installBarBox->addWidget(newBar);
      installPluginBars.push_back(newBar);
    }else{
      removeBox->setEnabled(true);
      index=removePluginBars.size();
      pluginsRemoveIndex[name]=index;
      QLabel *newLabel=new QLabel(name.c_str(),this);
      QProgressBar *newBar=new QProgressBar(this);
      newBar->setMinimum(0);
      newBar->setMaximum(100);
      removeLabelBox->addWidget(newLabel);
      removeBarBox->addWidget(newBar);
      removePluginBars.push_back(newBar);
    }
    processNumber++;
  }

  void InstallPluginDialog::closeEvent(QCloseEvent *e){
    if(terminate==false){
      e->ignore();
      cancelInstall();
    }
    else{
      reject();
    }
  }

  void InstallPluginDialog::terminated(){
    if(exitDialog->isVisible()){
      exitDialog->reject();
    }
    labelTitle->setText("Completed operations");
    QPalette pal;
    pal.setColor(labelTitle->foregroundRole(), Qt::blue);
    labelTitle->setAutoFillBackground(true);
    labelTitle->setPalette(pal);

    cancel->setEnabled(false);
    ok->setEnabled(true);
  }

  void InstallPluginDialog::installPart(const std::string &name, float percent) {
    map<string, unsigned int>::iterator it;
    it=pluginsInstallIndex.find(name);
    if(it!=pluginsInstallIndex.end()){
      unsigned int index=(*it).second;
      QProgressBar *bar=installPluginBars[index];
      assert(bar);
      bar->setValue((int)(percent*100));
      return;
    }
    it=pluginsRemoveIndex.find(name);
    if(it!=pluginsRemoveIndex.end()){
      unsigned int index=(*it).second;
      QProgressBar *bar=removePluginBars[index];
      assert(bar);
      bar->setValue((int)(percent*100));
      return;
    }
    assert(false);
  } 

  

  void InstallPluginDialog::cancelInstall(){
    exitDialog->show();
  }

}
