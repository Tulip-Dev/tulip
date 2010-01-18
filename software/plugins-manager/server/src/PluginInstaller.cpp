#include "PluginGenerate.h"

#include <PluginLoaderWithInfo.h>
#include <UpdatePlugin.h>

#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QTextStream>

#include <tulip/TlpTools.h>
#include <tulip/GlyphManager.h>
#include <tulip/InteractorManager.h>
#include <tulip/ViewPluginsManager.h>
#include <tulip/ControllerPluginsManager.h>
#include <tulip/View.h>

using namespace std;
using namespace tlp;

int main(int argc,char **argv) {
  if(argc != 5 && argc != 6)  {
    cout << "How to use :" << endl;
    cout << " First arg : plugin file" << endl;
    cout << " Second arg : doxygen generated plugin doc file" << endl;
    cout << " Third arg : target directory" << endl;
    cout << " 4th arg : generate .xml/.doc files (yes/no)" << endl;
    cout << " 5th arg : i386/i64 : default i386" << endl;
    exit(1);
  }

  QString pluginPath=argv[1];
  QString pluginDocPath=argv[2];
  QString targetPath=argv[3];

  cout << "plugin path : " << pluginPath.toStdString() << endl;
  cout << "plugin doc path : " << pluginDocPath.toStdString() << endl;
  cout << "target path : " << targetPath.toStdString() << endl;

  bool generateDoc=true;
  if(QString(argv[4])=="no")
    generateDoc=false;
  QString subDir("i386");
  if(argc>=6)
    subDir=QString(argv[5]);
    
  QFileInfo fileInfo(pluginPath);
  QDir srcDir = fileInfo.dir();
  QString suffix = fileInfo.suffix();

  char *getEnvTlp=getenv("TLP_DIR");
  setenv("TLP_DIR","",true);
  PluginLoaderWithInfo plug;
  initTulipLib(NULL);
  loadPlugin(pluginPath.toStdString(), &plug);
  //GlyphManager::getInst().loadPlugins(&plug); //Glyph plugins
  InteractorManager::getInst().loadPlugins(&plug);
  ViewPluginsManager::getInst().loadPlugins(&plug);
  ControllerPluginsManager::getInst().loadPlugins(&plug);
  if(getEnvTlp)
    setenv("TLP_DIR",getEnvTlp,true);
  else
    unsetenv("TLP_DIR");

  TemplateFactory<GlyphFactory, Glyph, GlyphContext>::ObjectCreator::const_iterator itGlyphs;
  vector<string> glyphsName;
  for (itGlyphs=GlyphFactory::factory->objMap.begin();itGlyphs != GlyphFactory::factory->objMap.end();++itGlyphs) {
    glyphsName.push_back((itGlyphs)->first);
  }

  TemplateFactory<InteractorFactory, Interactor, InteractorContext>::ObjectCreator::const_iterator itInteractors;
  vector<string> interactorsName;
  for (itInteractors=InteractorFactory::factory->objMap.begin();itInteractors != InteractorFactory::factory->objMap.end();++itInteractors) {
    interactorsName.push_back((itInteractors)->first);
  }

  TemplateFactory<ViewFactory, View, ViewContext>::ObjectCreator::const_iterator itViews;
  vector<string> viewsName;
  for (itViews=ViewFactory::factory->objMap.begin();itViews != ViewFactory::factory->objMap.end();++itViews) {
    viewsName.push_back((itViews)->first);
  }


  for(int i=0;i<plug.pluginsList.size();++i){
    for(vector<string>::iterator it=glyphsName.begin();it!=glyphsName.end();++it){
      if(plug.pluginsList[i].name==(*it)){
	plug.pluginsList[i].type="Glyph";
      }
    }
    for(vector<string>::iterator it=interactorsName.begin();it!=interactorsName.end();++it){
      if(plug.pluginsList[i].name==(*it)){
	plug.pluginsList[i].type="Interactor";
      }
    }
    for(vector<string>::iterator it=viewsName.begin();it!=viewsName.end();++it){
      if(plug.pluginsList[i].name==(*it)){
	plug.pluginsList[i].type="View";
      }
    }
    if(plug.pluginsList[i].type=="")
      plug.pluginsList[i].type="Algorithm";
  }

  LocalPluginInfo pluginInfo;
  if(plug.pluginsList.size()>1)  {
    bool equal=true;
    for(int i=0;i<plug.pluginsList.size()-1;++i){
      if(plug.pluginsList[i].type!=plug.pluginsList[i+1].type){
	equal=false;
	break;
      }
    }

    if(!equal){
      for(int i=0;i<plug.pluginsList.size();++i){
	if(plug.pluginsList[i].type=="View"){
	  pluginInfo = plug.pluginsList[i];
	  break;
	}  
      }
    }else{
      pluginInfo = plug.pluginsList[0];
    }
  }

  
  if(plug.pluginsList.size()==1){
    pluginInfo = plug.pluginsList[0];
  }

  pluginInfo.displayType= PluginInfo::getPluginDisplayType(pluginInfo.name);
  if(pluginInfo.displayType == "Glyph")
    pluginInfo.type = "Glyph";

  QDir dir;
  dir.mkpath(targetPath);
  dir.mkpath(targetPath+"/plugins/");

  QString path= pluginInfo.fileName.c_str()+QString(".")+(QString(pluginInfo.version.c_str()).split(" "))[1];

  QDir dstDir(targetPath+"/plugins/"+path);
  dstDir.mkpath(targetPath+"/plugins/"+path);
  QDir dstSubDir(targetPath+"/plugins/"+path+"/"+subDir);
  dstSubDir.mkpath(targetPath+"/plugins/"+path+"/"+subDir);

  if(generateDoc)
    if (!generatePluginInfoFile(pluginInfo, dstDir))
      return EXIT_FAILURE;

  if (suffix == "so")
    UpdatePlugin::copyFile(srcDir,
			   QString(pluginInfo.fileName.c_str())+".so",
			   dstSubDir,
			   QString(pluginInfo.fileName.c_str())+".so");
  else if (suffix == "dylib")
    UpdatePlugin::copyFile(srcDir,
			   QString(pluginInfo.fileName.c_str())+".dylib",
			   dstSubDir,
			   QString(pluginInfo.fileName.c_str())+".dylib");
  else if (suffix == "dll")
    UpdatePlugin::copyFile(srcDir,
			   QString(pluginInfo.fileName.c_str())+".dll",
			   dstSubDir,
			   QString(pluginInfo.fileName.c_str())+".dll");

  // Documentation
  if(generateDoc){
    QString fileName(pluginInfo.fileName.c_str());
    QString version(pluginInfo.version.c_str());
    QFile docFile(pluginDocPath);
    generatePluginDocFile(fileName, version, docFile, dstDir);
  }    

  return EXIT_SUCCESS;
}
