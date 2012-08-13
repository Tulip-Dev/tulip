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

#ifndef PROPERTYALGORITHM_H
#define PROPERTYALGORITHM_H

#include <tulip/Algorithm.h>
#include <tulip/TemplateAlgorithm.h>

namespace tlp {
class PluginContext;

class BooleanProperty;
static const std::string BOOLEAN_ALGORITHM_CATEGORY = QObject::trUtf8("Filtering").toStdString();

/// Interface for selection plug-ins
class TLP_SCOPE BooleanAlgorithm : public TemplateAlgorithm<tlp::BooleanProperty> {
protected:
  ///
  BooleanAlgorithm (const tlp::PluginContext*);
  virtual std::string category() const {
    return BOOLEAN_ALGORITHM_CATEGORY;
  }
};

class ColorProperty;
static const std::string COLOR_ALGORITHM_CATEGORY = QObject::trUtf8("Coloring").toStdString();

/// Interface for color plug-ins
class TLP_SCOPE ColorAlgorithm : public TemplateAlgorithm<tlp::ColorProperty> {
protected:
  ///
  ColorAlgorithm (const tlp::PluginContext*);
  virtual std::string category() const {
    return COLOR_ALGORITHM_CATEGORY;
  }
};

class DoubleProperty;
static const std::string DOUBLE_ALGORITHM_CATEGORY = QObject::trUtf8("Metric").toStdString();

/// Interface for metric plug-ins
class TLP_SCOPE DoubleAlgorithm : public TemplateAlgorithm<tlp::DoubleProperty> {
protected:
  ///
  DoubleAlgorithm (const tlp::PluginContext*);
  virtual std::string category() const {
    return DOUBLE_ALGORITHM_CATEGORY;
  }
};

class IntegerProperty;
static const std::string INTEGER_ALGORITHM_CATEGORY = QObject::trUtf8("Metric").toStdString();

/// Interface for int plug-ins
class TLP_SCOPE IntegerAlgorithm : public TemplateAlgorithm<tlp::IntegerProperty> {
protected:
  ///
  IntegerAlgorithm (const tlp::PluginContext*);
  virtual std::string category() const {
    return INTEGER_ALGORITHM_CATEGORY;
  }
};

class LayoutProperty;
static const std::string LAYOUT_ALGORITHM_CATEGORY = QObject::trUtf8("Layout").toStdString();

/// Interface for layout plug-ins
class TLP_SCOPE LayoutAlgorithm : public TemplateAlgorithm<tlp::LayoutProperty> {
protected:
  ///
  LayoutAlgorithm (const tlp::PluginContext*);
  virtual std::string category() const {
    return LAYOUT_ALGORITHM_CATEGORY;
  }
};

class SizeProperty;
static const std::string SIZE_ALGORITHM_CATEGORY = QObject::trUtf8("Resizing").toStdString();

/// Interface for size plug-ins
class TLP_SCOPE SizeAlgorithm : public TemplateAlgorithm<tlp::SizeProperty> {
protected:
  ///
  SizeAlgorithm (const tlp::PluginContext*);
  virtual std::string category() const {
    return SIZE_ALGORITHM_CATEGORY;
  }
};

class StringProperty;
static const std::string STRING_ALGORITHM_CATEGORY = QObject::trUtf8("Labeling").toStdString();

/// Interface for string plug-ins
class TLP_SCOPE StringAlgorithm : public TemplateAlgorithm<tlp::StringProperty> {
protected:
  ///
  StringAlgorithm (const tlp::PluginContext*);
  virtual std::string category() const {
    return STRING_ALGORITHM_CATEGORY;
  }
};
}

#endif // PROPERTYALGORITHM_H
