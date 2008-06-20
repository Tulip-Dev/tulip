// +-------------------------------------------------------------------------+
// | Tulip Software                                                          |
// | Author:  Morgan Mathiaut                                                |
// +-------------------------------------------------------------------------+
// | License:                                                                |
// |                                                                         |
// | Tulip is free software; you can redistribute it and/or modify           |
// | it under the terms of the GNU General Public License as published by    |
// | the Free Software Foundation; either version 2 of the License, or       |
// | (at your option) any later version.                                     |
// |                                                                         |
// | Tulip is distributed in the hope that it will be useful,                |
// | but WITHOUT ANY WARRANTY; without even the implied warranty of          |
// | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the            |
// | GNU General Public License for more details.                            |
// |                                                                         |
// | You should have received a copy of the GNU General Public License       |
// | along with QSimpleSheet; if not, write to the Free Software             |
// | Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA|
// |                                                                         |
// +-------------------------------------------------------------------------+

#ifndef Tulip_SPREADVALUE_H
#define Tulip_SPREADVALUE_H

#include <QtCore/QtCore>

#include <tulip/Coord.h>
#include <tulip/Color.h>

namespace tlp {
  
  struct TLP_QT_SCOPE SpreadValue {
    
    enum Type{none=-1,doublev=0,coordv,colorv};
    
    Type valueType;
    
    double doubleV;
    Coord coordV;
    Color colorV;

    SpreadValue():valueType(none) {}
    SpreadValue(Type valueType):valueType(valueType) {}
    SpreadValue(const double& value):valueType(doublev),doubleV(value){}
    SpreadValue(const Coord& value):valueType(coordv),coordV(value) {}
    SpreadValue(const Color& value):valueType(colorv),colorV(value) {}
    SpreadValue(const QString &value);

    static SpreadValue computeOpp2Value(const QChar &,const SpreadValue&, const SpreadValue&);
    static SpreadValue computeOpp2EqualValueType(const QChar &,const SpreadValue&, const SpreadValue&);
    static bool transform2SameValueType(const SpreadValue&,const SpreadValue&,SpreadValue&,SpreadValue&);
    static void value2List(const SpreadValue &,QList<double> &);
    static bool valueList2List(const QList<SpreadValue> &,QList<QList<double> > &);
    static void list2Value(QList<double> &,unsigned int,SpreadValue::Type,SpreadValue &);

    QString toString() {
      if(valueType==doublev)
	return QString::number(doubleV);
      else if(valueType==coordv)
	return QString("("+QString::number(coordV[0])+","+QString::number(coordV[1])+","+QString::number(coordV[2])+")");
      else
	return QString("("+QString::number(colorV[0])+","+QString::number(colorV[1])+","+QString::number(colorV[2])+","+QString::number(colorV[3])+")");
    }
  };

}

#endif

