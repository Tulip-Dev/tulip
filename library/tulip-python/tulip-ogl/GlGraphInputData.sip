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
// | Tulip Python Bindings                               |
// | inspired for bindings by the Booggie project development team       |
// | (http://booggie.org/)                               |
// +-------------------------------------------------------------------------+


namespace tlp {

class GlGraphInputData /Abstract/ {
%TypeHeaderCode
#include <tulip/GlGraphInputData.h>
%End
public:

    GlGraphInputData(tlp::Graph* graph, tlp::GlGraphRenderingParameters* parameters);

    ~GlGraphInputData();

    /**
     * Return the graph of this inputData
     */

    tlp::Graph* getGraph();

    /**
     * Reload layoutProperty of the graph
     */
    void reloadLayoutProperty();

    /**
     * Return the layout of this inputData
     */
    tlp::LayoutProperty* getLayoutProperty();

    /**
     * Reload viewLabel Property of the graph
     */
    void reloadAllProperties();

    /**
     * Set property name for elementColor
     */
    void setElementColorPropName(const std::string &name);

    /**
     * Get property name for elementColor
     */
    std::string getElementColorPropName();

    /**
     * Set property name for elementLabelColor
     */
    void setElementLabelColorPropName(const std::string &name);

    /**
     * Get property name for elementLabelColor
     */
    std::string getElementLabelColorPropName();

    /**
     * Set property name for elementSize
     */
     void setElementSizePropName(const std::string &name);

     /**
      * Get property name for elementSize
      */
     std::string getElementSizePropName();

     /**
      * Set property name for elementLabelPosition
      */
     void setElementLabelPositionPropName(const std::string &name);

     /**
      * Get property name for elementLabelPosition
      */
     std::string getElementLabelPositionPropName();

     /**
      * Set property name for elementShape
      */
     void setElementShapePropName(const std::string &name);

     /**
      * Get property name for elementShape
      */
     std::string getElementShapePropName();

     /**
      * Set property name for elementRotation
      */
     void setElementRotationPropName(const std::string &name);

     /**
      * Get property name for elementRotation
      */
     std::string getElementRotationPropName();

     /**
      * Set property name for elementSelected
      */
     void setElementSelectedPropName(const std::string &name);

      /**
      * Get property name for elementSelected
      */
      std::string getElementSelectedPropName();

      /**
       * Set property name for elementFont
       */
      void setElementFontPropName(const std::string &name);

      /**
       * Get property name for elementFont
       */
      std::string getElementFontPropName();

      /**
       * Set property name for elementFontSize
       */
      void setElementFontSizePropName(const std::string &name);

      /**
       * Get property name for elementFontSize
       */
      std::string getElementFontSizePropName();

      /**
       * Set property name for elementLabel
       */
      void setElementLabelPropName(const std::string &name);

      /**
       * Get property name for elementLabel
       */
      std::string getElementLabelPropName();

      /**
       * Set property name for elementTexture
       */
      void setElementTexturePropName(const std::string &name);

      /**
       * Get property name for elementTexture
       */
      std::string getElementTexturePropName();

      /**
       * Set property name for elementBorderColor
       */
      void setElementBorderColorPropName(const std::string &name);

      /**
       * Get property name for elementBorderColor
       */
      std::string getElementBorderColorPropName();

      /**
       * Set property name for elementBorderWidth
       */
      void setElementBorderWidthPropName(const std::string &name);

      /**
       * Get property name for elementBorderWidth
       */
      std::string getElementBorderWidthPropName();

      /**
       * Set property name for elementLayout
       */
      void setElementLayoutPropName(const std::string &name);

      /**
       * Get property name for elementLayout
       */
      std::string getElementLayoutPropName();

      /**
       * Get property name for elementSourceAnchorShape
       */
      std::string getElementSourceAnchorShapePropName();

      /**
       * Set property name for elementSourceAnchorShape
       */
      void setElementSourceAnchorShapePropName(const std::string &name);

      /**
       * Get property name for elementSourceAnchorSize
       */
      std::string getElementSourceAnchorSizePropName();

      /**
       * Set property name for elementSourceAnchorSize
       */
      void setElementSourceAnchorSizePropName(const std::string& name);

      /**
       * Get property name for elementTargetAnchorShape
       */
      std::string getElementTargetAnchorShapePropName();

      /**
       * Set property name for elementTargetAnchorShape
       */
      void setElementTargetAnchorShapePropName(const std::string &name);

      /**
       * Get property name for elementTargetAnchorSize
       */
      std::string getElementTargetAnchorSizePropName();

      /**
       * Set property name for elementTargetAnchorSize
       */
      void setElementTargetAnchorSizePropName(const std::string& name);

      /**
       * Return a pointer on the property used to elementColor
       */
      tlp::ColorProperty * getElementColor();

      void setElementColor(tlp::ColorProperty *property);

      /**
       * Return a pointer on the property used to elementLabelColor
       */
      tlp::ColorProperty *getElementLabelColor();

      /**
       * Set the pointer on the property used to elementLabelColor
       */
      void setElementLabelColor(tlp::ColorProperty *property);

      /**
       * Return a pointer on the property used to elementSize
       */
      tlp::SizeProperty *getElementSize();

      /**
       * Set the pointer on the property used to elementSize
       */
      void setElementSize(tlp::SizeProperty *property);

