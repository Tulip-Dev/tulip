#include "UpdatePlugin.h"
#include "MultiServerManager.h"
#include "AuthorizationInstallDependencies.h"
#include "InstallPluginDialog.h"
#include "PluginsHelp.h"

#include <iostream>

#include <QtCore/QDir>
#include <QtCore/QTextStream>
#include <QtCore/QProcess>
#include <QtGui/QApplication>

using namespace std;

namespace tlp {

  static QString getSubDir(const string &str) {
    if(str=="Glyph")
      return "glyphs/";
    if(str=="Interactor")
      return "interactors/" ;
    if(str=="View")
      return "view/" ;
    if(str=="Controller")
      return "controller/" ;
    return "";
  }

  UpdatePlugin::UpdatePlugin(QObject *parent):QObject(parent),openDialog(false),partNumber(0),currentPart(0){
    std::string installPathStr(tlp::TulipLibDir);
    installPathStr+="tlp/toInstall/";
    installPath = QDir::toNativeSeparators(installPathStr.c_str()).toStdString();
    QDir tmp(installPath.c_str());
    tmp.mkpath(installPath.c_str());
  }

  int UpdatePlugin::pluginsCheckAndUpdate(MultiServerManager *msm,std::set<DistPluginInfo,PluginCmp> &pluginsToInstall, std::set<LocalPluginInfo,PluginCmp> &pluginsToRemove,QWidget *parent){
    if(openDialog==true)delete pluginDialog;
    this->msm = msm;

    std::vector<string>pluginsToInstallNames;
    std::vector<string>pluginsToRemoveNames;

    set<DistPluginInfo,PluginCmp> depNoInstalled;
    set<LocalPluginInfo,PluginCmp> depToRemove;
    set<LocalPluginInfo,PluginCmp> depToRemoveWithoutAsk;

    for(set<DistPluginInfo,PluginCmp>::iterator it = pluginsToInstall.begin(); it!=pluginsToInstall.end(); ++it) {
      bool ok=msm->getPluginDependenciesToInstall(*it,depNoInstalled,depToRemoveWithoutAsk);

      if(ok==false){
        windowToDisplayError((*it).name,parent);
        return 0;

      }
    }

    for(set<DistPluginInfo,PluginCmp>::iterator it = pluginsToInstall.begin(); it!=pluginsToInstall.end(); ++it) {
      set<DistPluginInfo,PluginCmp>::iterator itD=depNoInstalled.find(*it);
      if(itD!=depNoInstalled.end())
        depNoInstalled.erase(itD);
    }

    if(depNoInstalled.size()>0){
      // Ask the user if he wants to install all dependancy
      AuthorizationInstallDependencies* authoriz = new AuthorizationInstallDependencies(&depNoInstalled,&pluginsToInstall);
      authoriz->exec();
    }

    for (set<LocalPluginInfo,PluginCmp>::iterator it = pluginsToRemove.begin(); it!=pluginsToRemove.end(); ++it) {
      bool ok=msm->getPluginDependenciesToRemove(*it,depToRemove);

      if(ok==false){
        windowToDisplayError((*it).name,parent);
        return 0;
      }
    }

    for(set<LocalPluginInfo,PluginCmp>::iterator it = pluginsToRemove.begin(); it!=pluginsToRemove.end(); ++it) {
      set<LocalPluginInfo,PluginCmp>::iterator itD=depToRemove.find(*it);
      if(itD!=depToRemove.end())
        depToRemove.erase(itD);
    }

    if(depToRemove.size()>0){
      // Ask the user if he wants to install all dependancy
      AuthorizationInstallDependencies* authoriz = new AuthorizationInstallDependencies(&depToRemove,&pluginsToRemove);
      authoriz->exec();
    }
    for(set<LocalPluginInfo,PluginCmp>::iterator it = depToRemoveWithoutAsk.begin(); it!=depToRemoveWithoutAsk.end(); ++it) {
      pluginsToRemove.insert(*it);
    }

    for(set<DistPluginInfo,PluginCmp>::iterator it=pluginsToInstall.begin();it!=pluginsToInstall.end();++it)
      pluginsToInstallNames.push_back((*it).name);
    for(set<LocalPluginInfo,PluginCmp>::iterator it=pluginsToRemove.begin();it!=pluginsToRemove.end();++it)
      pluginsToRemoveNames.push_back((*it).name);

    pluginDialog = new InstallPluginDialog(pluginsToInstallNames,pluginsToRemoveNames,parent);
    openDialog = true;
    pluginDialog->show();

    for (set<LocalPluginInfo,PluginCmp>::iterator it= pluginsToRemove.begin(); it!=pluginsToRemove.end(); ++it) {
      UpdatePlugin *plug=new UpdatePlugin();
      connect(plug, SIGNAL(pluginUninstalled(UpdatePlugin*,const LocalPluginInfo &)), this, SLOT(terminatePluginUninstall(UpdatePlugin*,const LocalPluginInfo &)));
      connect(plug, SIGNAL(installPart(const std::string&,int,int)), pluginDialog, SLOT(installPart(const std::string&, int, int)));
      //pluginDialog->addPlugin(false,(*it).name);
      plug->uninstall((*it));
    }

    for (set<DistPluginInfo,PluginCmp>::iterator it = pluginsToInstall.begin(); it!=pluginsToInstall.end(); ++it) {
      // Installing current plugin
      UpdatePlugin* plug = new UpdatePlugin(msm);
      pluginUpdaters.push_back(plug);
      connect(plug, SIGNAL(pluginInstalled(UpdatePlugin*,const DistPluginInfo &)), this, SLOT(terminatePluginInstall(UpdatePlugin*,const DistPluginInfo &)));
      connect(plug, SIGNAL(installPart(const std::string&,int,int)), pluginDialog, SLOT(installPart(const std::string&, int, int)));

      string serverAddr = getAddr((*it).server);
      //pluginDialog->addPlugin(true,(*it).name);

      plug->install(serverAddr,(*it));
    }

    return pluginsToRemove.size()+pluginsToInstall.size();
  }

