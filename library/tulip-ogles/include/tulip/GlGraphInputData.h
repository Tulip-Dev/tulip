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
 * Copyright (c) 2016 Antoine Lambert, Thales Services SAS
 * antoine-e.lambert@thalesgroup.com / antoine.lambert33@gmail.com
 *
 */

#ifndef GLGRAPHINPUTDATA_H
#define GLGRAPHINPUTDATA_H

#include <tulip/Observable.h>
#include <tulip/LayoutProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/StringProperty.h>
#include <tulip/BooleanProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/ColorProperty.h>

namespace tlp {

/**
 * Class used to store the input rendering data of a graph
 */
class TLP_GLES_SCOPE GlGraphInputData : public Observable {

  friend class GlGraph;

public:

  /**
   * GlGraphInputData available properties
   */
  enum ViewPropertyType {
    VIEW_COLOR=0, /**< color of nodes/edges */
    VIEW_LABELCOLOR, /**< color of labels */
    VIEW_SIZE, /**< size of nodes/edges */
    VIEW_LABELPOSITION, /**< position of labels */
    VIEW_SHAPE, /**< shape of nodes/edges */
    VIEW_ROTATION, /**< rotation apply on nodes */
    VIEW_SELECTION, /**< nodes/edges selected */
    VIEW_FONT, /**< font name of labels */
    VIEW_FONTSIZE, /**< font size of labels */
    VIEW_LABEL, /**< text of labels */
    VIEW_LAYOUT, /**< position of nodes */
    VIEW_TEXTURE, /**< texture of nodes/edges */
    VIEW_BORDERCOLOR, /**< border color of nodes/edges */
    VIEW_BORDERWIDTH, /**< border width of nodes/edges */
    VIEW_SRCANCHORSHAPE, /**< shape of source arrow edge extremity */
    VIEW_SRCANCHORSIZE, /**< size of source arrow edge extremity */
    VIEW_TGTANCHORSHAPE, /**< shape of target arrow edge extremity */
    VIEW_TGTANCHORSIZE, /**< size of target arrow edge extremity */
    VIEW_FONTAWESOMEICON, /**< font awesome icon name for the font awesome icon glyph*/
    VIEW_GLOW, /**< draw a glow around graph elements ?*/
    VIEW_GLOWCOLOR, /**< color of glow effect */
    VIEW_MATERIALDESIGNICON, /**< material design icon name for the material design icon glyph*/
    NB_PROPS /** must be the last, give the number of enum props */
  };

  /**
   * Return the graph of this inputData
   */
  tlp::Graph* getGraph() const {
    return _graph;
  }

  void treatEvent(const tlp::Event &ev);

  /**
   * Function to get the PropertyInterface* corresponding
   * to a given name
   */
  tlp::PropertyInterface* getProperty(const std::string& name) const;

  /**
   * Function to get the typed PropertyInterface* for a given ViewPropertyType
   * See ViewPropertyType enum for more details on available properties
   */
  template<typename T>
  T* getProperty(ViewPropertyType ViewPropertyType) const {
    return static_cast<T*>(_propertiesMap[ViewPropertyType]);
  }

  /**
   * Function to set the PropertyInterface* for a given ViewPropertyType
   * See ViewPropertyType enum for more details on available properties
   */
  void setProperty(ViewPropertyType ViewPropertyType, tlp::PropertyInterface *property);

  /**
   * Function to set the PropertyInterface* for a given name
   */
  bool setProperty(const std::string& name, tlp::PropertyInterface *property);

  /**
   * Function to set a bunch of named PropertyInterface*
   */
  bool installProperties(const std::map<std::string, tlp::PropertyInterface*>& propsMap);

