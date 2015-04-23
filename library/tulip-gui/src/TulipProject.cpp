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

#include "tulip/TulipProject.h"

#include <QDebug>
#include <QMetaProperty>
#include <QDir>
#include <QCoreApplication>
#include <QTextStream>
#include <QDomDocument>

#include <tulip/SimplePluginProgress.h>
#include <tulip/QuaZIPFacade.h>

#include <cstdlib>

#define DATA_DIR_NAME "data"
#define INFOS_FILE_NAME "project.xml"
#define TLPPROJ_VERSION "1.0"

namespace tlp {

TulipProject::TulipProject(): _isValid(false) {
  // This private constructer should never been called. It has been privately declared to prevent use of default constructor
}

TulipProject::TulipProject(const QString &path)
  : _rootDir(path),
    _dataDir(_rootDir.absoluteFilePath(DATA_DIR_NAME)), _isValid(true) {
  writeMetaInfos();
}

TulipProject::~TulipProject() {
  removeAllDirPrivate(_rootDir.absolutePath());
}

TulipProject *TulipProject::newProject() {
  QString rootPath = temporaryPath();
  QDir rootDir(temporaryPath());
  bool dirOk = rootDir.mkpath(rootPath) && rootDir.mkdir(DATA_DIR_NAME);

  if (!dirOk) {
    TulipProject *result = new TulipProject;
    result->_lastError = "Failed to create a temporary path: " + rootPath;
    return result;
  }

  return new TulipProject(rootPath);
}

bool TulipProject::openProjectFile(const QString &file,
                                   tlp::PluginProgress *progress) {
  if(!QFileInfo(file).exists()) {
    _isValid = false;
    _lastError = "File "+file+" not found";
    return false;
  }

  bool deleteProgress = false;

  if (!progress) {
    progress = new tlp::SimplePluginProgress;
    deleteProgress = true;
  }

  if (!QuaZIPFacade::unzip(_rootDir.absolutePath(),file,progress)) {
    _isValid = false;
    _lastError = "Failed to unzip project.";

    if (deleteProgress)
      delete progress;

    return false;
  }

  readMetaInfos();

  if (deleteProgress)
    delete progress;

  _projectFile = file;
  emit projectFileChanged(file);
  return true;
}

TulipProject *TulipProject::openProject(const QString &file, tlp::PluginProgress *progress) {
  TulipProject *project = TulipProject::newProject();

  if (!project->isValid())
    return project;

  project->openProjectFile(file, progress);
  return project;
}

bool TulipProject::write(const QString &file, tlp::PluginProgress *progress) {
  bool deleteProgress = false;

  if (!progress) {
    progress = new tlp::SimplePluginProgress;
    deleteProgress = true;
  }

  if (!writeMetaInfos()) {
    _lastError = "Failed to save meta-informations.";
    return false;
  }

  if (!QuaZIPFacade::zipDir(_rootDir.absolutePath(),file)) {
    _lastError = "Failed to zip project.";
    return false;
  }

  if (deleteProgress)
    delete progress;

  _projectFile = file;
  emit projectFileChanged(file);
  return true;
}

TulipProject *TulipProject::restoreProject(const QString &path) {
  TulipProject *project = new TulipProject(path);
  project->_isValid = project->readMetaInfos();
  return project;
}

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
  return dir.entryList(nameFilters,filters,sort);
}

QStringList TulipProject::entryList(const QString &relativePath, QDir::Filters filters, QDir::SortFlags sort) {
  QString path(toAbsolutePath(relativePath));
  QFileInfo info(path);

  if (!info.exists() || !info.isDir())
    return QStringList();

  QDir dir(path);
  return dir.entryList(filters,sort);
}

bool TulipProject::isDir(const QString &path) {
  QFileInfo info(toAbsolutePath(path));
  return info.isDir();
}

bool TulipProject::mkpath(const QString &path) {
  return _rootDir.mkpath(toAbsolutePath(path));
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
  return removeAllDirPrivate(toAbsolutePath(path));
}

