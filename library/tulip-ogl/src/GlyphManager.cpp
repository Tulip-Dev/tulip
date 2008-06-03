#include "tulip/GlyphManager.h"

#include <tulip/hash_string.h>

#include "tulip/Glyph.h"

//====================================================
#ifdef _WIN32 
#ifdef DLL_EXPORT
tlp::GlyphManager* tlp::GlyphManager::inst=0;
#endif
#else
tlp::GlyphManager* tlp::GlyphManager::inst=0;
#endif

using namespace std;

namespace tlp
{
  static stdext::hash_map<int,std::string>   glyphIdToName;
  static stdext::hash_map<std::string, int>  nameToGlyphId;

  GlyphManager::GlyphManager() {
    GlyphFactory::initFactory();
  }
  //====================================================
  string GlyphManager::glyphName(int id) {
    if (glyphIdToName.find(id)!=glyphIdToName.end())
      return glyphIdToName[id];
    else {
      cerr << __PRETTY_FUNCTION__ << endl;
      cerr << "Invalid glyph id" << endl;
      return string("invalid");
    }
  }
  //====================================================
  int GlyphManager::glyphId(string name) {
    if (nameToGlyphId.find(name)!=nameToGlyphId.end())
      return nameToGlyphId[name];
    else {
      cerr << __PRETTY_FUNCTION__ << endl;
      cerr << "Invalid glyph name" << endl;
      return 0;
    }
  }
  //====================================================
  void GlyphManager::loadGlyphPlugins() {
    Iterator<string> *itS=GlyphFactory::factory->availablePlugins();
    while (itS->hasNext()) {
      string pluginName=itS->next();
      int pluginId=GlyphFactory::factory->objMap[pluginName]->getId();
      glyphIdToName[pluginId]=pluginName;
      nameToGlyphId[pluginName]=pluginId;
    } delete itS;
  }
  //====================================================
  void GlyphManager::loadPlugins(PluginLoader *plug) {
    GlyphFactory::initFactory();
    string::const_iterator begin=tlp::TulipPluginsPath.begin();
    string::const_iterator end=begin;
    glyphIdToName.clear();
    nameToGlyphId.clear();
    while (end!=tlp::TulipPluginsPath.end())
      if ((*end)==tlp::PATH_DELIMITER) {
	if (begin!=end)
	  tlp::loadPluginsFromDir(string(begin,end)+"/glyphs", "Glyph", plug);
	++end;
	begin=end;
      } else
	++end;
    if (begin!=end) {
      tlp::loadPluginsFromDir(string(begin,end)+"/glyphs", "Glyph", plug);
    }
    loadGlyphPlugins();
  }
  //
  void GlyphManager::initGlyphList(Graph **graph,GlGraphInputData* glGraphInputData,MutableContainer<Glyph *>& glyphs) {
    GlyphContext gc = GlyphContext(graph,glGraphInputData);
    glyphs.setAll(0);
    Iterator<string> *itS = GlyphFactory::factory->availablePlugins();
    while (itS->hasNext()) {
      string glyphName=itS->next();
      //cerr << "loading : " << glyphName << endl;
      Glyph *newGlyph = GlyphFactory::factory->getPluginObject(glyphName, &gc);
      //    cerr << newGlyph << endl;
      //if (GlyphFactory::factory->objMap[glyphName]->getId() == 0) 
      //  glyphs.setAll(newGlyph);
      //    cerr << " id:" << GlyphFactory::factory->objMap[glyphName]->getId() << endl;
      glyphs.set(GlyphFactory::factory->objMap[glyphName]->getId(), newGlyph);
    //    cerr << glyphs.get(GlyphFactory::factory->objMap[glyphName]->getId()) << endl;
    } delete itS;
  }

  void GlyphManager::clearGlyphList(Graph **graph,GlGraphInputData* glGraphInputData,MutableContainer<Glyph *>& glyphs) {
    Iterator<string> *itS = GlyphFactory::factory->availablePlugins();
    while (itS->hasNext()) {
      string glyphName=itS->next();
      delete glyphs.get(GlyphFactory::factory->objMap[glyphName]->getId());
    } delete itS;
  }
} 
