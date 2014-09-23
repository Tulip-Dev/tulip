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

#ifndef COPYPROPERTYDIALOG_H
#define COPYPROPERTYDIALOG_H

#include <tulip/tulipconf.h>

#include <QDialog>

namespace Ui {
class CopyPropertyDialogData;
}

namespace tlp {
class Graph;
class PropertyInterface;

/**
  * @brief Provide a dialog that allow user to copy a property in an existing property or in a new one.
  *
  * The easiest way to use this class is to use the copyProperty static function.
  * @code
  * Graph* g;
  * PropertyInterface* source = g->getLocalProperty<BooleanProperty>("viewSelection");
  * PropertyInterface* clonedProperty = CopyPropertyDialog::copyProperty(g, source);
  * @endcode
  *
  *
  **/

class TLP_QT_SCOPE CopyPropertyDialog : public QDialog {
  Q_OBJECT
public:
  CopyPropertyDialog(QWidget *parent = NULL);
  ~CopyPropertyDialog();

  /**
    * @brief Init dialog with required parameters. To make the copy this dialog need to have a valid source property and destination graph.
    **/
  void init(tlp::Graph* graph,tlp::PropertyInterface* toCopy);

  /**
    * @brief Perform the copy of the property in function of the parameters given by user. If parameters are invalid return a null pointer and fill the errorMsg with the description of the error.
    *
    * This function don't hold observers during the copy process. It's up to user to call Observable::holdObserver and Observable::unholdObserver before and after calling this funtion.
    **/
  tlp::PropertyInterface* copyProperty(QString& errorMsg);

  /**
    * @brief Get the name of the destintation property.
    **/
  QString destinationPropertyName()const;

  enum PropertyScope {
    NEW,
    LOCAL,
    INHERITED
  };

  /**
    * @brief Return the scope where the new property will be created.
    **/
  PropertyScope destinationPropertyScope()const;

  /**
    * @brief This is a convenience static function that copy property in function of user parameters. If the user presses Cancel or an error occur, it returns a null pointer.
    *
    * The function creates a modal property copy dialog with the given source property, graph and parent widget.
    *
    * @param graph The graph to use as parent for the properties to create.
    * @param source The property to copy.
    * @param askBeforePropertyOverwriting If set to true and user try to create a new property with the same name than another existing ask user before overwriting them.
    **/
  static PropertyInterface* copyProperty(tlp::Graph* graph,tlp::PropertyInterface* source,bool askBeforePropertyOverwriting=false,QWidget* parent=NULL);

private:
  Ui::CopyPropertyDialogData *ui;
  tlp::Graph* _graph;
  tlp::PropertyInterface* _source;

private slots:
  void checkValidity();
};
}
#endif // COPYPROPERTYDIALOG_H
///@endcond
