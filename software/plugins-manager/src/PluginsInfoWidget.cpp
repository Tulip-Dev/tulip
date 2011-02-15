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
#include <iostream>

#include <QtCore/QFile>
#include <QtCore/QTextStream>


#include "PluginsInfoWidget.h"
#include "Request.h"
#include "Server.h"
#include "MultiServerManager.h"

using namespace std;

namespace tlp {

  PluginsInfoWidget::PluginsInfoWidget(MultiServerManager *multiServerManager,QWidget *parent) : QWidget(parent),multiServerManager(multiServerManager){
    setupUi(this);
  }

  void PluginsInfoWidget::setPluginInfo(const PluginInfo *plugin, string addrWS){
    //poll the webservice for plugin info
    if(plugin->local){
      LocalPluginInfo *localPlugin=(LocalPluginInfo*)plugin;
      //TODOproblem with local pluginName, check pluginInfo
      QString pluginFileNameQStr=localPlugin->fileName.c_str();

      string pluginInfoPath(string(tlp::TulipLibDir+"tlp/")+pluginFileNameQStr.split("/").last().toStdString()+".doc");
      QFile pluginInfoFile(pluginInfoPath.c_str());
      

      if (!pluginInfoFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        CompletePluginsList pluginsList;
        multiServerManager->getPluginsList(pluginsList);
        for(CompletePluginsList::iterator it=pluginsList.begin();it!=pluginsList.end();++it){
          if((*it).first->name==localPlugin->name && !(*it).first->local){
            string addr;
            multiServerManager->getAddr((*it).first->server,addr);
            setPluginInfo((*it).first,addr);
          }
        }
        return;
      }

      vector<string> dependencies;
      for(vector<PluginDependency>::iterator it=localPlugin->dependencies.begin();it!=localPlugin->dependencies.end();++it) {
        dependencies.push_back((*it).name);
      }

      formatInfo(localPlugin->name,localPlugin->author,localPlugin->type,localPlugin->date,localPlugin->info,localPlugin->version,dependencies);
      addPluginDocXML(QString(pluginInfoFile.readAll()).toStdString());

      textBrowser->setText((htmlBegin+pluginInfoXML+pluginDocXML+htmlEnd).c_str());

    }else{
      DistPluginInfo *distPlugin=(DistPluginInfo*)plugin;
      Server* sm = new Server(addrWS);

      string versionStr=distPlugin->version;
      versionStr=versionStr.substr(versionStr.rfind(" ")+1,versionStr.size()-versionStr.rfind(" ")-1);

      GetPluginInfoTreatment *infoTreat = new GetPluginInfoTreatment(this);
      GetPluginInfoRequest* infoReq = new GetPluginInfoRequest(distPlugin->fileName,versionStr,infoTreat);
      sm->send(infoReq);

      GetPluginDocTreatment *docTreat = new GetPluginDocTreatment(this);
      GetPluginDocRequest* docReq = new GetPluginDocRequest(distPlugin->fileName,versionStr,docTreat);
      sm->send(docReq);
    }
  }

  bool PluginsInfoWidget::haveInfo(const PluginInfo *plugin) {
    if(!plugin->local)
      return true;
    else {
      LocalPluginInfo *localPlugin=(LocalPluginInfo*)plugin;
      QString pluginFileNameQStr=localPlugin->fileName.c_str();

      string pluginInfoPath(string(tlp::TulipLibDir+"tlp/")+pluginFileNameQStr.split("/").last().toStdString()+".doc");
      QFile pluginInfoFile(pluginInfoPath.c_str());

      return pluginInfoFile.exists();
    }
  }

  string PluginsInfoWidget::formatXMLDoc(QDomElement elt){
    QString str;
    QDomNode n = elt.firstChild();//elt.firstChild();

    while(!n.isNull()) {

      QDomElement xmlElem = n.toElement();
      if(!xmlElem.isNull()) {
        if(xmlElem.tagName() == "detaileddescription"){
          QTextStream stream(&str);
          xmlElem.save(stream,2);
          str.remove("<detaileddescription>");
          str.remove("</detaileddescription>");
          str.remove("\n");
          str.replace("<para>","<p>");
          str.replace("</para>","</p>");
          str.replace("<itemizedlist>","<ul>");
          str.replace("</itemizedlist>","</ul>");
          str.replace("<listitem>","<li>");
          str.replace("</listitem>","</li>");
          str.replace("<bold>","<b>");
          str.replace("</bold>","</b>");
          str.replace("<simplesect","<blockquote><");
          str.replace("</simplesect>","</blockquote><br>");
          str.replace("<linebreak/>", "<br>");
        }
      }
      n = n.nextSibling();
    }
    return str.toStdString();
  }

