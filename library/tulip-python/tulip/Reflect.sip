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
// | Original Bindings by the Booggie project developpment team              |
// | (http://booggie.org/)                                                   |
// | Improved and maintained by Antoine Lambert 			                 |
// | Contact:  antoine.lambert@labri.fr 								     |
// +-------------------------------------------------------------------------+
// | License:                                                                |
// |                                                                         |
// | Tulip Python Bindings is free software; you can redistribute it         |
// | and/or modify  it under the terms of the GNU General Public License     |
// | as published by the Free Software Foundation; either version 2 of the   |
// | License, or (at your option) any later version.                         |
// |                                                                         |
// | Tulip Python Bindings is distributed in the hope that it will be        |
// | useful, but WITHOUT ANY WARRANTY; without even the implied warranty of  |
// | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the            |
// | GNU General Public License for more details.                            |
// |                                                                         |
// | You should have received a copy of the GNU General Public License       |
// | along with this program.  If not, see <http://www.gnu.org/licenses/>.   |
// |                                                                         |
// +-------------------------------------------------------------------------+

namespace tlp {


struct DataSet {
	DataSet();
	DataSet(const tlp::DataSet &set);

	bool getDataSet(const std::string &str, tlp::DataSet &v /Out/);
%MethodCode
	sipRes = (sipSelfWasArg ? sipCpp->tlp::DataSet::get<tlp::DataSet>(*a0, *a1) :
			sipCpp->get<tlp::DataSet>(*a0, *a1));
%End
	
	void setDataSet(const std::string &str, const tlp::DataSet &v);
%MethodCode
	(sipSelfWasArg ? sipCpp->tlp::DataSet::set<tlp::DataSet>(*a0, *a1) :
			sipCpp->set<tlp::DataSet>(*a0, *a1));
%End

	bool getString(const std::string &str, std::string &v /Out/);
%MethodCode
	sipRes = (sipSelfWasArg ? sipCpp->tlp::DataSet::get<std::string>(*a0, *a1) :
			sipCpp->get<std::string>(*a0, *a1));
%End
	
	void setString(const std::string &str, const std::string &v);
%MethodCode
	(sipSelfWasArg ? sipCpp->tlp::DataSet::set<std::string>(*a0, *a1) :
			sipCpp->set<std::string>(*a0, *a1));
%End

	bool getBool(const std::string &str, bool *v);
%MethodCode
	sipRes = (sipSelfWasArg ? sipCpp->tlp::DataSet::get<bool>(*a0, a1) :
			sipCpp->get<bool>(*a0, a1));
%End
	
	void setBool(const std::string &str, const bool v);
%MethodCode
	(sipSelfWasArg ? sipCpp->tlp::DataSet::set<bool>(*a0, a1) :
			sipCpp->set<bool>(*a0, a1));
%End

	bool getInt(const std::string &str, int *v);
%MethodCode
	sipRes = (sipSelfWasArg ? sipCpp->tlp::DataSet::get<int>(*a0, a1) :
			sipCpp->get<int>(*a0, a1));
%End
	
	void setInt(const std::string &str, const int v);
%MethodCode
	(sipSelfWasArg ? sipCpp->tlp::DataSet::set<int>(*a0, a1) :
			sipCpp->set<int>(*a0, a1));
%End

	bool getUInt(const std::string &str, unsigned int *v);
%MethodCode
	sipRes = (sipSelfWasArg ? sipCpp->tlp::DataSet::get<unsigned int>(*a0, a1) :
			sipCpp->get<unsigned int>(*a0, a1));
%End
	
	void setUInt(const std::string &str, const unsigned int v);
%MethodCode
	(sipSelfWasArg ? sipCpp->tlp::DataSet::set<unsigned int>(*a0, a1) :
			sipCpp->set<unsigned int>(*a0, a1));
%End

	bool getFloat(const std::string &str, float *v);
%MethodCode
	sipRes = (sipSelfWasArg ? sipCpp->tlp::DataSet::get<float>(*a0, a1) :
			sipCpp->get<float>(*a0, a1));
%End
	
