#include "ParallelCoordinatesConfigDialog.h"
#include "ParallelTools.h"

#include <QtGui/QHBoxLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QFileDialog>
#include <QtGui/QPalette>
#include <QtGui/QColor>
#include <QtGui/QColorDialog>
#include <QtGui/QTabWidget>

#include <tulip/TlpTools.h>

#include <algorithm>

using namespace std;

namespace tlp {

  ParallelCoordinatesConfigDialog::ParallelCoordinatesConfigDialog(ParallelCoordinatesGraphProxy *data, QWidget *parent) : QDialog(parent), graphProxy(data) {
	  setupUi(this);

	  vector<string> propertyTypesFilter;
	  propertyTypesFilter.push_back("double");
	  propertyTypesFilter.push_back("int");
	  propertyTypesFilter.push_back("string");
	  graphPropertiesSelectionWidget->setWidgetParameters(graphProxy, propertyTypesFilter);

	  // if number of data to plot is consequent, don't draw points on axis
	  // by default to speed up rendering
	  if (graphProxy->getDataCount() > PROGRESS_BAR_DISPLAY_NB_DATA_THRESHOLD) {
		  setDrawPointOnAxis(false);
	  }

	  connect(browseButton,SIGNAL(clicked()),this,SLOT(pressButtonBrowse()));
	  connect(userTexture, SIGNAL(toggled(bool)), this, SLOT(userTextureRbToggled(bool)));
	  connect(minAxisPointSize, SIGNAL(valueChanged(int)), this, SLOT(minAxisPointSizeValueChanged(int)));
	  connect(maxAxisPointSize, SIGNAL(valueChanged(int)), this, SLOT(maxAxisPointSizeValueChanged(int)));
	  connect(bgColorButton,SIGNAL(clicked()),this,SLOT(pressColorButton()));
  }

  void ParallelCoordinatesConfigDialog::updateSelectedProperties() {
  	  Iterator<string> *properties = graphProxy->getProperties();
  	  selectedProperties = graphProxy->getSelectedProperties();
  	  vector<string> stringList;
  	  vector<string>::iterator it;
  	  string propertyName;

  	  graphPropertiesSelectionWidget->clearLists();

  	  while (properties->hasNext()) {
  		  propertyName = properties->next();
  		  if (std::find(selectedProperties.begin(), selectedProperties.end(), propertyName) == selectedProperties.end()) {
  			  stringList.push_back(propertyName);
  		  }
  	  }
  	  delete properties;

  	  graphPropertiesSelectionWidget->setInputPropertiesList(stringList);

  	  stringList.clear();
  	  for (it = selectedProperties.begin() ; it != selectedProperties.end() ; ++it) {
  		  stringList.push_back(*it);
  	  }
  	  graphPropertiesSelectionWidget->setOutputPropertiesList(stringList);

    }

  vector<string> ParallelCoordinatesConfigDialog::getSelectedProperties()  const {
    return selectedProperties;
  }


  void ParallelCoordinatesConfigDialog::accept(){
    selectedProperties = graphPropertiesSelectionWidget->getSelectedProperties();
    graphProxy->setSelectedProperties(selectedProperties);
    graphProxy->setDataLocation(getDataLocation());
    QDialog::accept();
  }

  void ParallelCoordinatesConfigDialog::reject(){
      restoreBackupConfiguration();
      QDialog::reject();
  }

  void ParallelCoordinatesConfigDialog::pressButtonBrowse() {
	  QString fileName = QFileDialog::getOpenFileName(this, tr("Open Texture File"), "./", tr("Image Files (*.png *.jpg *.bmp)"));
	  userTextureFile->setText(fileName);
  }

  const ElementType ParallelCoordinatesConfigDialog::getDataLocation() const {
    if (nodesButton->isChecked()) {
      return NODE;
    } else {
      return EDGE;
    }
  }

  void ParallelCoordinatesConfigDialog::setDataLocation(ElementType location) {
    if (location == NODE) {
      edgesButton->setChecked(false);
      nodesButton->setChecked(true);
    } else {
      edgesButton->setChecked(true);
      nodesButton->setChecked(false);
    }
  }

  unsigned int ParallelCoordinatesConfigDialog::getAxisHeight() const {
  	  return axisHeight->value();
  }

  void ParallelCoordinatesConfigDialog::setAxisHeight(const unsigned int aHeight) {
	  axisHeight->setValue(aHeight);
  }

  bool ParallelCoordinatesConfigDialog::drawPointOnAxis() const {
	  return gBoxAxisPoints->isChecked();
  }

  string ParallelCoordinatesConfigDialog::getLinesTextureFilename() const {
	  if (gBoxLineTexture->isChecked()) {
		  if (defaultTexture->isChecked()) {
			  return string(TulipBitmapDir + DEFAULT_TEXTURE_FILE);
		  } else {
			  return userTextureFile->text().toStdString();
		  }
	  } else {
		  return "";
	  }
  }

  void ParallelCoordinatesConfigDialog::setLinesTextureFilename(const std::string &linesTextureFileName) {
	  if (linesTextureFileName != "") {
		  gBoxLineTexture->setChecked(true);
		  if (linesTextureFileName == string(TulipBitmapDir + DEFAULT_TEXTURE_FILE)) {
			  defaultTexture->setChecked(true);
		  } else {
			  userTexture->setChecked(true);
			  userTextureFile->setText(QString(linesTextureFileName.c_str()));
		  }
	  } else {
		  gBoxLineTexture->setChecked(false);
	  }
  }

