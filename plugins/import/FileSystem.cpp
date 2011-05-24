/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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
#include <tulip/TulipPlugin.h>

#include <QtCore/QStack>
#include <QtCore/QPair>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QDateTime>

namespace {
  static const char * paramHelp[] = {
    // directory
    HTML_HELP_OPEN()				    \
    HTML_HELP_DEF( "type", "directory pathname" )		    \
    HTML_HELP_BODY()						      \
    "The root directory of the filesystem part to import."	      \
    HTML_HELP_CLOSE(),
  };
}

/** \addtogroup import */
/*@{*/
/// Import a tree representation of a file system directory.
/** This plugin enables to capture in a tree the full hierarchy of
 *  of a given file system directory
 */
class FileSystem:public tlp::ImportModule {
public:
  FileSystem(tlp::AlgorithmContext context):ImportModule(context) {
    addParameter<std::string>("dir::directory", paramHelp[0]);
  }
  ~FileSystem(){}

  bool import(const std::string &) {
    if (dataSet == 0)
      return false;

    std::string rootPathStr;
    dataSet->get("dir::directory",rootPathStr);
    QFileInfo rootInfo(rootPathStr.c_str());
    if (!rootInfo.exists())
      return false;

    _absolutePaths = graph->getProperty<tlp::StringProperty>("Absolute paths");
    _baseNames = graph->getProperty<tlp::StringProperty>("Base name");
    _createdDates = graph->getProperty<tlp::StringProperty>("Creation date");
    _fileNames = graph->getProperty<tlp::StringProperty>("File name");
    _isExecutable = graph->getProperty<tlp::BooleanProperty>("Is executable");
    _isReadable = graph->getProperty<tlp::BooleanProperty>("Is readable");
    _isSymlink = graph->getProperty<tlp::BooleanProperty>("Is symbolic link");
    _isWritable = graph->getProperty<tlp::BooleanProperty>("Is writable");
    _lastModifiedDates = graph->getProperty<tlp::StringProperty>("Last modification date");
    _lastReadDates = graph->getProperty<tlp::StringProperty>("Last read date");
    _owners = graph->getProperty<tlp::StringProperty>("Owner");
    _permissions = graph->getProperty<tlp::IntegerProperty>("Permission ID");
    _suffixes = graph->getProperty<tlp::StringProperty>("Suffix");

    tlp::node rootNode = addFileNode(rootInfo,graph);
    if (!rootInfo.isDir())
      return true;

    bool deleteProgress=false;
    if (!pluginProgress) {
      deleteProgress=true;
      pluginProgress = new tlp::SimplePluginProgress;
    }

    pluginProgress->progress(0,100);
    QStack<QPair<QString,tlp::node> > fsStack;
    fsStack.push(QPair<QString,tlp::node>(rootInfo.absoluteFilePath(),rootNode));
    while (!fsStack.empty()) {
      QPair<QString,tlp::node> elem = fsStack.pop();
      QDir currentDir(QDir(elem.first));

      pluginProgress->setComment(("Visiting " + currentDir.absolutePath()).toStdString());

      tlp::node parentNode(elem.second);
      QFileInfoList entries(currentDir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst));

      int i=0;
      for (QFileInfoList::iterator it = entries.begin(); it != entries.end(); ++it) {
        pluginProgress->progress(i++,entries.size());
        QFileInfo fileInfos(*it);
        tlp::node fileNode=addFileNode(fileInfos, graph);
        graph->addEdge(parentNode,fileNode);
        if (fileInfos.isDir())
          fsStack.push_back(QPair<QString,tlp::node>(fileInfos.absoluteFilePath(),fileNode));
      }
    }

    if (deleteProgress)
      delete pluginProgress;

    return true;
  }

private:
  tlp::node addFileNode(const QFileInfo &infos, tlp::Graph *g) {
    tlp::node n = g->addNode();
    _absolutePaths->setNodeValue(n,infos.absoluteFilePath().toStdString());
    _baseNames->setNodeValue(n,infos.baseName().toStdString());
    _createdDates->setNodeValue(n,infos.created().toString().toStdString());
    _fileNames->setNodeValue(n,infos.fileName().toStdString());
    _isExecutable->setNodeValue(n,infos.isExecutable());
    _isReadable->setNodeValue(n,infos.isReadable());
    _isSymlink->setNodeValue(n,infos.isSymLink());
    _isWritable->setNodeValue(n,infos.isWritable());
    _lastModifiedDates->setNodeValue(n,infos.lastModified().toString().toStdString());
    _lastReadDates->setNodeValue(n,infos.lastRead().toString().toStdString());
    _owners->setNodeValue(n,infos.owner().toStdString());
    _permissions->setNodeValue(n,(int)(infos.permissions()));
    _suffixes->setNodeValue(n,infos.suffix().toStdString());
    return n;
  }

  tlp::StringProperty *_absolutePaths;
  tlp::StringProperty *_baseNames;
  tlp::StringProperty *_createdDates;
  tlp::StringProperty *_fileNames;
  tlp::BooleanProperty *_isExecutable;
  tlp::BooleanProperty *_isReadable;
  tlp::BooleanProperty *_isSymlink;
  tlp::BooleanProperty *_isWritable;
  tlp::StringProperty *_lastModifiedDates;
  tlp::StringProperty *_lastReadDates;
  tlp::StringProperty *_owners;
  tlp::IntegerProperty *_permissions;
  tlp::StringProperty *_suffixes;
};
/*@}*/
IMPORTPLUGINOFGROUP(FileSystem, "File System Directory", "Auber", "16/12/2002", "", "2.0", "Misc")
