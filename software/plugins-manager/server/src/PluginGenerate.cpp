/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
#include <algorithm>
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
    
  cout << "type : " << pInfo.type << " dptype : " << pInfo.displayType << endl;

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

  QFile pluginXmlFile(dstDir.absolutePath() + "/" + pInfo.fileName.c_str() + ".xml");
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

void generatePluginDocFile(QString& pFileName, QString&, QFile& pDoxFile, QDir& dstDir) {
  QFile dstFile(dstDir.absolutePath()+ "/"+ pFileName + ".doc");
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
