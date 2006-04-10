//-*-c++-*
/**
 Author: David Auber
 Email : auber@labri.fr
 Last modification : 26/09/2001
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/
//====================================================================================
#include <iostream>
#ifdef _WIN32
#include <stdarg.h>
#include <windef.h>
#include <winbase.h>
#else
#include <dlfcn.h>
#include <dirent.h>
#endif
#include <tulip/StlIterator.h>
#ifdef _WIN32
#define BUFSIZE 70
#endif

#include <tulip/Plugin.h>
//implemented in TlpTools.cpp
int __tulip_selectSO(const struct dirent *ent);


template<class ObjectFactory, class ObjectType, class Parameter>
Iterator<std::string>* TemplateFactory<ObjectFactory, ObjectType, Parameter>::availablePlugins() {
  return new StlIterator<std::string,std::set<std::string>::const_iterator>(objNames.begin(), objNames.end());
}

template<class ObjectFactory, class ObjectType, class Parameter>
bool TemplateFactory<ObjectFactory, ObjectType, Parameter>::exists(const std::string &pluginName) {
  return (objMap.find(pluginName) != objMap.end());
}


template<class ObjectFactory, class ObjectType, class Parameter>
void TemplateFactory<ObjectFactory,ObjectType,Parameter>::getPluginParameters(PluginLoader *loader) {
  Plugin *tmpObjectFactory=(Plugin *)createObj();
  if(dynamic_cast< ObjectFactory* >(tmpObjectFactory) != 0) {
    ObjectFactory *objectFactory=(ObjectFactory *)tmpObjectFactory;
    objNames.insert(objectFactory->getName());
    objMap[objectFactory->getName()]=objectFactory;
    if (loader!=0) loader->loaded(
				  objectFactory->getName(),
				  objectFactory->getAuthor(),
				  objectFactory->getDate(),
				  objectFactory->getInfo(),
				  objectFactory->getRelease(),
				  objectFactory->getVersion()
				  );
    Parameter p = Parameter();
    WithParameter *withParam=objectFactory->createObject(p);
    objParam[objectFactory->getName()]=withParam->getParameters();
  }
}

#ifdef _WIN32
template<class ObjectFactory, class ObjectType, class Parameter>
void TemplateFactory<ObjectFactory,ObjectType,Parameter>::loadWindows(std::string pluginPath,std::string type,PluginLoader *loader) {
	 bool result=true;std::string msg;
	if (loader!=0) loader->start(pluginPath.c_str(),type);
  	HANDLE hFind; 
	WIN32_FIND_DATA FindData; 
	int n = 0;
	TCHAR currentDirectory[BUFSIZE];
	DWORD dwRet;
    dwRet = GetCurrentDirectory(BUFSIZE, currentDirectory);
    if(dwRet == 0) {
   	    msg=std::string("Scandir error");//perror("scandir");
	  	result=false;
	  	return;
   	 }
	SetCurrentDirectory (pluginPath.c_str()); 
	hFind=FindFirstFile ("*.dll", &FindData); 
	if (hFind != INVALID_HANDLE_VALUE) { 
		n++;	
		char *error;
      	std::string tmpStr;
      	tmpStr= pluginPath +"/"+ FindData.cFileName;
      	if (loader!=0) loader->loading(FindData.cFileName);
      	createObj = loadWin32plugin(tmpStr, loader);
	if(createObj)
	  this->getPluginParameters(loader);
      	}
	while (FindNextFile (hFind, &FindData))	{
		n++;	
		char *error;
      	std::string tmpStr;
      	tmpStr= pluginPath +"/"+ FindData.cFileName;
      	if (loader!=0) loader->loading(FindData.cFileName);
      	createObj = loadWin32plugin(tmpStr, loader);
	if(createObj)
	  this->getPluginParameters(loader);
	}  
	if(n < 0) {
		msg=std::string("Scandir error");//perror("scandir");
		SetCurrentDirectory (currentDirectory); 
    	result=false;
		}
	  if (loader!=0) loader->numberOfFile(n);
	  if(loader!=0) loader->finished(result,msg);
	  SetCurrentDirectory (currentDirectory); 
}
#endif

template<class ObjectFactory, class ObjectType, class Parameter>
void TemplateFactory<ObjectFactory,ObjectType,Parameter>::load(std::string pluginPath,std::string type,PluginLoader *loader) {
  bool result=true;std::string msg;
  #ifdef _WIN32
  loadWindows(pluginPath, type, loader);
  #else
  struct dirent **namelist;
  if (loader!=0) loader->start(pluginPath.c_str(),type);
  int n = scandir((const char *) pluginPath.c_str(), &namelist, __tulip_selectSO, alphasort);
  if (loader!=0) loader->numberOfFile(n);
  if (n < 0) {
    msg=std::string("Scandir error");//perror("scandir");
    result=false;
  }
  else {
    while(n-->0) {
      std::string tmpStr;
      tmpStr= pluginPath +"/"+ std::string(namelist[n]->d_name);
      if (loader!=0) loader->loading(std::string(namelist[n]->d_name));
      createObj = loadUnixPlugin(tmpStr, loader);
      if(createObj != NULL)
	getPluginParameters(loader);
    }
  }
  if (loader!=0) loader->finished(result,msg);
  #endif
}

template<class ObjectFactory, class ObjectType, class Parameter>
bool TemplateFactory<ObjectFactory,ObjectType,Parameter>::load(std::string file) {
  createObj = getCreationFunc(file);
  if(createObj == NULL)
    return false;
  else {
    ObjectFactory *tmpObjectFactory=(ObjectFactory *)createObj();
    objMap[tmpObjectFactory->getName()]=tmpObjectFactory;
    Parameter p;
    WithParameter *withParam=tmpObjectFactory->createObject(p);
    objParam[tmpObjectFactory->getName()]=withParam->getParameters();
  }
 return true;
}

template<class ObjectFactory, class ObjectType, class Parameter>
ObjectType * TemplateFactory<ObjectFactory,ObjectType,Parameter>::getObject(std::string name,Parameter p)
{
  typename ObjectCreator::iterator it;
#ifndef NDEBUG 
  std::cerr << "TemplateFactory::GetObject :" << name << std::endl;  
#endif
  it=objMap.find(name);
  if (it!=objMap.end()) return (*it).second->createObject(p);
  return 0;
}
template<class ObjectFactory, class ObjectType, class Parameter>
StructDef TemplateFactory<ObjectFactory,ObjectType,Parameter>::getParam(std::string name){
  assert(objMap.find(name)!=objMap.end());
  return objParam[name];
}
