#include "tulip/QuaZIPFacade.h"

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>

#include <tulip/SimplePluginProgress.h>

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


bool zipDirContent(QDir &currentDir, QuaZip &archive, const QString &archivePath, tlp::PluginProgress *progress) {
  QFileInfoList entries = currentDir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst);
  QFileInfo info;
  progress->setComment(("Compressing directory " + currentDir.absolutePath()).toStdString());
  int i=0;
  progress->progress(i,entries.size());
  foreach(info, entries) {
    progress->progress(i++,entries.size());
    if (info.isDir()) { // Recurse in directories if they are different from . and ..
      QDir childDir(info.absoluteFilePath());
      QFileInfo childInfo(childDir.absolutePath());
      zipDirContent(childDir,archive,archivePath + childInfo.fileName() + QDir::separator(),progress);
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

bool QuaZIPFacade::zipDir(const QString &rootPath, const QString &archivePath, tlp::PluginProgress *progress) {
  QFileInfo rootInfo(rootPath);
  if (!rootInfo.exists() || !rootInfo.isDir())
    return false;
  QDir rootDir(rootPath);

  QuaZip archive(archivePath);
  if (!archive.open(QuaZip::mdCreate))
    return false;

  bool deleteProgress = false;
  if (!progress) {
    progress = new tlp::SimplePluginProgress;
    deleteProgress = true;
  }

  bool result = zipDirContent(rootDir,archive,"",progress);
  archive.close();

  if (deleteProgress) delete progress;

  return result;
}

bool QuaZIPFacade::unzip(const QString &rootPath, const QString &archivePath, tlp::PluginProgress *progress) {

  QFileInfo rootPathInfo(rootPath);
  if (rootPathInfo.exists() && !rootPathInfo.isDir())
    return false;
  QDir rootDir(rootPath);
  if (!rootDir.exists() && !rootDir.mkpath(rootPath))
    return false;

  QuaZip archive(archivePath);
  if (!archive.open(QuaZip::mdUnzip))
    return false;

  bool deleteProgress = false;
  if (!progress) {
    progress = new tlp::SimplePluginProgress;
    deleteProgress = true;
  }
  progress->setComment(("Uncompressing archive " + archivePath).toStdString());
  int i=0,n=archive.getEntriesCount();
  progress->progress(i,n);

  for (bool readMore = archive.goToFirstFile(); readMore; readMore = archive.goToNextFile()) {
    progress->progress(i++,n);

    QuaZipFile inFile(&archive);
    QuaZipFileInfo inInfo;
    inFile.getFileInfo(&inInfo);

    QFileInfo outInfo(rootDir.absoluteFilePath(inInfo.name));
    rootDir.mkpath(outInfo.absolutePath());

    QFile outFile(outInfo.absoluteFilePath());
    if (!outFile.open(QIODevice::WriteOnly) || !inFile.open(QIODevice::ReadOnly))
      return false;

    copy(inFile,outFile);
  }

  if (deleteProgress) delete progress;

  return true;
}

