#ifndef TULIPSETTINGS_H
#define TULIPSETTINGS_H

#include <QtCore/QSettings>
#include <tulip/tulipconf.h>

/**
  * @brief This class provides convenience functions to access to the Tulip settings file (using QSettings)
  * TulipSettings is a wrapper for QSettings providing quick access to common keys provided in the tulip configuration file.
  * This object does not mask any method from the QSettings class. Which mean that the user can still access to custom keys by invoking the QSettings::value method.
  */
class TLP_QT_SCOPE TulipSettings: public QSettings {
public:
  static TulipSettings &instance();

  /**
    @brief Retrieves the list of documents recently opened with tulip
    */
  QList<QString> recentDocuments() const;

  /**
    @brief Registers a file in the list of recently opened documents
    */
  void addToRecentDocuments(const QString &);

private:
  TulipSettings();

  static TulipSettings *_instance;
};

#endif // TULIPSETTINGS_H
