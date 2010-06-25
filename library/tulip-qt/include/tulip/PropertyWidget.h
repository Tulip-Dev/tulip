/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
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
#ifndef PROPERTYWIDGETS_H
#define PROPERTYWIDGETS_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string>

#include <QtGui/qwidget.h>
#include <QtGui/QFileDialog>

#include <tulip/Graph.h>
#include <tulip/Size.h>
#include <tulip/Coord.h>
#include <tulip/TulipTableWidget.h>

#include "tulip/ChooseFileNameDialogData.h"

namespace tlp {

typedef TLP_HASH_MAP<int, std::string> GlyphTableString;
class PropertyInterface;

}

/** \addtogroup Tulip_Widgets */ 
/*@{*/
/* this table handle displaying and editing node or edge properties */
class TLP_QT_SIMPLE_SCOPE PropertyWidget : public tlp::TulipTableWidget {
  Q_OBJECT;

private:
  tlp::Graph *graph;

public:
  PropertyWidget(QWidget *, const char * = 0);
  ~PropertyWidget();
  tlp::Graph *getGraph() const;

public slots:
  void setGraph(tlp::Graph *);

protected:
  virtual void connectNotify (const char * signal);

private :
  int vScrollPos;
  bool displayNode;
  bool _filterSelection;
  bool showProperties;
  tlp::BooleanProperty *filter;
  unsigned int nbElement;
  tlp::PropertyInterface *editedProperty;
  std::string editedPropertyName;
  bool inUpdate;
  
private slots:
  void showContextMenu(const QPoint & pos); 

public slots:  
  void changeProperty(tlp::Graph *,const std::string &name);
  void selectNodeOrEdge(bool );
  void changePropertyValue(int,int);
  void changePropertyNodeValue(int,int);
  void changePropertyEdgeValue(int,int);
  void filterSelection(bool);
  void scroll(int);
  void update();
  void updateNodes();
  void updateEdges();
  void updateNbElements();
  void setAllEdgeValue();
  void setAllNodeValue();
  void setAll();

signals:
  void tulipNodePropertyChanged(tlp::Graph *, const tlp::node &, const QString &property, const QString &value);
  void tulipEdgePropertyChanged(tlp::Graph *, const tlp::edge &, const QString &property, const QString &value);
  void showElementProperties(unsigned int eltId, bool isNode);
};

class TLP_QT_SIMPLE_SCOPE ChooseFileNameDialog : public QDialog,public Ui::ChooseFileNameDialogData {

  Q_OBJECT

public :

  ChooseFileNameDialog(const QString &filter,QWidget *parent=NULL):filter(filter),basePath(QString()) {
    setupUi(this);
    connect(fileOpenButton,SIGNAL(clicked()),this,SLOT(openFile()));
  }

ChooseFileNameDialog(const QString &filter,const QString &basePath,QWidget *parent=NULL):filter(filter),basePath(basePath) {
    setupUi(this);
    connect(fileOpenButton,SIGNAL(clicked()),this,SLOT(openFile()));
  }

QString getText(){return fileName->text();}

protected slots:

  void openFile() {
    QString fileNameText = QFileDialog::getOpenFileName(this, tr("Open File"),basePath,filter);
    if(fileNameText!="")
      fileName->setText(fileNameText);
  }

protected :

    QString filter;
    QString basePath;

};
/*@}*/
#endif //PROPERTYWIDGETS_H
