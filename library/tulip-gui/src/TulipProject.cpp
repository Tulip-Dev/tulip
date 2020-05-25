/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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

#include "tulip/TulipProject.h"

#include <QMetaProperty>
#include <QDir>
#include <QCoreApplication>
#include <QTextStream>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

#include <tulip/SimplePluginProgress.h>
#include <tulip/QuaZIPFacade.h>
#include <tulip/TlpQtTools.h>

#include <cstdlib>

#define DATA_DIR_NAME "data"
#define INFO_FILE_NAME "project.xml"
#define TLPPROJ_VERSION "1.0"

namespace tlp {

TulipProject::TulipProject(QTemporaryDir *path) : _rootDir(path) {}

TulipProject::~TulipProject() {
  delete _rootDir;
}

TulipProject *TulipProject::newProject() {
  QTemporaryDir *tempdir = new QTemporaryDir();
  bool dirOk = tempdir->isValid() && QDir(tempdir->path()).mkdir(DATA_DIR_NAME);

  if (!dirOk) {
    std::string err =
        "Failed to create a temporary path " + tlp::QStringToTlpString(tempdir->path());
#if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
    err += ": " + tlp::QStringToTlpString(tempdir->errorString());
#endif
    tlp::error() << err << std::endl;
    delete tempdir;
    return nullptr;
  }

  return new TulipProject(tempdir);
}

bool TulipProject::openProjectFile(const QString &file, tlp::PluginProgress *progress) {

  QFileInfo fileInfo(file);

  if (!fileInfo.exists()) {
    progress->setError("File " + tlp::QStringToTlpString(file) + " not found");
    return false;
  }

  if (fileInfo.isDir()) {
    progress->setError(tlp::QStringToTlpString(file) + " is a directory, not a regular file");
    return false;
  }

  bool deleteProgress = false;

  if (!progress) {
    progress = new tlp::SimplePluginProgress;
    deleteProgress = true;
  }

  if (!QuaZIPFacade::unzip(rootDir(), file, progress)) {
    progress->setError("Failed to unzip project.");

    if (deleteProgress)
      delete progress;

    return false;
  }

  readMetaInfo();

  if (deleteProgress)
    delete progress;

  _projectFile = file;
  emit projectFileChanged(file);
  return true;
}

TulipProject *TulipProject::openProject(const QString &file, tlp::PluginProgress *progress) {
  TulipProject *project = TulipProject::newProject();

  if (project != nullptr) {
    if (!project->openProjectFile(file, progress)) {
      return nullptr;
    }
  }
  return project;
}

bool TulipProject::write(const QString &file, tlp::PluginProgress *progress) {
  bool deleteProgress = false;

  if (!progress) {
    progress = new tlp::SimplePluginProgress;
    deleteProgress = true;
  }

  if (!writeMetaInfo()) {
    progress->setError("Failed to save meta-information.");
    return false;
  }

  if (!QuaZIPFacade::zipDir(rootDir(), file)) {
    progress->setError("Failed to zip project.");
    return false;
  }

  if (deleteProgress)
    delete progress;

  _projectFile = file;
  emit projectFileChanged(file);
  return true;
}

// TulipProject *TulipProject::restoreProject(const QString &path) {
//  TulipProject *project = new TulipProject(path);
//  project->_isValid = project->readMetaInfo();
//  return project;
//}

// ==============================
//      FILES MANIPULATION
// ==============================
QStringList TulipProject::entryList(const QString &relativePath, const QStringList &nameFilters,
                                    QDir::Filters filters, QDir::SortFlags sort) {
  QString path(toAbsolutePath(relativePath));
  QFileInfo info(path);

  if (!info.exists() || !info.isDir())
    return QStringList();

  QDir dir(path);
  return dir.entryList(nameFilters, filters, sort);
}

QStringList TulipProject::entryList(const QString &relativePath, QDir::Filters filters,
                                    QDir::SortFlags sort) {
  QString path(toAbsolutePath(relativePath));
  QFileInfo info(path);

  if (!info.exists() || !info.isDir())
    return QStringList();

  QDir dir(path);
  return dir.entryList(filters, sort);
}

bool TulipProject::isDir(const QString &path) {
  QFileInfo info(toAbsolutePath(path));
  return info.isDir();
}

bool TulipProject::mkpath(const QString &path) {
  return QDir(rootDir()).mkpath(toAbsolutePath(path));
}

bool TulipProject::exists(const QString &path) {
  return QFileInfo(toAbsolutePath(path)).exists();
}

bool TulipProject::touch(const QString &path) {
  QFile f(toAbsolutePath(path));
  bool result = f.open(QIODevice::WriteOnly);
  f.close();
  return result;
}

bool TulipProject::removeFile(const QString &path) {
  return QFile::remove(toAbsolutePath(path));
}

bool TulipProject::removeDir(const QString &path) {
  QDir dir(toAbsolutePath(path));
  return dir.rmdir(dir.absolutePath());
}

bool TulipProject::removeAllDir(const QString &path) {
  QDir dir(toAbsolutePath(path));
  return dir.removeRecursively();
}

bool TulipProject::copy(const QString &source, const QString &destination) {
  return QFile::copy(source, toAbsolutePath(destination));
}

std::fstream *TulipProject::stdFileStream(const QString &path, std::ios_base::openmode mode) {
  QString filePath(toAbsolutePath(path));
  std::fstream *result = new std::fstream();
  result->open(QStringToTlpString(filePath).c_str(), mode);

  if (!result->is_open()) {
    delete result;
    result = nullptr;
  }

  return result;
}

QIODevice *TulipProject::fileStream(const QString &path, QIODevice::OpenMode mode) {
  QFile *result = new QFile(toAbsolutePath(path));
  result->open(mode);
  return result;
}

QString TulipProject::absoluteRootPath() const {
  return QDir(rootDir()).absolutePath();
}

// ==============================
//      META-INFORMATION
// ==============================
QString TulipProject::name() const {
  return _name;
}

void TulipProject::setName(const QString &n) {
  _name = n;
}

QString TulipProject::description() const {
  return _description;
}

void TulipProject::setDescription(const QString &d) {
  _description = d;
}

QString TulipProject::author() const {
  return _author;
}

void TulipProject::setAuthor(const QString &a) {
  _author = a;
}

QString TulipProject::perspective() const {
  return _perspective;
}

void TulipProject::setPerspective(const QString &p) {
  _perspective = p;
}

QString TulipProject::version() const {
  return TLPPROJ_VERSION;
}

bool TulipProject::writeMetaInfo() {
  QFile out(QDir(rootDir()).absoluteFilePath(INFO_FILE_NAME));

  if (!out.open(QIODevice::WriteOnly | QIODevice::Truncate))
    return false;

  QXmlStreamWriter doc(&out);
  doc.setAutoFormatting(true);
  doc.writeStartElement("tuliproject");
  doc.writeAttribute("version", TLPPROJ_VERSION);

  const QMetaObject *mo = metaObject();

  for (int i = mo->propertyOffset(); i < mo->propertyCount(); ++i) {
    QMetaProperty prop(mo->property(i));

    if (QString(prop.name()) == "objectName")
      continue;

    doc.writeTextElement(prop.name(), property(prop.name()).toString());
  }

  doc.writeEndDocument();
  out.close();
  return !doc.hasError();
}

bool TulipProject::readMetaInfo() {
  QFile in(QDir(rootDir()).absoluteFilePath(INFO_FILE_NAME));

  if (!in.open(QIODevice::ReadOnly))
    return false;

  QXmlStreamReader doc(&in);

  if (doc.hasError()) {
    in.close();
    tlp::debug() << "Error opening xml meta information file: "
                 << QStringToTlpString(doc.errorString()) << std::endl;
    return false;
  }

  while (!doc.atEnd()) {
    if (doc.readNextStartElement()) {
      if (doc.hasError()) {
        tlp::debug() << "Error reading xml meta information: "
                     << QStringToTlpString(doc.errorString()) << std::endl;
        in.close();
        return false;
      }

      std::string name = QStringToTlpString(doc.name().toString());

      if (property(name.c_str()).isValid())
        setProperty(name.c_str(), doc.readElementText());
    }
  }

  in.close();
  return true;
}

QString TulipProject::toAbsolutePath(const QString &relativePath) {
  QString path(relativePath);

  if (relativePath.startsWith("/"))
    path = path.remove(0, 1);

  return QDir(rootDir() + "/" + QString(DATA_DIR_NAME)).absoluteFilePath(path);
}

bool TulipProject::clearProject() {
  QFileInfo pathInfo(QDir(rootDir()).absolutePath());

  if (!pathInfo.isDir() || !pathInfo.exists())
    return false;

  QDir dir(pathInfo.absoluteFilePath());
  QFileInfoList entries(dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden |
                                              QDir::AllDirs | QDir::Files,
                                          QDir::DirsFirst));

  for (const QFileInfo &info : entries) {
    bool result = true;
    if (info.isDir()) {
      QDir dird(info.absoluteFilePath());
      result = dird.removeRecursively();
    } else
      result = dir.remove(info.absoluteFilePath());

    if (!result)
      return false;
  }
  return true;
}

void TulipProject::setProjectFile(const QString &projectFile) {
  _projectFile = projectFile;
  emit projectFileChanged(projectFile);
}
} // namespace tlp
