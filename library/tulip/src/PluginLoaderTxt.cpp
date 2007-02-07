
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include "tulip/PluginLoaderTxt.h"

using namespace std;
using namespace tlp;
void PluginLoaderTxt::start(const string &path,const string &type)
{
  cout << "Start loading " << type << " plug-ins in " << path << endl;
}

void PluginLoaderTxt::loading(const string &filename)
{
  cout << "loading file : " << filename << endl;
}

void PluginLoaderTxt::loaded(const string &name,
			     const string &author,
			     const string &date, 
			     const string &info,
			     const string &release,
			     const string &version,
			     const list <Dependency> &deps)
{
  cout << "Plug-in " << name << " loaded, Author:"<< author << " Date: " << date << " Release:" << release << " Version: "<< version <<  endl;
  // ouput dependencies if any
  if (deps.size()) {
    unsigned int i = deps.size();
    cout << "depending on ";
    list<Dependency>::const_iterator itD = deps.begin();
    for (i--; itD != deps.end(); itD++, i--) {
      std::string factoryDepName = (*itD).factoryName;
      std::string pluginDepName = (*itD).pluginName;
      cout << factoryDepName << " " << pluginDepName;
      if (i > 0)
	cout << ", ";
      else
	cout << endl;
    }
  }
}

void PluginLoaderTxt::aborted(const string &filename,const  string &erreurmsg)
{
  cout << "Aborted loading of "<< filename << " Error:" << erreurmsg << endl;
}

void PluginLoaderTxt::finished(bool state,const string &msg)
{
  if (state)
    cout << "Loading complete" << endl;
  else
      cout << "Loading error " << msg << endl;
}
