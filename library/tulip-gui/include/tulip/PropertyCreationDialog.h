/*
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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
///@cond DOXYGEN_HIDDEN


#ifndef PROPERTYCREATIONDIALOG_H
#define PROPERTYCREATIONDIALOG_H

#include <QDialog>

#include <tulip/tulipconf.h>

namespace Ui {
class PropertyCreationDialog;
}

namespace tlp {

class PropertyInterface;
class Graph;

/**
  * @brief Provide a dialog that allow user to create a new property.
  *
  * The easiest way to use this class is to use the static function.
  * @code
  * Graph* g;
  * QWidget* parent;
  * PropertyCreationDialog::createNewProperty(g,parent);
  * @endcode
  *
  *
  **/
class TLP_QT_SCOPE PropertyCreationDialog : public QDialog {
  Q_OBJECT

public:
  /**
    * @brief Constructs a property creation dialog with the given parent.
    **/
  explicit PropertyCreationDialog(QWidget *parent = NULL);
  /**
    * @brief Constructs a property creation dialog with the given parent graph and parent widget.
    **/
  explicit PropertyCreationDialog(tlp::Graph* graph,QWidget *parent = NULL,
                                  const std::string& selectedType = "");
  ~PropertyCreationDialog();


  /**
    * @brief Try to create a new property from the givent parameters.
    * To get the created property use the createdProperty() function.
    **/
  void accept();

  /**
    * @brief Change the graph to use as parent for the properties to create.
    **/
  void setGraph(tlp::Graph* graph);


  tlp::Graph* getGraph()const {
    return _graph;
  }

  /**
    * @brief Return the property created. You need to call this function after the accept() function.
    *
    * @return The last created property or NULL if no property there is an error during the property creation.
    **/
  tlp::PropertyInterface* createdProperty()const {
    return _createdProperty;
  }

  /**
    * @brief This is a convenience static function that create a new property using user parameters. If the user presses Cancel or an error occur, it returns a null pointer.
    *
    * The function creates a modal property creation dialog with the given graph and parent widget.
    *
    * @param graph The graph to use as parent for the properties to create.
    **/
  static PropertyInterface* createNewProperty(tlp::Graph* graph,
      QWidget* parent=NULL,
      const std::string& selectedType = "");
private slots:
  void checkValidity();
private:
  void initGui();

  Ui::PropertyCreationDialog *ui;
  QPushButton* _createPropertyButton;
  Graph* _graph;
  PropertyInterface* _createdProperty;
};
}

#endif // PROPERTYCREATIONDIALOG_H
///@endcond
