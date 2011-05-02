#ifndef PLUGININFORMATIONS_H
#define PLUGININFORMATIONS_H

#include <tulip/Plugin.h>
#include <QDate>
#include <QIcon>

class PluginInformations {
  public:
    QString identifier() const;
    QString name() const;

    QString shortDescription() const;
    QString longDescription() const;

    QString iconPath() const;
    QDate installDate() const;
    
    QString type() const;
    QStringList dependencies() const;
    QString version() const;
};

#endif //PLUGININFORMATIONS_H
