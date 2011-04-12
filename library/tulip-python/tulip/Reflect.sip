/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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

// +-------------------------------------------------------------------------+
// | Tulip Python Bindings                                                   |
// | inspired from bindings by the Booggie project development team          |
// | (http://booggie.org/)                                                   |
// +-------------------------------------------------------------------------+

namespace tlp {


struct DataSet {
	DataSet();
	DataSet(const tlp::DataSet &set);

	bool getDataSet(const std::string &str, tlp::DataSet &v /Out/);
%MethodCode
	sipRes = sipCpp->get(*a0, *a1);
%End
	
	void setDataSet(const std::string &str, const tlp::DataSet &v);
%MethodCode
	sipCpp->set(*a0, *a1);
%End

	bool getString(const std::string &str, std::string &v /Out/);
%MethodCode
	sipRes = sipCpp->get(*a0, *a1);
%End
	
	void setString(const std::string &str, const std::string &v);
%MethodCode
	sipCpp->set(*a0, *a1);
%End

	bool getBoolean(const std::string &str, bool &v /Out/);
%MethodCode
	sipRes = sipCpp->get(*a0, a1);
%End
	
	void setBoolean(const std::string &str, const bool v);
%MethodCode
	sipCpp->set(*a0, a1);
%End

	bool getInteger(const std::string &str, int &v /Out/);
%MethodCode
	sipRes = sipCpp->get(*a0, a1);
%End
	
	void setInteger(const std::string &str, const int v);
%MethodCode
	sipCpp->set(*a0, a1);
%End

	bool getFloat(const std::string &str, double &v /Out/);
%MethodCode
	sipRes = sipCpp->get(*a0, a1);
%End
	
	void setFloat(const std::string &str, const double v);
%MethodCode
	sipCpp->set(*a0, a1);
%End
	
	bool getSize(const std::string &str, tlp::Size &v /Out/);
%MethodCode
	sipRes = sipCpp->get(*a0, *a1);
%End

	void setSize(const std::string &str, const tlp::Size &v);
%MethodCode
	sipCpp->set(*a0, *a1);
%End

	bool getCoord(const std::string &str, tlp::Coord &v /Out/);
%MethodCode
	sipRes = sipCpp->get(*a0, *a1);
%End

	void setCoord(const std::string &str, const tlp::Coord &v);
%MethodCode
	sipCpp->set(*a0, *a1);
%End

	bool getColor(const std::string &str, tlp::Color &v /Out/);
%MethodCode
	sipRes = sipCpp->get(*a0, *a1);
%End

	void setColor(const std::string &str, const tlp::Color &v);
%MethodCode
	sipCpp->set(*a0, *a1);
%End

	bool getColorScale(const std::string &str, tlp::ColorScale &v /Out/);
%MethodCode
	sipRes = sipCpp->get(*a0, *a1);
%End

	void setColorScale(const std::string &str, const tlp::ColorScale &v);
%MethodCode
	sipCpp->set(*a0, *a1);
%End

	bool getStringCollection(const std::string &str, tlp::StringCollection &v /Out/);
%MethodCode
	sipRes = sipCpp->get(*a0, *a1);
%End

	void setStringCollection(const std::string &str, const tlp::StringCollection &v);
%MethodCode
	sipCpp->set(*a0, *a1);
%End

	bool getLayoutProperty(const std::string &str, tlp::LayoutProperty *&v /Out/);
%MethodCode
	a1 = NULL;
	sipRes = sipCpp->get(*a0, a1);
%End	
	
	void setLayoutProperty(const std::string &str, const tlp::LayoutProperty *v );
%MethodCode
	sipCpp->set(*a0, a1);
%End

	bool getDoubleProperty(const std::string &str, tlp::DoubleProperty *&v /Out/);
%MethodCode
	a1 = NULL;
	sipRes = sipCpp->get(*a0, a1);
%End	

	void setDoubleProperty(const std::string &str, const tlp::DoubleProperty *v);
%MethodCode
	sipCpp->set(*a0, a1);
%End

	bool getIntegerProperty(const std::string &str, tlp::IntegerProperty *&v /Out/);
%MethodCode
	a1 = NULL;
	sipRes = sipCpp->get(*a0, a1);
%End

	void setIntegerProperty(const std::string &str, const tlp::IntegerProperty *v);
%MethodCode
	sipCpp->set(*a0, a1);
%End

	bool getBooleanProperty(const std::string &str, tlp::BooleanProperty *&v /Out/);
%MethodCode
	a1 = NULL;
	sipRes = sipCpp->get(*a0, a1);
%End		

	void setBooleanProperty(const std::string &str, const tlp::BooleanProperty *v);
%MethodCode
	sipCpp->set(*a0, a1);
%End

