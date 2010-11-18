#include "MagicSelectionInteractorConfigWidget.h"

#include <tulip/Graph.h>
#include <tulip/DoubleProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/PropertyInterface.h>

using namespace std;
using namespace tlp;

MagicSelectionInteractorConfigWidget::MagicSelectionInteractorConfigWidget(QWidget* parent ): QWidget(parent), _graph(NULL), _currentProperty(NULL) {
  setupUi(this);
  
  connect(selectionAdd, SIGNAL(toggled(bool)), this, SLOT(pushButtontoggled(bool)));
  connect(selectionRemove, SIGNAL(toggled(bool)), this, SLOT(pushButtontoggled(bool)));
  connect(selectionReplace, SIGNAL(toggled(bool)), this, SLOT(pushButtontoggled(bool)));
  connect(selectionIntersection, SIGNAL(toggled(bool)), this, SLOT(pushButtontoggled(bool)));
  
  connect(properties, SIGNAL(currentIndexChanged(QString)), this, SLOT(selectedPropertyChanged(const QString&)));
}

void MagicSelectionInteractorConfigWidget::pushButtontoggled(bool pushed) {
  std::vector<QPushButton*> selectionButtons(4);
  selectionButtons[0] = selectionAdd;
  selectionButtons[1] = selectionRemove;
  selectionButtons[2] = selectionReplace;
  selectionButtons[3] = selectionIntersection;
  
  if(pushed) {
    for(std::vector<QPushButton*>::const_iterator it = selectionButtons.begin(); it != selectionButtons.end(); ++it) {
      if(*it != QObject::sender()) {
        (*it)->setChecked(false);
      }
    }
  }
  else {
    bool canToggle = false;
    QPushButton* sender = (QPushButton*)QObject::sender();
    
    for(std::vector<QPushButton*>::const_iterator it = selectionButtons.begin(); it != selectionButtons.end(); ++it) {
      if(*it != sender && (*it)->isChecked()) {
        canToggle = true;
        break;
      }
    }
    if(!canToggle) {
      sender->setChecked(false);
    }
  }
}

Selectionbehavior MagicSelectionInteractorConfigWidget::selectionBehavior() const {
  if(selectionAdd->isChecked()) {
    return Add;
  }
  else if(selectionIntersection->isChecked()){
    return Intersect;
  }
  else if(selectionRemove->isChecked()){
    return Remove;
  }
  else if(selectionReplace->isChecked()){
    return Replace;
  }
  
  return Replace;
}

QString MagicSelectionInteractorConfigWidget::propertyName() const {
  return properties->currentText();
}

void MagicSelectionInteractorConfigWidget::setGraph(tlp::Graph* graph) {
  if(_graph) {
    _graph->removeObserver(this);
  }
  
  _graph = graph;
  _graph->addObserver(this);
  
  updateAvailableProperties();
}

void MagicSelectionInteractorConfigWidget::selectedPropertyChanged(const QString& propertyName) {
  if(_currentProperty) {
    _currentProperty->removeObserver(this);
  }
  
  if(!propertyName.isEmpty()) {
    _currentProperty = _graph->getProperty(propertyName.toStdString());
    _currentProperty->addObserver(this);
  }
  else {
    _currentProperty = NULL;
  }
}

void MagicSelectionInteractorConfigWidget::updateAvailableProperties() {
  
  QString previouslySelected = properties->currentText();
  
  Iterator<string>* propIt = _graph->getProperties();
  properties->clear();
  while(propIt->hasNext()) {
    string propertyName = propIt->next();
    PropertyInterface* propertyInterface = _graph->getProperty(propertyName);
    if(propertyInterface->getTypename() == "double" || propertyInterface->getTypename() == "int") {
      properties->addItem(QString::fromStdString(propertyName));
    }
  }
  delete propIt;
  
  for(int i = 0; i < properties->count(); ++i) {
    if(properties->itemText(i) == previouslySelected) {
      properties->setCurrentIndex(i);
      break;
    }
  }
  
}

bool MagicSelectionInteractorConfigWidget::directedSelection() const {
  return directed->isChecked();
}

double MagicSelectionInteractorConfigWidget::lowerBound() const {
  return underValue->value();
}

double MagicSelectionInteractorConfigWidget::upperBound() const {
  return overValue->value();
}

void MagicSelectionInteractorConfigWidget::update(std::set< Observable* >::iterator, std::set< Observable* >::iterator) {
  updateAvailableProperties();
}
