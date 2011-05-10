#ifndef TULIPQVARIANTBUILDER_H
#define TULIPQVARIANTBUILDER_H

#include <QtCore/QVariant>
#include <tulip/Graph.h>
enum TulipPropertyType{BOOLEAN_PROPERTY_RTTI=1001,COLORPROPERTY_RTTI = 1002,DOUBLEPROPERTY_RTTI=1003,GRAPHPROPERTY_RTTI=1004, INTEGER_PROPERTY_RTTI=1005,LAYOUTPROPERTY_RTTI=1006,SIZEPROPERTY_RTTI=1007,STRINGPROPERTY_RTTI=1010,BOOLEANVECTORPROPERTY_RTTI=1011,COLORVECTORPROPERTY_RTTI=1012,COORDVECTORPROPERTY_RTTI=1013 ,DOUBLEVECTORPROPERTY_RTTI=1014,INTEGERVECTORPROPERTY_RTTI=1015,SIZEVECTORPROPERTY_RTTI=1016,STRINGVECTORPROPERTY_RTTI=1017, EDGEEXTREMITYGLYPHPROPERTY_RTTI=1018,EDGESHAPEPROPERTY_RTTI=1019,  FILEPROPERTY_RTTI = 1020,FONTFILEPROPERTY_RTTI=1021 ,LABELPOSITIONPROPERTY_RTTI=1008, NODEGLYPHPROPERTY_RTTI= 1022, TEXTUREPROPERTY_RTTI=1024};
enum TulipGraphTableModelRole{DataTypeRole=33};



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
        * @brief Return the QVariant containing the data to display for the given element and property.
        *@param elementType The type of the element can be EDGE or NODE.
        *@param elementId The id of the element.
        *@param propertyType The type of the property. This parameter is not the class of the of the property but more the kind of data this property contains.
        *@param property The property interface.
        **/
        QVariant displayRoleData(tlp::ElementType elementType,unsigned int elementId,TulipPropertyType propertyType,tlp::PropertyInterface* property) const;

        /**
          * @brief Convinience function.
          **/
        inline QVariant displayRoleData(tlp::ElementType elementType,unsigned int elementId,tlp::PropertyInterface* property) const{
            return displayRoleData(elementType,elementId,getPropertyType(elementType,property),property);
        }

        /**
        * @brief Return the QVariant containing the data to create the edition widget for the given element and property.
        *@param elementType The type of the element can be EDGE or NODE.
        *@param elementId The id of the element.
        *@param propertyType The type of the property. This parameter is not the class of the of the property but more the kind of data this property contains.
        *@param property The property interface.
        **/
        QVariant editRoleData(tlp::ElementType elementType,unsigned int elementId,TulipPropertyType propertyType,tlp::PropertyInterface* property)const;

        /**
          * @brief Convinience function.
          **/
        inline QVariant editRoleData(tlp::ElementType elementType,unsigned int elementId,tlp::PropertyInterface* property) const{
            return editRoleData(elementType,elementId,getPropertyType(elementType,property),property);
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