  void UpdatePlugin::windowToDisplayError(string pluginName,QWidget *parent){
    QDialog* dia = new QDialog(parent);
    QVBoxLayout* box = new QVBoxLayout(dia);
    QLabel* labelName = new QLabel(pluginName.c_str(),dia);
    QLabel* label=new QLabel("Dependencies check faild\nInstallation cancel",dia);
    box->addWidget(labelName);
    box->addWidget(label);

    QPushButton* ok = new QPushButton("Yes",dia);
    box->addWidget(ok);
    connect(ok, SIGNAL(clicked()),dia, SLOT(reject()));

    //dia->setLayout(box);
    dia->exec();set<DistPluginInfo *> pluginsToUpdate;

  }

  string UpdatePlugin::getAddr(string name){
      vector<string> names;
      vector<string> addrs;
      msm->getNames(names);
      msm->getAddrs(addrs);
      vector<string>::iterator itNames=names.begin();
      vector<string>::iterator itAddrs=addrs.begin();

      for(;itNames!=names.end();++itNames) {
        if((*itNames) == name)
    return (*itAddrs);
        ++itAddrs;
      }

      return "ERROR";
    }

  void UpdatePlugin::terminatePluginInstall(UpdatePlugin* terminatedUpdater, const DistPluginInfo &pluginInfo){
    pluginDialog->installFinished(pluginInfo.name, pluginInfo.installIsOK);
    pluginUpdaters.removeAll(terminatedUpdater);
    disconnect(terminatedUpdater, SIGNAL(pluginInstalled(UpdatePlugin*,const DistPluginInfo &)), this, SLOT(terminatePluginInstall(UpdatePlugin*,const DistPluginInfo &)));
    if (pluginInfo.installIsOK)
      msm->addLocalPlugin(&pluginInfo);
    // in a distant future, we might like to pass the plugin's name to this signal
    emit pluginInstalled();
    delete terminatedUpdater;
  }

  void UpdatePlugin::terminatePluginUninstall(UpdatePlugin* terminatedUpdater,const LocalPluginInfo &pluginInfo){
    pluginDialog->installFinished(pluginInfo.name, true);
    disconnect(terminatedUpdater, SIGNAL(pluginUninstalled(UpdatePlugin*,const LocalPluginInfo &)), this, SLOT(terminatePluginUninstall(UpdatePlugin*,const LocalPluginInfo &)));
    msm->removeLocalPlugin(&pluginInfo);
    emit pluginUninstalled();
    delete terminatedUpdater;
  }

