#include "PushToPluginServer.h"

#include <PluginLoaderWithInfo.h>
#include <UpdatePlugin.h>
#include <tulip/GlyphManager.h>
#include <tulip/ViewPluginsManager.h>
#include <tulip/ControllerPluginsManager.h>

#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QTextStream>
#include <QtCore/QCoreApplication>
#include <QtCore/QBuffer>

using namespace std;
using namespace tlp;



int main(int argc,char **argv) {
  QCoreApplication app(argc, argv);
  if(argc != 6 && argc != 7)  {
    cout << "How to use :" << endl;
    cout << " First arg : plugin file" << endl;
    cout << " Second arg : doxygen root dir" << endl;
    cout << " Third arg : server address" << endl;
    cout << " 4th arg : url of the pluginserver" << endl;
    cout << " 5th arg : base64 encoded credentials" << endl;
    cout << " 6th arg : i386/i64 : default i386" << endl;
    QByteArray array = QByteArray(string("huet:./1fALlabri").c_str());
    cout << QString(array.toBase64()).toStdString()<< endl;
    
    exit(1);
  }

  /** setup the variables **/
  QString pluginPath = argv[1];
  QString pluginDocRootPath = argv[2];
  QString serverURL = argv[3];
  QString pluginServerURL = argv[4];
  QString base64credentials = argv[5];

  QString architectureDir("i386");
  if(argc>=6)
    architectureDir=QString(argv[6]);
    
  QFileInfo fileInfo(pluginPath);

  /** load the plugin in Tulip to retrieve a pluginInfo **/
  char *getEnvTlp=getenv("TLP_DIR");
#if defined(_WIN32)
  putenv("TLP_DIR=");
#else
  setenv("TLP_DIR","",true);
#endif
  PluginLoaderWithInfo plug;
  initTulipLib(NULL);
  loadPlugin(pluginPath.toStdString(), &plug);
  //GlyphManager::getInst().loadPlugins(&plug); //Glyph plugins
  InteractorManager::getInst().loadPlugins(&plug);
  ViewPluginsManager::getInst().loadPlugins(&plug);
  ControllerPluginsManager::getInst().loadPlugins(&plug);
#if defined(_WIN32)
  if (getEnvTlp)
    putenv((string("TLP_DIR=") + getEnvTlp).c_str());
  else
    putenv("TLP_DIR=");
#else
  if (getEnvTlp)
    setenv("TLP_DIR",getEnvTlp,true);
  else
    unsetenv("TLP_DIR");
#endif

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
  
  for(size_t i=0;i<plug.pluginsList.size();++i){
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
    for(size_t i=0;i<plug.pluginsList.size()-1;++i){
      if(plug.pluginsList[i].type!=plug.pluginsList[i+1].type){
  equal=false;
  break;
      }
    }

    if(!equal){
      for(size_t i=0;i<plug.pluginsList.size();++i){
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

  /** upload the plugin to the server **/
  QString pluginFolderName = pluginInfo.fileName.c_str()+QString(".")+(QString(pluginInfo.version.c_str()).split(" "))[1];
  
  WebDavManager manager(serverURL, pluginServerURL, base64credentials);
  manager.mkdir(pluginFolderName);
  manager.mkdir(pluginFolderName + "/" + architectureDir);
  
  QFile pluginFile(pluginPath);
  pluginFile.open(QIODevice::ReadOnly);
  manager.putFile(pluginFolderName + "/" + architectureDir + "/" + fileInfo.fileName(), &pluginFile);
  
  
  QDomDocument pluginInfoDocument("pluginInfoDocument");
  QDomElement infoElement = pluginInfoDocument.createElement("pluginInfo");
  pluginInfoDocument.appendChild(infoElement);
  infoElement.setAttribute("name", QString::fromStdString(pluginInfo.name));
  infoElement.setAttribute("type", QString::fromStdString(pluginInfo.type));
  infoElement.setAttribute("displayType", QString::fromStdString(pluginInfo.displayType));
  infoElement.setAttribute("author", QString::fromStdString(pluginInfo.author));
  infoElement.setAttribute("date", QString::fromStdString(pluginInfo.date));
  infoElement.setAttribute("info", QString::fromStdString(pluginInfo.info));
  infoElement.setAttribute("fileName", QString::fromStdString(pluginInfo.fileName));
  infoElement.setAttribute("version", QString::fromStdString(pluginInfo.version));
  
  for(vector<PluginDependency>::const_iterator it = pluginInfo.dependencies.begin(); it != pluginInfo.dependencies.end(); ++it) {
    QDomElement dependencyElement = pluginInfoDocument.createElement("dependency");
    dependencyElement.setAttribute("name", QString::fromStdString(it->name));
    dependencyElement.setAttribute("type", QString::fromStdString(it->type));
    dependencyElement.setAttribute("version", QString::fromStdString(it->version));
  }
  
//   pluginInfo.fileName + ".xml"
  
  /** Generate the documentation and upload it **/
  QString fileName = QString::fromStdString(pluginInfo.fileName);
  QString libName = fileName.split("-").first();
  libName=libName.split("lib").last();
  QString documentName(pluginDocRootPath + "/class" + libName + ".xml");
  
  QDir pluginsDocDir(pluginDocRootPath);
  QStringList filters;
  filters << "class*.xml";
  QStringList pluginsDocs = pluginsDocDir.entryList(filters);
  QList<QString>::iterator iter = pluginsDocs.begin();
    
  while(iter != pluginsDocs.end()){
    
    QString docFile(*iter);
    docFile = (docFile.mid(5,docFile.size()-9));
          
    if(docFile.compare(libName,Qt::CaseInsensitive)==0){
      documentName = QString(pluginDocRootPath + "/" + (*iter));
    }
    ++iter;
  }
  
  QDomDocument pluginClassDocumentationDocument;
  QFile documentationFile(documentName);
  documentationFile.open(QIODevice::ReadOnly);
  pluginClassDocumentationDocument.setContent(&documentationFile);
  
  documentationFile.close();
  
//   std::cout << documentName.toStdString() << std::endl; 
  QDomNodeList tempList = pluginClassDocumentationDocument.elementsByTagName("compounddef");
//   std::cout << pluginClassDocumentationDocument.toString().toStdString() << std::endl; 
  QDomNode classElement = tempList.at(0);
//   std::cout << classElement.nodeName().toStdString() << ": " << classElement.nodeValue().toStdString() << std::endl; 
  QDomNode briefDescriptionElement = classElement.firstChildElement("briefdescription");
  QDomNode detailedDescriptionElement = classElement.firstChildElement("detaileddescription");
  
  QDomDocument pluginDocDocument;
  QDomElement docElement = pluginDocDocument.createElement("doc");
  pluginDocDocument.appendChild(docElement);
//   std::cout << briefDescriptionElement.nodeName().toStdString() << ": " << briefDescriptionElement.nodeValue().toStdString() << std::endl; 
  docElement.appendChild(briefDescriptionElement);
  docElement.appendChild(detailedDescriptionElement);

  
  QByteArray tempInfo(pluginInfoDocument.toByteArray());
  QBuffer pluginInfoDevice(&tempInfo);
  pluginInfoDevice.open(QBuffer::ReadOnly);
  manager.putFile(pluginFolderName + "/" + "/" + QString::fromStdString(pluginInfo.fileName) + ".xml", &pluginInfoDevice);
  
  QByteArray tempDoc(pluginDocDocument.toByteArray());
  QBuffer pluginDocDevice(&tempDoc);
  pluginDocDevice.open(QBuffer::ReadOnly);
  manager.putFile(pluginFolderName + "/" + "/" + QString::fromStdString(pluginInfo.fileName) + ".doc", &pluginDocDevice);
  
  app.processEvents();
  manager.finish();
  return 0;
}


