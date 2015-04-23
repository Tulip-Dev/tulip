/*
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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
///@cond DOXYGEN_HIDDEN


#ifndef SIMPLEPLUGINPROGRESS_H
#define SIMPLEPLUGINPROGRESS_H

#include <tulip/PluginProgress.h>
#include <string>
namespace tlp {
/**
  * @brief Basic implementation for PluginProgress interface.
  *
  **/
class TLP_SCOPE SimplePluginProgress : public PluginProgress {
public:

  SimplePluginProgress();
  virtual ProgressState progress(int step, int max_step);
  virtual void cancel();
  virtual void stop();
  virtual bool isPreviewMode() const ;
  virtual void setPreviewMode(bool);
  virtual void showPreview(bool);
  virtual ProgressState state() const;
  virtual std::string getError() {
    return _error;
  }
  virtual void setError(const std::string& error) {
    _error = error;
  }
  virtual void setComment(const std::string&) {}
  virtual void setTitle(const std::string&) {}


protected:
  virtual void progress_handler(int step, int max_step);
  //call when the _preview variable change.
  virtual void preview_handler(bool);
private:
  ProgressState _state;
  bool _preview;
  std::string _error;
};
}
#endif // SIMPLEPLUGINPROGRESS_H
///@endcond
