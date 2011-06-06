#ifndef QUAZIPFACADE_H
#define QUAZIPFACADE_H

#include <QtCore/QString>

#include <tulip/tulipconf.h>

namespace tlp {
class PluginProgress;
}

/**
  @brief This class is a facade to the QuaZIP libraries features.
  It provides with convenience methods allowing to manipulate the QuaZIP library on a higher level.
  QuaZIPFacade contains only static methods, as it should only work as a toolbox.
  */
class TLP_QT_SCOPE QuaZIPFacade {
  QuaZIPFacade();
public:

  /**
    @brief Compress (ZIPs) a directory into an archive.
    This method compress the content of a directory (without taking the top directory) into an archive.
    @param rootPath The absolute path of the directory to compress.
    @param archivePath The absolute file path of the ZIP file to create.
    @param progress A progress handler
    @warning If an archive already exists with that name, it will be overwritten.
    @return true if compression process went sucessfuly
    */
  static bool zipDir(const QString &rootPath, const QString &archivePath, tlp::PluginProgress *progress=NULL);

  /**
    @brief Uncompress (un-ZIPs) an archive into a directory.
    Files contained in the archive will be stored inside the specified directory. Making it the root path of the uncompressed files.
    @param rootPath The absolute path of the directory where files should be written.
    @param archivePath The absolute file path of the ZIP file to uncompress.
    @param progress A progress handler
    @warning If some files already exist in the target directory, they will be overwritten.
    @return true if uncompression process went sucessfuly
    */
  static bool unzip(const QString &rootPath, const QString &archivePath, tlp::PluginProgress *progress=NULL);
};

#endif // QUAZIPFACADE_H
