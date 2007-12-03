#ifndef PROPERTYWIDGETS_H
#define PROPERTYWIDGETS_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#if (__GNUC__ < 3)
#include <hash_map>
#else
#include <ext/hash_map>
#endif
#include <string>

#include <QtGui/qwidget.h>
#include <QtGui/qscrollbar.h>
#include "tulip/Qt3ForTulip.h"

#include <tulip/Graph.h>
#include <tulip/Size.h>
#include <tulip/Coord.h>
#include <tulip/TulipTableWidget.h>

namespace tlp {

typedef stdext::hash_map<int, std::string> GlyphTableString;
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
  QScrollBar *vScroll;
  int vScrollPos;
  bool displayNode;
  bool _filterSelection;
  bool showProperties;
  tlp::BooleanProperty *filter;
  unsigned int nbElement;
  tlp::PropertyInterface *editedProperty;
  std::string editedPropertyName;
  
private slots:
  void showContextMenu(int row, int col, const QPoint & pos); 

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
/*@}*/
#endif //PROPERTYWIDGETS_H
