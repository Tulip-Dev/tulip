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
#ifndef QtProgress_TULIP
#define QtProgress_TULIP
#include <string>
#include <tulip/PluginProgress.h>
#include <tulip/View.h>
#include <QtGui/QDialog>

namespace tlp {
    class PluginProgressWidget;
    /**
      * @brief QDialog wrapping a PluginProgressWidget. See the PluginProgressWidget and PluginProgress documentation for more information.
      *
      * Display and control a PluginProgress widget inside a QDialog.
      **/
    class TLP_QT_SCOPE QtProgress : public QDialog, public tlp::PluginProgress{
        Q_OBJECT
    public:
        QtProgress(QWidget* parent, std::string text, View *view=NULL,int updateInterval=200);
        virtual ~QtProgress();
    signals:
        void sendProgress(int, int);
    public slots:
        ProgressState progress(int step, int max_step);
        void cancel();
        void stop();
        bool isPreviewMode() const ;
        void setPreviewMode(bool);
        void showPreview(bool);
        ProgressState state() const;
        std::string getError();
        void setError(std::string error);
        void setComment(std::string);
    private:
        PluginProgressWidget* progressWidget;
        bool firstCall;
    };
}

#endif