bool TulipProject::copy(const QString &source, const QString &destination) {
  return QFile::copy(source,toAbsolutePath(destination));
}

std::fstream *TulipProject::stdFileStream(const QString &path, std::ios_base::openmode mode) {
  QString filePath(toAbsolutePath(path));
  std::fstream *result = new std::fstream();
  result->open(filePath.toUtf8().data(), mode);

  if (!result->is_open()) {
    delete result;
    result = NULL;
  }

  return result;
}

QIODevice *TulipProject::fileStream(const QString &path, QIODevice::OpenMode mode) {
  QFile *result = new QFile(toAbsolutePath(path));
  result->open(mode);
  return result;
}

QString TulipProject::absoluteRootPath() const {
  return _rootDir.absolutePath();
}

// ==============================
//      META-INFORMATIONS
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

bool TulipProject::writeMetaInfos() {
  QDomDocument doc;
  QDomElement rootNode(doc.createElement("tulipproject"));
  rootNode.setAttribute("version",TLPPROJ_VERSION);
  doc.appendChild(rootNode);

  const QMetaObject *mo = metaObject();

  for (int i=0; i < mo->propertyCount(); ++i) {
    QMetaProperty prop(mo->property(i));

    if (QString(prop.name()) == "objectName")
      continue;

    QDomElement e(doc.createElement(prop.name()));
    rootNode.appendChild(e);
    QDomText txt = doc.createTextNode(property(prop.name()).toString());
    e.appendChild(txt);
  }

  QFile out(_rootDir.absoluteFilePath(INFOS_FILE_NAME));

  if (!out.open(QIODevice::WriteOnly | QIODevice::Truncate))
    return false;

  out.write(doc.toByteArray(2));
  out.close();

  return true;
}

bool TulipProject::readMetaInfos() {
  QFile in(_rootDir.absoluteFilePath(INFOS_FILE_NAME));

  if (!in.open(QIODevice::ReadOnly))
    return false;

  QDomDocument doc;

  if (!doc.setContent(&in))
    return false;

  in.close();

  QDomElement rootElement = doc.documentElement();
  QDomNodeList children = rootElement.childNodes();

  for (int i=0; i < children.count(); ++i) {
    QDomNode n = children.at(i);

    if (!n.isElement())
      continue;

    QDomElement e = n.toElement();

    // On MacOS, converting e.tagName() to a const char* only lives for the duration of the QString, to avoid the char* to be NULL, we don't use temporary variables
    if (property(e.tagName().toStdString().c_str()).isValid())
      setProperty(e.tagName().toStdString().c_str(),e.text());
  }

  return true;
}

QString TulipProject::toAbsolutePath(const QString &relativePath) {
  QString path(relativePath);

  if (relativePath.startsWith("/"))
    path = path.remove(0,1);

  return _dataDir.absoluteFilePath(path);
}

// Some hack: Qt does not provide method to create temporary DIRS.
QString TulipProject::temporaryPath() {
  QString basePath(QDir::tempPath() + QDir::separator() + QCoreApplication::applicationName() + "-" + QString::number(QCoreApplication::applicationPid()) + "-");
  int prefix=0;
  QString result;

  do {
    result = basePath + QString::number(prefix++);
  }
  while (QDir(result).exists());

  return result;
}

bool TulipProject::removeAllDirPrivate(const QString &path) {
  QFileInfo pathInfo(path);

  if (!pathInfo.isDir() || !pathInfo.exists())
    return false;

  QDir dir(pathInfo.absoluteFilePath());
  QFileInfoList entries(dir.entryInfoList(QDir::NoDot | QDir::NoDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst));

  for (QFileInfoList::iterator it = entries.begin(); it != entries.end(); ++it) {
    QFileInfo info(*it);
    bool result = true;

    if (info.isDir()) {
      result = removeAllDirPrivate(info.absoluteFilePath());
    }
    else
      result = dir.remove(info.absoluteFilePath());

    if (!result)
      return false;
  }

  dir.rmdir(pathInfo.absoluteFilePath());
  return true;
}

}
