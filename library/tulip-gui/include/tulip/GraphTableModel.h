#ifndef GRAPHTABLEMODEL_H
#define GRAPHTABLEMODEL_H
#include <QtCore/QAbstractItemModel>
#include <tulip/Graph.h>
#include <deque>

#include "PropertyToQVariantMapper.h"



namespace tlp {



/**
  * @brief Simple structure used to store element id and propertiy corresponding to a cell in the GraphTableModel.
  **/
struct TLP_QT_SCOPE GraphTableModelIndex {
  GraphTableModelIndex(unsigned int element,tlp::PropertyInterface* property);

  bool isValid()const {
    return _element!=UINT_MAX && _property != NULL;
  }

  unsigned int element()const {
    return _element;
  }

  tlp::PropertyInterface* property()const {
    return _property;
  }


private:
  unsigned int _element;
  tlp::PropertyInterface* _property;
};
/**
  * @brief The GraphTableModel class allows to visualize and edit a Tulip Graph object with Qt's model/view framework. This model is made to work with a GraphTableItemDelegate class as QItemDelegate. If you use another one you cannot see both values and histogram distibution in double properties cells.
  **/
class TLP_QT_SCOPE GraphTableModel : public QAbstractTableModel, public tlp::Observable {
public:

  enum GraphTableModelDataRole {
    //Return a double value between 0 and 1 where 0 correspond to elements with the lowest value and 1 the highest.
    NormalizedValueRole = 33
  };

  GraphTableModel(tlp::Graph* graph,tlp::ElementType elementType=tlp::NODE,QObject* parent = NULL);

  //Get set parameters
  tlp::Graph* graph()const {
    return _graph;
  }
  /**
  * @brief Change the displayed graph. Display the same type of elements than before (by default display nodes).
  **/
  void setGraph(tlp::Graph* newGraph);

  tlp::ElementType elementType()const {
    return _elementType;
  }

  /**
  * @brief Change the kind of displayed elements.
  **/
  void setElementType(tlp::ElementType type);

  Qt::Orientation orientation()const {
    return _orientation;
  }
  void setOrientation(Qt::Orientation orientation);

  //Data access methods
  /**
  * @brief Convinience function. See the element(int row, int column,const QModelIndex& parent = QModelIndex()) function for more informations.
  */
  inline GraphTableModelIndex element(const QModelIndex& modelIndex)const {
    return element(modelIndex.row(),modelIndex.column(),modelIndex.parent());
  }
  /**
  * @brief Return a structure containing the element and the property for a given position in the model.
  **/
  inline GraphTableModelIndex element(int row, int column,const QModelIndex& parent = QModelIndex())const {
    return _orientation == Qt::Vertical?GraphTableModelIndex(idForIndex(row,parent),propertyForIndex(column,parent)):GraphTableModelIndex(idForIndex(column,parent),propertyForIndex(row,parent));
  }

  /**
  * @brief Convinience function to get the id for an index call the idForIndex function with the right index in function of elements orientation in the model.
  **/
  inline unsigned int idForIndex(const QModelIndex& index)const {
    return idForIndex(_orientation==Qt::Vertical?index.row():index.column());
  }
  /**
  * @brief Get the id at the given index. If the index is not valid return UINT_MAX.
  **/
  virtual unsigned int idForIndex(int index,const QModelIndex& parent = QModelIndex()) const;

  /**
  * @brief Retrieve the indexes of the given ids. If an id is not in the table it will be ignored.
  *
  **/
  virtual QList<int> indexesForIds(const std::set<unsigned int>& ids)const;


  /**
  * @brief Convinience function to get the property for an index call the propertyForIndex function with the right index in function of properties orientation in the model.
  **/
  tlp::PropertyInterface* propertyForIndex(const QModelIndex& index ) const {
    return propertyForIndex(_orientation==Qt::Horizontal?index.column():index.row());
  }
  /**
  * @brief Get the id property the given index. If the index is not valid or if the property is marked for deletion return NULL.
  **/
  virtual tlp::PropertyInterface* propertyForIndex(int index,const QModelIndex& parent = QModelIndex()) const;

  /**
  * @brief Retrieve the indexes of the given properties. If a property is not in the table it will be ignored.
  *
  **/
  virtual QList<int> indexesForProperties(const std::set<tlp::PropertyInterface*>& properties)const;

  /**
  * @brief Compute the area for the list of elements. Return top left and bottom right bounds.
  **/
  std::pair<QModelIndex,QModelIndex> computeElementsArea(const std::set<unsigned int>& elementsIds)const;

  /**
  * @brief Compute the area for the list of elements. Return top left and bottom right bounds.
  **/
  std::pair<QModelIndex,QModelIndex> computePropertiesArea(const std::set<tlp::PropertyInterface*>& properties)const;

