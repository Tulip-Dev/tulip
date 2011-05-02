#include <tulip/Plugin.h>
#include <QDate>
#include <QIcon>

class PluginInformations {
  public:
    QString identifier() const;
    QString name() const;

    QString shortDescription() const;
    QString longDescription() const;

    QIcon icon() const;
    QDate installDate() const;
    
    QString type() const;
    QStringList dependencies() const;
    QString version() const;
};
