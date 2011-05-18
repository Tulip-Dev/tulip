#include "QuaZIPFacade.h"

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>

#include "quazip.h"
#include "quazipfile.h"

QuaZIPFacade::QuaZIPFacade() {
}

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

bool QuaZIPFacade::zipDir(const QString &rootPath, const QString &archivePath) {
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

bool QuaZIPFacade::unzip(const QString &rootPath, const QString &archivePath) {
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

  return true;
}

