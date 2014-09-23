/*
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

#ifndef PROPERTYALGORITHM_H
#define PROPERTYALGORITHM_H

#include <tulip/Algorithm.h>
#include <tulip/TemplateAlgorithm.h>

namespace tlp {
class PluginContext;

class BooleanProperty;
static const std::string BOOLEAN_ALGORITHM_CATEGORY = "Selection";

/**
 * @ingroup Plugins
 * @brief The boolean algorithm takes a graph as input and output its results as a tlp::BooleanProperty
 */
class TLP_SCOPE BooleanAlgorithm : public TemplateAlgorithm<tlp::BooleanProperty> {
protected:
  BooleanAlgorithm (const tlp::PluginContext*);

public:
  virtual std::string category() const {
    return BOOLEAN_ALGORITHM_CATEGORY;
  }
};

class ColorProperty;
static const std::string COLOR_ALGORITHM_CATEGORY = "Coloring";

/**
 * @ingroup Plugins
 * @brief The color algorithm takes a graph as input and output its results as a tlp::ColorProperty
 */
class TLP_SCOPE ColorAlgorithm : public TemplateAlgorithm<tlp::ColorProperty> {
protected:
  ColorAlgorithm (const tlp::PluginContext*);
public:
  virtual std::string category() const {
    return COLOR_ALGORITHM_CATEGORY;
  }
};

class DoubleProperty;
static const std::string DOUBLE_ALGORITHM_CATEGORY = "Measure";

/**
 * @ingroup Plugins
 * @brief The double algorithm takes a graph as input and output its results as a tlp::DoubleProperty
 */
class TLP_SCOPE DoubleAlgorithm : public TemplateAlgorithm<tlp::DoubleProperty> {
protected:
  ///
  DoubleAlgorithm (const tlp::PluginContext*);
public:
  virtual std::string category() const {
    return DOUBLE_ALGORITHM_CATEGORY;
  }
};

class IntegerProperty;
static const std::string INTEGER_ALGORITHM_CATEGORY = "Measure";

/**
 * @ingroup Plugins
 * @brief The integer algorithm takes a graph as input and output its results as a tlp::IntegerProperty
 */
class TLP_SCOPE IntegerAlgorithm : public TemplateAlgorithm<tlp::IntegerProperty> {
protected:
  IntegerAlgorithm (const tlp::PluginContext*);
public:
  virtual std::string category() const {
    return INTEGER_ALGORITHM_CATEGORY;
  }
};

class LayoutProperty;
static const std::string LAYOUT_ALGORITHM_CATEGORY = "Layout";

/**
 * @ingroup Plugins
 * @brief The layout algorithm takes a graph as input and output its results as a tlp::LayoutProperty
 */
class TLP_SCOPE LayoutAlgorithm : public TemplateAlgorithm<tlp::LayoutProperty> {
protected:
  ///
  LayoutAlgorithm (const tlp::PluginContext*);
public:
  virtual std::string category() const {
    return LAYOUT_ALGORITHM_CATEGORY;
  }
};

class SizeProperty;
static const std::string SIZE_ALGORITHM_CATEGORY = "Resizing";

/**
 * @ingroup Plugins
 * @brief The size algorithm takes a graph as input and output its results as a tlp::SizeProperty
 */
class TLP_SCOPE SizeAlgorithm : public TemplateAlgorithm<tlp::SizeProperty> {
protected:
  SizeAlgorithm (const tlp::PluginContext*);
public:
  virtual std::string category() const {
    return SIZE_ALGORITHM_CATEGORY;
  }
};

class StringProperty;
static const std::string STRING_ALGORITHM_CATEGORY = "Labeling";

/**
 * @ingroup Plugins
 * @brief The string algorithm takes a graph as input and output its results as a tlp::StringProperty
 */
class TLP_SCOPE StringAlgorithm : public TemplateAlgorithm<tlp::StringProperty> {
protected:
  ///
  StringAlgorithm (const tlp::PluginContext*);
public:
  virtual std::string category() const {
    return STRING_ALGORITHM_CATEGORY;
  }
};
}

#endif // PROPERTYALGORITHM_H
