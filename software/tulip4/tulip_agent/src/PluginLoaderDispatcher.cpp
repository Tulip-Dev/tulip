#include "PluginLoaderDispatcher.h"

PluginLoaderDispatcher::PluginLoaderDispatcher():
  tlp::PluginLoader() {
}

void PluginLoaderDispatcher::start(const std::string &path) {
  tlp::PluginLoader *ldr;
  Q_FOREACH(ldr,_loaders)
    ldr->start(path);
}

void PluginLoaderDispatcher::numberOfFiles(int n) {
  tlp::PluginLoader *ldr;
  Q_FOREACH(ldr,_loaders)
    ldr->numberOfFiles(n);
}

void PluginLoaderDispatcher::loading(const std::string &filename) {
  tlp::PluginLoader *ldr;
  Q_FOREACH(ldr,_loaders)
    ldr->loading(filename);
}

void PluginLoaderDispatcher::loaded(const tlp::AbstractPluginInfo* infos, const std::list <tlp::Dependency>& dependencies) {
  tlp::PluginLoader *ldr;
  Q_FOREACH(ldr,_loaders)
    ldr->loaded(infos,dependencies);
}

void PluginLoaderDispatcher::aborted(const std::string &filename,const  std::string &errormsg) {
  tlp::PluginLoader *ldr;
  Q_FOREACH(ldr,_loaders)
    ldr->aborted(filename,errormsg);
}

void PluginLoaderDispatcher::finished(bool state,const std::string &msg) {
  tlp::PluginLoader *ldr;
  Q_FOREACH(ldr,_loaders)
    ldr->finished(state,msg);
}

void PluginLoaderDispatcher::registerLoader(tlp::PluginLoader *loader) {
  _loaders.insert(loader);
}

void PluginLoaderDispatcher::removeLoader(tlp::PluginLoader *loader) {
  _loaders.remove(loader);
}
