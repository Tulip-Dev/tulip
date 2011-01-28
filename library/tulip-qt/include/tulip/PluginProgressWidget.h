#ifndef PLUGINPROGRESSWIDGET_H
#define PLUGINPROGRESSWIDGET_H

#include <tulip/tulipconf.h>
#include <tulip/SimplePluginProgress.h>
#include <tulip/View.h>
#include <QtGui/QWidget>
#include <QtCore/QTime>
#include <string>
namespace Ui {
    class PluginProgressWidget;
}
namespace tlp{
    /**
      * @brief Display the progression of a process and allow user to stop or cancel it.
      *
      * Use the stop or cancel buttons to control the process.
      * If user give a View and if the preview check box is checked each time the progress_handler function is called the init function of the view is called.
      * Be careful when using the preview mode as it can slow down the process.
      **/
class TLP_QT_SCOPE PluginProgressWidget : public QWidget, public tlp::SimplePluginProgress
{
    Q_OBJECT

public:
    PluginProgressWidget(QWidget *parent = 0);
    /**
      * @brief Create a PluginProgressWidget.
      * @param view The view to update if the preview mode is enabled. Can be NULL if you don't want to update a view.
      * @param updateInterval The minimum time interval (in msec) between two update the progress bar. Avoid to perform too many call to progressBar redraw function that can slow down the process.
      * @parma parent The parent widget.
      **/
    PluginProgressWidget(View *view=NULL,int updateInterval=200,QWidget *parent = 0);

    ~PluginProgressWidget();
    /**
     * @brief Show or hide the preview check box in the widget.
     **/
    void showPreview(bool);

  public slots:
    /**
      * @brief This slot set the progress state to STOP. This will stop process, if it handle this state. See the PluginProgress class for more information.
      **/
    void stopCompute();
    /**
      * @brief This slot set the progress state to CANCEL. This will cancel the process, if it handle this state. See the PluginProgress class for more information.
      **/
    void cancelCompute();
    /**
      * @brief Enable or disable the preview mode.
      **/
    void changePreview(bool);
    void setComment(std::string msg);
protected:
    void progress_handler(int i,int j);
    void preview_handler(bool b);

  private:    
    View *view;
    int updateIterval;
    QTime time;
    Ui::PluginProgressWidget *ui;
};
}
#endif // PLUGINPROGRESSWIDGET_H