	void setFloat(const std::string &str, const float v);
%MethodCode
	(sipSelfWasArg ? sipCpp->tlp::DataSet::set<float>(*a0, a1) :
			sipCpp->set<float>(*a0, a1));
%End

	bool getDouble(const std::string &str, double *v);
%MethodCode
	sipRes = (sipSelfWasArg ? sipCpp->tlp::DataSet::get<double>(*a0, a1) :
			sipCpp->get<double>(*a0, a1));
%End
	
	void setDouble(const std::string &str, const double v);
%MethodCode
	(sipSelfWasArg ? sipCpp->tlp::DataSet::set<double>(*a0, a1) :
			sipCpp->set<double>(*a0, a1));
%End
	
	bool getSize(const std::string &str, tlp::Size &v /Out/);
%MethodCode
	sipRes = (sipSelfWasArg ? sipCpp->tlp::DataSet::get<tlp::Size>(*a0, *a1) :
			sipCpp->get<tlp::Size>(*a0, *a1));
%End

	void setCoord(const std::string &str, const tlp::Coord &v);
%MethodCode
	(sipSelfWasArg ? sipCpp->tlp::DataSet::set<tlp::Coord>(*a0, *a1) :
			sipCpp->set<tlp::Coord>(*a0, *a1));
%End

	bool getCoord(const std::string &str, tlp::Coord &v /Out/);
%MethodCode
	sipRes = (sipSelfWasArg ? sipCpp->tlp::DataSet::get<tlp::Coord>(*a0, *a1) :
			sipCpp->get<tlp::Coord>(*a0, *a1));
%End

	void setColor(const std::string &str, const tlp::Color &v);
%MethodCode
	(sipSelfWasArg ? sipCpp->tlp::DataSet::set<tlp::Color>(*a0, *a1) :
			sipCpp->set<tlp::Color>(*a0, *a1));
%End

	bool getColor(const std::string &str, tlp::Color &v /Out/);
%MethodCode
	sipRes = (sipSelfWasArg ? sipCpp->tlp::DataSet::get<tlp::Color>(*a0, *a1) :
			sipCpp->get<tlp::Color>(*a0, *a1));
%End

	void setColorScale(const std::string &str, const tlp::ColorScale &v);
%MethodCode
	(sipSelfWasArg ? sipCpp->tlp::DataSet::set<tlp::ColorScale>(*a0, *a1) :
			sipCpp->set<tlp::ColorScale>(*a0, *a1));
%End

	bool getColorScale(const std::string &str, tlp::ColorScale &v /Out/);
%MethodCode
	sipRes = (sipSelfWasArg ? sipCpp->tlp::DataSet::get<tlp::ColorScale>(*a0, *a1) :
			sipCpp->get<tlp::ColorScale>(*a0, *a1));
%End

	void setStringCollection(const std::string &str, const tlp::StringCollection &v);
%MethodCode
	(sipSelfWasArg ? sipCpp->tlp::DataSet::set<tlp::StringCollection>(*a0, *a1) :
			sipCpp->set<tlp::StringCollection>(*a0, *a1));
%End

	bool getStringCollection(const std::string &str, tlp::StringCollection &v /Out/);
%MethodCode
	sipRes = (sipSelfWasArg ? sipCpp->tlp::DataSet::get<tlp::StringCollection>(*a0, *a1) :
			sipCpp->get<tlp::StringCollection>(*a0, *a1));
%End	
	
	void setLayoutProperty(const std::string &str, const tlp::LayoutProperty *v );
%MethodCode
	(sipSelfWasArg ? sipCpp->tlp::DataSet::set(*a0, a1) :
			sipCpp->set(*a0, a1));
%End

	bool getLayoutProperty(const std::string &str, tlp::LayoutProperty *&v /Out/);
%MethodCode
	a1 = NULL;
	sipRes = (sipSelfWasArg ? sipCpp->tlp::DataSet::get<tlp::LayoutProperty*>(*a0, a1) :
			sipCpp->get<tlp::LayoutProperty*>(*a0, a1));
%End	

