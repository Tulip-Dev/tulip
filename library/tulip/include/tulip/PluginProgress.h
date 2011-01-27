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

/**
  * @brief Interface to notify and control the progression of a process.
  *
  * To notify the progression use the progress function. You can ask
  * @code
  * PluginProgress *progress;
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
  * @endcode
  **/
class TLP_SCOPE PluginProgress {
 public:  
     virtual ~PluginProgress(){}
  /**
    * @brief Notify the progression of the process.
    * @brief The current step number.
    * @brief The maximum step number.
    **/
  virtual ProgressState progress(int step, int max_step)=0;
  /**
    * @brief Set the state flag to cancel to notify to the process that user want to cancel it.
    *
    * Canceling a process must stop it and revert all the changes performed since its start.
    **/
  virtual void cancel()=0;
  /**
    * @brief Set the state flag to stop to notify to the process that user want to stop it.
    *
    * Stopping a process dont revert changes.
    **/
  virtual void stop()=0;
  virtual bool isPreviewMode() const =0;
  virtual void setPreviewMode(bool)=0;
  virtual void showPreview(bool)=0;
  /**
    * @brief Get the internal state of the PluginProgress.
    **/
  virtual ProgressState state() const=0;
  /**
    * @brief Return a message describing the error encountered during the process.
    **/
  virtual std::string getError()=0;
  /**
    * @brief Set the message describing the error encountered during the process.
    **/
  virtual void setError(std::string error)=0;
  /**
    * @brief Change the comment about the process progression.
    **/
  virtual void setComment(std::string)=0;

};

}
#endif
