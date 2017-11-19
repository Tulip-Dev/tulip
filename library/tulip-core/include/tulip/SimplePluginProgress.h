/*
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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
  ProgressState progress(int step, int max_step) override;
  void cancel() override;
  void stop() override;
  bool isPreviewMode() const override;
  void setPreviewMode(bool) override;
  void showPreview(bool) override;
  ProgressState state() const override;
  std::string getError() override {
    return _error;
  }
  void setError(const std::string &error) override {
    _error = error;
  }
  void setComment(const std::string &) override {}
  void setTitle(const std::string &) override {}

protected:
  virtual void progress_handler(int step, int max_step);
  // call when the _preview variable change.
  virtual void preview_handler(bool);

private:
  ProgressState _state;
  bool _preview;
  std::string _error;
};
}
#endif // SIMPLEPLUGINPROGRESS_H
///@endcond