  /**
   * Return a pointer on the property used to elementColor
   */
  tlp::ColorProperty * getElementColor() const {
    return getProperty<tlp::ColorProperty>(VIEW_COLOR);
  }
  /**
   * Set the pointer on the property used to elementColor
   */
  void setElementColor(tlp::ColorProperty *property) {
    setProperty(VIEW_COLOR,property);
  }
  /**
   * Return a pointer on the property used to elementLabelColor
   */
  tlp::ColorProperty *getElementLabelColor() const {
    return getProperty<tlp::ColorProperty>(VIEW_LABELCOLOR);
  }
  /**
   * Set the pointer on the property used to elementLabelColor
   */
  void setElementLabelColor(tlp::ColorProperty *property) {
    setProperty(VIEW_LABELCOLOR,property);
  }
  /**
   * Return a pointer on the property used to elementSize
   */
  tlp::SizeProperty *getElementSize() const {
    return getProperty<tlp::SizeProperty>(VIEW_SIZE);
  }
  /**
   * Set the pointer on the property used to elementSize
   */
  void setElementSize(tlp::SizeProperty *property) {
    setProperty(VIEW_SIZE,property);
  }
  /**
   * Return a pointer on the property used to elementLabelPosition
   */
  tlp::IntegerProperty *getElementLabelPosition() const {
    return getProperty<tlp::IntegerProperty>(VIEW_LABELPOSITION);
  }
  /**
   * Set the pointer on the property used to elementLabelPosition
   */
  void setElementLabelPosition(tlp::IntegerProperty *property) {
    setProperty(VIEW_LABELPOSITION,property);
  }
  /**
   * Return a pointer on the property used to elementShape
   */
  tlp::IntegerProperty *getElementShape() const {
    return getProperty<tlp::IntegerProperty>(VIEW_SHAPE);
  }
  /**
   * Set the pointer on the property used to elementShape
   */
  void setElementShape(tlp::IntegerProperty *property) {
    setProperty(VIEW_SHAPE,property);
  }
  /**
   * Return a pointer on the property used to elementRotation
   */
  tlp::DoubleProperty *getElementRotation() const {
    return getProperty<tlp::DoubleProperty>(VIEW_ROTATION);
  }
  /**
   * Set the pointer on the property used to elementRotation
   */
  void setElementRotation(tlp::DoubleProperty *property) {
    setProperty(VIEW_ROTATION,property);
  }
  /**
   * Return a pointer on the property used to elementSelected
   */
  tlp::BooleanProperty *getElementSelection() const {
    return getProperty<tlp::BooleanProperty>(VIEW_SELECTION);
  }
  /**
   * Set the pointer on the property used to elementSelected
   */
  void setElementSelection(tlp::BooleanProperty *property) {
    setProperty(VIEW_SELECTION,property);
  }
  /**
   * Return a pointer on the property used to elementFont
   */
  tlp::StringProperty *getElementFont() const {
    return getProperty<tlp::StringProperty>(VIEW_FONT);
  }
  /**
   * Set the pointer on the property used to elementFont
   */
  void setElementFont(tlp::StringProperty *property) {
    setProperty(VIEW_FONT, property);
  }
  /**
   * Return a pointer on the property used to elementFontSize
   */
  tlp::IntegerProperty *getElementFontSize() const {
    return getProperty<tlp::IntegerProperty>(VIEW_FONTSIZE);
  }
  /**
   * Set the pointer on the property used to elementFontSize
   */
  void setElementFontSize(tlp::IntegerProperty *property) {
    setProperty(VIEW_FONTSIZE, property);
  }
  /**
   * Return a pointer on the property used to elementLabel
   */
  tlp::StringProperty *getElementLabel() const {
    return getProperty<tlp::StringProperty>(VIEW_LABEL);
  }
  /**
   * Set the pointer on the property used to elementLabel
   */
  void setElementLabel(tlp::StringProperty *property) {
    setProperty(VIEW_LABEL,property);
  }
  /**
   * Return a pointer on the property used to elementLayout
   */
  tlp::LayoutProperty *getElementLayout() const {
    return getProperty<tlp::LayoutProperty>(VIEW_LAYOUT);
  }
  /**
   * Set the pointer on the property used to elementLayout
   */
  void setElementLayout(tlp::LayoutProperty *property) {
    setProperty(VIEW_LAYOUT,property);
  }
  /**
   * Return a pointer on the property used to elementTexture
   */
  tlp::StringProperty *getElementTexture() const {
    return getProperty<tlp::StringProperty>(VIEW_TEXTURE);
  }
  /**
   * Set the pointer on the property used to elementTexture
   */
  void setElementTexture(tlp::StringProperty *property) {
    setProperty(VIEW_TEXTURE,property);
  }
  /**
   * Return a pointer on the property used to elementBorderColor
   */
  tlp::ColorProperty *getElementBorderColor() const {
    return getProperty<tlp::ColorProperty>(VIEW_BORDERCOLOR);
  }
  /**
   * Set the pointer on the property used to elementBorderColor
   */
  void setElementBorderColor(tlp::ColorProperty *property) {
    setProperty(VIEW_BORDERCOLOR,property);
  }
  /**
   * Return a pointer on the property used to elementBorderWidth
   */
  tlp::DoubleProperty *getElementBorderWidth() const {
    return getProperty<tlp::DoubleProperty>(VIEW_BORDERWIDTH);
  }
  /**
   * Set the pointer on the property used to elementBorderWidth
   */
  void setElementBorderWidth(tlp::DoubleProperty *property) {
    setProperty(VIEW_BORDERWIDTH,property);
  }
  /**
   * Return a pointer on the property used to elementSrcAnchorShape
   */
  tlp::IntegerProperty *getElementSrcAnchorShape() const {
    return getProperty<tlp::IntegerProperty>(VIEW_SRCANCHORSHAPE);
  }
  /**
   * Set the pointer on the property used to elementSrcAnchorShape
   */
  void setElementSrcAnchorShape(tlp::IntegerProperty *property) {
    setProperty(VIEW_SRCANCHORSHAPE,property);
  }
  /**
   * Return a pointer on the property used to elementSrcAnchorSize
   */
  tlp::SizeProperty *getElementSrcAnchorSize() const {
    return getProperty<tlp::SizeProperty>(VIEW_SRCANCHORSIZE);
  }
  /**
   * Set the pointer on the property used to elementSrcAnchorSize
   */
  void setElementSrcAnchorSize(tlp::SizeProperty *property) {
    setProperty(VIEW_SRCANCHORSIZE,property);
  }
  /**
   * Return a pointer on the property used to elementTgtAnchorShape
   */
  tlp::IntegerProperty *getElementTgtAnchorShape() const {
    return getProperty<tlp::IntegerProperty>(VIEW_TGTANCHORSHAPE);
  }
  /**
   * Set the pointer on the property used to elementTgtAnchorShape
   */
  void setElementTgtAnchorShape(tlp::IntegerProperty *property) {
    setProperty(VIEW_TGTANCHORSHAPE,property);
  }
  /**
   * Return a pointer on the property used to elementTgtAnchorSize
   */
  tlp::SizeProperty *getElementTgtAnchorSize() const {
    return getProperty<tlp::SizeProperty>(VIEW_TGTANCHORSIZE);
  }
  /**
   * Set the property name for elementSourceAnchorSize
   */
  void setElementTgtAnchorSize(tlp::SizeProperty *property) {
    setProperty(VIEW_TGTANCHORSIZE,property);
  }
  /**
   * Return a pointer on the property used to elementFontAwesomeIcon
   */
  tlp::StringProperty *getElementFontAwesomeIcon() const {
    return getProperty<tlp::StringProperty>(VIEW_FONTAWESOMEICON);
  }
  /**
   * Set the pointer on the property used to elementFontAwesomeIcon
   */
  void setElementFontAwesomeIcon(tlp::StringProperty *property) {
    setProperty(VIEW_FONTAWESOMEICON,property);
  }

