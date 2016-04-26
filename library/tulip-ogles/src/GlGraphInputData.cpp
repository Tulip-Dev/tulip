/**
 *
 * This file is part of Tulip (www.tulip-software.org)
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

/**
 *
 * tulip-ogles library : rewrite of old tulip-ogl library using OpenGL ES API
 * Copyright (c) 2016 Antoine Lambert, Thales Services
 * antoine-e.lambert@thalesgroup.com / antoine.lambert33@gmail.com
 *
 */

#include <algorithm>

#include <tulip/GlGraphInputData.h>

using namespace tlp;

std::map<std::string, GlGraphInputData::ViewPropertyType> GlGraphInputData::_propertiesNameMap;

void GlGraphInputData::setGraph(tlp::Graph* graph) {

  if (_graph) {
    _graph->removeListener(this);
  }

  _graph = graph;

  reloadGraphProperties(true);

  if(_graph) {
    _graph->addListener(this);
  }

}

std::string GlGraphInputData::getViewPropertyName(ViewPropertyType viewPropertyType) {
  auto it = std::find_if (_propertiesNameMap.begin(), _propertiesNameMap.end(),
                          [&](const std::pair<std::string, ViewPropertyType> &mapEntry) {
                            return mapEntry.second == viewPropertyType;
                          });
  return it->first;
}

void GlGraphInputData::reloadGraphProperties(bool resetDefaulProperties) {

  if (_propertiesNameMap.empty() || resetDefaulProperties) {
    _propertiesNameMap["viewColor"]=VIEW_COLOR;
    _propertiesNameMap["viewLabelColor"]=VIEW_LABELCOLOR;
    _propertiesNameMap["viewSize"]=VIEW_SIZE;
    _propertiesNameMap["viewLabel"]=VIEW_LABEL;
    _propertiesNameMap["viewLabelPosition"]=VIEW_LABELPOSITION;
    _propertiesNameMap["viewShape"]=VIEW_SHAPE;
    _propertiesNameMap["viewRotation"]=VIEW_ROTATION;
    _propertiesNameMap["viewSelection"]=VIEW_SELECTION;
    _propertiesNameMap["viewFont"]=VIEW_FONT;
    _propertiesNameMap["viewFontSize"]=VIEW_FONTSIZE;
    _propertiesNameMap["viewTexture"]=VIEW_TEXTURE;
    _propertiesNameMap["viewBorderColor"]=VIEW_BORDERCOLOR;
    _propertiesNameMap["viewBorderWidth"]=VIEW_BORDERWIDTH;
    _propertiesNameMap["viewLayout"]=VIEW_LAYOUT;
    _propertiesNameMap["viewSrcAnchorShape"]=VIEW_SRCANCHORSHAPE;
    _propertiesNameMap["viewSrcAnchorSize"]=VIEW_SRCANCHORSIZE;
    _propertiesNameMap["viewTgtAnchorShape"]=VIEW_TGTANCHORSHAPE;
    _propertiesNameMap["viewTgtAnchorSize"]=VIEW_TGTANCHORSIZE;
    _propertiesNameMap["viewFontAwesomeIcon"]=VIEW_FONTAWESOMEICON;
    _propertiesNameMap["viewGlow"]= VIEW_GLOW;
    _propertiesNameMap["viewGlowColor"]= VIEW_GLOWCOLOR;
  }

  if (_graph) {
    _properties.clear();
    _propertiesMap[VIEW_COLOR]=_graph->getProperty<ColorProperty>(getViewPropertyName(VIEW_COLOR));
    _properties.insert(_propertiesMap[VIEW_COLOR]);
    _propertiesMap[VIEW_LABELCOLOR]=_graph->getProperty<ColorProperty>(getViewPropertyName(VIEW_LABELCOLOR));
    _properties.insert(_propertiesMap[VIEW_LABELCOLOR]);
    _propertiesMap[VIEW_SIZE]=_graph->getProperty<SizeProperty>(getViewPropertyName(VIEW_SIZE));
    _properties.insert(_propertiesMap[VIEW_SIZE]);
    _propertiesMap[VIEW_LABEL]=_graph->getProperty<StringProperty>(getViewPropertyName(VIEW_LABEL));
    _properties.insert(_propertiesMap[VIEW_LABEL]);
    _propertiesMap[VIEW_LABELPOSITION]=_graph->getProperty<IntegerProperty>(getViewPropertyName(VIEW_LABELPOSITION));
    _properties.insert(_propertiesMap[VIEW_LABELPOSITION]);
    _propertiesMap[VIEW_SHAPE]=_graph->getProperty<IntegerProperty>(getViewPropertyName(VIEW_SHAPE));
    _properties.insert(_propertiesMap[VIEW_SHAPE]);
    _propertiesMap[VIEW_ROTATION]=_graph->getProperty<DoubleProperty>(getViewPropertyName(VIEW_ROTATION));
    _properties.insert(_propertiesMap[VIEW_ROTATION]);
    _propertiesMap[VIEW_SELECTION]=_graph->getProperty<BooleanProperty>(getViewPropertyName(VIEW_SELECTION));
    _properties.insert(_propertiesMap[VIEW_SELECTION]);
    _propertiesMap[VIEW_FONT]=_graph->getProperty<StringProperty>(getViewPropertyName(VIEW_FONT));
    _properties.insert(_propertiesMap[VIEW_FONT]);
    _propertiesMap[VIEW_FONTSIZE]=_graph->getProperty<IntegerProperty>(getViewPropertyName(VIEW_FONTSIZE));
    _properties.insert(_propertiesMap[VIEW_FONTSIZE]);
    _propertiesMap[VIEW_TEXTURE]=_graph->getProperty<StringProperty>(getViewPropertyName(VIEW_TEXTURE));
    _properties.insert(_propertiesMap[VIEW_TEXTURE]);
    _propertiesMap[VIEW_BORDERCOLOR]=_graph->getProperty<ColorProperty>(getViewPropertyName(VIEW_BORDERCOLOR));
    _properties.insert(_propertiesMap[VIEW_BORDERCOLOR]);
    _propertiesMap[VIEW_BORDERWIDTH]=_graph->getProperty<DoubleProperty>(getViewPropertyName(VIEW_BORDERWIDTH));
    _properties.insert(_propertiesMap[VIEW_BORDERWIDTH]);
    _propertiesMap[VIEW_LAYOUT]=_graph->getProperty<LayoutProperty>(getViewPropertyName(VIEW_LAYOUT));
    _properties.insert(_propertiesMap[VIEW_LAYOUT]);
    _propertiesMap[VIEW_SRCANCHORSHAPE]=_graph->getProperty<IntegerProperty>(getViewPropertyName(VIEW_SRCANCHORSHAPE));
    _properties.insert(_propertiesMap[VIEW_SRCANCHORSHAPE]);
    _propertiesMap[VIEW_SRCANCHORSIZE]=_graph->getProperty<SizeProperty>(getViewPropertyName(VIEW_SRCANCHORSIZE));
    _properties.insert(_propertiesMap[VIEW_SRCANCHORSIZE]);
    _propertiesMap[VIEW_TGTANCHORSHAPE]=_graph->getProperty<IntegerProperty>(getViewPropertyName(VIEW_TGTANCHORSHAPE));
    _properties.insert(_propertiesMap[VIEW_TGTANCHORSHAPE]);
    _propertiesMap[VIEW_TGTANCHORSIZE]=_graph->getProperty<SizeProperty>(getViewPropertyName(VIEW_TGTANCHORSIZE));
    _properties.insert(_propertiesMap[VIEW_TGTANCHORSIZE]);
    _propertiesMap[VIEW_FONTAWESOMEICON]=_graph->getProperty<StringProperty>(getViewPropertyName(VIEW_FONTAWESOMEICON));
    _properties.insert(_propertiesMap[VIEW_FONTAWESOMEICON]);
    _propertiesMap[VIEW_GLOW]=_graph->getProperty<BooleanProperty>(getViewPropertyName(VIEW_GLOW));
    _properties.insert(_propertiesMap[VIEW_GLOW]);
    _propertiesMap[VIEW_GLOWCOLOR]=_graph->getProperty<ColorProperty>(getViewPropertyName(VIEW_GLOWCOLOR));
    _properties.insert(_propertiesMap[VIEW_GLOWCOLOR]);
    notifyModified();
  }
}