  /**
  * @brief Reimplementation of QAbstractTableModel::columnCount(). Return the size of the property table if the orientation of the model is equal to Qt::Vertical else return the size of the elements table.
  **/
  virtual int columnCount(const QModelIndex& parent = QModelIndex()) const;
  /**
  * @brief Reimplementation of QAbstractTableModel::rowCount(). Return the size of the elements table if the orientation of the model is equal to Qt::Vertical else return the size of the properties table.
  **/
  virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;


  virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
  virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;


  /**
  * @brief Convinience function.
  **/
  bool setAllElementsData(const QModelIndex& index,const QVariant & value, int role = Qt::EditRole) {
    return setAllElementsData(_orientation==Qt::Horizontal?index.row():index.column(),value,role);
  }

  /**
  * @brief Set the given value to all the elements in the model. Call the setAllNodeValue or setAllEdgeValue in function of the kind of elements managed by the model.
  **/
  virtual bool setAllElementsData(unsigned int propertyIndex,const QVariant & value, int role = Qt::EditRole);

  virtual bool setData( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );

  Qt::ItemFlags flags ( const QModelIndex & index ) const;

  //Data modification methods
  void sort( int column, Qt::SortOrder order = Qt::AscendingOrder );
  //Data destruction methods
  bool removeColumns( int column, int count, const QModelIndex & parent = QModelIndex());
  bool removeRows(int row, int count, const QModelIndex &parent= QModelIndex());

  bool removeColumns( const QModelIndexList& toRemove);
  bool removeRows( const QModelIndexList& toRemove);

  //Observable management
  void treatEvent(const tlp::Event &evt);
  void addLocalProperty(tlp::Graph*, const std::string&);
  void beforeDelLocalProperty(tlp::Graph *, const std::string &);
  void addInheritedProperty(tlp::Graph*, const std::string&);
  void beforeDelInheritedProperty(tlp::Graph *, const std::string &);

  /**
  * @brief Apply all the modifications occured in the graph on the model.
  **/
  void treatEvents(const  std::vector<tlp::Event> &events );
  void update();


protected:

  /**
  * @brief Fill the idTable.
  **/
  virtual void updateElementsTable();

  /**
  * @brief Fill the propertyTable. For each property in the graph call the useProperty function to know if we add the property in the table.
  **/
  virtual void updatePropertyTable();

  /**
  * @brief Return if the property must be integrated in the property table. This function is called before each insertion in the property table.
  **/
  virtual bool useProperty(tlp::PropertyInterface* property) const;

  /**
  * @brief Sort elements according to a given property values.
  **/
  virtual void sortElements(tlp::PropertyInterface* property, Qt::SortOrder order);

  virtual bool removeElements(int first,int last,const QModelIndex& parent = QModelIndex());
  virtual bool removeProperties(int first,int last,const QModelIndex& parent = QModelIndex());

private:

  template<typename T>
  void removeFromVector(const std::set<T>& objects,std::vector<T>& vect,TLP_HASH_MAP<T,int>& objToIndexes,bool deleteRows);

  template<typename T, class Compare>
  void addToVector(const std::set<T>& objects,std::vector<T>& vect,TLP_HASH_MAP<T,int>& objToIndex,bool addRows, Compare* comp);

  template<typename T>
  void updateIndexMap(const std::vector<T>& table,TLP_HASH_MAP<T,int>& indexMap);

  template<typename T>
  std::pair<unsigned int,unsigned int> computeArea(const std::set<T>& elementsToFind,const std::vector<T>& elements,const TLP_HASH_MAP<T,int>& objToIndex)const ;

  tlp::Graph* _graph;
  tlp::ElementType _elementType;
  Qt::Orientation _orientation;
  std::vector<unsigned int> _idTable;
  TLP_HASH_MAP<unsigned int,int> _idToIndex;
  std::vector<tlp::PropertyInterface*> _propertiesTable;
  TLP_HASH_MAP<tlp::PropertyInterface*,int> _propertyToIndex;

  PropertyToQVariantMapper _propertyConverter;

  //Sorting var
  tlp::PropertyInterface* _sortingProperty;
  Qt::SortOrder _order;

  //Graph update buffers
  std::set<tlp::PropertyInterface*> _propertiesToDelete;
  std::set<tlp::PropertyInterface*> _propertiesToAdd;
  std::set<unsigned int> _idsToDelete;
  std::set<unsigned int> _idsToAdd;
  std::set<tlp::PropertyInterface*> _propertiesUpdated;
  std::vector<GraphTableModelIndex> _dataUpdated;
};
}

#include "cxx/GraphTableModel.cxx"
#endif // GRAPHTABLEMODEL_H
