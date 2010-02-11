#include "ComparePictureData.h"

class ComparePictureMainWindow : public QMainWindow,public Ui::MainWindow {

  Q_OBJECT

 public :

  ComparePictureMainWindow();

 protected slots:

  void differentButtonClicked();
  void sameAndRejectButtonClicked();

 protected :

  void checkPicture();

  QString currentTest;

  QStringList modelFilesList;
  QStringList resultFilesList;

  QStringList okTestsList;
  QStringList failedTestsList;

};
