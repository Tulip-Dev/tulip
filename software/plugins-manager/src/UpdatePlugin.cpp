#include "UpdatePlugin.h"

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

  UpdatePlugin::UpdatePlugin(QObject *parent):QObject(parent),partNumber(0),currentPart(0){
    std::string installPathStr(tlp::TulipLibDir);
    installPathStr+="tlp/toInstall/";
    installPath = QDir::toNativeSeparators(installPathStr.c_str()).toStdString();
    QDir tmp(installPath.c_str());
    tmp.mkpath(installPath.c_str());
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

    int returnCode =
      QProcess::execute(tlp_check_pl_path,
			QStringList(pluginInstallPath.c_str()));

    bool loadCheckOK = returnCode == TLP_CHECK_PL_RETURN_SUCCESS;

    if (loadCheckOK) {
      QFile installFile(QString(installPath.c_str())+"toInstall.dat");
      if(!installFile.open(QIODevice::ReadWrite | QIODevice::Text))
	return;
      QTextStream out(&installFile);
      out.readAll();
      QString subDir=getSubDir(distPluginInfo.type);
      
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
    #if defined(_WIN32)
    out << subDir << pluginInfo.fileName.c_str() << ".dll" << "\n" ;
    #elif defined(__APPLE__)
    out << subDir << pluginInfo.fileName.c_str() << ".dylib" << "\n" ;
    #else
    out << subDir << pluginInfo.fileName.c_str() << ".so" << "\n" ;
    #endif
    removeFile.close();
    endUninstallation();
    return 0;
  }

  void UpdatePlugin::updatePartNumber() {
    currentPart++;
    emit installPart(distPluginInfo.name,((float)currentPart)/((float)partNumber));
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
	dstDir.remove(line);
      }
      removeFile.close();
      srcDir.remove("toRemove.dat");
    }

    QFile installFile(QString(srcPath.c_str())+"toInstall.dat");
    if(installFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
      QTextStream installStream(&installFile);
      while(!installStream.atEnd()) {
	QString line = installStream.readLine();
	if(line.contains(QDir::separator())){
	  QDir tmpDir=dstDir.absolutePath()+QDir::separator()+line.split(QDir::separator()).first();
	  copyFile(srcDir,line.split(QDir::separator()).last(),tmpDir,line.split(QDir::separator()).last());
	}else{
	  copyFile(srcDir,line,dstDir,line);
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
