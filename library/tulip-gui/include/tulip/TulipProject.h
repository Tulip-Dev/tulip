#ifndef TULIPPROJECT_H
#define TULIPPROJECT_H

#include <fstream>

#include <QtCore/QDir>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QDate>
#include <QtCore/QObject>
#include <QtCore/QIODevice>

#include <tulip/tulipconf.h>
#include <tulip/PluginProgress.h>

namespace tlp {

/**
  @brief The TulipProject object handles the content of a Tulip project.

  All tulip projects contain a set of defined static meta-informations:
  @list
  @li name (QString): the name of the project
  @li description (QString): Comments about the project
  @li author (QString): the author of the project
  @li perspective (QString): the name of the perspective plugin associated to the project
  @li date (QDate): the date of project's last modification
  @li version (QString): the version of the Tulip project format
  @endlist

  Alongside those informations, one can store any kind of file into a Tulip project. Since a project is meant to be associated to a specific perspective, the responisbility of those file
  is left to the perspective.

  A TulipProject DOES NOT automatically save to disk. One will have to call the write() method to serialize data.
  @warning Precise implementation of the TulipProject object should NOT be known or used by the user since it could be subject to changes.

  If something wrong happens when calling a method from TulipProject, this method will return either false or a invalid result (see specific method documentation). The last error message can be retrieved with the lastError() method.

  After opening and before saving a project, user will be able to list/delete files and directories available in the project and open them using std filestreams or Qt's QIODevice.
  Files can be opened using the stdFileStram and fileStream methods. They will always be opened in Read/Write mode.

  Files in a tulip project are identified by their path. Those path ar similar to the path of a standard file system and use the "/" character as a separator.
  The root directory is identified by "/".

  @warning TulipProject paths ALWAYS use the "/" character as a directory separator. This is OS-independant.

  @example A file called graph.tlp located at the top-level directory will be identified by the "/graph.tlp" path while the file "graph.tlp" located in the "data" directory will be identified by "/data/graph.tlp".
  */
class TLP_QT_SCOPE TulipProject: public QObject {
  Q_OBJECT

  TulipProject();
  TulipProject(const QString &);

public:
  virtual ~TulipProject();

  /**
    @brief Starts a new TulipProject from scratch
    This method builds up a new TulipProject file without taking any input.
    @see openProject()
    */
  static TulipProject *newProject();

  /**
    @brief Opens a previously saved tulip project file
    This method will unpack a tulip project file into some directory and allow the user to manipulate the files.
    @see TulipProject::save()
    @param file The file to open.
    @param progress A progress handler.
    @return a pointer to a TulipProject object or a NULL pointer if the method failed.
    */
  static TulipProject *openProject(const QString &file, tlp::PluginProgress *progress=NULL);

  /**
    @brief Writes files in the TulipProject into a packed archive.
    This method packs every file in the project into a single archive.
    @note This method DOES NOT close the project. It only  commits changes to the specified file. A TulipProject is only closed when destroyed.
    @param file Absolute path where files should be packed.
    @param progress A progress handler
    @return False if method failed
    */
  bool write(const QString &file,tlp::PluginProgress *progress=NULL);

  /**
    @brief Lists entries in a directory
    @param path The path to scan. @see TulipProject
    @return The list of files and directories present in the given directory
    */
  QStringList entryList(const QString &path);

  /**
    @brief Checks if the given path is a directory.
    @param path The path to check. @see TulipProject
    @return true/false wether the path is a directory.
    */
  bool isDir(const QString &path);

  /**
    @brief Removes a file from the project.
    If the given path points to a directory, or if the file does not exist, this method will fail and return false
    @param path The path to delete. @see TulipProject
    */
  bool removeFile(const QString &path);

  /**
    @brief Removes a directory from the project.
    If the given file points to a file, or if the directory does not exist, or if the directory is not empty, this method will fail and return false.
    @see removeAllDir to remove a non-empty directory.
    @param path The path to delete. @see TulipProject
    */
  bool removeDir(const QString &path);

  /**
    @brief Removes a directory and all its content from the project.
    If the given file points to a file, or if the directory does not exist, this method will fail and return false.
    @warning This will remove every file stored in the specified directory.
    @param path The path to delete. @see TulipProject
    */
  bool removeAllDir(const QString &path);

  /**
    @brief Gets a STL file stream (R/W access mode) to the given path.
    @warning This method does not check if the given path is a directory or a file. User might get an invalid filestream.
    @param path The path to open. @see TulipProject
    @return an opened filestream on the given path.
    */
  std::fstream *stdFileStream(const QString &path);

  /**
    @brief Gets a Qt I/O device (R/W access mode) to the given path.
    @warning This method does not check if the given path is a directory or a file. User might get an invalid filestream.
    @warning User SHOULD NOT cast the QIODevice returned by this method into any of its subclass since the implementation might change in future versions.
    @param path The path to open. @see TulipProject
    @return an opened Qt device on the given path.
    */
  QIODevice *fileStream(const QString &path);

  /**
    @brief Returns the last error raised.
    @note The returned string is null if no error was raised.
    */
  QString lastError() const { return _lastError; }

  /**
    @brief Check if the object is a valid TulipProject.
    @warning Calling methods on invalid TulipProject instances may result in undefined behavior.
    */
  bool isValid() const { return _isValid; }

  // Developer note: Every field in the TulipProject tagged as a Q_PROPERTY will automaticaly be serialized in the project.xml file
  /**
    @brief the name of the project
    */
Q_PROPERTY(QString name READ name WRITE setName)
  QString name() const;

  /**
    @brief User-written description of the project
    */
Q_PROPERTY(QString description READ description WRITE setDescription)
  QString description() const;

  /**
    @brief Name of the author
    */
Q_PROPERTY(QString author READ author WRITE setAuthor)
  QString author() const;

  /**
    @brief name of the perspective associated to the project.
    When user open a project from Tulip, this porperty is first read to identify find kind of perspective plugin should be launched to
    open the project

    @warning If the perspective name associated to the project is invalid or correspond to a missing plugin, tulip may not be able to open the file.
    */
Q_PROPERTY(QString perspective READ perspective WRITE setPerspective)
  QString perspective() const;

  /**
    @brief The version of the Tulip project format with which the file was created.
    Project from older format versions will always by saved into the newest version available.
    */
  QString version() const;

public slots:
  void setName(const QString &);
  void setDescription(const QString &);
  void setAuthor(const QString &);
  void setPerspective(const QString &);

private:
  static QString temporaryPath();

  bool writeMetaInfos();
  void readMetaInfos();

  QString toAbsolutePath(const QString &relativePath);
  bool removeAllDirPrivate(const QString &path);

  // Core fileset
  QDir _rootDir;
  QDir _dataDir;

  // Meta informations
  QString _author;
  QString _name;
  QString _description;
  QString _perspective;

  // Error handling
  QString _lastError;
  bool _isValid;
};

}
#endif // TULIPPROJECT_H
