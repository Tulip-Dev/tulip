#ifndef PYTHONPANEL_H
#define PYTHONPANEL_H

#include <QtGui/QWidget>


namespace Ui {
class PythonPanel;
}

namespace tlp {
class GraphHierarchiesModel;
}

class QPropertyAnimation;
class QAbstractScrollArea;
class OutputPanelButton;

class PythonPanel : public QWidget {


    Q_OBJECT
    Ui::PythonPanel *_ui;
    OutputPanelButton *_button;
    QPropertyAnimation *_animation;

public :

    explicit PythonPanel(QWidget *parent = 0);

    void setModel(tlp::GraphHierarchiesModel* model);

    void setPanelButton(OutputPanelButton *button);

    QAbstractScrollArea *consoleWidget();

private slots:

    void graphComboIndexChanged();

    void clearConsole();

    void newOutputInConsole();

    void stopButtonAnimation();

private:

    void startButtonAnimation();

};

#endif // PYTHONPANEL_H
