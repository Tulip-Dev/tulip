#include "tulip/PluginProgress.h"

PluginProgress::PluginProgress():_state(TLP_CONTINUE){}
PluginProgress::~PluginProgress(){}
ProgressState PluginProgress::progress(int step, int max_step) {
  progress_handler(step,max_step);
  return _state;
}
void PluginProgress::stop() {_state=TLP_STOP;}
void PluginProgress::cancel() {_state=TLP_CANCEL;}
ProgressState PluginProgress::state() const {return _state;}
void PluginProgress::progress_handler(int step, int max_step) {}