tlp::PropertyInterface* GlGraphInputData::getProperty(const std::string& name) const {
  std::map<std::string, ViewPropertyType>::iterator it = _propertiesNameMap.find(name);

  if (it != _propertiesNameMap.end())
    return _propertiesMap[it->second];

  return NULL;
}

bool GlGraphInputData::setProperty(const std::string& name, PropertyInterface *property) {
  std::map<std::string, ViewPropertyType>::iterator it = _propertiesNameMap.find(name);
  bool result = it != _propertiesNameMap.end();

  if (result)
    setProperty(it->second, property);

  return result;
}

void GlGraphInputData::setProperty(ViewPropertyType viewPropertyType, tlp::PropertyInterface *property) {
  _properties.erase(_propertiesMap[viewPropertyType]);
  _propertiesMap[viewPropertyType]=property;
  _properties.insert(property);
  if (!property->getName().empty()) {
    _propertiesNameMap[property->getName()] = viewPropertyType;
  }
  notifyModified();
}

bool GlGraphInputData::installProperties(const std::map<std::string, tlp::PropertyInterface*>& propsMap) {
  std::map<std::string, tlp::PropertyInterface*>::const_iterator pmIt = propsMap.begin();
  bool result = false;

  for(; pmIt != propsMap.end(); ++pmIt) {
    if (setProperty(pmIt->first, pmIt->second))
      result = true;
  }

  return result;
}

void GlGraphInputData::treatEvent(const Event &ev) {
  if (dynamic_cast<const GraphEvent*>(&ev) != NULL) {
    const GraphEvent* graphEv = static_cast<const GraphEvent*>(&ev);

    if (graphEv->getType() == GraphEvent::TLP_ADD_LOCAL_PROPERTY ||
        graphEv->getType() == GraphEvent::TLP_AFTER_DEL_LOCAL_PROPERTY ||
        graphEv->getType() == GraphEvent::TLP_ADD_INHERITED_PROPERTY ||
        graphEv->getType() == GraphEvent::TLP_AFTER_DEL_INHERITED_PROPERTY) {
      if(_propertiesNameMap.count(graphEv->getPropertyName())!=0) {
        PropertyInterface *oldProperty=_propertiesMap[_propertiesNameMap[graphEv->getPropertyName()]];
        _properties.erase(oldProperty);
        _propertiesMap[_propertiesNameMap[graphEv->getPropertyName()]]=_graph->getProperty(graphEv->getPropertyName());
        _properties.insert(_propertiesMap[_propertiesNameMap[graphEv->getPropertyName()]]);
      }
    }
  }
}


void GlGraphInputData::notifyModified() {
  sendEvent(Event(*this, Event::TLP_MODIFICATION));
}
