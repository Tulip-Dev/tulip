/*
 *
 * This file is part of Tulip (http://tulip.labri.fr)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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
// | (https://sourceforge.net/projects/booggie/)                             |
// +-------------------------------------------------------------------------+

// Proxy class used internally allowing to automatically create graph properties
// trough type inference by using the following syntax in Python :
//      # graph is a reference to a tlp.Graph object and n to a tlp.node object
//      # a double property named 'metric' will be automatically created
//      # no need to call 'graph.getDoubleProperty('metric') before anymore
//      graph['weight'][n] = 2.5

%ModuleHeaderCode

namespace tlp {

class PropertyProxy {

public:

  PropertyProxy(tlp::Graph *graph, const std::string &propertyName);

  void setNodeValue(const tlp::node &n, bool val);

  void setNodeValue(const tlp::node &n, int val);

  void setNodeValue(const tlp::node &n, double val);

  void setNodeValue(const tlp::node &n, const std::string &val);

  void setNodeValue(const tlp::node &n, const tlp::PySize &val);

  void setNodeValue(const tlp::node &n, const tlp::Coord &val);

  void setNodeValue(const tlp::node &n, const tlp::Color &val);

  void setNodeValue(const tlp::node &n, const std::vector<bool> &val);

  void setNodeValue(const tlp::node &n, const std::vector<int> &val);

  void setNodeValue(const tlp::node &n, const std::vector<double> &val);

  void setNodeValue(const tlp::node &n, const std::vector<std::string> &val);

  void setNodeValue(const tlp::node &n, const std::vector<tlp::PySize> &val);

  void setNodeValue(const tlp::node &n, const std::vector<tlp::Coord> &val);

  void setNodeValue(const tlp::node &n, const std::vector<tlp::Color> &val);

  void setEdgeValue(const tlp::edge &e, bool val);

  void setEdgeValue(const tlp::edge &e, int val);

  void setEdgeValue(const tlp::edge &e, double val);

  void setEdgeValue(const tlp::edge &e, const std::string &val);

  void setEdgeValue(const tlp::edge &e, const tlp::PySize &val);

  void setEdgeValue(const tlp::edge &e, const std::vector<tlp::Coord> &val);

  void setEdgeValue(const tlp::edge &e, const tlp::Color &val);

  void setEdgeValue(const tlp::edge &e, const std::vector<bool> &val);

  void setEdgeValue(const tlp::edge &e, const std::vector<int> &val);

  void setEdgeValue(const tlp::edge &e, const std::vector<double> &val);

  void setEdgeValue(const tlp::edge &e, const std::vector<std::string> &val);

  void setEdgeValue(const tlp::edge &e, const std::vector<tlp::PySize> &val);

  void setEdgeValue(const tlp::edge &e, const std::vector<tlp::Color> &val);

  void setAllNodeValue(bool val);

  void setAllNodeValue(int val);

  void setAllNodeValue(double val);

  void setAllNodeValue(const std::string &val);

  void setAllNodeValue(const tlp::PySize &val);

  void setAllNodeValue(const tlp::Coord &val);

  void setAllNodeValue(const tlp::Color &val);

  void setAllNodeValue(const std::vector<bool> &val);

  void setAllNodeValue(const std::vector<int> &val);

  void setAllNodeValue(const std::vector<double> &val);

  void setAllNodeValue(const std::vector<std::string> &val);

  void setAllNodeValue(const std::vector<tlp::PySize> &val);

  void setAllNodeValue(const std::vector<tlp::Coord> &val);

  void setAllNodeValue(const std::vector<tlp::Color> &val);

  void setAllEdgeValue(bool val);

  void setAllEdgeValue(int val);

  void setAllEdgeValue(double val);

  void setAllEdgeValue(const std::string &val);

  void setAllEdgeValue(const tlp::PySize &val);

  void setAllEdgeValue(const std::vector<tlp::Coord> &val);

  void setAllEdgeValue(const tlp::Color &val);

  void setAllEdgeValue(const std::vector<bool> &val);

  void setAllEdgeValue(const std::vector<int> &val);

  void setAllEdgeValue(const std::vector<double> &val);

  void setAllEdgeValue(const std::vector<std::string> &val);

  void setAllEdgeValue(const std::vector<tlp::PySize> &val);

  void setAllEdgeValue(const std::vector<tlp::Color> &val);

  tlp::Graph *getGraph() const {
    return _graph;
  }

  tlp::PropertyInterface *getProperty() const {
    return _property;
  }

  std::string getPropertyName() const {
    return _propertyName;
  }

private:

  tlp::Graph *_graph;
  std::string _propertyName;
  tlp::PropertyInterface *_property;

};

}

%End


%ModuleCode

tlp::PropertyProxy::PropertyProxy(tlp::Graph *graph, const std::string &propertyName) :
  _graph(graph), _propertyName(propertyName), _property(NULL) {}

void tlp::PropertyProxy::setNodeValue(const tlp::node &n, bool val) {
  _property = _graph->getProperty<tlp::BooleanProperty>(_propertyName);
  static_cast<tlp::BooleanProperty*>(_property)->setNodeValue(n, val);
}

void tlp::PropertyProxy::setNodeValue(const tlp::node &n, int val) {
  _property = _graph->getProperty<tlp::IntegerProperty>(_propertyName);
  static_cast<tlp::IntegerProperty*>(_property)->setNodeValue(n, val);
}

void tlp::PropertyProxy::setNodeValue(const tlp::node &n, double val) {
  _property = _graph->getProperty<tlp::DoubleProperty>(_propertyName);
  static_cast<tlp::DoubleProperty*>(_property)->setNodeValue(n, val);
}

void tlp::PropertyProxy::setNodeValue(const tlp::node &n, const std::string &val) {
  _property = _graph->getProperty<tlp::StringProperty>(_propertyName);
  static_cast<tlp::StringProperty*>(_property)->setNodeValue(n, val);
}

void tlp::PropertyProxy::setNodeValue(const tlp::node &n, const tlp::Coord &val) {
  _property = _graph->getProperty<tlp::LayoutProperty>(_propertyName);
  static_cast<tlp::LayoutProperty*>(_property)->setNodeValue(n, val);
}

void tlp::PropertyProxy::setNodeValue(const tlp::node &n, const tlp::PySize &val) {
  _property = _graph->getProperty<tlp::SizeProperty>(_propertyName);
  static_cast<tlp::SizeProperty*>(_property)->setNodeValue(n, val);
}

void tlp::PropertyProxy::setNodeValue(const tlp::node &n, const tlp::Color &val) {
  _property = _graph->getProperty<tlp::ColorProperty>(_propertyName);
  static_cast<tlp::ColorProperty*>(_property)->setNodeValue(n, val);
}

void tlp::PropertyProxy::setNodeValue(const tlp::node &n, const std::vector<bool> &val) {
  if (val.size() == 0) return;
  _property = _graph->getProperty<tlp::BooleanVectorProperty>(_propertyName);
  static_cast<tlp::BooleanVectorProperty*>(_property)->setNodeValue(n, val);
}

void tlp::PropertyProxy::setNodeValue(const tlp::node &n, const std::vector<int> &val) {
  if (val.size() == 0) return;
  _property = _graph->getProperty<tlp::IntegerVectorProperty>(_propertyName);
  static_cast<tlp::IntegerVectorProperty*>(_property)->setNodeValue(n, val);
}

void tlp::PropertyProxy::setNodeValue(const tlp::node &n, const std::vector<double> &val) {
  if (val.size() == 0) return;
  _property = _graph->getProperty<tlp::DoubleVectorProperty>(_propertyName);
  static_cast<tlp::DoubleVectorProperty*>(_property)->setNodeValue(n, val);
}

void tlp::PropertyProxy::setNodeValue(const tlp::node &n, const std::vector<std::string> &val) {
  if (val.size() == 0) return;
  _property = _graph->getProperty<tlp::StringVectorProperty>(_propertyName);
  static_cast<tlp::StringVectorProperty*>(_property)->setNodeValue(n, val);
}

void tlp::PropertyProxy::setNodeValue(const tlp::node &n, const std::vector<tlp::Coord> &val) {
  if (val.size() == 0) return;
  _property = _graph->getProperty<tlp::CoordVectorProperty>(_propertyName);
  static_cast<tlp::CoordVectorProperty*>(_property)->setNodeValue(n, val);
}

void tlp::PropertyProxy::setNodeValue(const tlp::node &n, const std::vector<tlp::PySize> &val) {
  if (val.size() == 0) return;
  _property = _graph->getProperty<tlp::SizeVectorProperty>(_propertyName);
  static_cast<tlp::SizeVectorProperty*>(_property)->setNodeValue(n, reinterpret_cast<const std::vector<tlp::Size> &>(val));
}

void tlp::PropertyProxy::setNodeValue(const tlp::node &n, const std::vector<tlp::Color> &val) {
  if (val.size() == 0) return;
  _property = _graph->getProperty<tlp::ColorVectorProperty>(_propertyName);
  static_cast<tlp::ColorVectorProperty*>(_property)->setNodeValue(n, val);
}

void tlp::PropertyProxy::setEdgeValue(const tlp::edge &e, bool val) {
  _property = _graph->getProperty<tlp::BooleanProperty>(_propertyName);
  static_cast<tlp::BooleanProperty*>(_property)->setEdgeValue(e, val);
}

void tlp::PropertyProxy::setEdgeValue(const tlp::edge &e, int val) {
  _property = _graph->getProperty<tlp::IntegerProperty>(_propertyName);
  static_cast<tlp::IntegerProperty*>(_property)->setEdgeValue(e, val);
}

void tlp::PropertyProxy::setEdgeValue(const tlp::edge &e, double val) {
  _property = _graph->getProperty<tlp::DoubleProperty>(_propertyName);
  static_cast<tlp::DoubleProperty*>(_property)->setEdgeValue(e, val);
}

void tlp::PropertyProxy::setEdgeValue(const tlp::edge &e, const std::string &val) {
  _property = _graph->getProperty<tlp::StringProperty>(_propertyName);
  static_cast<tlp::StringProperty*>(_property)->setEdgeValue(e, val);
}

void tlp::PropertyProxy::setEdgeValue(const tlp::edge &e, const std::vector<tlp::Coord> &val) {
  if (val.size() == 0) return;
  _property = _graph->getProperty<tlp::LayoutProperty>(_propertyName);
  static_cast<tlp::LayoutProperty*>(_property)->setEdgeValue(e, val);
}

void tlp::PropertyProxy::setEdgeValue(const tlp::edge &e, const tlp::PySize &val) {
  _property = _graph->getProperty<tlp::SizeProperty>(_propertyName);
  static_cast<tlp::SizeProperty*>(_property)->setEdgeValue(e, val);
}

void tlp::PropertyProxy::setEdgeValue(const tlp::edge &e, const tlp::Color &val) {
  _property = _graph->getProperty<tlp::ColorProperty>(_propertyName);
  static_cast<tlp::ColorProperty*>(_property)->setEdgeValue(e, val);
}

void tlp::PropertyProxy::setEdgeValue(const tlp::edge &e, const std::vector<bool> &val) {
  if (val.size() == 0) return;
  _property = _graph->getProperty<tlp::BooleanVectorProperty>(_propertyName);
  static_cast<tlp::BooleanVectorProperty*>(_property)->setEdgeValue(e, val);
}

void tlp::PropertyProxy::setEdgeValue(const tlp::edge &e, const std::vector<int> &val) {
  if (val.size() == 0) return;
  _property = _graph->getProperty<tlp::IntegerVectorProperty>(_propertyName);
  static_cast<tlp::IntegerVectorProperty*>(_property)->setEdgeValue(e, val);
}

void tlp::PropertyProxy::setEdgeValue(const tlp::edge &e, const std::vector<double> &val) {
  if (val.size() == 0) return;
  _property = _graph->getProperty<tlp::DoubleVectorProperty>(_propertyName);
  static_cast<tlp::DoubleVectorProperty*>(_property)->setEdgeValue(e, val);
}

void tlp::PropertyProxy::setEdgeValue(const tlp::edge &e, const std::vector<std::string> &val) {
  if (val.size() == 0) return;
  _property = _graph->getProperty<tlp::StringVectorProperty>(_propertyName);
  static_cast<tlp::StringVectorProperty*>(_property)->setEdgeValue(e, val);
}

void tlp::PropertyProxy::setEdgeValue(const tlp::edge &e, const std::vector<tlp::PySize> &val) {
  if (val.size() == 0) return;
  _property = _graph->getProperty<tlp::SizeVectorProperty>(_propertyName);
  static_cast<tlp::SizeVectorProperty*>(_property)->setEdgeValue(e, reinterpret_cast<const std::vector<tlp::Size> &>(val));
}

void tlp::PropertyProxy::setEdgeValue(const tlp::edge &e, const std::vector<tlp::Color> &val) {
  if (val.size() == 0) return;
  _property = _graph->getProperty<tlp::ColorVectorProperty>(_propertyName);
  static_cast<tlp::ColorVectorProperty*>(_property)->setEdgeValue(e, val);
}

void tlp::PropertyProxy::setAllNodeValue(bool val) {
  _property = _graph->getProperty<tlp::BooleanProperty>(_propertyName);
  static_cast<tlp::BooleanProperty*>(_property)->setAllNodeValue(val);
}

void tlp::PropertyProxy::setAllNodeValue(int val) {
  _property = _graph->getProperty<tlp::IntegerProperty>(_propertyName);
  static_cast<tlp::IntegerProperty*>(_property)->setAllNodeValue(val);
}

void tlp::PropertyProxy::setAllNodeValue(double val) {
  _property = _graph->getProperty<tlp::DoubleProperty>(_propertyName);
  static_cast<tlp::DoubleProperty*>(_property)->setAllNodeValue(val);
}

void tlp::PropertyProxy::setAllNodeValue(const std::string &val) {
  _property = _graph->getProperty<tlp::StringProperty>(_propertyName);
  static_cast<tlp::StringProperty*>(_property)->setAllNodeValue(val);
}

void tlp::PropertyProxy::setAllNodeValue(const tlp::Coord &val) {
  _property = _graph->getProperty<tlp::LayoutProperty>(_propertyName);
  static_cast<tlp::LayoutProperty*>(_property)->setAllNodeValue(val);
}

void tlp::PropertyProxy::setAllNodeValue(const tlp::PySize &val) {
  _property = _graph->getProperty<tlp::SizeProperty>(_propertyName);
  static_cast<tlp::SizeProperty*>(_property)->setAllNodeValue(val);
}

void tlp::PropertyProxy::setAllNodeValue(const tlp::Color &val) {
  _property = _graph->getProperty<tlp::ColorProperty>(_propertyName);
  static_cast<tlp::ColorProperty*>(_property)->setAllNodeValue(val);
}

void tlp::PropertyProxy::setAllNodeValue(const std::vector<bool> &val) {
  if (val.size() == 0) return;
  _property = _graph->getProperty<tlp::BooleanVectorProperty>(_propertyName);
  static_cast<tlp::BooleanVectorProperty*>(_property)->setAllNodeValue(val);
}

void tlp::PropertyProxy::setAllNodeValue(const std::vector<int> &val) {
  if (val.size() == 0) return;
  _property = _graph->getProperty<tlp::IntegerVectorProperty>(_propertyName);
  static_cast<tlp::IntegerVectorProperty*>(_property)->setAllNodeValue(val);
}

void tlp::PropertyProxy::setAllNodeValue(const std::vector<double> &val) {
  if (val.size() == 0) return;
  _property = _graph->getProperty<tlp::DoubleVectorProperty>(_propertyName);
  static_cast<tlp::DoubleVectorProperty*>(_property)->setAllNodeValue(val);
}

void tlp::PropertyProxy::setAllNodeValue(const std::vector<std::string> &val) {
  if (val.size() == 0) return;
  _property = _graph->getProperty<tlp::StringVectorProperty>(_propertyName);
  static_cast<tlp::StringVectorProperty*>(_property)->setAllNodeValue(val);
}

void tlp::PropertyProxy::setAllNodeValue(const std::vector<tlp::Coord> &val) {
  if (val.size() == 0) return;
  _property = _graph->getProperty<tlp::CoordVectorProperty>(_propertyName);
  static_cast<tlp::CoordVectorProperty*>(_property)->setAllNodeValue(val);
}

void tlp::PropertyProxy::setAllNodeValue(const std::vector<tlp::PySize> &val) {
  if (val.size() == 0) return;
  _property = _graph->getProperty<tlp::SizeVectorProperty>(_propertyName);
  static_cast<tlp::SizeVectorProperty*>(_property)->setAllNodeValue(reinterpret_cast<const std::vector<tlp::Size> &>(val));
}

void tlp::PropertyProxy::setAllNodeValue(const std::vector<tlp::Color> &val) {
  if (val.size() == 0) return;
  _property = _graph->getProperty<tlp::ColorVectorProperty>(_propertyName);
  static_cast<tlp::ColorVectorProperty*>(_property)->setAllNodeValue(val);
}

void tlp::PropertyProxy::setAllEdgeValue(bool val) {
  _property = _graph->getProperty<tlp::BooleanProperty>(_propertyName);
  static_cast<tlp::BooleanProperty*>(_property)->setAllEdgeValue(val);
}

void tlp::PropertyProxy::setAllEdgeValue(int val) {
  _property = _graph->getProperty<tlp::IntegerProperty>(_propertyName);
  static_cast<tlp::IntegerProperty*>(_property)->setAllEdgeValue(val);
}

void tlp::PropertyProxy::setAllEdgeValue(double val) {
  _property = _graph->getProperty<tlp::DoubleProperty>(_propertyName);
  static_cast<tlp::DoubleProperty*>(_property)->setAllEdgeValue(val);
}

void tlp::PropertyProxy::setAllEdgeValue(const std::string &val) {
  _property = _graph->getProperty<tlp::StringProperty>(_propertyName);
  static_cast<tlp::StringProperty*>(_property)->setAllEdgeValue(val);
}

void tlp::PropertyProxy::setAllEdgeValue(const std::vector<tlp::Coord> &val) {
  if (val.size() == 0) return;
  _property = _graph->getProperty<tlp::LayoutProperty>(_propertyName);
  static_cast<tlp::LayoutProperty*>(_property)->setAllEdgeValue(val);
}

void tlp::PropertyProxy::setAllEdgeValue(const tlp::PySize &val) {
  _property = _graph->getProperty<tlp::SizeProperty>(_propertyName);
  static_cast<tlp::SizeProperty*>(_property)->setAllEdgeValue(val);
}

void tlp::PropertyProxy::setAllEdgeValue(const tlp::Color &val) {
  _property = _graph->getProperty<tlp::ColorProperty>(_propertyName);
  static_cast<tlp::ColorProperty*>(_property)->setAllEdgeValue(val);
}

void tlp::PropertyProxy::setAllEdgeValue(const std::vector<bool> &val) {
  if (val.size() == 0) return;
  _property = _graph->getProperty<tlp::BooleanVectorProperty>(_propertyName);
  static_cast<tlp::BooleanVectorProperty*>(_property)->setAllEdgeValue(val);
}

void tlp::PropertyProxy::setAllEdgeValue(const std::vector<int> &val) {
  if (val.size() == 0) return;
  _property = _graph->getProperty<tlp::IntegerVectorProperty>(_propertyName);
  static_cast<tlp::IntegerVectorProperty*>(_property)->setAllEdgeValue(val);
}

void tlp::PropertyProxy::setAllEdgeValue(const std::vector<double> &val) {
  if (val.size() == 0) return;
  _property = _graph->getProperty<tlp::DoubleVectorProperty>(_propertyName);
  static_cast<tlp::DoubleVectorProperty*>(_property)->setAllEdgeValue(val);
}

void tlp::PropertyProxy::setAllEdgeValue(const std::vector<std::string> &val) {
  if (val.size() == 0) return;
  _property = _graph->getProperty<tlp::StringVectorProperty>(_propertyName);
  static_cast<tlp::StringVectorProperty*>(_property)->setAllEdgeValue(val);
}

void tlp::PropertyProxy::setAllEdgeValue(const std::vector<tlp::PySize> &val) {
  if (val.size() == 0) return;
  _property = _graph->getProperty<tlp::SizeVectorProperty>(_propertyName);
  static_cast<tlp::SizeVectorProperty*>(_property)->setAllEdgeValue(reinterpret_cast<const std::vector<tlp::Size> &>(val));
}

void tlp::PropertyProxy::setAllEdgeValue(const std::vector<tlp::Color> &val) {
  if (val.size() == 0) return;
  _property = _graph->getProperty<tlp::ColorVectorProperty>(_propertyName);
  static_cast<tlp::ColorVectorProperty*>(_property)->setAllEdgeValue(val);
}

%End

namespace tlp {

class PropertyProxy /Abstract/ {

public:

  void __setitem__(const tlp::node n, bool val /Constrained/);
%MethodCode
  if (sipCpp->getGraph()->isElement(*a0)) {
    sipCpp->setNodeValue(*a0, a1);
  } else {
    sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
  }
%End

  void __setitem__(const tlp::node n, int val /Constrained/);
%MethodCode
  if (sipCpp->getGraph()->isElement(*a0)) {
    sipCpp->setNodeValue(*a0, a1);
  } else {
    sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
  }
%End

  void __setitem__(const tlp::node n, double val /Constrained/);
%MethodCode
  if (sipCpp->getGraph()->isElement(*a0)) {
    sipCpp->setNodeValue(*a0, a1);
  } else {
    sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
  }
%End

  void __setitem__(const tlp::node n, const std::string &val);
%MethodCode
  if (sipCpp->getGraph()->isElement(*a0)) {
    sipCpp->setNodeValue(*a0, *a1);
  } else {
    sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
  }
%End

  void __setitem__(const tlp::node n, const tlp::PySize &val /Constrained/);
%MethodCode
  if (sipCpp->getGraph()->isElement(*a0)) {
    sipCpp->setNodeValue(*a0, *a1);
  } else {
    sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
  }
%End

 void __setitem__(const tlp::node n, const tlp::Coord &val /GetWrapper/);
%MethodCode
  if (sipCpp->getGraph()->isElement(*a0)) {
    // when a tuple is provided as input, constrain its elements to float type
    // for creating a layout property otherwise create a color property
    const sipTypeDef *sipType = sipFindType("tlp::Color");
    if (sipCanConvertToType(a1Wrapper, sipType, SIP_NOT_NONE)) {
      int state=0, err=0;
      void *cppType = sipConvertToType(a1Wrapper, sipType, NULL, SIP_NOT_NONE, &state, &err);
      sipCpp->setNodeValue(*a0, *static_cast<tlp::Color*>(cppType));
      sipReleaseType(cppType, sipType, state);
    } else {
      sipCpp->setNodeValue(*a0, *a1);
    }
  } else {
    sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
  }
%End

  void __setitem__(const tlp::node n, const tlp::Color &val);
%MethodCode
 if (sipCpp->getGraph()->isElement(*a0)) {
    sipCpp->setNodeValue(*a0, *a1);
  } else {
    sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
  }
%End

  void __setitem__(const tlp::node n, const std::vector<bool> &val /Constrained/);
%MethodCode
  if (sipCpp->getGraph()->isElement(*a0)) {
    sipCpp->setNodeValue(*a0, *a1);
  } else {
    sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
  }
  delete a1;
%End

void __setitem__(const tlp::node n, const std::vector<int> &val /Constrained/);
%MethodCode
  if (sipCpp->getGraph()->isElement(*a0)) {
    sipCpp->setNodeValue(*a0, *a1);
  } else {
    sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
  }
  delete a1;
%End

  void __setitem__(const tlp::node n, const std::vector<double> &val /Constrained/);
%MethodCode
  if (sipCpp->getGraph()->isElement(*a0)) {
    sipCpp->setNodeValue(*a0, *a1);
  } else {
    sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
  }
  delete a1;
%End

  void __setitem__(const tlp::node n, const std::vector<std::string> &val /TypeHint="List[str]"/);
%MethodCode
  if (sipCpp->getGraph()->isElement(*a0)) {
    sipCpp->setNodeValue(*a0, *a1);
  } else {
    sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
  }
%End

  void __setitem__(const tlp::node n, const std::vector<tlp::PySize> &val /Constrained, TypeHint="List[tlp.Size]"/);
%MethodCode
  if (sipCpp->getGraph()->isElement(*a0)) {
    sipCpp->setNodeValue(*a0, *a1);
  } else {
    sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
  }
  delete a1;
%End

  void __setitem__(const tlp::node n, const std::vector<tlp::Coord> &val /GetWrapper/);
%MethodCode
  if (sipCpp->getGraph()->isElement(*a0)) {
    // when a list of tuples is provided as input, constrain tuples elements to float type
    // for creating a coord vector property otherwise create a color vector property
    const sipTypeDef *sipType = sipFindType("std::vector<tlp::Color>");
    if (sipCanConvertToType(a1Wrapper, sipType, SIP_NOT_NONE)) {
      int state=0, err=0;
      void *cppType = sipConvertToType(a1Wrapper, sipType, NULL, SIP_NOT_NONE, &state, &err);
      std::vector<tlp::Color> *vc = static_cast<std::vector<tlp::Color>*>(cppType);
      sipCpp->setNodeValue(*a0, *vc);
      sipReleaseType(cppType, sipType, state);
    } else {
      sipCpp->setNodeValue(*a0, *a1);
    }
  } else {
    sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
  }
%End

  void __setitem__(const tlp::node n, const std::vector<tlp::Color> &val);
%MethodCode
  if (sipCpp->getGraph()->isElement(*a0)) {
    sipCpp->setNodeValue(*a0, *a1);
  } else {
    sipIsErr = throwInvalidNodeException(sipCpp->getGraph(), *a0);
  }
%End

  void __setitem__(const tlp::node n, SIP_PYOBJECT val);
%MethodCode
  std::string pythonType(a1->ob_type->tp_name);
  std::string errMsg = "Error : unable to create a graph property for Python type '" + pythonType + "'.";
  PyErr_SetString(PyExc_Exception, errMsg.c_str());
  sipIsErr = -1;
%End

  void __setitem__(const tlp::edge e, bool val /Constrained/);
%MethodCode
  if (sipCpp->getGraph()->isElement(*a0)) {
    sipCpp->setEdgeValue(*a0, a1);
  } else {
    sipIsErr = throwInvalidEdgeException(sipCpp->getGraph(), *a0);
  }
%End

  void __setitem__(const tlp::edge e, int val /Constrained/);
%MethodCode
  if (sipCpp->getGraph()->isElement(*a0)) {
    sipCpp->setEdgeValue(*a0, a1);
  } else {
    sipIsErr = throwInvalidEdgeException(sipCpp->getGraph(), *a0);
  }
%End

  void __setitem__(const tlp::edge e, double val /Constrained/);
%MethodCode
  if (sipCpp->getGraph()->isElement(*a0)) {
    sipCpp->setEdgeValue(*a0, a1);
  } else {
    sipIsErr = throwInvalidEdgeException(sipCpp->getGraph(), *a0);
  }
%End

  void __setitem__(const tlp::edge e, const std::string &val);
%MethodCode
  if (sipCpp->getGraph()->isElement(*a0)) {
    sipCpp->setEdgeValue(*a0, *a1);
  } else {
    sipIsErr = throwInvalidEdgeException(sipCpp->getGraph(), *a0);
  }
%End

  void __setitem__(const tlp::edge e, const tlp::PySize &val /Constrained/);
%MethodCode
  if (sipCpp->getGraph()->isElement(*a0)) {
    sipCpp->setEdgeValue(*a0, *a1);
  } else {
    sipIsErr = throwInvalidEdgeException(sipCpp->getGraph(), *a0);
  }
%End

  void __setitem__(const tlp::edge e, const std::vector<tlp::Coord> &val);
%MethodCode
  if (sipCpp->getGraph()->isElement(*a0)) {
    sipCpp->setEdgeValue(*a0, *a1);
  } else {
    sipIsErr = throwInvalidEdgeException(sipCpp->getGraph(), *a0);
  }
%End

  void __setitem__(const tlp::edge e, const tlp::Color &val);
%MethodCode
  if (sipCpp->getGraph()->isElement(*a0)) {
    sipCpp->setEdgeValue(*a0, *a1);
  } else {
    sipIsErr = throwInvalidEdgeException(sipCpp->getGraph(), *a0);
  }
%End

  void __setitem__(const tlp::edge e, const std::vector<bool> &val /Constrained/);
%MethodCode
  if (sipCpp->getGraph()->isElement(*a0)) {
    sipCpp->setEdgeValue(*a0, *a1);
  } else {
    sipIsErr = throwInvalidEdgeException(sipCpp->getGraph(), *a0);
  }
  delete a1;
%End

  void __setitem__(const tlp::edge e, const std::vector<int> &val /Constrained/);
%MethodCode
  if (sipCpp->getGraph()->isElement(*a0)) {
    sipCpp->setEdgeValue(*a0, *a1);
  } else {
    sipIsErr = throwInvalidEdgeException(sipCpp->getGraph(), *a0);
  }
  delete a1;
%End

  void __setitem__(const tlp::edge e, const std::vector<double> &val /Constrained/);
%MethodCode
  if (sipCpp->getGraph()->isElement(*a0)) {
    sipCpp->setEdgeValue(*a0, *a1);
  } else {
    sipIsErr = throwInvalidEdgeException(sipCpp->getGraph(), *a0);
  }
  delete a1;
%End

  void __setitem__(const tlp::edge e, const std::vector<std::string> &val /TypeHint="List[str]"/);
%MethodCode
  if (sipCpp->getGraph()->isElement(*a0)) {
    sipCpp->setEdgeValue(*a0, *a1);
  } else {
    sipIsErr = throwInvalidEdgeException(sipCpp->getGraph(), *a0);
  }
%End

  void __setitem__(const tlp::edge e, const std::vector<tlp::PySize> &val /Constrained, TypeHint="List[tlp.Size]"/);
%MethodCode
  if (sipCpp->getGraph()->isElement(*a0)) {
    sipCpp->setEdgeValue(*a0, *a1);
  } else {
    sipIsErr = throwInvalidEdgeException(sipCpp->getGraph(), *a0);
  }
  delete a1;
%End

  void __setitem__(const tlp::edge e, const std::vector<tlp::Color> &val);
%MethodCode
  if (sipCpp->getGraph()->isElement(*a0)) {
    sipCpp->setEdgeValue(*a0, *a1);
  } else {
    sipIsErr = throwInvalidEdgeException(sipCpp->getGraph(), *a0);
  }
%End

  void __setitem__(const tlp::edge e, SIP_PYOBJECT val);
%MethodCode
  std::string pythonType(a1->ob_type->tp_name);
  std::string errMsg = "Error : unable to create a graph property for Python type '" + pythonType + "'.";
  PyErr_SetString(PyExc_Exception, errMsg.c_str());
  sipIsErr = -1;
%End

  void setAllNodeValue(bool val /Constrained/);
%MethodCode
  sipCpp->setAllNodeValue(a0);
%End

  void setAllNodeValue(int val /Constrained/);
%MethodCode
  sipCpp->setAllNodeValue(a0);
%End

  void setAllNodeValue(double val /Constrained/);
%MethodCode
  sipCpp->setAllNodeValue(a0);
%End

  void setAllNodeValue(const std::string &val);
%MethodCode
  sipCpp->setAllNodeValue(*a0);
%End

  void setAllNodeValue(const tlp::PySize &val /Constrained/);
%MethodCode
  sipCpp->setAllNodeValue(*a0);
%End

  void setAllNodeValue(const tlp::Coord &val /GetWrapper/);
%MethodCode
  // when a tuple is provided as input, constrain its elements to float type
  // for creating a layout property otherwise create a color property
  const sipTypeDef *sipType = sipFindType("tlp::Color");
  if (sipCanConvertToType(a0Wrapper, sipType, SIP_NOT_NONE)) {
    int state=0, err=0;
    void *cppType = sipConvertToType(a0Wrapper, sipType, NULL, SIP_NOT_NONE, &state, &err);
    sipCpp->setAllNodeValue(*static_cast<tlp::Color*>(cppType));
    sipReleaseType(cppType, sipType, state);
  } else {
    sipCpp->setAllNodeValue(*a0);
  }
%End

  void setAllNodeValue(const tlp::Color &val);
%MethodCode
  sipCpp->setAllNodeValue(*a0);
%End

  void setAllNodeValue(const std::vector<bool> &val /Constrained/);
%MethodCode
  sipCpp->setAllNodeValue(*a0);
  delete a0;
%End

  void setAllNodeValue(const std::vector<int> &val /Constrained/);
%MethodCode
  sipCpp->setAllNodeValue(*a0);
  delete a0;
%End

  void setAllNodeValue(const std::vector<double> &val /Constrained/);
%MethodCode
  sipCpp->setAllNodeValue(*a0);
  delete a0;
%End

  void setAllNodeValue(const std::vector<std::string> &val /TypeHint="List[str]"/);
%MethodCode
  sipCpp->setAllNodeValue(*a0);
%End

  void setAllNodeValue(const std::vector<tlp::PySize> &val /Constrained, TypeHint="List[tlp.Size]"/);
%MethodCode
  sipCpp->setAllNodeValue(*a0);
  delete a0;
%End

  void setAllNodeValue(const std::vector<tlp::Coord> &val /GetWrapper/);
%MethodCode
  // when a list of tuples is provided as input, constrain tuples elements to float type
  // for creating a coord vector property otherwise create a color vector property
  const sipTypeDef *sipType = sipFindType("std::vector<tlp::Color>");
  if (sipCanConvertToType(a0Wrapper, sipType, SIP_NOT_NONE)) {
    int state=0, err=0;
    void *cppType = sipConvertToType(a0Wrapper, sipType, NULL, SIP_NOT_NONE, &state, &err);
    std::vector<tlp::Color> *vc = static_cast<std::vector<tlp::Color>*>(cppType);
    sipCpp->setAllNodeValue(*vc);
    sipReleaseType(cppType, sipType, state);
  } else {
    sipCpp->setAllNodeValue(*a0);
  }
%End

  void setAllNodeValue(const std::vector<tlp::Color> &val);
%MethodCode
  sipCpp->setAllNodeValue(*a0);
%End

void setAllNodeValue(SIP_PYOBJECT val);
%MethodCode
  std::string pythonType(a0->ob_type->tp_name);
  std::string errMsg = "Error : unable to create a graph property for Python type '" + pythonType + "'.";
  PyErr_SetString(PyExc_Exception, errMsg.c_str());
  sipIsErr = -1;
%End

  void setAllEdgeValue(bool val /Constrained/);
%MethodCode
  sipCpp->setAllEdgeValue(a0);
%End

  void setAllEdgeValue(int val /Constrained/);
%MethodCode
  sipCpp->setAllEdgeValue(a0);
%End

  void setAllEdgeValue(double val /Constrained/);
%MethodCode
  sipCpp->setAllEdgeValue(a0);
%End

  void setAllEdgeValue(const std::string &val);
%MethodCode
  sipCpp->setAllEdgeValue(*a0);
%End

  void setAllEdgeValue(const tlp::PySize &val /Constrained/);
%MethodCode
  sipCpp->setAllEdgeValue(*a0);
%End

  void setAllEdgeValue(const std::vector<tlp::Coord> &val);
%MethodCode
  sipCpp->setAllEdgeValue(*a0);
%End

  void setAllEdgeValue(const tlp::Color &val);
%MethodCode
  sipCpp->setAllEdgeValue(*a0);
%End

  void setAllEdgeValue(const std::vector<bool> &val /Constrained/);
%MethodCode
  sipCpp->setAllEdgeValue(*a0);
  delete a0;
%End

  void setAllEdgeValue(const std::vector<int> &val /Constrained/);
%MethodCode
  sipCpp->setAllEdgeValue(*a0);
  delete a0;
%End

  void setAllEdgeValue(const std::vector<double> &val /Constrained/);
%MethodCode
  sipCpp->setAllEdgeValue(*a0);
  delete a0;
%End

  void setAllEdgeValue(const std::vector<std::string> &val /TypeHint="List[str]"/);
%MethodCode
  sipCpp->setAllEdgeValue(*a0);
%End

  void setAllEdgeValue(const std::vector<tlp::PySize> &val /Constrained, TypeHint="List[tlp.Size]"/);
%MethodCode
  sipCpp->setAllEdgeValue(*a0);
  delete a0;
%End

  void setAllEdgeValue(const std::vector<tlp::Color> &val);
%MethodCode
  sipCpp->setAllEdgeValue(*a0);
%End

  void setAllEdgeValue(SIP_PYOBJECT val);
%MethodCode
  std::string pythonType(a0->ob_type->tp_name);
  std::string errMsg = "Error : unable to create a graph property for Python type '" + pythonType + "'.";
  PyErr_SetString(PyExc_Exception, errMsg.c_str());
  sipIsErr = -1;
%End

  SIP_PYOBJECT __getitem__(const tlp::node n /GetWrapper/);
%MethodCode
  if (sipCpp->getProperty()) {
    PyObject *pyProp = sipConvertFromType(sipCpp->getProperty(), sipFindType("tlp::PropertyInterface"), NULL);
  #if PY_MAJOR_VERSION >= 3
    PyObject *pName = PyUnicode_FromString("__getitem__");
  #else
    PyObject *pName = PyString_FromString("__getitem__");
  #endif
    sipRes = PyObject_CallMethodObjArgs(pyProp, pName, a0Wrapper, NULL);
    Py_XDECREF(pName);
    Py_XDECREF(pyProp);
  } else {
    PyErr_SetString(PyExc_Exception, "Error : Unable to get node data as no graph property has been created through that proxy.");
    sipIsErr = 1;
  }
%End

  SIP_PYOBJECT __getitem__(const tlp::edge e /GetWrapper/);
%MethodCode
  if (sipCpp->getProperty()) {
    PyObject *pyProp = sipConvertFromType(sipCpp->getProperty(), sipFindType("tlp::PropertyInterface"), NULL);
  #if PY_MAJOR_VERSION >= 3
    PyObject *pName = PyUnicode_FromString("__getitem__");
  #else
    PyObject *pName = PyString_FromString("__getitem__");
  #endif
    sipRes = PyObject_CallMethodObjArgs(pyProp, pName, a0Wrapper, NULL);
    Py_XDECREF(pName);
    Py_XDECREF(pyProp);
  } else {
    PyErr_SetString(PyExc_Exception, "Error : Unable to get edge data as no graph property has been created through that proxy.");
    sipIsErr = 1;
  }
%End

};

};
