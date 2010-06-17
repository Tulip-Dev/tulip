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
#include "SpreadValue.h"

using namespace std;

namespace tlp {

  void SpreadValue::value2List(const SpreadValue &value,QList<double> &out) {
    if(value.valueType==SpreadValue::doublev){
      out.append(value.doubleV);
    }else if(value.valueType==SpreadValue::coordv){
      out.append(value.coordV[0]);
      out.append(value.coordV[1]);
      out.append(value.coordV[2]);
    }else if(value.valueType==SpreadValue::colorv){
      out.append(value.colorV[0]);
      out.append(value.colorV[1]);
      out.append(value.colorV[2]);
      out.append(value.colorV[3]);
    }else{
      assert(false);
    }
  }

  bool SpreadValue::valueList2List(const QList<SpreadValue> &values,QList<QList<double> > &out) {
    if(values[0].valueType==SpreadValue::doublev){
      out.append(QList<double>());
      QList<double> *outList=&out[0];
      foreach(SpreadValue element,values) {
	if(element.valueType!=SpreadValue::doublev)
	  return false;
	outList->append(element.doubleV);
      }
    }else if(values[0].valueType==SpreadValue::coordv){
      out.append(QList<double>());out.append(QList<double>());out.append(QList<double>());
      QList<double> *outList1=&out[0];QList<double> *outList2=&out[1];QList<double> *outList3=&out[2];
      foreach(SpreadValue element,values) {
	if(element.valueType!=SpreadValue::coordv)
	  return false;
	outList1->append(element.coordV[0]);
	outList2->append(element.coordV[1]);
	outList3->append(element.coordV[2]);
      }
    }else if(values[0].valueType==SpreadValue::colorv){
      out.append(QList<double>());out.append(QList<double>());out.append(QList<double>());out.append(QList<double>());
      QList<double> *outList1=&out[0];QList<double> *outList2=&out[1];QList<double> *outList3=&out[2];QList<double> *outList4=&out[3];
      foreach(SpreadValue element,values) {
	if(element.valueType!=SpreadValue::colorv)
	  return false;
	outList1->append(element.colorV[0]);
	outList2->append(element.colorV[1]);
	outList3->append(element.colorV[2]);
	outList4->append(element.colorV[3]);
      }
    }else{
      assert(false);
    }
    return true;
  }

  void SpreadValue::list2Value(QList<double> &in,unsigned int begin,SpreadValue::Type type,SpreadValue &value) {
    if(type==SpreadValue::doublev){
      assert(in.size()>=begin);
      value.valueType=SpreadValue::doublev;
      value.doubleV=in[begin];
    }else if(type==SpreadValue::coordv){
      assert(in.size()>=begin+3);
      value.valueType=SpreadValue::coordv;
      value.coordV=Coord(in[begin],in[begin+1],in[begin+2]);
    }else if(type==SpreadValue::colorv){
      assert(in.size()>=begin+4);
      for(int i=0;i<4;++i){
	if(in[begin+i]>255)
	  in[begin+i]=255;
	if(in[begin+i]<0)
	  in[begin+i]=0;
      }
      value.valueType=SpreadValue::colorv;
      value.colorV=Color((char)(in[begin]),(char)(in[begin+1]),(char)(in[begin+2]),(char)(in[begin+3]));
    }else{
      assert(false); 
    }
  }

  SpreadValue::SpreadValue(const QString &value) {
    int position=0;
    if(value[position]!='(') {
      if(value[position].isDigit()){
	valueType=doublev;
	doubleV=value.toDouble();
      }else
	assert(false);
    }else{
      std::vector<double> result;
      int beginPosition=position=1;
      while(value[position]!=')') {
	if(value[position]==','){
	  result.push_back(value.mid(beginPosition,position-beginPosition).toDouble());
	  beginPosition=position+1;
	}
	position++;
      }
      result.push_back(value.mid(beginPosition,position-beginPosition).toDouble());
      if(result.size()==3) {
	coordV=Coord(result[0],result[1],result[2]);
	valueType=coordv;
      }else if(result.size()==4) {
	colorV=Color((char)(result[0]),(char)(result[1]),(char)(result[2]),(char)(result[3]));
	valueType=colorv;
      }
    }
  }

