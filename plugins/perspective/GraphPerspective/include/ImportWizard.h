#ifndef IMPORTWIZARD_H
#define IMPORTWIZARD_H

#include <QtGui/QWizard>
#include <tulip/WithParameter.h>

namespace Ui {
class ImportWizard;
}

class ImportWizard: public QWizard {
  Q_OBJECT

  Ui::ImportWizard* _ui;
public:
  explicit ImportWizard(QWidget *parent = 0);
  virtual ~ImportWizard();

  QString algorithm() const;
  QString group() const;
  tlp::DataSet parameters() const;

protected slots:
  void groupSelected(const QString&);
  void algorithmSelected(const QString&);

  void updateFinishButton();
};

#endif // IMPORTWIZARD_H
