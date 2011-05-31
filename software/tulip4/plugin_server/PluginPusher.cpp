#include <stdlib.h>

#include "WebDavManager.h"
#include <QDir>
#include <QDomDocument>
#include <tulip/PluginManager.h>

using namespace std;

void uploadfolder(const QString& origin, WebDavManager& manager) {
  if(!manager.folderExists(origin)) {
      manager.mkdir(origin);
  }

  QDir originDir(origin);
  foreach(const QString& element, originDir.entryList(QDir::Files | QDir::NoSymLinks)) {
    QFile file(originDir.canonicalPath() + "/" + element);
    bool opened = file.open(QIODevice::ReadOnly);
    if(opened) {
      manager.putFile(origin + "/" + element, &file);
      file.close();
    }
    else {
      std::cout << "could not open file! :(" << std::endl;
    }
  }
  foreach(const QString& element, originDir.entryList(QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot)) {
    uploadfolder(origin + "/" + element, manager);
  }
}

int main(int argc, char** argv) {
  if(argc < 4) {
    std::cout << "pushPlugin pluginsPath serverURL base64EncodedCredentials" << std::endl;
    cout << "destinationDir defaults to the current dir" << endl;
    exit(0);
  }

  QCoreApplication app(argc, argv);

  QString path = argv[1];
  QString serverURL = argv[2];
  QString credentials = argv[3];

  WebDavManager manager("webdav." + serverURL, "/perso/huet", credentials);

  QFile description(path + "/serverDescription.xml");
  if(!description.open(QIODevice::ReadOnly)) {
    std::cout << "could not open " << path.toStdString() << "/serverDescription.xml in Read mode." << std::endl;
    exit(0);
  }
  QString localDescription(description.readAll());

  QString location = "http://www." + serverURL + "/perso/huet/" + path;
  
  QString remoteDescription = tlp::PluginManager::getPluginServerDescription(location);

  QStringList pluginList;
  
  QDomDocument localDocument;
  localDocument.setContent(localDescription);
  QDomNodeList localPlugins(localDocument.elementsByTagName("plugin"));
  for(int i = 0; i < localPlugins.count(); ++i) {
    QDomNode currentNode(localPlugins.at(i));
    QDomElement currentElement(currentNode.toElement());
    QString pluginName(currentElement.attribute("name"));
    pluginList << pluginName;
  }

  if(!remoteDescription.isNull()) {
    QDomDocument remoteDocument;
    remoteDocument.setContent(remoteDescription);
    std::cout << remoteDescription.toStdString() << std::endl;
    QDomNodeList remotePlugins(remoteDocument.elementsByTagName("plugin"));
    for(int i = 0; i < remotePlugins.count(); ++i) {
      QDomNode currentNode(remotePlugins.at(i));
      QDomElement currentElement(currentNode.toElement());
      QString pluginName(currentElement.attribute("name"));

      if(!pluginList.contains(pluginName)) {
        localDocument.documentElement().appendChild(currentNode);
        std::cout << "keeping remote plugin: " << pluginName.toStdString() << std::endl;
      }
      else {
        std::cout << "discarding remote plugin: " << pluginName.toStdString() << std::endl;
      }
    }
  }
//Debug output
//   QFile outputXML("serverDescription.xml");
//   outputXML.open(QIODevice::ReadWrite);
//   outputXML.write(localDocument.toByteArray());
//   outputXML.close();
  description.close();
  description.open(QIODevice::ReadWrite | QIODevice::Truncate);
  description.write(localDocument.toByteArray());
  description.close();

  uploadfolder(path, manager);
}