  /**
   * Return a pointer on the property used to elementMaterialDesignIcon
   */
  tlp::StringProperty *getElementMaterialDesignIcon() const {
    return getProperty<tlp::StringProperty>(VIEW_MATERIALDESIGNICON);
  }
  /**
   * Set the pointer on the property used to elementMaterialDesignIcon
   */
  void setElementMaterialDesignIcon(tlp::StringProperty *property) {
    setProperty(VIEW_MATERIALDESIGNICON,property);
  }

  /**
   * Return a pointer on the property used to elementGlow
   */
  tlp::BooleanProperty *getElementGlow() const {
    return getProperty<tlp::BooleanProperty>(VIEW_GLOW);
  }
  /**
   * Set the pointer on the property used to elementGlow
   */
  void setElementGlow(tlp::BooleanProperty *property) {
    setProperty(VIEW_GLOW,property);
  }

  /**
   * Return a pointer on the property used to elementGlowColor
   */
  tlp::ColorProperty *getElementGlowColor() const {
    return getProperty<tlp::ColorProperty>(VIEW_GLOWCOLOR);
  }
  /**
   * Set the pointer on the property used to elementGlow
   */
  void setElementGlowColor(tlp::ColorProperty *property) {
    setProperty(VIEW_GLOWCOLOR,property);
  }

  const std::set<tlp::PropertyInterface*> &getProperties() const {
    return _properties;
  }

  /**
   * @brief reloadGraphProperties restore the properties of the GlGraphInputData from the the graph.
   */
  void reloadGraphProperties(bool resetDefaulProperties = false);

private:

  GlGraphInputData() : _graph(nullptr) {}

  std::string getViewPropertyName(ViewPropertyType viewPropertyType);

  void setGraph(tlp::Graph* graph);

  void notifyModified();

  tlp::Graph *_graph;

  std::set<tlp::PropertyInterface*> _properties;

  tlp::PropertyInterface* _propertiesMap[NB_PROPS];
  static std::map<std::string,ViewPropertyType> _propertiesNameMap;

};

}

#endif