  void ParallelCoordinatesConfigDialog::showEvent (QShowEvent * event) {
	  updateSelectedProperties();
	  backupConfiguration();


	  tabWidget->setCurrentIndex(0);
	  QWidget::showEvent(event);
  }

  Size ParallelCoordinatesConfigDialog::getAxisPointMinSize() const {
	  float pointSize = minAxisPointSize->text().toFloat();
	  return Size(pointSize, pointSize, pointSize);
  }

  Size ParallelCoordinatesConfigDialog::getAxisPointMaxSize() const {
	  float pointSize = maxAxisPointSize->text().toFloat();
	  return Size(pointSize, pointSize, pointSize);
  }

  void ParallelCoordinatesConfigDialog::setAxisPointMinSize(const unsigned int axisPointMinSize) {
		  minAxisPointSize->setValue(axisPointMinSize);
  }

  void ParallelCoordinatesConfigDialog::setAxisPointMaxSize(const unsigned int axisPointMaxSize) {
	  maxAxisPointSize->setValue(axisPointMaxSize);
  }

  void ParallelCoordinatesConfigDialog::userTextureRbToggled(bool checked) {
	  if (checked) {
		  userTextureFile->setEnabled(true);
		  browseButton->setEnabled(true);
	  } else {
		  userTextureFile->setEnabled(false);
		  browseButton->setEnabled(false);
	  }
  }

  void ParallelCoordinatesConfigDialog::minAxisPointSizeValueChanged(int newValue) {
	  if (maxAxisPointSize->value() < newValue) {
		  maxAxisPointSize->setValue(newValue + 1);
	  }
  }

  void ParallelCoordinatesConfigDialog::maxAxisPointSizeValueChanged(int newValue) {
	  if (minAxisPointSize->value() > newValue) {
		  minAxisPointSize->setValue(newValue - 1);
	  }
  }

  void ParallelCoordinatesConfigDialog::setLinesColorAlphaValue(unsigned int value) {
	  linesColorAlphaValue->setValue(value);
  }

  unsigned int ParallelCoordinatesConfigDialog::getLinesColorAlphaValue() const {
	  return (unsigned int) linesColorAlphaValue->value();
  }

  Color ParallelCoordinatesConfigDialog::getBackgroundColor() const {
	  QColor bgColor = bgColorButton->palette().color(QPalette::Button);
	  return Color(bgColor.red(), bgColor.green(), bgColor.blue());
  }

  void ParallelCoordinatesConfigDialog::setBackgroundColor(const Color &color) {
	  QPalette palette;
	  palette.setColor(QPalette::Button, QColor(color.getR(), color.getG(), color.getB(), color.getA()));
	  bgColorButton->setPalette(palette);
  }

  void ParallelCoordinatesConfigDialog::pressColorButton() {
	  QColor newColor = QColorDialog::getColor(bgColorButton->palette().color(QPalette::Button), this);
	  if (newColor.isValid()) {
		  QPalette palette;
		  palette.setColor(QPalette::Button, newColor);
		  bgColorButton->setPalette(palette);
	  }
  }

  void ParallelCoordinatesConfigDialog::backupConfiguration() {
	  if (nodesButton->isChecked()) {
		  dataLocationBak = NODE;
	  } else {
		  dataLocationBak = EDGE;
	  }
	  bgColorBak = getBackgroundColor();
	  axisHeightBak = axisHeight->value();
	  drawPointOnAxisBak = gBoxAxisPoints->isChecked();
	  minAxisPointSizeBak = minAxisPointSize->value();
	  maxAxisPointSizeBak = maxAxisPointSize->value();
	  linesColorAlphaValueBak = linesColorAlphaValue->value();
	  linesTextureBak = gBoxLineTexture->isChecked();
	  userTextureBak = userTexture->isChecked();
	  userTextureFileBak = userTextureFile->text().toStdString();
  }

  void ParallelCoordinatesConfigDialog::restoreBackupConfiguration() {
	  nodesButton->setChecked(dataLocationBak == NODE);
	  edgesButton->setChecked(dataLocationBak == EDGE);

	  QPalette palette;
	  palette.setColor(QPalette::Button, QColor(bgColorBak.getR(), bgColorBak.getG(), bgColorBak.getB()));
	  bgColorButton->setPalette(palette);

	  axisHeight->setValue(axisHeightBak);
	  gBoxAxisPoints->setChecked(drawPointOnAxisBak);
	  minAxisPointSize->setValue(minAxisPointSizeBak);
	  maxAxisPointSize->setValue(maxAxisPointSizeBak);
	  linesColorAlphaValue->setValue(linesColorAlphaValueBak);
	  gBoxLineTexture->setChecked(linesTextureBak);
	  defaultTexture->setChecked(!userTextureBak);
	  userTexture->setChecked(userTextureBak);
	  userTextureFile->setText(QString(userTextureFileBak.c_str()));
  }

  unsigned int ParallelCoordinatesConfigDialog::getSpaceBetweenAxis() const {
	  return spaceBetweenAxis->value();
  }

  void ParallelCoordinatesConfigDialog::setSpaceBetweenAxis(const unsigned int spaceBetweenAxis) {
	  this->spaceBetweenAxis->setValue(spaceBetweenAxis);
  }

  void ParallelCoordinatesConfigDialog::setDrawPointOnAxis(const bool drawPointOnAxis) {
	  gBoxAxisPoints->setChecked(drawPointOnAxis);
  }

}
