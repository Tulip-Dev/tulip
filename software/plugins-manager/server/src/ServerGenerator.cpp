#include "ServerGenerator.h"

#include <PluginLoaderWithInfo.h>
#include <UpdatePlugin.h>

#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtCore/QDir>

#include <tulip/TlpTools.h>
#include <tulip/GlyphManager.h>

using namespace std;
using namespace tlp;

int main(int argc,char **argv)
{
  if(argc<=2)  {
    cout << "How to use :" << endl;
    cout << " First arg : doc source directory (where directory \"doxygen\" are)" << endl;
    cout << " Second arg : target directory" << endl;
    cout << " Third arg : source directory for mac/windows plugins" << endl;
    exit(1);
  }

  QString docPath=argv[1];
  QString targetPath=argv[2];

  PluginLoaderWithInfo plug;
  initTulipLib(NULL);
  loadPlugins(&plug);    // library side plugins
  GlyphManager::getInst().loadPlugins(&plug); //Glyph plugins 

  vector<LocalPluginInfo> pluginsList=plug.pluginsList;

  for(vector<LocalPluginInfo>::iterator it=pluginsList.begin();it!=pluginsList.end();++it) {
    if((*it).type!="Glyph")
      (*it).displayType=PluginInfo::getPluginDisplayType((*it).name);
    else
      (*it).displayType=(*it).type;
  }

  QDir dir;
  dir.mkpath(targetPath);
  dir.mkpath(targetPath+"/plugins/");

  for(vector<LocalPluginInfo>::iterator it=pluginsList.begin();it!=pluginsList.end();++it) {

    xmlDocPtr doc = NULL;       /* document pointer */
    xmlNodePtr rootNode = NULL;
    
    doc = xmlNewDoc(BAD_CAST "1.0");
    
    rootNode = xmlNewNode(NULL, BAD_CAST "pluginInfo");
    xmlDocSetRootElement(doc, rootNode);
    
    xmlNewProp(rootNode, BAD_CAST "name", BAD_CAST (*it).name.c_str());
    xmlNewProp(rootNode, BAD_CAST "type", BAD_CAST (*it).type.c_str());
    xmlNewProp(rootNode, BAD_CAST "displayType", BAD_CAST (*it).displayType.c_str());
    xmlNewProp(rootNode, BAD_CAST "author", BAD_CAST (*it).author.c_str());
    xmlNewProp(rootNode, BAD_CAST "date", BAD_CAST (*it).date.c_str());
    xmlNewProp(rootNode, BAD_CAST "info", BAD_CAST (*it).info.c_str());
    xmlNewProp(rootNode, BAD_CAST "fileName", BAD_CAST (*it).fileName.c_str());
    xmlNewProp(rootNode, BAD_CAST "version", BAD_CAST (*it).version.c_str());
    
    AddToDomDocument mapf(rootNode);
    for_each((*it).dependencies.begin(),(*it).dependencies.end(),mapf);

    xmlChar *xmlbuff;
    int buffersize;

    xmlDocDumpFormatMemory(doc, &xmlbuff, &buffersize, 1);
    QString stringDoc=(char *)xmlbuff;

    QString path=(*it).fileName.c_str();

    QDir srcDir((TulipLibDir+"tlp/").c_str());
    QDir secondSrcDir;
    if(argc==4) {
      secondSrcDir=QDir(argv[3]);
    }
     
    QDir dstDir(targetPath+"/plugins/"+path);
    dstDir.mkpath(targetPath+"/plugins/"+path);

    cout << dstDir.absolutePath().toStdString() << endl;

    QFile pluginXmlFile(dstDir.absolutePath() + "/" + (*it).fileName.c_str()+".xml."+QString((*it).version.c_str()).replace(" ","."));
    if(!pluginXmlFile.open(QIODevice::WriteOnly)) {
      cerr << "Error in write file : " << pluginXmlFile.fileName().toStdString() << endl;
      return 0;
    }else{
      cout << "Plugin : " << (*it).fileName << " is create" << endl;
    }
    pluginXmlFile.write(stringDoc.toStdString().c_str(),strlen(stringDoc.toStdString().c_str()));
    pluginXmlFile.close();
    if(srcDir.exists(QString((*it).fileName.c_str())+".so")) {
      UpdatePlugin::copyFile(srcDir,QString((*it).fileName.c_str())+".so",dstDir,QString((*it).fileName.c_str())+".so."+QString((*it).version.c_str()).replace(" ",".")+".i386");
    }
    if(argc==4) {
      if(secondSrcDir.exists(QString((*it).fileName.c_str())+".so")) {
	UpdatePlugin::copyFile(secondSrcDir,QString((*it).fileName.c_str())+".so",dstDir,QString((*it).fileName.c_str())+".so."+QString((*it).version.c_str()).replace(" ",".")+".mac");
      }
      if(secondSrcDir.exists(QString((*it).fileName.c_str())+".dll")) {
	UpdatePlugin::copyFile(secondSrcDir,QString((*it).fileName.c_str())+".dll",dstDir,QString((*it).fileName.c_str())+".dll."+QString((*it).version.c_str()).replace(" ","."));
      }
    }
    // Documentation :
    QDir pluginsDocDir(docPath+"/doxygen/xml");

    QStringList filters;
    filters << "class*.xml";
    QStringList pluginsDocs = pluginsDocDir.entryList(filters); 
    QString libName = QString((*it).fileName.c_str()).split("-").first();
    libName=libName.split("lib").last();
    cout << "lib name : " << libName.toStdString() << endl;
  
    QList<QString>::iterator iter = pluginsDocs.begin();
    
    while(iter != pluginsDocs.end()){
      
      QString docFile(*iter);
      docFile = (docFile.mid(5,docFile.size()-9));
            
      if(docFile.compare(libName,Qt::CaseInsensitive)==0){
	QFile srcFile(docPath+"/doxygen/xml" + "/" + (*iter));
	QFile dstFile(dstDir.absolutePath()+"/"+QString((*it).fileName.c_str())+".doc."+QString((*it).version.c_str()).replace(" ","."));
	srcFile.open(QIODevice::ReadOnly | QIODevice::Text);
	dstFile.open(QIODevice::WriteOnly | QIODevice::Text);

	QString srcStr(srcFile.readAll());
	QString endStr("</detaileddescription>");
	int beginPos = srcStr.lastIndexOf("<briefdescription>");
	int endPos = srcStr.lastIndexOf(endStr);
	
	QTextStream out(&dstFile);
	out << "<?xml version='1.0' encoding='UTF-8' standalone='no'?>" << "\n" << "<doc>" << "\n";
	out << srcStr.mid(beginPos,(endPos+endStr.length())-beginPos);
	out << "</doc>" << "\n" ;
	
	//QFile pluginDocFile(docPath+"/doxygen/xml" + "/" + (*iter));
	//UpdatePlugin::copyFile(docPath+"/doxygen/xml" + "/",(*iter),dstDir,QString((*it).fileName.c_str())+".doc."+QString((*it).version.c_str()).replace(" ","."));
      }
      ++iter;
    }
    }
    
    
  return 0;
  
  }