  void UpdatePlugin::install(const string &serverAddr,const DistPluginInfo &pluginInfo){
    distPluginInfo=pluginInfo;
    version=pluginInfo.version;
    while(version.find(" ")!=string::npos) {
      version.replace(version.find(" "),1,".");
    }
    partNumber=2;
    string realServerAddr=serverAddr.substr(0,serverAddr.rfind("/")+1);
    serverWS=new Server(serverAddr);
    serverGet=new Server(realServerAddr);
    serverWS->send(new DownloadPluginRequest(pluginInfo.name));
    serverGet->send(new GetPluginRequest(new PluginDownloadFinish(this),pluginInfo.fileName+"/"+pluginInfo.fileName+string(".doc.")+version,installPath+pluginInfo.fileName+std::string(".doc")));
    serverGet->send(new GetPluginRequest(new PluginDownloadFinish(this),pluginInfo.fileName+"/"+pluginInfo.fileName+string(".helpdoc.")+version,installPath+pluginInfo.fileName+std::string(".helpdoc")));
    #if defined(__APPLE__)
      serverGet->send(new GetPluginRequest(new EndPluginDownloadFinish(this),pluginInfo.fileName+"/"+pluginInfo.fileName+string(".dylib.")+version,installPath+pluginInfo.fileName+std::string(".dylib")));
    #elif defined(_WIN32)
      serverGet->send(new GetPluginRequest(new EndPluginDownloadFinish(this),pluginInfo.fileName+"/"+pluginInfo.fileName+string(".dll.")+version,installPath+pluginInfo.fileName+std::string(".dll")));
    #elif defined(I64)
      serverGet->send(new GetPluginRequest(new EndPluginDownloadFinish(this),pluginInfo.fileName+"/"+pluginInfo.fileName+string(".so.")+version+".i64",installPath+pluginInfo.fileName+std::string(".so")));
    #else
      serverGet->send(new GetPluginRequest(new EndPluginDownloadFinish(this),pluginInfo.fileName+"/"+pluginInfo.fileName+string(".so.")+version+".i386",installPath+pluginInfo.fileName+std::string(".so")));
    #endif
  }

  void UpdatePlugin::endInstallation() {
    // check if plugin can be loaded
    QString tlp_check_pl_path = QCoreApplication::applicationDirPath() + "/"
      "tulip_check_pl";
    string pluginInstallPath = installPath + distPluginInfo.fileName +
    #if defined(_WIN32)
      ".dll";
    #elif defined(__APPLE__)
      ".dylib";
    #else
      ".so";
    #endif

    QProcess process;

    process.start(tlp_check_pl_path,
			QStringList(pluginInstallPath.c_str()));

    if(!process.waitForStarted()){
      assert(false);
    }
    process.waitForFinished();
    QString result=QString(process.readAll());

    bool loadCheckOK = result.contains("pluginLoaded");

    if (loadCheckOK) {
      QFile installFile(QString(installPath.c_str())+"toInstall.dat");
      if(!installFile.open(QIODevice::ReadWrite | QIODevice::Text))
	return;
      QTextStream out(&installFile);
      out.readAll();
      QString subDir=getSubDir(distPluginInfo.type);

      out << subDir << distPluginInfo.fileName.c_str() << ".helpdoc" << "\n" ;
      out << subDir << distPluginInfo.fileName.c_str() << ".doc" << "\n" ;
#if defined(_WIN32)
      out << subDir << distPluginInfo.fileName.c_str() << ".dll" << "\n" ;
#elif defined(__APPLE__)
      out << subDir << distPluginInfo.fileName.c_str() << ".dylib" << "\n" ;
#else
      out << subDir << distPluginInfo.fileName.c_str() << ".so" << "\n" ;
#endif

      installFile.close();
    } else {
      // remove downloaded files
      QFile::remove(pluginInstallPath.c_str());
      string pluginsDocPath = installPath + distPluginInfo.fileName + ".doc";
      QFile::remove(QString(pluginsDocPath.c_str()));
    }
    distPluginInfo.installIsOK = loadCheckOK;
    emit pluginInstalled(this, distPluginInfo);
  }