      /**
       * Return a pointer on the property used to elementLabelPosition
       */
      tlp::IntegerProperty *getElementLabelPosition();

      /**
       * Set the pointer on the property used to elementLabelPosition
       */
      void setElementLabelPosition(tlp::IntegerProperty *property);

      /**
       * Return a pointer on the property used to elementShape
       */
      tlp::IntegerProperty *getElementShape();

      /**
       * Set the pointer on the property used to elementShape
       */
      void setElementShape(tlp::IntegerProperty *property);

      /**
       * Return a pointer on the property used to elementRotation
       */
      tlp::DoubleProperty *getElementRotation();

      /**
       * Set the pointer on the property used to elementRotation
       */
      void setElementRotation(tlp::DoubleProperty *property);

      /**
       * Return a pointer on the property used to elementSelected
       */
      tlp::BooleanProperty *getElementSelected();

      /**
       * Set the pointer on the property used to elementSelected
       */
      void setElementSelected(tlp::BooleanProperty *property);

      /**
       * Return a pointer on the property used to elementFont
       */
      tlp::StringProperty *getElementFont();

      /**
       * Set the pointer on the property used to elementFont
       */
      void setElementFont(tlp::StringProperty *property);

      /**
       * Return a pointer on the property used to elementFontSize
       */
      tlp::IntegerProperty  *getElementFontSize();

      /**
       * Set the pointer on the property used to elementFontSize
       */
      void setElementFontSize(tlp::IntegerProperty *property);

      /**
       * Return a pointer on the property used to elementLabel
       */
      tlp::StringProperty *getElementLabel();

      /**
       * Set the pointer on the property used to elementLabel
       */
      void setElementLabel(tlp::StringProperty *property);

      /**
       * Return a pointer on the property used to elementLayout
       */
      tlp::LayoutProperty *getElementLayout();

      /**
       * Set the pointer on the property used to elementLayout
       */
      void setElementLayout(tlp::LayoutProperty *property);

      /**
       * Return a pointer on the property used to elementTexture
       */
      tlp::StringProperty *getElementTexture();

      /**
       * Set the pointer on the property used to elementTexture
       */
      void setElementTexture(tlp::StringProperty *property);

      /**
       * Return a pointer on the property used to elementBorderColor
       */
      tlp::ColorProperty *getElementBorderColor();

      /**
       * Set the pointer on the property used to elementBorderColor
       */
      void setElementBorderColor(tlp::ColorProperty *property);

      /**
       * Return a pointer on the property used to elementBorderWidth
       */
      tlp::DoubleProperty *getElementBorderWidth();

      /**
       * Set the pointer on the property used to elementBorderWidth
       */
      void setElementBorderWidth(tlp::DoubleProperty *property);

      /**
       * Return a pointer on the property used to elementSrcAnchorShape
       */
      tlp::IntegerProperty *getElementSrcAnchorShape();

      /**
       * Set the pointer on the property used to elementSrcAnchorShape
       */
      void setElementSrcAnchorShape(tlp::IntegerProperty *property);

      /**
       * Return a pointer on the property used to elementSrcAnchorSize
       */
      tlp::SizeProperty *getElementSrcAnchorSize();

      /**
       * Set the pointer on the property used to elementSrcAnchorSize
       */
      void setElementSrcAnchorSize(tlp::SizeProperty *property);

      /**
       * Return a pointer on the property used to elementTgtAnchorShape
       */
      tlp::IntegerProperty *getElementTgtAnchorShape();

      /**
       * Set the pointer on the property used to elementTgtAnchorShape
       */
      void setElementTgtAnchorShape(tlp::IntegerProperty *property);

      /**
       * Return a pointer on the property used to elementTgtAnchorSize
       */
      tlp::SizeProperty *getElementTgtAnchorSize();

      /**
       * Set the property name for elementSourceAnchorSize
       */
      void setElementTgtAnchorSize(tlp::SizeProperty *property);

      /**
       * Return a pointer on the property used to elementAnimationFrame
       */
      tlp::IntegerProperty *getElementAnimationFrame();

      /**
       * Set the pointer on the property used to elementAnimationFrame
       */
      void setElementAnimationFrame(tlp::IntegerProperty *property);

      tlp::ColorProperty *elementColor;
      tlp::ColorProperty *elementLabelColor;
      tlp::SizeProperty *elementSize;
      tlp::IntegerProperty *elementLabelPosition;
      tlp::IntegerProperty *elementShape;
      tlp::DoubleProperty *elementRotation;
      tlp::BooleanProperty *elementSelected;
      tlp::StringProperty   *elementFont;
      tlp::IntegerProperty  *elementFontSize;
      tlp::StringProperty *elementLabel;
      tlp::LayoutProperty *elementLayout;
      tlp::StringProperty *elementTexture;
      tlp::ColorProperty *elementBorderColor;
      tlp::DoubleProperty *elementBorderWidth;
      tlp::IntegerProperty *elementSrcAnchorShape;
      tlp::SizeProperty *elementSrcAnchorSize;
      tlp::IntegerProperty *elementTgtAnchorShape;
      tlp::SizeProperty *elementTgtAnchorSize;
      tlp::IntegerProperty *elementAnimationFrame;

      tlp::Graph* graph;

      tlp::GlGraphRenderingParameters* parameters;
};
};

