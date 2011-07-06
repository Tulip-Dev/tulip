#ifndef GRAPHTABLEMODEL_H
#define GRAPHTABLEMODEL_H
#include <QtCore/QAbstractItemModel>
#include <tulip/Graph.h>
#include <deque>

struct GraphTableModelIndex{
    GraphTableModelIndex(unsigned int element,tlp::PropertyInterface* property);

    bool isValid()const{
        return _element!=UINT_MAX && _property != NULL;
    }

    unsigned int element()const{
        return _element;
    }

    tlp::PropertyInterface* property()const{
        return _property;
    }


private:
    unsigned int _element;
    tlp::PropertyInterface* _property;
};
/**
  * @brief The GraphTableModel class allows to visualize and edit a Tulip Graph object with Qt's model/view framework. You can only use this model only with TulipItemDelegate class instead you cannot see double values and histogram.
  **/
class GraphTableModel : public QAbstractTableModel, public tlp::Observable , public tlp::GraphObserver, public tlp::PropertyObserver
{
    Q_OBJECT
public:
    GraphTableModel(tlp::Graph* graph,tlp::ElementType elementType=tlp::NODE,QObject* parent = 0);

    //Get set parameters
    tlp::Graph* graph()const{
        return _graph;
    }
    /**
      * @brief Change the displayed graph. Display the same type of elements than before (by default display nodes).
      **/
    void setGraph(tlp::Graph* newGraph);

    tlp::ElementType elementType()const{
        return _elementType;
    }

    /**
      * @brief Change the kind of displayed elements.
      **/
    void setElementType(tlp::ElementType type);

    Qt::Orientation orientation()const{
        return _orientation;
    }
    void setOrientation(Qt::Orientation orientation);




    //Data access methods
    /**
      * @brief Convinience function. See the element(int row, int column,const QModelIndex& parent = QModelIndex()) function for more informations.
      */
    inline GraphTableModelIndex element(const QModelIndex& modelIndex)const{
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
    inline unsigned int idForIndex(const QModelIndex& index)const{
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
    tlp::PropertyInterface* propertyForIndex(const QModelIndex& index ) const{
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
    bool setAllElementsData(const QModelIndex& index,const QVariant & value, int role = Qt::EditRole){
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
    void addNode(tlp::Graph *, const tlp::node );
    void addEdge(tlp::Graph *, const tlp::edge );
    void delNode(tlp::Graph *,const tlp::node );
    void delEdge(tlp::Graph *,const tlp::edge );
    void addLocalProperty(tlp::Graph*, const std::string&);
    void beforeDelLocalProperty(tlp::Graph *, const std::string &);
    void addInheritedProperty(tlp::Graph*, const std::string&);
    void beforeDelInheritedProperty(tlp::Graph *, const std::string &);
    void afterSetNodeValue(tlp::PropertyInterface*, const tlp::node);
    void afterSetEdgeValue(tlp::PropertyInterface*, const tlp::edge);
    void afterSetAllNodeValue(tlp::PropertyInterface*);
    void afterSetAllEdgeValue(tlp::PropertyInterface*);    

    /**
      * @brief Apply all the modifications occured in the graph on the model.
      **/
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
    void removeFromVector(const std::set<T>& objects,std::vector<T>& vect,TLP_HASH_MAP<T,int>& objToIndexes,bool deleteRows){
        std::set<int> indexes;        
        for( typename std::set<T>::const_iterator it = objects.begin();it != objects.end();++it){
            assert(objToIndexes.find(*it)!=objToIndexes.end());
            indexes.insert(objToIndexes[*it]);
            objToIndexes.erase(*it);
        }

        for(std::set<int>::reverse_iterator it = indexes.rbegin() ; it != indexes.rend();++it){
            if(deleteRows){
                beginRemoveRows(QModelIndex(),(*it),(*it));
            }else{
                beginRemoveColumns(QModelIndex(),(*it),(*it));
            }
            //Update objToIndex map
            for(unsigned int i = *it+1 ; i < vect.size() ; ++i){
                objToIndexes[vect[i]] = static_cast<int>(i-1);
            }
            vect.erase(vect.begin()+(*it));

            if(deleteRows){
                endRemoveRows();
            }else{
                endRemoveColumns();
            }            
        }
    }

    template<typename T, class Compare>
    void addToVector(const std::set<T>& objects,std::vector<T>& vect,TLP_HASH_MAP<T,int>& objToIndex,bool addRows, Compare* comp){
        if(vect.capacity() <= vect.size() + objects.size()){
            vect.reserve(vect.size() + objects.size());
        }
        if(addRows){
            beginInsertRows(QModelIndex(),vect.size(),vect.size()+objects.size()-1);
        }else{
            beginInsertColumns(QModelIndex(),vect.size(),vect.size()+objects.size()-1);
        }
        for(typename std::set<T>::const_iterator it = objects.begin();it != objects.end() ; ++it){
            unsigned int i = vect.size();
            objToIndex[*it]=i;
            vect.push_back((*it));
        }
        if(addRows){
            endInsertRows();
        }else{
            endInsertColumns();
        }
        if(comp != NULL){
            std::stable_sort(vect.begin(),vect.end(),*comp);
            updateIndexMap<T>(vect,objToIndex);
        }        
    }

    template<typename T>
    void updateIndexMap(const std::vector<T>& table,TLP_HASH_MAP<T,int>& indexMap){
        indexMap.clear();
        for(size_t i = 0 ; i < table.size();++i){
            indexMap[table[i]]=i;
        }
    }

    template<typename T>
    std::pair<unsigned int,unsigned int> computeArea(const std::set<T>& elementsToFind,const std::vector<T>& elements,const TLP_HASH_MAP<T,int>& objToIndex)const{
        //Init from and to values
        int first=elements.size()-1,last=0;
        for(typename std::set<T>::const_iterator it = elementsToFind.begin();it != elementsToFind.end() ; ++it){
            assert(objToIndex.find(*it) != objToIndex.end());
            int index = (*objToIndex.find(*it)).second;
            first = std::min(first,index);
            last = std::max(last,index);
        }
        first = std::max(first,0);
        last = std::min(last,static_cast<int>(elements.size()-1));
        return std::make_pair(first,last);
    }

    tlp::Graph* _graph;
    tlp::ElementType _elementType;
    Qt::Orientation _orientation;
    std::vector<unsigned int> _idTable;
    TLP_HASH_MAP<unsigned int,int> _idToIndex;
    std::vector<tlp::PropertyInterface*> _propertiesTable;
    TLP_HASH_MAP<tlp::PropertyInterface*,int> _propertyToIndex;

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

#endif // GRAPHTABLEMODEL_H
