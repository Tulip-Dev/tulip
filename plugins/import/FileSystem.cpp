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

#include <tulip/TulipPluginHeaders.h>
#include <tulip/TlpQtTools.h>
#include <tulip/TulipFontAwesome.h>
#include <tulip/TulipViewSettings.h>

#include <QStack>
#include <QPair>
#include <QDir>
#include <QFileInfo>
#include <QDateTime>

using namespace tlp;

static const char *paramHelp[] = {
    // directory
    "The directory to scan recursively.",

    // icons
    "If true, set icons as node shapes according to file mime types.",

    // tree layout
    "If true, apply the 'Bubble Tree' layout algorithm on the imported graph.",

    // directory color
    "This parameter indicates the color used to display directories.",

    // other color
    "This parameter indicates the color used to display other files.",

    // hidden files
    "If true, also include hidden files.",

    // symlinks
    "If true, follow symlinks on Unix (including Mac OS X) or .lnk file on Windows."};

static const char *commonTextFilesExtArray[] = {"log", "msg", "odt", "pages", "rtf", "json",
                                                "tex", "txt", "wpd", "wps",   "srt", "nfo"};
static const std::vector<std::string> commonTextFilesExt(
    commonTextFilesExtArray,
    commonTextFilesExtArray + sizeof(commonTextFilesExtArray) / sizeof(commonTextFilesExtArray[0]));

static const char *commonAudioFilesExtArray[] = {"aif", "iff", "m3u", "m4a", "mid", "mp3",
                                                 "mpa", "ogg", "ra",  "wav", "wma", "flac"};
static const std::vector<std::string>
    commonAudioFilesExt(commonAudioFilesExtArray,
                        commonAudioFilesExtArray +
                            sizeof(commonAudioFilesExtArray) / sizeof(commonAudioFilesExtArray[0]));

static const char *commonVideoFilesExtArray[] = {"3g2", "3gp", "asf", "asx", "avi",
                                                 "flv", "m4v", "mkv", "mov", "mp4",
                                                 "mpg", "rm",  "swf", "vob", "wmv"};
static const std::vector<std::string>
    commonVideoFilesExt(commonVideoFilesExtArray,
                        commonVideoFilesExtArray +
                            sizeof(commonVideoFilesExtArray) / sizeof(commonVideoFilesExtArray[0]));

static const char *commonImageFilesExtArray[] = {"bmp", "dds",      "gif", "jpg", "jpeg", "png",
                                                 "psd", "pspimage", "tga", "thm", "tif",  "tiff",
                                                 "yuv", "ai",       "eps", "ps",  "svg"};
static const std::vector<std::string>
    commonImageFilesExt(commonImageFilesExtArray,
                        commonImageFilesExtArray +
                            sizeof(commonImageFilesExtArray) / sizeof(commonImageFilesExtArray[0]));

static const char *commonArchiveFilesExtArray[] = {
    "7z", "cbr", "deb", "gz", "pkg", "rar", "rpm", "sitx", "tar", "zip", "zipx", "bz2", "lzma"};
static const std::vector<std::string> commonArchiveFilesExt(
    commonArchiveFilesExtArray,
    commonArchiveFilesExtArray +
        sizeof(commonArchiveFilesExtArray) / sizeof(commonArchiveFilesExtArray[0]));

static const char *commonDevFilesExtArray[] = {
    "c",       "cc",        "class", "cpp", "cs",   "dtd", "fla", "h",   "hh",
    "hpp",     "java",      "lua",   "m",   "pl",   "py",  "sh",  "sln", "swift",
    "vcxproj", "xcodeproj", "css",   "js",  "html", "xml", "htm", "php", "xhtml"};
static const std::vector<std::string> commonDevFilesExt(commonDevFilesExtArray,
                                                        commonDevFilesExtArray +
                                                            sizeof(commonDevFilesExtArray) /
                                                                sizeof(commonDevFilesExtArray[0]));

