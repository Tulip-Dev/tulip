/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
#ifndef TLP_PROGRESS
#define TLP_PROGRESS

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <tulip/tulipconf.h>
#include <string>

namespace tlp {

enum ProgressState{TLP_CONTINUE, TLP_CANCEL , TLP_STOP };

class TLP_SCOPE PluginProgress {
 public:
  PluginProgress();
  virtual ~PluginProgress();
  ProgressState progress(int step, int max_step);
  void cancel();
  void stop();
  bool isPreviewMode() const ;
  void setPreviewMode(bool);
  virtual void showPreview(bool);
  ProgressState state() const;
  std::string getError() { return _error; }
  void setError(std::string error) { _error = error; }
  virtual void setComment(std::string) {}
  virtual void progress_handler(int step, int max_step);
  //call when the _preview variable change.
  virtual void preview_handler(bool);
 private:
  ProgressState _state;
  bool _preview;
  std::string _error;
};

}
#endif
