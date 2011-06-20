#include "tulip/TulipProject.h"

#include <tulip/SimplePluginProgress.h>

#include <QtCore/QMetaProperty>
#include <QtCore/QDir>
#include <QtCore/QCoreApplication>
#include <QtCore/QTextStream>
#include <QtXml/QDomDocument>

#include <stdlib.h>
#include <tulip/QuaZIPFacade.h>

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
}

TulipProject::~TulipProject() {
  if (!isValid())
    return;
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

TulipProject *TulipProject::openProject(const QString &file, tlp::PluginProgress *progress) {
  bool deleteProgress = false;
  if (!progress) {
    progress = new tlp::SimplePluginProgress;
    deleteProgress = true;
  }

  TulipProject *project = TulipProject::newProject();
  if (!project->isValid())
    return project;

  if (!QuaZIPFacade::unzip(project->_rootDir.absolutePath(),file)) {
    project->_isValid = false;
    project->_lastError = "Failed to unzip project.";
    return project;
  }
  project->readMetaInfos();

  if (deleteProgress)
    delete progress;

  project->_projectFile = file;
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
  return true;
}

// ==============================
//      FILES MANIPULATION
// ==============================
QStringList TulipProject::entryList(const QString &relativePath) {
  QString path(toAbsolutePath(relativePath));
  QFileInfo info(path);
  if (!info.exists() || !info.isDir())
    return QStringList();
  QDir dir(path);
  return dir.entryList();
}

bool TulipProject::isDir(const QString &path) {
  QFileInfo info(toAbsolutePath(path));
  return info.isDir();
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

std::fstream *TulipProject::stdFileStream(const QString &path) {
  QString filePath(toAbsolutePath(path));
  return new std::fstream(filePath.toStdString().c_str(),std::fstream::in | std::fstream::out);
}

QIODevice *TulipProject::fileStream(const QString &path) {
  QFile *result = new QFile(toAbsolutePath(path));
  result->open(QIODevice::ReadWrite);
  return result;
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

void TulipProject::readMetaInfos() {
  QFile in(_rootDir.absoluteFilePath(INFOS_FILE_NAME));
  if (!in.open(QIODevice::ReadOnly))
    return;
  QDomDocument doc;
  doc.setContent(&in);
  in.close();

  QDomElement rootElement = doc.documentElement();
  QDomNodeList children = rootElement.childNodes();

  for (int i=0; i < children.count(); ++i) {
    QDomNode n = children.at(i);
    if (!n.isElement())
      continue;
    QDomElement e = n.toElement();

    const char *propName = e.tagName().toStdString().c_str();
    if (property(propName).isValid())
      setProperty(propName,e.text());
  }
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
  } while (QDir(result).exists());
  return result;
}

bool TulipProject::removeAllDirPrivate(const QString &path) {
  QFileInfo pathInfo(path);
  if (!pathInfo.isDir() || !pathInfo.exists())
    return false;
  QDir dir(pathInfo.absoluteFilePath());
  QFileInfoList entries(dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst));
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