static tlp::DataSet getDefaultAlgorithmParameters(const std::string &algoName, tlp::Graph *graph) {
  tlp::DataSet result;
  const tlp::ParameterDescriptionList &parameters =
      tlp::PluginLister::getPluginParameters(algoName);
  parameters.buildDefaultDataSet(result, graph);
  return result;
}

/** \addtogroup import */

/// Imports a tree representation of a file system directory.
/** This plugin enables to capture in a tree the full hierarchy of
 *  of a given file system directory
 */
class FileSystem : public tlp::ImportModule {
public:
  PLUGININFORMATION("File System Directory", "Auber", "16/12/2002",
                    "Imports a tree representation of a file system directory.", "2.2", "Misc")
  FileSystem(tlp::PluginContext *context)
      : ImportModule(context), _absolutePaths(NULL), _baseNames(NULL), _createdDates(NULL),
        _fileNames(NULL), _isDir(NULL), _isExecutable(NULL), _isReadable(NULL), _isSymlink(NULL),
        _isWritable(NULL), _lastModifiedDates(NULL), _lastReadDates(NULL), _owners(NULL),
        _permissions(NULL), _suffixes(NULL), _sizes(NULL), _fontIcon(NULL), _useIcons(true),
        _treeLayout(true), dirColor(255, 255, 127, 128) {
    addInParameter<std::string>("dir::directory", paramHelp[0], "");
    addInParameter<bool>("include hidden files", paramHelp[5], "true");
    addInParameter<bool>("follow symlinks", paramHelp[6], "true");
    addInParameter<bool>("icons", paramHelp[1], "true");
    addInParameter<bool>("tree layout", paramHelp[2], "true");
    addInParameter<tlp::Color>("directory color", paramHelp[3], "(255, 255, 127, 255)");
    addInParameter<tlp::Color>("other color", paramHelp[4], "(85, 170, 255, 255)");
  }

