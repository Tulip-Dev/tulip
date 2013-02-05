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

%TypeCode
#include <tulip/PythonCppTypesConverter.h>
%End

%Docstring
This class is used to store a set of parameters to transmit to a Tulip algorithm.
Below is an exhaustive list of data types that can be stored in a Tulip data set:

	* boolean
	* integer 
	* float
	* string
	* :class:`tlp.Coord`
	* :class:`tlp.Color`
	* :class:`tlp.Size`
	* :class:`tlp.DataSet`
	* :class:`tlp.ColorScale`
	* :class:`tlp.StringCollection`
	* :class:`tlp.BooleanProperty`
	* :class:`tlp.ColorProperty`
	* :class:`tlp.DoubleProperty`
	* :class:`tlp.IntegerProperty`
	* :class:`tlp.LayoutProperty`
	* :class:`tlp.SizeProperty`
	* :class:`tlp.StringProperty`
	* :class:`tlp.PropertyInterface`

If you try to store an unsupported data type, an exception will be thrown.	
Each value stored in a data set is associated to a string key. Reading / Writing a value in 
a data set is done through the [] operator as illustrated below::

	# data set creation
	dataSet = tlp.DataSet()
	
	# writing integer values
	dataSet["height"] = 100
	dataSet["width"] = 300
	
	# reading integer values
	area = dataSet["height"] * dataSet["width"] 

When reading a data set value, a reference is returned not a copy.

%End

	DataSet();
	
	DataSet(const tlp::DataSet &set);

	bool exist(const std::string &str) const;
%Docstring
tlp.DataSet.exist(key)

Checks if the data set contains a value associated to a particular key.

:param key: the key to check
:type key: string
:rtype: boolean
%End

	void remove(const std::string &str);
%Docstring
tlp.DataSet.remove(key)

Remove the value associated to a particular key from the data set if it exists.

:param key: the key of the value to remove.
:type key: string
%End
	
	unsigned int size() const;
%Docstring
tlp.DataSet.size()

Returns the number of values stored in the data set.

:rtype: integer
%End
	
	std::vector<std::string> getKeys() const;
%Docstring
tlp.DataSet.getKeys()

Returns the list of keys associated to the values stored in the data set.

:rtype: list of string
%End	
	
%MethodCode
	std::vector<std::string> ret;
	tlp::Iterator< std::pair<std::string, tlp::DataType*> > *it = sipCpp->getValues();
	while (it->hasNext()) {
		ret.push_back(it->next().first);
	}
	delete it;
	sipRes = new std::vector<std::string>(ret);
%End	
	
	void __setitem__(const std::string &attributeName, SIP_PYOBJECT po);
%MethodCode
        tlp::DataType* dataType = sipCpp->getData(*a0);
        if (dataType && dataType->getTypeName() == std::string(typeid(tlp::StringCollection).name()) &&
#if PY_MAJOR_VERSION >= 3
        PyUnicode_Check(a1)) {
#else
        PyString_Check(a1)) {
#endif
            tlp::StringCollection *sc = reinterpret_cast<tlp::StringCollection*>(dataType->value);
#if PY_MAJOR_VERSION >= 3
            std::string entry(convertPythonUnicodeObjectToStdString(a1));
#else
            std::string entry(PyString_AS_STRING(a1));
#endif

            std::vector<std::string> values = sc->getValues();
            if (std::find(values.begin(), values.end(), entry) != values.end()) {
                sc->setCurrent(entry);
            } else {
                sipIsErr = 1;
                std::string msg = "Value \"";
                msg += entry;
                msg += "\" does not exist in the strings collection stored in the \"";
                msg += ((*a0) + "\" entry in the dataset.\n");
                msg += "Possible values are : ";
                for (size_t i = 0 ; i < values.size() ; ++i) {
                    replaceAll(values[i], "\n", "\\n");
                    msg += values[i];
                    if (i != values.size() -1) {
                        msg += ", ";
                    }
                }
                PyErr_SetString(PyExc_Exception, msg.c_str());
            }

        } else {
            ValueSetter valSetter(sipCpp, *a0);
            if (!setCppValueFromPyObject(a1, valSetter, dataType)) {
                sipIsErr = 1;
                std::string msg = "Object of type ";
                msg += std::string(a1->ob_type->tp_name);
                msg += " can not be store in a Tulip dataset.";
                PyErr_SetString(PyExc_Exception, msg.c_str());
            }
        }
        delete dataType;
%End
	
	
	SIP_PYOBJECT __getitem__(const std::string &attributeName);
%MethodCode
    sipRes = NULL;
    tlp::Iterator< std::pair<std::string, tlp::DataType*> > *it = sipCpp->getValues();
    tlp::DataType* dataType = NULL;
    while (it->hasNext()) {
        std::pair<std::string, tlp::DataType*> p = it->next();
        if (p.first == *a0) {
            dataType = p.second;
        }
    }
    delete it;
	
    sipRes = getPyObjectFromDataType(dataType, true);
	
    if (!sipRes) {
        std::ostringstream oss;
        oss << "Dataset entry \"" << *a0 << "\" does not exist.";
        sipIsErr = 1;
        PyErr_SetString(PyExc_AttributeError, oss.str().c_str());
    }
%End	

};
};

