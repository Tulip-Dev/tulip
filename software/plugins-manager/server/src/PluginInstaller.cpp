#include "PluginGenerate.h"

#include <PluginLoaderWithInfo.h>
#include <UpdatePlugin.h>

#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QTextStream>

#include <tulip/TlpTools.h>
#include <tulip/GlyphManager.h>

using namespace std;
using namespace tlp;

int main(int argc,char **argv) {
  if(argc != 4)  {
    cout << "How to use :" << endl;
    cout << " First arg : plugin file" << endl;
    cout << " Second arg : doxygen generated plugin doc file" << endl;
    cout << " Third arg : target directory" << endl;
    exit(1);
  }

  QString pluginPath=argv[1];
  QString pluginDocPath=argv[2];
  QString targetPath=argv[3];
  QFileInfo fileInfo(pluginPath);
  QDir srcDir = fileInfo.dir();
  QString suffix = fileInfo.suffix();

  PluginLoaderWithInfo plug;
  initTulipLib(NULL);
  loadPlugin(pluginPath.toStdString(), &plug);
  //GlyphManager::getInst().loadPlugins(&plug); //Glyph plugins 

  LocalPluginInfo pluginInfo = plug.pluginsList[0];

  pluginInfo.displayType= PluginInfo::getPluginDisplayType(pluginInfo.name);
  if(pluginInfo.displayType != "Glyph")
    pluginInfo.type = "Algorithm";
  else
    pluginInfo.type = "Glyph";

  QDir dir;
  dir.mkpath(targetPath);
  dir.mkpath(targetPath+"/plugins/");

  QString path= pluginInfo.fileName.c_str();

  QDir dstDir(targetPath+"/plugins/"+path);
  dstDir.mkpath(targetPath+"/plugins/"+path);

  if (!generatePluginInfoFile(pluginInfo, dstDir))
    return EXIT_FAILURE;

  if (suffix == "so")
    UpdatePlugin::copyFile(srcDir,
			   QString(pluginInfo.fileName.c_str())+".so",
			   dstDir,
			   QString(pluginInfo.fileName.c_str())+".so." + QString(pluginInfo.version.c_str()).replace(" ",".")+".i386");
  else if (suffix == "dylib")
    UpdatePlugin::copyFile(srcDir,
			   QString(pluginInfo.fileName.c_str())+".dylib",
			   dstDir,
			   QString(pluginInfo.fileName.c_str())+".dylib."+QString(pluginInfo.version.c_str()).replace(" ","."));
  else if (suffix == "dll")
    UpdatePlugin::copyFile(srcDir,
			   QString(pluginInfo.fileName.c_str())+".dll",
			   dstDir,
			   QString(pluginInfo.fileName.c_str())+".dll."+QString(pluginInfo.version.c_str()).replace(" ","."));

  // Documentation
  QString fileName(pluginInfo.fileName.c_str());
  QString version(pluginInfo.version.c_str());
  QFile docFile(pluginDocPath);
  generatePluginDocFile(fileName, version, docFile, dstDir);
    
  return EXIT_SUCCESS;
}
