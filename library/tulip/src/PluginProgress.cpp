#include "tulip/PluginProgress.h"

using namespace tlp;

//====================================================
PluginProgress::PluginProgress() :
  _state(TLP_CONTINUE),
  _preview(false) {
}
//====================================================
PluginProgress::~PluginProgress(){
}
//====================================================
ProgressState PluginProgress::progress(int step, int max_step) {
  progress_handler(step,max_step);
  return _state;
}
//====================================================
void PluginProgress::stop() {
  _state=TLP_STOP;
}
//====================================================
void PluginProgress::cancel() {
  _state=TLP_CANCEL;
}
//====================================================
bool PluginProgress::isPreviewMode() const {
  return _preview;
}
//====================================================
void PluginProgress::setPreviewMode(bool b) {
  _preview = b;
}
//====================================================
void PluginProgress::showPreview(bool b) {
}
//====================================================
ProgressState PluginProgress::state() const {
  return _state;
}
//====================================================
void PluginProgress::progress_handler(int step, int max_step) {
}
//====================================================
