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
#include "PluginsHelp.h"

#include <cassert>

#include <QtCore/QDir>

#include <tulip/TulipRelease.h>
#include <tulip/TlpTools.h>

#include "PluginInfo.h"

using namespace std;

#if defined(_WIN32)
static const QString viewPluginsDirName=QString(tlp::PluginInfo::localTulipDirectory.c_str())+"/help/viewPluginsHandbook/";
static const QString helpDirName=QString(tlp::PluginInfo::localTulipDirectory.c_str())+"/help/";
static const QString installPluginsDirName=QString(tlp::PluginInfo::localTulipDirectory.c_str())+"/plugins/";
#else
static const QString viewPluginsDirName=QString(tlp::PluginInfo::localTulipDirectory.c_str())+"/help/viewPluginsHandbook/";
static const QString helpDirName=QString(tlp::PluginInfo::localTulipDirectory.c_str())+"/help/";
static const QString installPluginsDirName=QString(tlp::PluginInfo::localTulipDirectory.c_str())+"/plugins/";
#endif

static QByteArray toByteArray(const qint64 &number){
	QByteArray byteArray;
	byteArray.fill('0',20-QByteArray::number(number).size());
	byteArray.append(QByteArray::number(number));
	return byteArray;
}

static qint64 fromByteArray(QByteArray &byteArray){
	QByteArray tmp=byteArray.left(20);
	byteArray=byteArray.mid(20,-1);
	while(tmp.startsWith('0'))
		tmp.remove(0,1);
	qint64 result=tmp.toLong();
	return result;
}

