#ifndef _Tulip_TULIPPLUGINLOADER_H_
#define _Tulip_TULIPPLUGINLOADER_H_

#include <vector>

#include <PluginLoaderWithInfo.h>

#include "AppStartUp.h"

namespace tlp {

  //==============================================================
  struct TulipPluginLoader:public PluginLoaderWithInfo {
    AppStartUp *appStartUp;
    int progress;

    virtual void start(const std::string &path,const std::string &type) {
      appStartUp->setProgress(0);
      progress=0;
      PluginLoaderWithInfo::start(path,type);
    }

    virtual void numberOfFiles(int nbFile) {
      appStartUp->setTotalSteps(nbFile);
      qApp->processEvents();
    }

    virtual void loaded(const std::string &name,
			const std::string &author,
			const std::string &date, 
			const std::string &info,
			const std::string &release,
			const std::string &version,
			const std::list <Dependency> &deps)
    {
      progress++;
      appStartUp->setLabel(name);
      appStartUp->setProgress(progress);
      qApp->processEvents();

      PluginLoaderWithInfo::loaded(name,author,date,info,release,version,deps);
    }

    virtual void aborted(const std::string &filename,const  std::string &errormsg) {
      progress++;
      std::string msg("Error when loading ");
      msg += filename + '\n' + errormsg;
      appStartUp->addErrorMsg(msg);
      appStartUp->setLabel("Error");
      appStartUp->setProgress(progress);
      qApp->processEvents();

      PluginLoaderWithInfo::aborted(filename,errormsg);
    }
  };
  
}
#endif
