#include <libxml/tree.h>
#include <libxml/parser.h>

#include <PluginInfo.h>

#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QTextStream>

using namespace std;
using namespace tlp;

class AddToDomDocument {

  xmlNodePtr rootNode;

public :

 AddToDomDocument(xmlNodePtr rootNode):rootNode(rootNode){};

  void operator() (const tlp::PluginDependency& dep) {
    xmlNodePtr depNode = xmlNewChild(rootNode,NULL, BAD_CAST "dependency",NULL);
    xmlNewProp(depNode, BAD_CAST "name", BAD_CAST dep.name.c_str());
    xmlNewProp(depNode, BAD_CAST "type", BAD_CAST dep.type.c_str());
    xmlNewProp(depNode, BAD_CAST "version", BAD_CAST dep.version.c_str());
    
  }

};

bool generatePluginInfoFile(LocalPluginInfo& pInfo, QDir& dstDir) {
  xmlDocPtr doc = NULL;       /* document pointer */
  xmlNodePtr rootNode = NULL;
    
  doc = xmlNewDoc(BAD_CAST "1.0");
    
  rootNode = xmlNewNode(NULL, BAD_CAST "pluginInfo");
  xmlDocSetRootElement(doc, rootNode);
    
  xmlNewProp(rootNode, BAD_CAST "name", BAD_CAST pInfo.name.c_str());
  xmlNewProp(rootNode, BAD_CAST "type", BAD_CAST pInfo.type.c_str());
  xmlNewProp(rootNode, BAD_CAST "displayType", BAD_CAST pInfo.displayType.c_str());
  xmlNewProp(rootNode, BAD_CAST "author", BAD_CAST pInfo.author.c_str());
  xmlNewProp(rootNode, BAD_CAST "date", BAD_CAST pInfo.date.c_str());
  xmlNewProp(rootNode, BAD_CAST "info", BAD_CAST pInfo.info.c_str());
  xmlNewProp(rootNode, BAD_CAST "fileName", BAD_CAST pInfo.fileName.c_str());
  xmlNewProp(rootNode, BAD_CAST "version", BAD_CAST pInfo.version.c_str());
    
  AddToDomDocument mapf(rootNode);
  for_each(pInfo.dependencies.begin(),pInfo.dependencies.end(),mapf);

  xmlChar *xmlbuff;
  int buffersize;

  xmlDocDumpFormatMemory(doc, &xmlbuff, &buffersize, 1);
  xmlFreeDoc(doc);

  QFile pluginXmlFile(dstDir.absolutePath() + "/" + pInfo.fileName.c_str() + ".xml." +
		      QString(pInfo.version.c_str()).replace(" ","."));
  if(!pluginXmlFile.open(QIODevice::WriteOnly)) {
    cerr << "Error in write file : " << pluginXmlFile.fileName().toStdString() << endl;
    return false;
  }
  
  pluginXmlFile.write((char *)xmlbuff, buffersize);
  pluginXmlFile.close();
  xmlFree(xmlbuff);
  
  cout << "Plugin : " << pInfo.fileName.c_str() << " installed" << endl;

  return true;
}

void generatePluginDocFile(QString& pFileName, QString& pVersion, QFile& pDoxFile, QDir& dstDir) {
  QFile dstFile(dstDir.absolutePath()+ "/"+ pFileName + ".doc." + pVersion.replace(" ","."));
  pDoxFile.open(QIODevice::ReadOnly | QIODevice::Text);
  dstFile.open(QIODevice::WriteOnly | QIODevice::Text);

  QString srcStr(pDoxFile.readAll());
  QString endStr("</detaileddescription>");
  int beginPos = srcStr.lastIndexOf("<briefdescription>");
  int endPos = srcStr.lastIndexOf(endStr);
	
  QTextStream out(&dstFile);
  out << "<?xml version='1.0' encoding='UTF-8' standalone='no'?>" << "\n" << "<doc>" << "\n";
  out << srcStr.mid(beginPos,(endPos+endStr.length())-beginPos);
  out << "</doc>" << "\n" ;
}
