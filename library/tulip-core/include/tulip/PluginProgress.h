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

#ifndef TLP_PROGRESS
#define TLP_PROGRESS

#include <tulip/tulipconf.h>
#include <string>

namespace tlp {

/**
 * @ingroup Plugins
 * @brief The ProgressPreviewHandler class handles the way a process handled by a PluginProgress is handled
 *
 * When PluginProgress::setPreview() is called, the associated ProgressPreviewHandler will be enabled. Allowing it to implement custom behavior to allow the user to preview the result of the underleying process
 * Once enabled, the progressStateChanged method will be called back each time PluginProgress::progress is called to allow synchronizing the preview with progression.
 */
class TLP_SCOPE ProgressPreviewHandler {
public:
  virtual ~ProgressPreviewHandler();

  /**
   * @brief @brief Called back after PluginProgress::progress has been invoked.
   */
  virtual void progressStateChanged(int step,int max_step)=0;
};

/**
 * @ingroup Plugins
 *
 * @brief This enum describes callback actions for the underleying system when calling tlp::PluginProgress::progress();
 * @list
 * @li TLP_CONTINUE: tells that the process monitored by the the progress should continue.
 * @li TLP_CANCEL: The process should be cancelled, reverting all changes since it was started.
 * @li TLP_STOP: The process should stop, leaving all the changes made since the beginning
 * @endlist
 *
 * @see tlp::PluginProgress
 **/
enum ProgressState {
  /** The plugin should continue its execution. */
  TLP_CONTINUE,
  /** The plugin should cancel, reverting all performed changes since the plugin was called. */
  TLP_CANCEL,
  /** The plugin should stop, leaving the graph in its current state. */
  TLP_STOP
};

/**
 * @ingroup Plugins
 * @brief PluginProcess subclasses are meant to notify about the progress state of some process (typically a plugin)
 *
 * PluginProgress are mainly used alongside with tlp::Plugin instances to give user a visual feedback about the progress of the plugin.
 * Every plugin in tulip got a pluginProgress member they can call to give progress feedbacks. When running, the plugin should make a call to tlp::PluginProgress::progress() indicating the current state of the compuation.
 * The tlp::PluginProgress returns a tlp::ProgressState indicating what behavior the underleying system should have (see tlp::ProgressState for details)
 **/
class TLP_SCOPE PluginProgress {
  ProgressPreviewHandler* _previewHandler;

public:
  PluginProgress();
  virtual ~PluginProgress();
  void setPreviewHandler(ProgressPreviewHandler*);

  /**
   * @brief Notifies the progression of the process.
   *
   * @param step The current step number.
   * @param max_step The total number of steps.
   *
   * * @warning For default previsualisation handling to work, be sure to call PluginProgress::progress in this method (the return value can be ignored)
   *
   * @return tlp::ProgressState a value indicating whether the progress has been stopped, cancelled, or will continue.
   * @see tlp::ProgressState
   **/
  virtual ProgressState progress(int step, int max_step);

  /**
   * @brief Sets the state flag to cancel, notifying to the process that the user wants to cancel it.
   * Canceling a process must stop it and revert all the changes performed since its start.
   *
   * @return void
   **/
  virtual void cancel()=0;

  /**
  * @brief Sets the state flag to stop, notifying to the process that the user wants to stop it.
  * Stopping a process does not revert changes.
  * @return void
  **/
  virtual void stop()=0;

  /**
   * @brief The preview mode redraws the graph while applying the algorithm, making it slower.
   *
   * @return bool Whether the preview mode is activated.
   **/
  virtual bool isPreviewMode() const =0;


  /**
   * @brief The preview mode redraws the graph while applying the algorithm, making it slower.
   *
   * @param drawPreview Whether the preview should be drawn.
   * @return void
   **/
  virtual void setPreviewMode(bool drawPreview)=0;

  /**
   * @brief This tells the widget if it should show a preview checkbox, allowing the user to decide if the algorithm should draw a preview or not.
   *
   * @param showPreview Whether the progress widget should contain a preview checkbox or not.
   * @return void
   **/
  virtual void showPreview(bool showPreview)=0;

  /**
   * @brief Gets the current internal state of the PluginProgress.
   *
   * @return tlp::ProgressState The current state.
   **/
  virtual ProgressState state() const=0;

  /**
   * @brief Returns a message describing the error encountered during the process. If no error has been encountered, an empty string is returned.
   *
   * @return :string A description of the encountered error, if any.
   **/
  virtual std::string getError()=0;

  /**
   * @brief Sets the message describing the error encountered during the process.
   *
   * @param error The description of the encountered error.
   * @return void
   **/
  virtual void setError(const std::string& error)=0;

  /**
   * @brief Changes the comment about the process progression.
   *
   * @param comment A description of what the plugin is currently doing, displayed to inform the user.
   * @return void
   **/
  virtual void setComment(const std::string& comment)=0;

  /**
   * @brief Changes the title of that plugin progress
   *
   * @param title the title to set
   * @return void
   **/
  virtual void setTitle(const std::string& title)=0;


};

}
#endif
