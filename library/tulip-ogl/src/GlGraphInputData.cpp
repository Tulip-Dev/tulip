/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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
#include <tulip/GlGraphInputData.h>
#include <tulip/Graph.h>
#include <tulip/GlyphManager.h>
#include <tulip/EdgeExtremityGlyphManager.h>
#include <tulip/GlVertexArrayManager.h>
#include <tulip/GlMetaNodeRenderer.h>
#include <tulip/GlGlyphRenderer.h>

namespace tlp {
GlGraphInputData::GlGraphInputData(Graph *graph, GlGraphRenderingParameters *parameters,
                                   GlMetaNodeRenderer *renderer)
    : graph(graph), parameters(parameters) {

  reloadGraphProperties();

  if (graph) {
    graph->addListener(this);
  }

  GlyphManager::initGlyphList(&this->graph, this, glyphs);

  EdgeExtremityGlyphManager::initGlyphList(&this->graph, this, extremityGlyphs);

  if (renderer)
    _metaNodeRenderer = renderer;
  else
    _metaNodeRenderer = new GlMetaNodeRenderer(this);

  _glVertexArrayManager = new GlVertexArrayManager(this);
  _glGlyphRenderer = new GlGlyphRenderer(this);
}

void GlGraphInputData::setMetaNodeRenderer(GlMetaNodeRenderer *renderer,
                                           bool deleteOldMetaNodeRenderer) {
  if (deleteOldMetaNodeRenderer)
    delete _metaNodeRenderer;

  _metaNodeRenderer = renderer;
}

GlGraphInputData::~GlGraphInputData() {
  delete _glVertexArrayManager;
  GlyphManager::clearGlyphList(&this->graph, this, glyphs);
  EdgeExtremityGlyphManager::clearGlyphList(&this->graph, this, extremityGlyphs);
  delete _metaNodeRenderer;
  delete _glGlyphRenderer;
  // delete invisible properties
  for (auto prop : _invisibleProperties)
    delete prop;
}

// add this class to ensure proper deletion of the viewAnimationFrame property
// as the property is referenced in other structure as View::_triggers
// it must remain alive until graph destruction
class GlViewAnimationFrameProperty : public IntegerProperty {
public:
  GlViewAnimationFrameProperty(Graph *g) : IntegerProperty(g, "viewAnimationFrame") {
    needGraphListener = true;
    graph->addListener(this);
  }
  using IntegerProperty::operator=;
  void treatEvent(const Event &evt) override {
    Graph *g = static_cast<Graph *>(evt.sender());

    if (graph == g && evt.type() == Event::TLP_DELETE) {
      delete this;
    } else {
      needGraphListener = true;
      IntegerMinMaxProperty::treatEvent(evt);
    }
  }
};

tlp_hash_map<std::string, GlGraphInputData::PropertyName> GlGraphInputData::_propertiesNameMap;

void GlGraphInputData::reloadGraphProperties() {
  if (_propertiesNameMap.empty()) {
    _propertiesNameMap["viewColor"] = VIEW_COLOR;
    _propertiesNameMap["viewLabelColor"] = VIEW_LABELCOLOR;
    _propertiesNameMap["viewLabelBorderColor"] = VIEW_LABELBORDERCOLOR;
    _propertiesNameMap["viewLabelBorderWidth"] = VIEW_LABELBORDERWIDTH;
    _propertiesNameMap["viewSize"] = VIEW_SIZE;
    _propertiesNameMap["viewLabel"] = VIEW_LABEL;
    _propertiesNameMap["viewLabelPosition"] = VIEW_LABELPOSITION;
    _propertiesNameMap["viewShape"] = VIEW_SHAPE;
    _propertiesNameMap["viewRotation"] = VIEW_ROTATION;
    _propertiesNameMap["viewSelection"] = VIEW_SELECTED;
    _propertiesNameMap["viewFont"] = VIEW_FONT;
    _propertiesNameMap["viewFontSize"] = VIEW_FONTSIZE;
    _propertiesNameMap["viewTexture"] = VIEW_TEXTURE;
    _propertiesNameMap["viewBorderColor"] = VIEW_BORDERCOLOR;
    _propertiesNameMap["viewBorderWidth"] = VIEW_BORDERWIDTH;
    _propertiesNameMap["viewLayout"] = VIEW_LAYOUT;
    _propertiesNameMap["viewSrcAnchorShape"] = VIEW_SRCANCHORSHAPE;
    _propertiesNameMap["viewSrcAnchorSize"] = VIEW_SRCANCHORSIZE;
    _propertiesNameMap["viewTgtAnchorShape"] = VIEW_TGTANCHORSHAPE;
    _propertiesNameMap["viewTgtAnchorSize"] = VIEW_TGTANCHORSIZE;
    _propertiesNameMap["viewAnimationFrame"] = VIEW_ANIMATIONFRAME;
    _propertiesNameMap["viewIcon"] = VIEW_ICON;
    _propertiesNameMap["viewLengthRatio"] = VIEW_LENGTHRATIO;
  }

  if (graph) {
    _properties.clear();
    _propertiesMap[VIEW_COLOR] = graph->getProperty<ColorProperty>("viewColor");
    _properties.insert(_propertiesMap[VIEW_COLOR]);
    _propertiesMap[VIEW_LABELCOLOR] = graph->getProperty<ColorProperty>("viewLabelColor");
    _properties.insert(_propertiesMap[VIEW_LABELCOLOR]);
    _propertiesMap[VIEW_LABELBORDERCOLOR] =
        graph->getProperty<ColorProperty>("viewLabelBorderColor");
    _properties.insert(_propertiesMap[VIEW_LABELBORDERCOLOR]);
    _propertiesMap[VIEW_LABELBORDERWIDTH] =
        graph->getProperty<DoubleProperty>("viewLabelBorderWidth");
    _properties.insert(_propertiesMap[VIEW_LABELBORDERWIDTH]);
    _propertiesMap[VIEW_SIZE] = graph->getProperty<SizeProperty>("viewSize");
    _properties.insert(_propertiesMap[VIEW_SIZE]);
    _propertiesMap[VIEW_LABEL] = graph->getProperty<StringProperty>("viewLabel");
    _properties.insert(_propertiesMap[VIEW_LABEL]);
    _propertiesMap[VIEW_LABELPOSITION] = graph->getProperty<IntegerProperty>("viewLabelPosition");
    _properties.insert(_propertiesMap[VIEW_LABELPOSITION]);
    _propertiesMap[VIEW_SHAPE] = graph->getProperty<IntegerProperty>("viewShape");
    _properties.insert(_propertiesMap[VIEW_SHAPE]);
    _propertiesMap[VIEW_ROTATION] = graph->getProperty<DoubleProperty>("viewRotation");
    _properties.insert(_propertiesMap[VIEW_ROTATION]);
    _propertiesMap[VIEW_SELECTED] = graph->getProperty<BooleanProperty>("viewSelection");
    _properties.insert(_propertiesMap[VIEW_SELECTED]);
    _propertiesMap[VIEW_FONT] = graph->getProperty<StringProperty>("viewFont");
    _properties.insert(_propertiesMap[VIEW_FONT]);
    _propertiesMap[VIEW_FONTSIZE] = graph->getProperty<IntegerProperty>("viewFontSize");
    _properties.insert(_propertiesMap[VIEW_FONTSIZE]);
    _propertiesMap[VIEW_TEXTURE] = graph->getProperty<StringProperty>("viewTexture");
    _properties.insert(_propertiesMap[VIEW_TEXTURE]);
    _propertiesMap[VIEW_BORDERCOLOR] = graph->getProperty<ColorProperty>("viewBorderColor");
    _properties.insert(_propertiesMap[VIEW_BORDERCOLOR]);
    _propertiesMap[VIEW_BORDERWIDTH] = graph->getProperty<DoubleProperty>("viewBorderWidth");
    _properties.insert(_propertiesMap[VIEW_BORDERWIDTH]);
    _propertiesMap[VIEW_LAYOUT] = graph->getProperty<LayoutProperty>("viewLayout");
    _properties.insert(_propertiesMap[VIEW_LAYOUT]);
    _propertiesMap[VIEW_SRCANCHORSHAPE] = graph->getProperty<IntegerProperty>("viewSrcAnchorShape");
    _properties.insert(_propertiesMap[VIEW_SRCANCHORSHAPE]);
    _propertiesMap[VIEW_SRCANCHORSIZE] = graph->getProperty<SizeProperty>("viewSrcAnchorSize");
    _properties.insert(_propertiesMap[VIEW_SRCANCHORSIZE]);
    _propertiesMap[VIEW_TGTANCHORSHAPE] = graph->getProperty<IntegerProperty>("viewTgtAnchorShape");
    _properties.insert(_propertiesMap[VIEW_TGTANCHORSHAPE]);
    _propertiesMap[VIEW_TGTANCHORSIZE] = graph->getProperty<SizeProperty>("viewTgtAnchorSize");
    _properties.insert(_propertiesMap[VIEW_TGTANCHORSIZE]);
    _propertiesMap[VIEW_ANIMATIONFRAME] = new GlViewAnimationFrameProperty(graph);
    _properties.insert(_propertiesMap[VIEW_ANIMATIONFRAME]);
    _propertiesMap[VIEW_ICON] = graph->getProperty<StringProperty>("viewIcon");
    _properties.insert(_propertiesMap[VIEW_ICON]);
    if (graph->existProperty("viewLengthRatio"))
      _propertiesMap[VIEW_LENGTHRATIO] = graph->getProperty<DoubleProperty>("viewLengthRatio");
    else {
      // use an invisible property
      auto prop = new DoubleProperty(graph, "viewLengthRatio");
      _propertiesMap[VIEW_LENGTHRATIO] = prop;
      _invisibleProperties.insert(prop);
    }
    _properties.insert(_propertiesMap[VIEW_LENGTHRATIO]);
  }
}

bool GlGraphInputData::setProperty(const std::string &name, PropertyInterface *property) {
  auto it = _propertiesNameMap.find(name);
  bool result = it != _propertiesNameMap.end();

  if (result)
    setProperty(it->second, property);

  return result;
}

bool GlGraphInputData::installProperties(
    const std::map<std::string, tlp::PropertyInterface *> &propsMap) {
  auto pmIt = propsMap.begin();
  bool result = false;

  for (; pmIt != propsMap.end(); ++pmIt) {
    if (setProperty(pmIt->first, pmIt->second))
      result = true;
  }

  if (result)
    getGlVertexArrayManager()->setHaveToComputeAll(true);

  return result;
}

void GlGraphInputData::treatEvent(const Event &ev) {
  Graph *g = static_cast<Graph *>(ev.sender());

  if (ev.type() == Event::TLP_DELETE && g == graph) {
    // avoid possible crash if graph is deleted before this
    // so ensure invisible properties are removed and deleted
    // before destructor call
    for (auto prop : _invisibleProperties)
      delete prop;
    _invisibleProperties.clear();
  }

  if (dynamic_cast<const GraphEvent *>(&ev) != nullptr) {
    const GraphEvent *graphEv = static_cast<const GraphEvent *>(&ev);

    if (graphEv->getType() == GraphEvent::TLP_ADD_LOCAL_PROPERTY ||
        graphEv->getType() == GraphEvent::TLP_AFTER_DEL_LOCAL_PROPERTY ||
        graphEv->getType() == GraphEvent::TLP_ADD_INHERITED_PROPERTY ||
        graphEv->getType() == GraphEvent::TLP_AFTER_DEL_INHERITED_PROPERTY) {
      if (_propertiesNameMap.count(graphEv->getPropertyName()) != 0) {
        auto propName = graphEv->getPropertyName();
        PropertyInterface *oldProperty = _propertiesMap[_propertiesNameMap[propName]];
        _properties.erase(oldProperty);
        if (_invisibleProperties.count(oldProperty)) {
          delete oldProperty;
          _invisibleProperties.erase(oldProperty);
        }
        if ((propName == "viewLengthRatio") && !graph->existProperty(propName)) {
          // use an invisible property
          auto prop = new DoubleProperty(graph, "viewLengthRatio");
          _propertiesMap[VIEW_LENGTHRATIO] = prop;
          _invisibleProperties.insert(prop);
        } else
          _propertiesMap[_propertiesNameMap[propName]] = graph->getProperty(propName);
        _properties.insert(_propertiesMap[_propertiesNameMap[propName]]);
      }
    }
  }
}
} // namespace tlp
