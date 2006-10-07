#ifndef TULIPELEMENTPROPERTIES_H
#define TULIPELEMENTPROPERTIES_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#if (QT_REL == 3)
#include <qstringlist.h>
#include <qsize.h>
#else
#include <QtCore/qstringlist.h>
#include <QtCore/qsize.h>
#include "tulip/Qt3ForTulip.h"
#endif

#include <tulip/Graph.h>
#include <tulip/ObservableGraph.h>
#include "tulip/ElementPropertiesWidgetData.h"

class TLP_QT_SIMPLE_SCOPE ElementPropertiesWidget : public tlp::ElementPropertiesWidgetData, public tlp::GraphObserver
{
  Q_OBJECT;
  // moc as of qt 3.0.5 doesn't understand '::'... silly
  //  Q_ENUMS(Tulip::AtomType);
  //  Q_PROPERTY(Tulip::AtomType displayMode READ getDisplayMode WRITE setDisplayMode);
  Q_PROPERTY(QStringList nodeListedProperties READ getNodeListedProperties WRITE setNodeListedProperties);
  Q_PROPERTY(QStringList edgeListedProperties READ getEdgeListedProperties WRITE setEdgeListedProperties);
  Q_PROPERTY(bool elementDisplayed READ isElementDisplayed);
protected:
  tlp::ElementType displayMode;
  tlp::node currentNode;
  tlp::edge currentEdge;
  bool nodeSet;
  bool edgeSet;
  bool displayAllProperties;
  tlp::Graph *graph;
  QStringList nodeListedProperties;
  QStringList edgeListedProperties;

  //inherited slots

  
public:
  ElementPropertiesWidget(tlp::Graph *, const QStringList &nodeListedProperties, const QStringList &edgeListedProperties,
                         QWidget *parent = 0, const char *name = 0, Qt::WFlags fl = 0);
  ElementPropertiesWidget(QWidget *parent = 0, const char *name = 0, Qt::WFlags fl = 0);
  ~ElementPropertiesWidget();
    
  QStringList getCurrentListedProperties() const;
  QStringList getNodeListedProperties() const;
  QStringList getEdgeListedProperties() const;
  tlp::Graph *getGraph() const;
  tlp::node getCurrentNode() const;
  tlp::edge getCurrentEdge() const;
  tlp::ElementType getDisplayMode() const;
  bool isElementDisplayed() const;
  
public slots:
  void updateTable();
  void propertyTableValueChanged(int, int);
  void setDisplayMode(const tlp::ElementType);
  void setCurrentNode(tlp::Graph *, const tlp::node &);
  void setCurrentEdge(tlp::Graph *, const tlp::edge &);
  void setGraph(tlp::Graph *, bool distroy = true);
  void setCurrentListedProperties(const QStringList &);
  void setNodeListedProperties(const QStringList &);
  void setEdgeListedProperties(const QStringList &);
  void nodePropertyChanged(tlp::Graph *, const tlp::node &, const QString &, const QString &);
  void edgePropertyChanged(tlp::Graph *, const tlp::edge &, const QString &, const QString &);
  
signals:
  void tulipNodePropertyChanged(tlp::Graph *, const tlp::node &, const QString &property, const QString &value);
  void tulipEdgePropertyChanged(tlp::Graph *, const tlp::edge &, const QString &property, const QString &value);


 private:
  void delNode(tlp::Graph *, tlp::node);
  void delEdge(tlp::Graph *, tlp::edge);
  void destroy(tlp::Graph *);
};

#endif //TULIPELEMENTPROPERTIES_H