	void setDoubleProperty(const std::string &str, const tlp::DoubleProperty *v);
%MethodCode
	(sipSelfWasArg ? sipCpp->tlp::DataSet::set(*a0, a1) :
			sipCpp->set(*a0, a1));
%End

	bool getDoubleProperty(const std::string &str, tlp::DoubleProperty *&v /Out/);
%MethodCode
	a1 = NULL;
	sipRes = (sipSelfWasArg ? sipCpp->tlp::DataSet::get<tlp::DoubleProperty*>(*a0, a1) :
			sipCpp->get<tlp::DoubleProperty*>(*a0, a1));
%End	

	void setIntegerProperty(const std::string &str, const tlp::IntegerProperty *v);
%MethodCode
	(sipSelfWasArg ? sipCpp->tlp::DataSet::set(*a0, a1) :
			sipCpp->set(*a0, a1));
%End

	bool getIntegerProperty(const std::string &str, tlp::IntegerProperty *&v /Out/);
%MethodCode
	a1 = NULL;
	sipRes = (sipSelfWasArg ? sipCpp->tlp::DataSet::get<tlp::IntegerProperty*>(*a0, a1) :
			sipCpp->get<tlp::IntegerProperty*>(*a0, a1));
%End	

	void setBooleanProperty(const std::string &str, const tlp::BooleanProperty *v);
%MethodCode
	(sipSelfWasArg ? sipCpp->tlp::DataSet::set(*a0, a1) :
			sipCpp->set(*a0, a1));
%End

	bool getBooleanProperty(const std::string &str, tlp::BooleanProperty *&v /Out/);
%MethodCode
	a1 = NULL;
	sipRes = (sipSelfWasArg ? sipCpp->tlp::DataSet::get<tlp::BooleanProperty*>(*a0, a1) :
			sipCpp->get<tlp::BooleanProperty*>(*a0, a1));
%End	

	void setStringProperty(const std::string &str, const tlp::StringProperty *v);
%MethodCode
	(sipSelfWasArg ? sipCpp->tlp::DataSet::set(*a0, a1) :
			sipCpp->set(*a0, a1));
%End

	bool getStringProperty(const std::string &str, tlp::StringProperty *&v /Out/);
%MethodCode
	a1 = NULL;
	sipRes = (sipSelfWasArg ? sipCpp->tlp::DataSet::get<tlp::StringProperty*>(*a0, a1) :
			sipCpp->get<tlp::StringProperty*>(*a0, a1));
%End	
	
	void setColorProperty(const std::string &str, const tlp::ColorProperty *v);
%MethodCode
	(sipSelfWasArg ? sipCpp->tlp::DataSet::set(*a0, a1) :
			sipCpp->set(*a0, a1));
%End

	bool getColorProperty(const std::string &str, tlp::ColorProperty *&v /Out/);
%MethodCode
	a1 = NULL;
	sipRes = (sipSelfWasArg ? sipCpp->tlp::DataSet::get<tlp::ColorProperty*>(*a0, a1) :
			sipCpp->get<tlp::ColorProperty*>(*a0, a1));
%End	

void setProperty(const std::string &str, const tlp::PropertyInterface *v);
%MethodCode
	(sipSelfWasArg ? sipCpp->tlp::DataSet::set(*a0, a1) :
			sipCpp->set(*a0, a1));
%End

	bool getProperty(const std::string &str, tlp::PropertyInterface *&v /Out/);
%MethodCode
	a1 = NULL;
	sipRes = (sipSelfWasArg ? sipCpp->tlp::DataSet::get<tlp::PropertyInterface*>(*a0, a1) :
			sipCpp->get<tlp::PropertyInterface*>(*a0, a1));
%End	
	
	bool exist(const std::string &str) const;

	void remove(const std::string &str);

};
};

