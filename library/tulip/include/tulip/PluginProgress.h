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

  /**
   * @brief Describes what the plug-in should do.
   * @enum tlp::ProgressState::TLP_CONTINUE The plugin should continue its execution.
   * @enum tlp::ProgressState::TLP_CANCEL The plugin should cancel, reverting all performed changes since the plugin was called.
   * @enum tlp::ProgressState::TLP_STOP The plugin should stop, leaving the graph in its current state.
   **/
  enum ProgressState{TLP_CONTINUE, TLP_CANCEL , TLP_STOP };

/**
  * @brief Interface to notify and control the progression of a process.
  *
  * To notify the progression use the progress function. You can ask
  * @code
  * PluginProgress *progress = new QtProgress(parentWidget, \"Awesome task #1\");
  * progress->setComment("First step");
  * for(int i = 0 ; i<50 ;++i){
  * //Perform some actions
  * //.....
  *
  * //Check internal state
  * if(progress->state()!=TLP_CONTINUE){
  * //User want to stop the process
  * break;
  * }
  *
  * //Notify progression.
  * progress->progress(i,50);
  * }
  *
  * //Delete the progress once you're done with it
  * delete progress;
  * @endcode
  **/
class TLP_SCOPE PluginProgress {
 public:  
     virtual ~PluginProgress(){}
  
  /**
   * @brief Notify the progression of the process.
   *
   * @param step The current step number.
   * @param max_step The total number of steps.
   * @return :ProgressState a value indicating whether the progress has been stopped, cancelled, or will continue.
   **/
  virtual ProgressState progress(int step, int max_step)=0;
    
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
   * @brief Get the current internal state of the PluginProgress.
   *
   * @return :ProgressState The current state.
   **/
  virtual ProgressState state() const=0;
  
  /**
   * @brief Return a message describing the error encountered during the process. If no error has been encountered, an empty string is returned.
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
  virtual void setError(std::string error)=0;
  
  /**
   * @brief Changes the comment about the process progression.
   *
   * @param comment A description of what the plugin is currently doing, displayed to inform the user.
   * @return void
   **/
  virtual void setComment(std::string comment)=0;

};

}
#endif
