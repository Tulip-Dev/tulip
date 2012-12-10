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
}
ProgressState PluginProgress::progress(int step, int max_step) {
  if (_previewHandler != NULL && isPreviewMode())
    _previewHandler->progressStateChanged(step,max_step);

  return TLP_CONTINUE;
}

ProgressPreviewHandler::~ProgressPreviewHandler() {
}
