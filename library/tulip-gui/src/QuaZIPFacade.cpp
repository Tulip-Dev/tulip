/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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
#include "tulip/QuaZIPFacade.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>

#include <tulip/SimplePluginProgress.h>

#include <quazip.h>
#include <quazipfile.h>

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
  QFileInfoList entries = currentDir.entryInfoList(QDir::NoDot | QDir::NoDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst);
  QFileInfo info;
  progress->setComment(("Compressing directory " + currentDir.absolutePath()).toStdString());
  int i=0;
  progress->progress(i,entries.size());
  foreach(info, entries) {
    progress->progress(i++,entries.size());

    if (info.isDir()) { // Recurse in directories if they are different from . and ..
      QDir childDir(info.absoluteFilePath());
      QFileInfo childInfo(childDir.absolutePath());
      zipDirContent(childDir,archive,archivePath + childInfo.fileName() + "/",progress);
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

  if (rootPathInfo.exists() && !rootPathInfo.isDir()) {
    progress->setError("Root path does not exists or is not a dir");
    return false;
  }

  QDir rootDir(rootPath);

  if (!rootDir.exists() && !rootDir.mkpath(rootPath)) {
    progress->setError("Could not create root path");
    return false;
  }

  QFile archiveFile(archivePath);

  if(!archiveFile.exists()) {
    progress->setError(QString("No such file : " + archivePath).toUtf8().data());
    return false;
  }

  QuaZip archive(archivePath);

  if (!archive.open(QuaZip::mdUnzip)) {
    progress->setError("Could not open archive");
    return false;
  }

  bool deleteProgress = false;

  if (!progress) {
    progress = new tlp::SimplePluginProgress;
    deleteProgress = true;
  }

  progress->setComment(("Uncompressing archive " + archivePath).toUtf8().data());
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

    if (!outFile.open(QIODevice::WriteOnly) || !inFile.open(QIODevice::ReadOnly)) {
      progress->setError("Could not write in folder or could not read from file");
      return false;
    }

    copy(inFile,outFile);
  }

  if (deleteProgress) delete progress;

  return true;
}

