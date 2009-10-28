#ifndef Tulip_CHOOSECONTROLLERDIALOG_H
#define Tulip_CHOOSECONTROLLERDIALOG_H

#include "ChooseControllerDialogData.h"

class ChooseControllerDialog : public QDialog, public Ui::ChooseControllerDialogData {
    Q_OBJECT

public:
  ChooseControllerDialog( QWidget* parent = 0);

  void setControllersAvailable(const std::vector<std::string> &controllersName);
  void setDefaultCheckedControllerName(const std::string &controllerName);
  void setDefaultControllerName(const std::string &controllerName);
  std::string getCheckedControllerName();

public slots:

  void accept();

protected slots:

  void itemClicked(QListWidgetItem *);

private :

  std::string defaultControllerName;

};

#endif
