#include "%ProjectName:c%ConfigWidget.h"
#include "%ProjectName:c%.h"

using namespace std;
using namespace tlp;

%ProjectName:
c%ConfigWidget:: %ProjectName:
c%ConfigWidget(%ProjectName:c% *view):view(view) {
  setupUi(this);
}
//==================================================
void %ProjectName:
c%ConfigWidget::setData(DataSet dataSet) {
  DataSet internalData;

  if(dataSet.exist("%ProjectName:c%Config")) {
    dataSet.get<DataSet>("%ProjectName:c%Config", internalData);
    //update your config according to the dataset;
  }
}
//==================================================
void %ProjectName:
c%ConfigWidget::getData(DataSet *dataSet) {
  DataSet internalData;
//add your data
  dataSet->set("%ProjectName:c%Config",internalData);
}
//==================================================
