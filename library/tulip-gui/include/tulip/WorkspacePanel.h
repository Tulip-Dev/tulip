#ifndef WORKSPACEPANEL_H
#define WORKSPACEPANEL_H

#include <QtGui/QWidget>
#include <tulip/tulipconf.h>

namespace Ui {
class WorkspacePanel;
}

namespace tlp {

class TLP_QT_SCOPE WorkspacePanel : public QWidget {
  Q_OBJECT

  Ui::WorkspacePanel* _ui;

public:
  explicit WorkspacePanel(QWidget* parent=0);
  virtual ~WorkspacePanel();
};

}

#endif // WORKSPACEPANEL_H
