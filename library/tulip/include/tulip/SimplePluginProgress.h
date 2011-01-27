#ifndef SIMPLEPLUGINPROGRESS_H
#define SIMPLEPLUGINPROGRESS_H

#include "tulip/PluginProgress.h"
#include <string>
namespace tlp{
    /**
      * @brief Basic implementation for PluginProgress interface.
      *
      **/
    class TLP_SCOPE SimplePluginProgress : public PluginProgress
    {
    public:

        SimplePluginProgress();
        virtual ProgressState progress(int step, int max_step);
        virtual void cancel();
        virtual void stop();
        virtual bool isPreviewMode() const ;
        virtual void setPreviewMode(bool);
        virtual void showPreview(bool);
        virtual ProgressState state() const;
        virtual std::string getError(){
            return _error;
        }
        virtual void setError(std::string error){
            _error = error;
        }
        virtual void setComment(std::string){}

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