  bool UpdatePlugin::uninstall(const LocalPluginInfo &pluginInfo){
    //string fileName = pluginName.split('.')[0];
    localPluginInfo=pluginInfo;
    QFile removeFile(QString(installPath.c_str())+"toRemove.dat");
    if(!removeFile.open(QIODevice::ReadWrite | QIODevice::Text))
      return 1;
    QTextStream out(&removeFile);
    QString subDir = getSubDir(pluginInfo.type);
    out.readAll();
    out << subDir << pluginInfo.fileName.c_str() << ".doc" << "\n" ;
    out << subDir << pluginInfo.fileName.c_str() << ".helpdoc" << "\n" ;
    #if defined(_WIN32)
    out << subDir << pluginInfo.fileName.c_str() << ".dll" << "\n" ;
    #elif defined(__APPLE__)
    out << subDir << pluginInfo.fileName.c_str() << ".dylib" << "\n" ;
    #else
    out << subDir << pluginInfo.fileName.c_str() << ".so" << "\n" ;
    #endif
    removeFile.close();
    emit installPart(pluginInfo.name,1,1);
    endUninstallation();

    return 0;
  }

  void UpdatePlugin::updatePartNumber() {
    currentPart++;
    emit installPart(distPluginInfo.name,currentPart,partNumber);
  }

  void UpdatePlugin::copyFile(const QDir& oldDir,const QString& oldName,const QDir& newDir,const QString& newName){
    QFile oldFile(QDir::toNativeSeparators(oldDir.absolutePath()+QString("/")+oldName));
    QFile newFile(QDir::toNativeSeparators(newDir.absolutePath()+QString("/")+newName));
    oldFile.open(QIODevice::ReadOnly);
    newFile.open(QIODevice::WriteOnly);
    newFile.write(oldFile.readAll());
    oldFile.close();
    newFile.close();
  }

  void UpdatePlugin::installWhenRestartTulip() {
    string srcPath = QDir::toNativeSeparators((tlp::TulipLibDir+"tlp/toInstall/").c_str()).toStdString();
    string dstPath = QDir::toNativeSeparators((tlp::TulipLibDir+"tlp/").c_str()).toStdString();

    QDir srcDir(srcPath.c_str());
    QDir dstDir(dstPath.c_str());

    QFile removeFile(QString(srcPath.c_str())+"toRemove.dat");
    if(removeFile.open(QIODevice::ReadOnly | QIODevice::Text)){
      QTextStream removeStream(&removeFile);
      while(!removeStream.atEnd()) {
        QString line = removeStream.readLine();
        if(!line.contains(".helpdoc"))
          dstDir.remove(line);
        else
          PluginsHelp::removeHelpDoc(line);
      }
      removeFile.close();
      srcDir.remove("toRemove.dat");
    }

    QFile installFile(QString(srcPath.c_str())+"toInstall.dat");
    if(installFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
      QTextStream installStream(&installFile);
      while(!installStream.atEnd()) {
        QString line = installStream.readLine();
        if(!line.contains(".helpdoc")){
          if(line.contains("/")){
            QDir tmpDir=dstDir.absolutePath()+"/"+line.split("/").first();
            copyFile(srcDir,line.split("/").last(),tmpDir,line.split("/").last());
          }else{
            copyFile(srcDir,line,dstDir,line);
          }
        }else{
          if(line.contains("/"))
            PluginsHelp::installHelpDoc(line.split("/").last());
          else
            PluginsHelp::installHelpDoc(line);
        }
      }
      installFile.close();
      srcDir.remove("toInstall.dat");
    }
  }

  bool UpdatePlugin::isInstallDirWritable() {
    std::string installDir(tlp::TulipLibDir);
    installDir += "/tlp";
    return QFileInfo(installDir.c_str()).isWritable();
  }

  bool UpdatePlugin::pluginUpdatesPending() {
    std::string installChangeDir(tlp::TulipLibDir);
    installChangeDir += "/tlp/toInstall/";
    return QFileInfo(QString(installChangeDir.c_str()) + "toInstall.dat").exists()
      || QFileInfo(QString(installChangeDir.c_str()) + "toRemove.dat").exists();
  }
}
