#ifndef TLP_PROGRESS
#define TLP_PROGRESS

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <tulip/tulipconf.h>
#include <string>

enum ProgressState{TLP_CONTINUE, TLP_CANCEL , TLP_STOP };

class TLP_SCOPE PluginProgress {
 public:
  PluginProgress();
  virtual ~PluginProgress();
  ProgressState progress(int step, int max_step);
  void cancel();
  void stop();
  ProgressState state() const;
  virtual void progress_handler(int step, int max_step);
 private:
  ProgressState _state;
};
#endif