  SpreadValue SpreadValue::computeOpp2Value(const QChar &opp,const SpreadValue& arg1, const SpreadValue& arg2) {
    if(arg1.valueType==SpreadValue::none) {
      assert(arg2.valueType!=SpreadValue::none);
      return arg2;
    }
    if(arg2.valueType==SpreadValue::none) {
      assert(arg1.valueType!=SpreadValue::none);
      return arg1;
    }
	
    assert(opp=='+' || opp=='-' || opp=='*' || opp=='/');

    SpreadValue newArg1;
    SpreadValue newArg2;

    if(transform2SameValueType(arg1,arg2,newArg1,newArg2))
      return computeOpp2EqualValueType(opp,newArg1,newArg2);
    else
      return SpreadValue(SpreadValue::none);
  }

  bool SpreadValue::transform2SameValueType(const SpreadValue& arg1,const SpreadValue& arg2,SpreadValue& resultArg1,SpreadValue& resultArg2) {
    if(arg1.valueType==arg2.valueType) {
      resultArg1=arg1;
      resultArg2=arg2;
      return true;
    }else{
      if(arg1.valueType!=SpreadValue::doublev && arg2.valueType!=SpreadValue::doublev) {
	return false;
      }else{
	if(arg1.valueType==SpreadValue::doublev) {
	  resultArg1.valueType=arg2.valueType;
	  resultArg2=arg2;
	  if(arg2.valueType==SpreadValue::coordv) {
	    resultArg1.coordV=Coord(arg1.doubleV,arg1.doubleV,arg1.doubleV);
	  }else{
	    char tmp;
	    if(arg1.doubleV<0)
	      tmp=0;
	    else if(arg1.doubleV>255)
	      tmp=255;
	    else
	      tmp=(char)(arg1.doubleV);
	    resultArg1.colorV=Color(tmp,tmp,tmp,tmp);
	  }
	}else{
	  resultArg2.valueType=arg1.valueType;
	  resultArg1=arg1;
	  if(arg1.valueType==SpreadValue::coordv) {
	    resultArg2.coordV=Coord(arg2.doubleV,arg2.doubleV,arg2.doubleV);
	  }else{
	    char tmp;
	    if(arg2.doubleV<0)
	      tmp=0;
	    else if(arg2.doubleV>255)
	      tmp=255;
	    else
	      tmp=(char)(arg2.doubleV);
	    resultArg2.colorV=Color(tmp,tmp,tmp,tmp);
	  }
	}
	return true;
      }
    }
  }

  SpreadValue SpreadValue::computeOpp2EqualValueType(const QChar &opp,const SpreadValue& arg1, const SpreadValue& arg2) {
    SpreadValue result(arg1.valueType);
    
    if(opp=='+') {
      switch(result.valueType) {
      case SpreadValue::doublev : {
	result.doubleV=arg1.doubleV+arg2.doubleV;
	break;
      }
      case SpreadValue::coordv : {
	result.coordV=arg1.coordV+arg2.coordV;
	break;
      }
      case SpreadValue::colorv : {
	result.colorV=arg1.colorV+arg2.colorV;
	break;
	}
      }
    }else if(opp=='-'){
      switch(result.valueType) {
      case SpreadValue::doublev : {
	result.doubleV=arg1.doubleV-arg2.doubleV;
	break;
	}
      case SpreadValue::coordv : {
	result.coordV=arg1.coordV-arg2.coordV;
	break;
      }
      case SpreadValue::colorv : {
	result.colorV=arg1.colorV-arg2.colorV;
	break;
	}
      }
    }else if(opp=='*'){
      switch(result.valueType) {
      case SpreadValue::doublev : {
	result.doubleV=arg1.doubleV*arg2.doubleV;
	break;
      }
      case SpreadValue::coordv : {
	result.coordV=arg1.coordV*arg2.coordV;
	break;
	}
      case SpreadValue::colorv : {
	result.colorV=arg1.colorV*arg2.colorV;
	break;
      }
      }
    }else{
      switch(result.valueType) {
      case SpreadValue::doublev : {
	result.doubleV=arg1.doubleV/arg2.doubleV;
	break;
      }
      case SpreadValue::coordv : {
	result.coordV=arg1.coordV/arg2.coordV;
	break;
      }
      case SpreadValue::colorv : {
	result.colorV=arg1.colorV/arg2.colorV;
	break;
      }
      }
    }
    return result;
  }

}
