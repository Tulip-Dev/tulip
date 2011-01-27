#include "tulip/SimplePluginProgress.h"
using namespace tlp;
using namespace std;
SimplePluginProgress::SimplePluginProgress(): _state(TLP_CONTINUE),
_preview(false)
{
}
ProgressState SimplePluginProgress::progress(int step, int max_step) {
  progress_handler(step,max_step);
  return _state;
}
//====================================================
void SimplePluginProgress::stop() {
  _state=TLP_STOP;
}
//====================================================
void SimplePluginProgress::cancel() {
  _state=TLP_CANCEL;
}
//====================================================
bool SimplePluginProgress::isPreviewMode() const {
  return _preview;
}
//====================================================
void SimplePluginProgress::setPreviewMode(bool b) {
    if (b == _preview) return;
    _preview = b;
    preview_handler(_preview);
}
//====================================================
void SimplePluginProgress::showPreview(bool) {
}
//====================================================
ProgressState SimplePluginProgress::state() const {
  return _state;
}
//====================================================
void SimplePluginProgress::progress_handler(int, int) {
}
//====================================================
void SimplePluginProgress::preview_handler(bool) {
}
//====================================================
