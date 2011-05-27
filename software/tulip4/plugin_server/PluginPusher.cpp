#include <stdlib.h>

#include "WebDavManager.h"
#include <QDir>

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

  WebDavManager manager(serverURL, "/perso/huet", credentials);

  uploadfolder(path, manager);
}