  bool importGraph() {
    if (dataSet == NULL)
      return false;

    std::string rootPathStr;
    dataSet->get("dir::directory", rootPathStr);
    QFileInfo rootInfo(tlpStringToQString(rootPathStr));

    dataSet->get("icons", _useIcons);
    dataSet->get("tree layout", _treeLayout);
    dataSet->get("directory color", dirColor);
    tlp::Color otherColor(85, 170, 255, 128);
    dataSet->get("other color", otherColor);
    bool hiddenFiles(true);
    dataSet->get("include hidden files", hiddenFiles);
    bool symlinks(true);
    dataSet->get("follow symlinks", symlinks);

    if (!rootInfo.exists()) {
#ifndef NDEBUG
      tlp::warning() << "Provided directory "
                     << tlp::QStringToTlpString(rootInfo.absoluteFilePath()) << " does not exist."
                     << std::endl;
#endif
      return false;
    }

    _absolutePaths = graph->getProperty<tlp::StringProperty>("Absolute paths");
    _baseNames = graph->getProperty<tlp::StringProperty>("Base name");
    _createdDates = graph->getProperty<tlp::StringProperty>("Creation date");
    _fileNames = graph->getProperty<tlp::StringProperty>("File name");
    _isDir = graph->getProperty<tlp::BooleanProperty>("Is directory");
    _isExecutable = graph->getProperty<tlp::BooleanProperty>("Is executable");
    _isReadable = graph->getProperty<tlp::BooleanProperty>("Is readable");
    _isSymlink = graph->getProperty<tlp::BooleanProperty>("Is symbolic link");
    _isWritable = graph->getProperty<tlp::BooleanProperty>("Is writable");
    _lastModifiedDates = graph->getProperty<tlp::StringProperty>("Last modification date");
    _lastReadDates = graph->getProperty<tlp::StringProperty>("Last read date");
    _owners = graph->getProperty<tlp::StringProperty>("Owner");
    _permissions = graph->getProperty<tlp::IntegerProperty>("Permission ID");
    _suffixes = graph->getProperty<tlp::StringProperty>("Suffix");
    _sizes = graph->getProperty<tlp::DoubleProperty>("Size");
    _fontIcon = graph->getProperty<tlp::StringProperty>("viewIcon");

    if (_useIcons) {
      tlp::IntegerProperty *viewShape = graph->getProperty<tlp::IntegerProperty>("viewShape");
      viewShape->setAllNodeValue(tlp::NodeShape::Icon);
      _fontIcon->setAllNodeValue(tlp::TulipFontAwesome::FileO);
    }

    tlp::ColorProperty *viewColor = graph->getProperty<tlp::ColorProperty>("viewColor");
    viewColor->setAllNodeValue(otherColor);

    tlp::node rootNode = addFileNode(rootInfo, graph);

    if (!rootInfo.isDir())
      return true;

    QStack<QPair<QString, tlp::node> > fsStack;
    fsStack.push(QPair<QString, tlp::node>(rootInfo.absoluteFilePath(), rootNode));

    while (!fsStack.empty()) {
      QPair<QString, tlp::node> elem = fsStack.pop();
      QDir currentDir(QDir(elem.first));

      tlp::node parentNode(elem.second);
      QFlags<QDir::Filter> filter =
          QDir::NoDotAndDotDot | QDir::System | QDir::AllDirs | QDir::Files;

      if (hiddenFiles)
        filter |= QDir::Hidden;

      QFileInfoList entries(currentDir.entryInfoList(filter, QDir::DirsFirst));

      int i = 0;

      pluginProgress->setComment("Reading contents of " +
                                 tlp::QStringToTlpString(currentDir.absolutePath()));
      pluginProgress->progress(i, entries.count());

      for (QFileInfoList::iterator it = entries.begin(); it != entries.end(); ++it) {
        QFileInfo fileInfo(*it);

        // consider files starting with a dot as hidden (for windows platform)
        if (!hiddenFiles && fileInfo.fileName().startsWith(".")) {
          continue;
        }

        tlp::node fileNode = addFileNode(fileInfo, graph);
        graph->addEdge(parentNode, fileNode);

        if (fileInfo.isDir() && (!fileInfo.isSymLink() || symlinks))
          fsStack.push_back(QPair<QString, tlp::node>(fileInfo.absoluteFilePath(), fileNode));

        if ((++i % 100) == 0)
          pluginProgress->progress(i, entries.count());

        if (pluginProgress->state() == TLP_CANCEL) {
          pluginProgress->setError("Import cancelled by user.");
          return false;
        }

        if (pluginProgress->state() == TLP_STOP)
          break;
      }
    }

    tlp::StringProperty *viewLabel = graph->getProperty<tlp::StringProperty>("viewLabel");
    viewLabel->copy(_fileNames);

    if (_treeLayout) {
      const std::string algoName = "Bubble Tree";
      tlp::DataSet defaultParameters = getDefaultAlgorithmParameters(algoName, graph);
      tlp::LayoutProperty *viewLayout = graph->getProperty<tlp::LayoutProperty>("viewLayout");
      std::string errMsg;
      graph->applyPropertyAlgorithm(algoName, viewLayout, errMsg, pluginProgress,
                                    &defaultParameters);
    }

    return true;
  }

private:
  tlp::node addFileNode(const QFileInfo &info, tlp::Graph *g) {
    tlp::node n = g->addNode();
    _absolutePaths->setNodeValue(n, tlp::QStringToTlpString(info.absoluteFilePath()));
    _baseNames->setNodeValue(n, tlp::QStringToTlpString(info.baseName()));
    _createdDates->setNodeValue(n, tlp::QStringToTlpString(info.created().toString()));
    _fileNames->setNodeValue(n, tlp::QStringToTlpString(info.fileName()));
    _isDir->setNodeValue(n, info.isDir());
    _isExecutable->setNodeValue(n, info.isExecutable());
    _isReadable->setNodeValue(n, info.isReadable());
    _isSymlink->setNodeValue(n, info.isSymLink());
    _isWritable->setNodeValue(n, info.isWritable());
    _lastModifiedDates->setNodeValue(n, tlp::QStringToTlpString(info.lastModified().toString()));
    _lastReadDates->setNodeValue(n, tlp::QStringToTlpString(info.lastRead().toString()));
    _owners->setNodeValue(n, tlp::QStringToTlpString(info.owner()));
    _permissions->setNodeValue(n, int(info.permissions()));
    _suffixes->setNodeValue(n, tlp::QStringToTlpString(info.suffix()));
    _sizes->setNodeValue(n, info.size());

    if (_useIcons) {
      std::string extension = QStringToTlpString(info.suffix());

      if (info.isDir()) {
        _fontIcon->setNodeValue(n, tlp::TulipFontAwesome::FolderO);
        tlp::ColorProperty *viewColor = graph->getProperty<tlp::ColorProperty>("viewColor");
        viewColor->setNodeValue(n, dirColor);
      } else if (std::find(commonTextFilesExt.begin(), commonTextFilesExt.end(), extension) !=
                 commonTextFilesExt.end()) {
        _fontIcon->setNodeValue(n, tlp::TulipFontAwesome::FileTextO);
      } else if (std::find(commonArchiveFilesExt.begin(), commonArchiveFilesExt.end(), extension) !=
                 commonArchiveFilesExt.end()) {
        _fontIcon->setNodeValue(n, tlp::TulipFontAwesome::FileArchiveO);
      } else if (std::find(commonAudioFilesExt.begin(), commonAudioFilesExt.end(), extension) !=
                 commonAudioFilesExt.end()) {
        _fontIcon->setNodeValue(n, tlp::TulipFontAwesome::FileAudioO);
      } else if (std::find(commonImageFilesExt.begin(), commonImageFilesExt.end(), extension) !=
                 commonImageFilesExt.end()) {
        _fontIcon->setNodeValue(n, tlp::TulipFontAwesome::FileImageO);
      } else if (std::find(commonVideoFilesExt.begin(), commonVideoFilesExt.end(), extension) !=
                 commonVideoFilesExt.end()) {
        _fontIcon->setNodeValue(n, tlp::TulipFontAwesome::FileVideoO);
      } else if (std::find(commonDevFilesExt.begin(), commonDevFilesExt.end(), extension) !=
                 commonDevFilesExt.end()) {
        _fontIcon->setNodeValue(n, tlp::TulipFontAwesome::FileCodeO);
      } else if (extension == "pdf") {
        _fontIcon->setNodeValue(n, tlp::TulipFontAwesome::FilePdfO);
      } else if (extension == "doc" || extension == "docx") {
        _fontIcon->setNodeValue(n, tlp::TulipFontAwesome::FileWordO);
      } else if (extension == "xls" || extension == "xlsx") {
        _fontIcon->setNodeValue(n, tlp::TulipFontAwesome::FileExcelO);
      } else if (extension == "ppt" || extension == "pptx") {
        _fontIcon->setNodeValue(n, tlp::TulipFontAwesome::FilePowerpointO);
      }
    }

    return n;
  }

  tlp::StringProperty *_absolutePaths;
  tlp::StringProperty *_baseNames;
  tlp::StringProperty *_createdDates;
  tlp::StringProperty *_fileNames;
  tlp::BooleanProperty *_isDir;
  tlp::BooleanProperty *_isExecutable;
  tlp::BooleanProperty *_isReadable;
  tlp::BooleanProperty *_isSymlink;
  tlp::BooleanProperty *_isWritable;
  tlp::StringProperty *_lastModifiedDates;
  tlp::StringProperty *_lastReadDates;
  tlp::StringProperty *_owners;
  tlp::IntegerProperty *_permissions;
  tlp::StringProperty *_suffixes;
  tlp::DoubleProperty *_sizes;
  tlp::StringProperty *_fontIcon;
  bool _useIcons;
  bool _treeLayout;
  tlp::Color dirColor;
};

PLUGIN(FileSystem)
