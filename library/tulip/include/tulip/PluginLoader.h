#ifndef TLP_PLUGINLOADER
#define TLP_PLUGINLOADER
#include <list>
#include <string>
#include <tulip/WithDependency.h>

namespace tlp {

struct TLP_SCOPE PluginLoader {
  virtual ~PluginLoader() {}
  virtual void start(const std::string &path,const std::string &type)=0;
  virtual void numberOfFiles(int nbFiles) {}
  virtual void loading(const std::string &filename)=0;
  virtual void loaded(const std::string &name,
		      const std::string &author,
		      const std::string &date, 
		      const std::string &info,
		      const std::string &release,
		      const std::string &version,
		      const std::list <Dependency>&)=0;
  virtual void aborted(const std::string &filename,const  std::string &erreurmsg)=0;
  virtual void finished(bool state,const std::string &msg)=0;
};

}
#endif