	bool getStringProperty(const std::string &str, tlp::StringProperty *&v /Out/);
%MethodCode
	a1 = NULL;
	sipRes = sipCpp->get(*a0, a1);
%End	

	void setStringProperty(const std::string &str, const tlp::StringProperty *v);
%MethodCode
	sipCpp->set(*a0, a1);
%End

	bool getColorProperty(const std::string &str, tlp::ColorProperty *&v /Out/);
%MethodCode
	a1 = NULL;
	sipRes = sipCpp->get(*a0, a1);
%End
		
	void setColorProperty(const std::string &str, const tlp::ColorProperty *v);
%MethodCode
	sipCpp->set(*a0, a1);
%End

	bool getProperty(const std::string &str, tlp::PropertyInterface *&v /Out/);
%MethodCode
	a1 = NULL;
	sipRes = sipCpp->get(*a0, a1);
%End		

	void setProperty(const std::string &str, const tlp::PropertyInterface *v);
%MethodCode
	sipCpp->set(*a0, a1);
%End

	bool exist(const std::string &str) const;

	void remove(const std::string &str);
	
	void __setitem__(const std::string &attributeName, SIP_PYOBJECT po);
%MethodCode
		int state=0, iserr=0;
		if (PyBool_Check(a1)) {
			long l = PyLong_AsLong(a1);
			sipCpp->set<bool>(*a0, (l > 0));
		} else if (PyInt_CheckExact(a1)) {
			sipCpp->set(*a0, PyInt_AsLong(a1));	
		} else if (PyLong_Check(a1)) {
			sipCpp->set(*a0, PyLong_AsLong(a1));
		} else if (PyFloat_Check(a1)) {
			sipCpp->set(*a0, PyFloat_AsDouble(a1));
		} else if (sipCanConvertToType(a1, sipType_std_string, SIP_NOT_NONE)) {		
			std::string *s = reinterpret_cast<std::string *>(sipConvertToType(a1, sipType_std_string, NULL, SIP_NOT_NONE, &state, &iserr));
			sipCpp->set(*a0, *s);	
		} else if (sipCanConvertToType(a1, sipType_tlp_Coord, SIP_NOT_NONE)) {		
			tlp::Coord *c = reinterpret_cast<tlp::Coord *>(sipConvertToType(a1, sipType_tlp_Coord, NULL, SIP_NOT_NONE, &state, &iserr));
			sipCpp->set(*a0, *c);
		} else if (sipCanConvertToType(a1, sipType_tlp_Color, SIP_NOT_NONE)) {		
			tlp::Color *c = reinterpret_cast<tlp::Color *>(sipConvertToType(a1, sipType_tlp_Color, NULL, SIP_NOT_NONE, &state, &iserr));
			sipCpp->set(*a0, *c);
		} else if (sipCanConvertToType(a1, sipType_tlp_Size, SIP_NOT_NONE)) {		
			tlp::Size *s = reinterpret_cast<tlp::Size *>(sipConvertToType(a1, sipType_tlp_Size, NULL, SIP_NOT_NONE, &state, &iserr));
			sipCpp->set(*a0, *s);
		} else if (sipCanConvertToType(a1, sipType_tlp_DataSet, SIP_NOT_NONE)) {		
			tlp::DataSet *ds = reinterpret_cast<tlp::DataSet *>(sipConvertToType(a1, sipType_tlp_DataSet, NULL, SIP_NOT_NONE, &state, &iserr));
			sipCpp->set(*a0, *ds);	
		} else if (sipCanConvertToType(a1, sipType_tlp_ColorScale, SIP_NOT_NONE)) {		
			tlp::ColorScale *cs = reinterpret_cast<tlp::ColorScale *>(sipConvertToType(a1, sipType_tlp_ColorScale, NULL, SIP_NOT_NONE, &state, &iserr));
			sipCpp->set(*a0, *cs);
		} else if (sipCanConvertToType(a1, sipType_tlp_StringCollection, SIP_NOT_NONE)) {		
			tlp::StringCollection *sc = reinterpret_cast<tlp::StringCollection *>(sipConvertToType(a1, sipType_tlp_StringCollection, NULL, SIP_NOT_NONE, &state, &iserr));
			sipCpp->set(*a0, *sc);		
		} else if (sipCanConvertToType(a1, sipType_tlp_BooleanProperty, SIP_NOT_NONE)) {		
			tlp::BooleanProperty *p = reinterpret_cast<tlp::BooleanProperty *>(sipConvertToType(a1, sipType_tlp_BooleanProperty, NULL, SIP_NOT_NONE, &state, &iserr));
			sipCpp->set(*a0, p);
		} else if (sipCanConvertToType(a1, sipType_tlp_ColorProperty, SIP_NOT_NONE)) {		
			tlp::ColorProperty *p = reinterpret_cast<tlp::ColorProperty *>(sipConvertToType(a1, sipType_tlp_ColorProperty, NULL, SIP_NOT_NONE, &state, &iserr));
			sipCpp->set(*a0, p);
		} else if (sipCanConvertToType(a1, sipType_tlp_DoubleProperty, SIP_NOT_NONE)) {		
			tlp::DoubleProperty *p = reinterpret_cast<tlp::DoubleProperty *>(sipConvertToType(a1, sipType_tlp_DoubleProperty, NULL, SIP_NOT_NONE, &state, &iserr));
			sipCpp->set(*a0, p);
		} else if (sipCanConvertToType(a1, sipType_tlp_IntegerProperty, SIP_NOT_NONE)) {		
			tlp::IntegerProperty *p = reinterpret_cast<tlp::IntegerProperty *>(sipConvertToType(a1, sipType_tlp_IntegerProperty, NULL, SIP_NOT_NONE, &state, &iserr));
			sipCpp->set(*a0, p);
		} else if (sipCanConvertToType(a1, sipType_tlp_LayoutProperty, SIP_NOT_NONE)) {		
			tlp::LayoutProperty *p = reinterpret_cast<tlp::LayoutProperty *>(sipConvertToType(a1, sipType_tlp_LayoutProperty, NULL, SIP_NOT_NONE, &state, &iserr));
			sipCpp->set(*a0, p);
		} else if (sipCanConvertToType(a1, sipType_tlp_SizeProperty, SIP_NOT_NONE)) {		
			tlp::SizeProperty *p = reinterpret_cast<tlp::SizeProperty *>(sipConvertToType(a1, sipType_tlp_SizeProperty, NULL, SIP_NOT_NONE, &state, &iserr));
			sipCpp->set(*a0, p);
		} else if (sipCanConvertToType(a1, sipType_tlp_StringProperty, SIP_NOT_NONE)) {		
			tlp::StringProperty *p = reinterpret_cast<tlp::StringProperty *>(sipConvertToType(a1, sipType_tlp_StringProperty, NULL, SIP_NOT_NONE, &state, &iserr));
			sipCpp->set(*a0, p);							
		} else if (sipCanConvertToType(a1, sipType_tlp_PropertyInterface, SIP_NOT_NONE)) {		
			tlp::PropertyInterface *p = reinterpret_cast<tlp::PropertyInterface *>(sipConvertToType(a1, sipType_tlp_PropertyInterface, NULL, SIP_NOT_NONE, &state, &iserr));
			sipCpp->set(*a0, p);
		}
%End
	
	
	SIP_PYOBJECT __getitem__(const std::string &attributeName) const;
%MethodCode
	bool ok = false;
	if (sipCpp->exist(*a0)) {
		tlp::DataType* dataType = sipCpp->getData(*a0);
		if (dataType->getTypeName() == std::string(typeid(bool).name())) {
			ok = true;
			sipRes = PyBool_FromLong(*(reinterpret_cast<bool*>(dataType->value)));
			
		} else if (dataType->getTypeName() == std::string(typeid(int).name())) {
			ok = true;
			sipRes = PyLong_FromLong(*(reinterpret_cast<int*>(dataType->value)));
			
		} else if (dataType->getTypeName() == std::string(typeid(long).name())) {
			ok = true;
			sipRes = PyLong_FromLong(*(reinterpret_cast<long*>(dataType->value)));
			
		} else if (dataType->getTypeName() == std::string(typeid(short).name())) {
			ok = true;
			sipRes = PyLong_FromLong(*(reinterpret_cast<short*>(dataType->value)));
			
		} else if (dataType->getTypeName() == std::string(typeid(unsigned int).name())) {
			ok = true;
			sipRes = PyLong_FromUnsignedLong(*(reinterpret_cast<unsigned int*>(dataType->value)));
			
		} else if (dataType->getTypeName() == std::string(typeid(unsigned long).name())) {
			ok = true;
			sipRes = PyLong_FromUnsignedLong(*(reinterpret_cast<unsigned long*>(dataType->value)));
							
		} else if (dataType->getTypeName() == std::string(typeid(unsigned short).name())) {
			ok = true;
			sipRes = PyLong_FromUnsignedLong(*(reinterpret_cast<unsigned short*>(dataType->value)));
			
		} else if (dataType->getTypeName() == std::string(typeid(double).name())) {
			ok = true;
			sipRes = PyFloat_FromDouble(*(reinterpret_cast<double*>(dataType->value)));
			
		} else if (dataType->getTypeName() == std::string(typeid(float).name())) {
			ok = true;
			sipRes = PyFloat_FromDouble(*(reinterpret_cast<float*>(dataType->value)));
			
		} else if (dataType->getTypeName() == std::string(typeid(std::string).name())) {
			ok = true;
			sipRes = PyString_FromString((*(reinterpret_cast<std::string*>(dataType->value))).c_str());
			
		} else if (dataType->getTypeName() == std::string(typeid(tlp::DataSet).name())) {
			ok = true;
			sipRes = sipConvertFromNewType(new tlp::DataSet(*(reinterpret_cast<tlp::DataSet *>(dataType->value))), sipType_tlp_DataSet, NULL);
			
		} else if (dataType->getTypeName() == std::string(typeid(tlp::DataSet).name())) {
			ok = true;
			sipRes = sipConvertFromNewType(new tlp::DataSet(*(reinterpret_cast<tlp::DataSet *>(dataType->value))), sipType_tlp_DataSet, NULL);
			
		} else if (dataType->getTypeName() == std::string(typeid(tlp::Coord).name())) {
			ok = true;
			sipRes = sipConvertFromNewType(new tlp::Coord(*(reinterpret_cast<tlp::Coord *>(dataType->value))), sipType_tlp_Coord, NULL);
			
		} else if (dataType->getTypeName() == std::string(typeid(tlp::Color).name())) {
			ok = true;
			sipRes = sipConvertFromNewType(new tlp::Color(*(reinterpret_cast<tlp::Color *>(dataType->value))), sipType_tlp_Color, NULL);
			
		} else if (dataType->getTypeName() == std::string(typeid(tlp::Size).name())) {
			ok = true;
			sipRes = sipConvertFromNewType(new tlp::Size(*(reinterpret_cast<tlp::Size *>(dataType->value))), sipType_tlp_Size, NULL);
			
		} else if (dataType->getTypeName() == std::string(typeid(tlp::ColorScale).name())) {
			ok = true;
			sipRes = sipConvertFromNewType(new tlp::ColorScale(*(reinterpret_cast<tlp::ColorScale *>(dataType->value))), sipType_tlp_ColorScale, NULL);
			
		} else if (dataType->getTypeName() == std::string(typeid(tlp::StringCollection).name())) {
			ok = true;
			sipRes = sipConvertFromNewType(new tlp::StringCollection(*(reinterpret_cast<tlp::StringCollection *>(dataType->value))), sipType_tlp_StringCollection, NULL);
			
		} else if (dataType->getTypeName() == std::string(typeid(tlp::PropertyInterface*).name())) {
			ok = true;
			sipRes = sipConvertFromType(*(reinterpret_cast<tlp::PropertyInterface **>(dataType->value)), sipType_tlp_PropertyInterface, NULL);
			
		} else if (dataType->getTypeName() == std::string(typeid(tlp::BooleanProperty*).name())) {
			ok = true;
			sipRes = sipConvertFromType(*(reinterpret_cast<tlp::BooleanProperty **>(dataType->value)), sipType_tlp_BooleanProperty, NULL);
			
		} else if (dataType->getTypeName() == std::string(typeid(tlp::ColorProperty*).name())) {
			ok = true;
			sipRes = sipConvertFromType(*(reinterpret_cast<tlp::ColorProperty **>(dataType->value)), sipType_tlp_ColorProperty, NULL);
			
		} else if (dataType->getTypeName() == std::string(typeid(tlp::DoubleProperty*).name())) {
			ok = true;
			sipRes = sipConvertFromType(*(reinterpret_cast<tlp::DoubleProperty **>(dataType->value)), sipType_tlp_DoubleProperty, NULL);
			
		} else if (dataType->getTypeName() == std::string(typeid(tlp::IntegerProperty*).name())) {
			ok = true;
			sipRes = sipConvertFromType(*(reinterpret_cast<tlp::IntegerProperty **>(dataType->value)), sipType_tlp_IntegerProperty, NULL);
			
		} else if (dataType->getTypeName() == std::string(typeid(tlp::LayoutProperty*).name())) {
			ok = true;
			sipRes = sipConvertFromType(*(reinterpret_cast<tlp::LayoutProperty **>(dataType->value)), sipType_tlp_LayoutProperty, NULL);
			
		} else if (dataType->getTypeName() == std::string(typeid(tlp::SizeProperty*).name())) {
			ok = true;
			sipRes = sipConvertFromType(*(reinterpret_cast<tlp::SizeProperty **>(dataType->value)), sipType_tlp_SizeProperty, NULL);
			
		} else if (dataType->getTypeName() == std::string(typeid(tlp::StringProperty*).name())) {
			ok = true;
			sipRes = sipConvertFromType(*(reinterpret_cast<tlp::StringProperty **>(dataType->value)), sipType_tlp_StringProperty, NULL);
			
		}
	}
	
	if (!ok) {
		std::ostringstream oss;
		oss << "Data set entry \"" << *a0 << "\" does not exist.";
		sipRes = NULL;
		PyErr_SetString(PyExc_AttributeError, oss.str().c_str());
	}
%End	

};
};