  void PluginsInfoWidget::formatInfo(const string &name,const string &author,const string &type,const string &date,const string &info,const string &version,const vector<string>&){
    nameLabel->setText(name.c_str());
    authorLabel->setText(author.c_str());
    typeLabel->setText(type.c_str());
    dateLabel->setText(date.c_str());
    versionInfoLabel->setText(info.c_str());
    versionLabel->setText(version.c_str());

    /*string pluginInfoStr("");

    pluginInfoStr += string("<table border='1'>");
    pluginInfoStr += string("<tr><td>");
    pluginInfoStr += string(" Name ");
    pluginInfoStr += string("</td><td>");
    pluginInfoStr += name;
    pluginInfoStr += string("</td></tr>");

    pluginInfoStr += string("<tr><td>");
    pluginInfoStr += string(" Author ");
    pluginInfoStr += string("</td><td>");
    pluginInfoStr += author;
    pluginInfoStr += string("</td></tr>");

    pluginInfoStr += string("<tr><td>");
    pluginInfoStr += string(" Type : ");
    pluginInfoStr += string("</td><td>");
    pluginInfoStr += type;
    pluginInfoStr += string("</td></tr>");

    pluginInfoStr += string("<tr><td>");
    pluginInfoStr += string(" Date : ");
    pluginInfoStr += string("</td><td>");
    pluginInfoStr += date;
    pluginInfoStr += string("</td></tr>");

    pluginInfoStr += string("<tr><td>");
    pluginInfoStr += string(" Info : ");
    pluginInfoStr += string("</td><td>");
    pluginInfoStr += info;
    pluginInfoStr += string("</td></tr>");

    pluginInfoStr += string("<tr><td>");
    pluginInfoStr += string(" Version : ");
    pluginInfoStr += string("</td><td>");
    pluginInfoStr += version;
    pluginInfoStr += string("</td></tr>");

    if(dependencies.size()!=0) {
      pluginInfoStr += string("<tr><td>");
      pluginInfoStr += string(" Dependency : ");
      pluginInfoStr += string("</td><td>");

      for(vector<string>::const_iterator it=dependencies.begin();it!=dependencies.end();++it) {
	if(it!=dependencies.begin())
	  pluginInfoStr += "<br>";
	pluginInfoStr += (*it);
      }
      pluginInfoStr += string("</td></tr>");
    }

    pluginInfoStr += string("</table><br><br>");

    pluginInfoXML = pluginInfoStr;*/

  }

  void PluginsInfoWidget::addPluginInfoXML(string xml){
    //  doc.appendChild(pluginNode);
    QDomDocument pluginInfoXMLDom("PluginInfoXML");
    string pluginInfoXMLStr("");

    pluginInfoXMLDom.setContent(QString(xml.c_str()));

    QDomElement pluginInfoXMLElem = pluginInfoXMLDom.documentElement();

    string name = pluginInfoXMLElem.attribute("name").toStdString();
    string author = pluginInfoXMLElem.attribute("author").toStdString();
    string type = pluginInfoXMLElem.attribute("type").toStdString();
    string date = pluginInfoXMLElem.attribute("date").toStdString();
    string info = pluginInfoXMLElem.attribute("info").toStdString();
    string version = pluginInfoXMLElem.attribute("version").toStdString();

    vector<string> dependencies;

    QDomNode n = pluginInfoXMLElem.firstChild();

    while(!n.isNull()) {
      QDomElement xmlElem = n.toElement();
      if(!xmlElem.isNull()) {
        if(xmlElem.tagName() == "dependency"){
          dependencies.push_back(xmlElem.attribute("name").toStdString());
        }
      }
      n = n.nextSibling();
    }

    formatInfo(name,author,type,date,info,version,dependencies);
  }

  void PluginsInfoWidget::addPluginDocXML(string xml){

    //  doc.appendChild(pluginNode);
    QDomDocument pluginInfoXMLDom("PluginInfoXML");
    string pluginInfoXMLStr("");

    pluginInfoXMLDom.setContent(QString(xml.c_str()));

    QDomElement pluginInfoXMLElem = pluginInfoXMLDom.documentElement();

    if(!pluginInfoXMLElem.isNull()) {
      if(pluginInfoXMLElem.tagName() == "doc"){
        pluginInfoXMLStr += formatXMLDoc(pluginInfoXMLElem);
      }
    }

    pluginDocXML = pluginInfoXMLStr;
  }

}