namespace tlp {

void PluginsHelp::appendFiles(const QString &baseDirectory,const QString &pluginType,const QStringList &filesList,QByteArray &outArray){
	outArray.clear();
	outArray.append(QString("HelpDoc").toLatin1());
	outArray.append(toByteArray(pluginType.size()));
	outArray.append(pluginType.toLatin1());
	outArray.append(toByteArray(filesList.size()));
	for(QStringList::const_iterator it=filesList.begin();it!=filesList.end();++it){
		QString fileName=(*it);
		fileName.replace(baseDirectory,"./");
		QFile file(*it);
		file.open(QIODevice::ReadOnly);
		outArray.append(toByteArray(fileName.size()));
		outArray.append(fileName.toLatin1());
		outArray.append(toByteArray(file.size()));
		outArray.append(file.readAll());
		file.close();
	}
}

QString PluginsHelp::getPluginType(const QByteArray &inArray){
	QByteArray inArrayTmp=inArray;
	QString check(inArrayTmp.left(7));
	if(check!="HelpDoc")
		return "";
	inArrayTmp=inArrayTmp.mid(7,-1);

	int typeLength=0;
	typeLength=fromByteArray(inArrayTmp);
	QString type(inArrayTmp.left(typeLength));
	return type;
}

bool PluginsHelp::partFiles(const QByteArray &inArray,const QString &baseDirectory){
	QByteArray inArrayTmp=inArray;
	QString check(inArrayTmp.left(7));
	if(check!="HelpDoc")
		return false;
	inArrayTmp=inArrayTmp.mid(7,-1);

	int typeLength=0;
	typeLength=fromByteArray(inArrayTmp);
	inArrayTmp=inArrayTmp.mid(typeLength,-1);

	int filesNumber;
	filesNumber=fromByteArray(inArrayTmp);

	for(int i=0;i<filesNumber;i++){
		int fileNameSize;
		fileNameSize=fromByteArray(inArrayTmp);

		QString fileName;
		fileName=QString(inArrayTmp.left(fileNameSize));
		inArrayTmp=inArrayTmp.mid(fileNameSize,-1);
		QDir baseDir(baseDirectory);
		baseDir.mkpath(fileName.left(fileName.lastIndexOf('/')));
		fileName.replace("./",baseDirectory);

		qint64 fileSize;
		fileSize=fromByteArray(inArrayTmp);

		QFile file(fileName);
		file.open(QIODevice::WriteOnly);
		file.write(inArrayTmp.left(fileSize));
		file.close();
		inArrayTmp=inArrayTmp.mid(fileSize,-1);
	}

	return true;
}

QString PluginsHelp::minimumHtmlFile(const QString &title) {
	int pos = TulipLibDir.length() - 2;
	pos = TulipLibDir.rfind("/", pos);

	return QString("<html>\n")+
			"  <head>\n"+
			"    <meta http-equiv=\"Content-Type\" content=\"text/html; charset=ISO-8859-1\">\n"+
			"      <title>"+title+"</title>\n"+
			"      <link rel=\"stylesheet\" href=\""+TulipLibDir.substr(0, pos + 1).c_str()+"share/tulip/common/tulip-default.css"+"\" type=\"text/css\">\n"+
			"      <link rel=\"start\" href=\"index.html\" title=\""+title+"\">\n"+
			"  </head>\n"+
			"  <body bgcolor=\"white\" text=\"black\" link=\"#0000FF\" vlink=\"#840084\" alink=\"#0000FF\">\n"+
			"    <div class=\"navheader\">\n"+
			"      <table width=\"100%\" summary=\"Navigation header\"><tr><th colspan=\"3\" align=\"center\">Tulip Views Manuel</th></tr><tr></tr></table>\n"+
			"      <hr></div><div class=\"book\" lang=\"en\"><div class=\"titlepage\"><div><div><h1 class=\"title\"><a name=\"d0e2\"></a>Tulip Views Manuel</h1></div></div><hr></div><div class=\"toc\"><p><b>Table of Contents</b></p>\n"+
			"      </div></body>\n"+
			"</html>\n";
}

void PluginsHelp::addInFile(const QString &fileName,const QString &textToAdd, const QString &textAfter) {
	QFile file(fileName);
	file.open(QIODevice::ReadWrite);
	QString baseText(file.readAll());
	int index=baseText.indexOf(textAfter);
	assert(index!=-1);
	baseText.insert(index,textToAdd);
	file.reset();
	file.write(baseText.toLatin1());
	file.close();
}

void PluginsHelp::removeInFile(const QString &fileName,const QString &textToRemove) {
	QFile file(fileName);
	file.open(QIODevice::ReadWrite);
	QString baseText(file.readAll());
	baseText.remove(textToRemove);
	file.reset();
	file.resize(0);
	file.write(baseText.toLatin1());
	file.close();
}

void PluginsHelp::checkViewHelp(){
	//check directory
	QDir viewPluginsDir(QDir::homePath()+"/"+viewPluginsDirName);
	if(!viewPluginsDir.exists()){
		viewPluginsDir=QDir(QDir::homePath());
		viewPluginsDir.mkpath(viewPluginsDirName);
		viewPluginsDir.cd(viewPluginsDirName);
	}

	//check if index.html file exist
	if(!viewPluginsDir.exists("index.html")){
		QString indexContent=minimumHtmlFile("View Plugins Help");
		QFile indexHtml(viewPluginsDir.absolutePath()+"/index.html");
		indexHtml.open(QIODevice::WriteOnly);
#ifndef NDEBUG
		int sizeWrited=indexHtml.write(indexContent.toStdString().c_str(),indexContent.size());
		assert(sizeWrited==indexContent.size());
#else
		indexHtml.write(indexContent.toStdString().c_str(),indexContent.size());
#endif
	}

	//check view help in profile.adp
	int pos = TulipLibDir.length() - 2;
	pos = TulipLibDir.rfind("/", pos);
	QFile profileAdp(QString(TulipLibDir.substr(0, pos + 1).c_str())+"share/tulip/profile.adp");
	if(profileAdp.exists()){
		profileAdp.open(QIODevice::ReadWrite);
		QString profileAdpStr(profileAdp.readAll());
		if(!profileAdpStr.contains("<dcf ref=\""+viewPluginsDir.absolutePath()+"/index.html"+"\" title=\"Tulip view plugins documentation\">")){
			profileAdpStr.insert(profileAdpStr.indexOf("</assistantconfig>"),"<dcf ref=\""+viewPluginsDir.absolutePath()+"/index.html"+"\" title=\"Tulip view plugins documentation\">\n</dcf>");
		}
		profileAdp.reset();
		profileAdp.write(profileAdpStr.toLatin1());
		profileAdp.close();
	}
}

void PluginsHelp::installHelpDoc(const QString &fileName){
	QString dirName=fileName.left(fileName.lastIndexOf('.'));
	QFile file(QDir::homePath()+"/"+installPluginsDirName+"toInstall/"+fileName);
	bool isOpen=file.open(QIODevice::ReadOnly);
	if (isOpen) {
		QByteArray byteArray=file.readAll();
		file.close();

		QString pluginType=getPluginType(byteArray);
		if(pluginType=="")
			return;

		QString helpBaseDirectory;
		if(pluginType=="View"){
			helpBaseDirectory=QDir::homePath()+"/"+viewPluginsDirName;
		}else{
			assert(false);
		}

		QDir baseDirTmp(helpBaseDirectory+dirName+"/");
		if(!baseDirTmp.exists()){
			baseDirTmp = QDir(helpBaseDirectory);
			baseDirTmp.mkpath(dirName);
		}
		partFiles(byteArray,helpBaseDirectory+dirName+"/");

		if(pluginType=="View"){
			QString str="<dl><dt><span class=\"chapter\"><a href=\""+dirName+"/html/index.html"+"\">"+dirName+"</a></span></dt></dl>";
			addInFile(helpBaseDirectory+"/index.html",str,"</div></body>");

			QFile typeFile(QDir::homePath()+"/"+helpDirName+dirName+".type");
			typeFile.open(QIODevice::WriteOnly);
			typeFile.write(QString("View").toLatin1());
			typeFile.close();
		}
	}
}

void PluginsHelp::removeHelpDoc(const QString &fileName){
	QString dirName=fileName.left(fileName.lastIndexOf('.'));
	dirName=dirName.right(dirName.size()-dirName.lastIndexOf('/')-1);

	QFile typeFile(QDir::homePath()+"/"+helpDirName+dirName+".type");
	typeFile.open(QIODevice::ReadOnly);
	QString pluginType(typeFile.readAll());
	typeFile.close();


	QString helpBaseDirectory;
	if(pluginType=="View"){
		helpBaseDirectory=QDir::homePath()+"/"+viewPluginsDirName;
	}

	if(pluginType=="View"){
		QString str="<dl><dt><span class=\"chapter\"><a href=\""+dirName+"/html/index.html"+"\">"+dirName+"</a></span></dt></dl>";
		removeInFile(helpBaseDirectory+"/index.html",str);
	}

}
}
