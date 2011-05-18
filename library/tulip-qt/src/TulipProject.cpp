#include "tulip/TulipProject.h"

#include <QtCore/QDir>
#include <QtCore/QCoreApplication>
#include <QtXml/QDomDocument>
#include <stdlib.h>
#include <assert.h>
#include <quazip.h>
#include <quazipfile.h>

#define DATA_DIR_NAME "data"
#define INFOS_FILE_NAME "project.xml"
#define TLPPROJ_VERSION "1.0"

void copy(QIODevice &in,QIODevice &out) {
  char buffer[40960];
  int cnt;
  while ((cnt = in.read(buffer,40960)))
    out.write(buffer,cnt);

  in.close();
  out.close();
}


bool zipDirContent(QDir &currentDir, QuaZip &archive, const QString &archivePath) {
  QDir parentDir(currentDir);
  parentDir.cdUp();

  QFileInfoList entries = currentDir.entryInfoList();
  for (QFileInfoList::iterator it = entries.begin(); it != entries.end(); ++it) {
    QFileInfo info(*it);

    if (info.isDir()) { // Recurse in directories if they are different from . and ..
      QDir childDir(info.absoluteFilePath());
      if (childDir.absolutePath() != currentDir.absolutePath() && childDir.absolutePath() != parentDir.absolutePath()) {
        QFileInfo childInfo(childDir.absolutePath());
        zipDirContent(childDir,archive,archivePath + childInfo.fileName() + QDir::separator());
      }
    }

    else {
      QuaZipFile outFile(&archive);
      QuaZipNewInfo newFileInfo(archivePath + info.fileName(),info.absoluteFilePath());
      newFileInfo.externalAttr = 0x81fd0000;
      QFile inFile(info.absoluteFilePath());

      if (!outFile.open(QIODevice::WriteOnly,newFileInfo) || !inFile.open(QIODevice::ReadOnly))
        return false;

      copy(inFile,outFile);

      if(outFile.getZipError()!=UNZ_OK)
        return false;
    }
  }
  return true;
}

bool zipDir(const QString &rootPath, const QString &archivePath) {
  QFileInfo rootInfo(rootPath);
  if (!rootInfo.exists() || !rootInfo.isDir())
    return false;
  QDir rootDir(rootPath);

  QuaZip archive(archivePath);
  if (!archive.open(QuaZip::mdCreate))
    return false;

  bool result = zipDirContent(rootDir,archive,"");
  archive.close();
  return result;
}

bool unzip(const QString &rootPath, const QString &archivePath) {
  QFileInfo rootPathInfo(rootPath);
  if (rootPathInfo.exists() && !rootPathInfo.isDir())
    return false;
  QDir rootDir(rootPath);
  if (!rootDir.exists() && !rootDir.mkpath(rootPath))
    return false;

  QuaZip archive(archivePath);
  if (!archive.open(QuaZip::mdUnzip))
    return false;

  for (bool readMore = archive.goToFirstFile(); readMore; readMore = archive.goToNextFile()) {
    QuaZipFile inFile(&archive);
    QuaZipFileInfo inInfo;
    inFile.getFileInfo(&inInfo);

    QFileInfo outInfo(rootDir.absoluteFilePath(inInfo.name));
    if (!rootDir.mkpath(outInfo.absolutePath()))
      return false;

    QFile outFile(outInfo.absoluteFilePath());
    if (!outFile.open(QIODevice::WriteOnly) || !inFile.open(QIODevice::ReadOnly))
      return false;

    copy(inFile,outFile);
  }
}

namespace tlp {

TulipProject::TulipProject() {
  abort(); // This private constructer should never been called. It has been privately declared to prevent use of default constructor
}

TulipProject::TulipProject(const QString &path)
  : _rootPath(path),
    _dataPath(_rootPath + QDir::separator() + DATA_DIR_NAME),
    _metaInfosFile(_rootPath + QDir::separator() + INFOS_FILE_NAME) {
}

TulipProject *TulipProject::newProject(int *errorCode) {
  QString rootPath = temporaryPath();
  QDir rootDir(temporaryPath());
  bool dirOk = rootDir.mkpath(rootPath) && rootDir.mkdir(DATA_DIR_NAME);
  if (!dirOk) {
    if (errorCode)
      *errorCode = CREATE_FILE_FAIL;
    return NULL;
  }
  return new TulipProject(rootPath);
}

TulipProject *TulipProject::openProject(const QString &file,int *errorCode, tlp::PluginProgress *progress) {
  TulipProject *project = TulipProject::newProject(errorCode);
  if (!project)
    return NULL;
  // Extract ZIP
}

int TulipProject::write(const QString &file, tlp::PluginProgress *progress) {

}

// ==============================
//      FILES MANIPULATION
// ==============================
QStringList TulipProject::entryList(const QString &path) {
}

bool TulipProject::isDir(const QString &path) {
}

bool TulipProject::removeFile(const QString &path) {
}

bool TulipProject::removeDir(const QString &path) {
}

std::fstream *TulipProject::stdFileStream(const QString &path) {
}

QIODevice *TulipProject::fileStream(const QString &path) {
}

// ==============================
//      META-INFORMATIONS
// ==============================
QString TulipProject::name() const {
}

void TulipProject::setName(const QString &n) {

}

QString TulipProject::description() const {

}
void TulipProject::setDescription(const QString &d) {

}

QString TulipProject::author() const {

}
void TulipProject::setAuthor(const QString &a) {

}

QString TulipProject::perspective() const {

}
void TulipProject::setPerspective(const QString &p) {

}

QDate TulipProject::date() const {

}

QString TulipProject::version() const {

}

// Some hack: Qt does not provide method to create temporary DIRS.
QString TulipProject::temporaryPath() {
  QString basePath(QDir::tempPath() + QDir::separator() + QCoreApplication::applicationName() + "-" + QString::number(QCoreApplication::applicationPid()) + "-");
  int prefix=0;
  QString result;
  do {
    result = basePath + QString::number(prefix++);
  } while (QDir(result).exists());
  return result;
}


}
