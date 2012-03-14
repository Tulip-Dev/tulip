namespace tlp {

class PluginLoaderTxt : tlp::PluginLoader {
%TypeHeaderCode
#include <tulip/PluginLoaderTxt.h>
%End
public:
  virtual void start(const std::string &path);
  virtual void loading(const std::string &filename);
  virtual void loaded(const tlp::Plugin* infos, const std::list <tlp::Dependency>&);
  virtual void aborted(const std::string &filename,const  std::string &erreurmsg);
  virtual void finished(bool state,const std::string &msg);
};

};
