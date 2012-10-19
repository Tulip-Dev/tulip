#include <tulip/PluginProgress.h>

using namespace tlp;

PluginProgress::PluginProgress(): _previewHandler(NULL) {
}

PluginProgress::~PluginProgress() {
  delete _previewHandler;
}

void PluginProgress::setPreviewHandler(ProgressPreviewHandler* handler) {
  delete _previewHandler;
  _previewHandler = handler;
  if (_previewHandler != NULL)
    _previewHandler->setEnabled(isPreviewMode());
}
ProgressState PluginProgress::progress(int step, int max_step) {
  if (_previewHandler != NULL)
    _previewHandler->progressStateChanged(step,max_step);
  return TLP_CONTINUE;
}

void PluginProgress::setPreviewMode(bool drawPreview) {
  if (_previewHandler != NULL)
    _previewHandler->setEnabled(drawPreview);
}


ProgressPreviewHandler::ProgressPreviewHandler():_enabled(false){
}

ProgressPreviewHandler::~ProgressPreviewHandler() {
}

void ProgressPreviewHandler::setEnabled(bool f) {
  if (f == _enabled)
    return;
  _enabled = f;
  previewStateChanged(f);
}

bool ProgressPreviewHandler::isEnabled() const {
  return _enabled;
}
