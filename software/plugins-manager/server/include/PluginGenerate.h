#ifndef Tulip_PLUGINGENERATE_H_
#define Tulip_PLUGINGENERATE_H_

#include <PluginInfo.h>

#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtCore/QDir>

extern bool generatePluginInfoFile(tlp::LocalPluginInfo& pInfo, QDir& dstDir);

extern void generatePluginDocFile(QString& pFileName, QString& pVersion,
				  QFile& pDoxFile, QDir& dstDir);

#endif
