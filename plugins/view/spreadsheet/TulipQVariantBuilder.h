#ifndef TULIPQVARIANTBUILDER_H
#define TULIPQVARIANTBUILDER_H

#include <tulip/tulipconf.h>
#include <QtCore/QVariant>
#include <tulip/Graph.h>

#include "ElementCollection.h"

/**
  * Add a value to each kind of property.
  * Used to determine the kind of data to use in the TulipQVariantBuilder::data and TulipQVariantBuilder::setData functions.
  **/
enum TulipPropertyType{
    BOOLEAN_PROPERTY_RTTI=1001,
    COLORPROPERTY_RTTI = 1002,
    DOUBLEPROPERTY_RTTI=1003,
    GRAPHPROPERTY_RTTI=1004,
    INTEGER_PROPERTY_RTTI=1005,
    LAYOUTPROPERTY_RTTI=1006,
    SIZEPROPERTY_RTTI=1007,
    STRINGPROPERTY_RTTI=1010,
    BOOLEANVECTORPROPERTY_RTTI=1011,
    COLORVECTORPROPERTY_RTTI=1012,
    COORDVECTORPROPERTY_RTTI=1013 ,
    DOUBLEVECTORPROPERTY_RTTI=1014,
    INTEGERVECTORPROPERTY_RTTI=1015,
    SIZEVECTORPROPERTY_RTTI=1016,
    STRINGVECTORPROPERTY_RTTI=1017,
    EDGEEXTREMITYGLYPHPROPERTY_RTTI=1018,
    EDGESHAPEPROPERTY_RTTI=1019,
    FILEPROPERTY_RTTI = 1020,
    FONTFILEPROPERTY_RTTI=1021 ,
    LABELPOSITIONPROPERTY_RTTI=1008,
    NODEGLYPHPROPERTY_RTTI= 1022,
    TEXTUREPROPERTY_RTTI=1024,
    INVALID_PROPERTY_RTTI=UINT_MAX
};

enum TulipItemDataRole{
    //Return a double value between 0 and 1 where 0 correspond to elements with the lowest value and 1 the highest.
    NormalizedValueRole = 33
};

template <class T>
        class Interval {
    T _min;
    T _value;
    T _max;

        public:
    Interval(){
    }
    Interval(const Interval& other):_min(other._min),_value(other._value),_max(other._max){}
    Interval(const T& min, const T& value, const T& max):_min(min),_value(value),_max(max){}
    const T& min()const{
        return _min;
    }
    const T& max()const{
        return _max;
    }
    const T& value()const{
        return _value;
    }
};

/**
  * @brief Use this class to edit a file with the TulipItemDelegate.
  *
  * Store a file path and filter string compatible with QFileChoose to choose new files.
  **/
struct FilteredUrl {
public:
    FilteredUrl(){
    }

    FilteredUrl(const QString& path, const QString& extensionFilters=QString()): _path(path),_extensionsFilters(extensionFilters){
    }
    FilteredUrl(const FilteredUrl& other):_path(other.path()),_extensionsFilters(other.extensionsFilters()){
    }

    QString path()const{
        return _path;
    }
    QString extensionsFilters()const{
        return _extensionsFilters;
    }
private:
    QString _path;
    QString _extensionsFilters;
};

Q_DECLARE_METATYPE(FilteredUrl)
Q_DECLARE_METATYPE(tlp::ElementCollection);
Q_DECLARE_METATYPE(Interval<double>);

/**
* @brief Class giving help function to convert Tulip data to Qt model/view programming.
**/
class TulipQVariantBuilder
{
public:
    TulipQVariantBuilder();

    /**
    * @brief Guess the property type of the given property in function of the displayed element types and the property interface.
    *
    * The property type is used to choose the right data to display or the right data to create the edition widget.
    **/
    TulipPropertyType getPropertyType(tlp::ElementType elementType,tlp::PropertyInterface* property)const;

    /**
      * @brief Transform nodes and edges properties values to QVariant compatible with Qt model/view framework. See QAbstractItemModel::data function.
      *
      * Handle a new role NormalizedValueRole. This role is used for
      **/
    QVariant data(tlp::Graph* graph,int displayRole,tlp::ElementType elementType,unsigned int elementId,TulipPropertyType propertyType,tlp::PropertyInterface* property)const;

    inline QVariant data(tlp::Graph* graph,int displayRole,tlp::ElementType elementType,unsigned int elementId,tlp::PropertyInterface* property) const{
        return data(graph,displayRole,elementType,elementId,getPropertyType(elementType,property),property);
    }

    Qt::ItemFlags flags(Qt::ItemFlags defaultFlags,tlp::ElementType elementType,unsigned int elementId,TulipPropertyType propertyType,tlp::PropertyInterface* property)const;

    inline Qt::ItemFlags flags(Qt::ItemFlags defaultFlags,tlp::ElementType elementType,unsigned int elementId,tlp::PropertyInterface* property)const{
        return flags(defaultFlags,elementType,elementId,getPropertyType(elementType,property),property);
    }

    /**
        * @brief Update the graph with the data returned by the edition widget.
        **/
    bool setData(const QVariant& value,tlp::ElementType elementType,unsigned int elementId,TulipPropertyType propertyType,tlp::PropertyInterface* property)const;

    /**
          * @brief Convinience function.
          **/
    inline bool setData(const QVariant& value,tlp::ElementType elementType,unsigned int elementId,tlp::PropertyInterface* property)const{
        return setData(value,elementType,elementId,getPropertyType(elementType,property),property);
    }

private:
    template<typename PROPERTYCLASS,typename NODETYPE,typename EDGETYPE>
    QVariant createQVariantFromTulipProperty(tlp::ElementType eltType,unsigned int eltId,PROPERTYCLASS* property) const{
        QVariant v;
        if(eltType == tlp::NODE){
            v.setValue<NODETYPE>(property->getNodeValue(tlp::node(eltId)));
        }else{
            v.setValue<EDGETYPE>(property->getEdgeValue(tlp::edge(eltId)));
        }
        return v;
    }

    template<typename PROPERTYCLASS,typename NODETYPE,typename EDGETYPE>
    bool setValueToTulipPropertyFromQVariant(const QVariant& data,tlp::ElementType eltType,unsigned int eltId,PROPERTYCLASS* property) const{
        QVariant v;
        if(eltType == tlp::NODE){
            const NODETYPE& oldValue = property->getNodeValue(tlp::node(eltId));
            NODETYPE newValue = data.value<NODETYPE>();
            if(oldValue != newValue){
                property->setNodeValue(tlp::node(eltId),newValue);
                return true;
            }
        }else{
            const EDGETYPE& oldValue = property->getEdgeValue(tlp::edge(eltId));
            EDGETYPE newValue = data.value<EDGETYPE>();
            if(oldValue != newValue){
                property->setEdgeValue(tlp::edge(eltId),data.value<EDGETYPE>());
                return true;
            }
        }
        return false;
    }
};

#endif // TULIPQVARIANTBUILDER_H
