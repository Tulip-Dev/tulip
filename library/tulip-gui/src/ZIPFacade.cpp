/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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
#include "tulip/ZIPFacade.h"

#include <QDir>

#include <tulip/SimplePluginProgress.h>
#include <tulip/TlpQtTools.h>

#include <zip.h>

bool zipDirContent(QDir &currentDir, zip_t *zipArchive, const QString &zipPath,
                   tlp::PluginProgress *progress) {
  // get currentDir entries  (. and .. are excluded)
  QFileInfoList entries = currentDir.entryInfoList(QDir::NoDotAndDotDot | QDir::System |
                                                       QDir::Hidden | QDir::AllDirs | QDir::Files,
                                                   QDir::DirsFirst);
  progress->setComment(
      tlp::QStringToTlpString("Compressing directory " + currentDir.absolutePath()));
  int i = 0;
  progress->progress(i, entries.size());

  // loop on currentDir entries
  for (const QFileInfo &info : entries) {
    progress->progress(i++, entries.size());

    if (info.isDir()) {
      // Recurse in directories
      QDir childDir(info.absoluteFilePath());
      zipDirContent(childDir, zipArchive, zipPath + info.fileName() + "/", progress);
    } else {
      auto fileName = tlp::QStringToTlpString(info.absoluteFilePath());
      auto zipFileName = tlp::QStringToTlpString(zipPath + info.fileName());
      zip_source_t *infoSrc = zip_source_file(zipArchive, fileName.c_str(), 0, 0);
      if (infoSrc == nullptr) {
        tlp::warning() << "Failed to add file to zip: " << zip_strerror(zipArchive) << std::endl;
        return false;
      }
      if (zip_file_add(zipArchive, zipFileName.c_str(), infoSrc, ZIP_FL_ENC_UTF_8) < 0) {
        zip_source_free(infoSrc);
        tlp::warning() << "Failed to add file to zip: " << zip_strerror(zipArchive) << std::endl;
        return false;
      }
    }
  }

  return true;
}

bool ZIPFacade::zipDir(const QString &rootPath, const QString &zipPath,
                       tlp::PluginProgress *progress) {
  QFileInfo rootInfo(rootPath);

  if (!rootInfo.exists() || !rootInfo.isDir())
    return false;

  QDir rootDir(rootPath);

  int error;
  std::string archive = tlp::QStringToTlpString(zipPath);
  zip_t *zipArchive = zip_open(archive.c_str(), ZIP_CREATE | ZIP_TRUNCATE, &error);
  if (zipArchive == nullptr) {
    zip_error_t zipError;
    zip_error_init_with_code(&zipError, error);
    tlp::warning() << "Failed to open file " << archive << ": " << zip_error_strerror(&zipError)
                   << std::endl;
    return false;
  }

  bool deleteProgress = false;

  if (!progress) {
    progress = new tlp::SimplePluginProgress;
    deleteProgress = true;
  }

  bool result = zipDirContent(rootDir, zipArchive, "", progress);
  zip_close(zipArchive);

  if (deleteProgress)
    delete progress;

  return result;
}

bool ZIPFacade::unzip(const QString &rootPath, const QString &archivePath,
                      tlp::PluginProgress *progress) {

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

  if (!archiveFile.exists()) {
    progress->setError(tlp::QStringToTlpString("No such file: " + archivePath));
    return false;
  }

  int error;
  zip_t *zipArchive = zip_open(tlp::QStringToTlpString(archivePath).c_str(), ZIP_RDONLY, &error);
  if (zipArchive == nullptr) {
    zip_error_t zipError;
    zip_error_init_with_code(&zipError, error);
    tlp::warning() << "Failed to open file " << tlp::QStringToTlpString(archivePath) << ": "
                   << zip_error_strerror(&zipError) << std::endl;
    return false;
  }

  bool deleteProgress = false;

  if (!progress) {
    progress = new tlp::SimplePluginProgress;
    deleteProgress = true;
  }

  progress->setComment(tlp::QStringToTlpString("Uncompressing archive " + archivePath));
  int nbEntries = zip_get_num_entries(zipArchive, 0);
  zip_stat_t zipInfo;

  for (int i = 0; i < nbEntries; i++) {
    progress->progress(i, nbEntries);
    error = zip_stat_index(zipArchive, i, 0, &zipInfo);
    if (error < 0) {
      progress->setError("Failed to uncompress");
      tlp::warning() << "Failed to uncompress: " << zip_strerror(zipArchive) << std::endl;
      break;
    }

    // create file
    QFileInfo outInfo(rootDir.absoluteFilePath(zipInfo.name));
    rootDir.mkpath(outInfo.absolutePath());

    QFile outFile(outInfo.absoluteFilePath());

    if (!outFile.open(QIODevice::WriteOnly)) {
      progress->setError("Could not write in folder");
      error = -1;
      break;
    }

    // read from zip archive
    char *data = static_cast<char *>(malloc(zipInfo.size));
    zip_file_t *zipFile = zip_fopen_index(zipArchive, i, 0);
    zip_fread(zipFile, data, zipInfo.size);
    zip_fclose(zipFile);
    // write into file
    outFile.write(data, zipInfo.size);
    free(data);
  }

  zip_close(zipArchive);

  if (deleteProgress)
    delete progress;

  return error == 0;
}
