/**
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

#include <tulip/PropertyAlgorithm.h>

#include <tulip/BooleanProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/LayoutProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/StringProperty.h>

tlp::BooleanAlgorithm::BooleanAlgorithm(const tlp::PluginContext *context)
    : tlp::TemplateAlgorithm<BooleanProperty>(context) {
  addOutParameter<tlp::BooleanProperty>(
      "result", "This parameter indicates the property to compute.", "viewSelection");
}

tlp::ColorAlgorithm::ColorAlgorithm(const tlp::PluginContext *context)
    : tlp::TemplateAlgorithm<ColorProperty>(context) {
  addOutParameter<tlp::ColorProperty>("result", "This parameter indicates the property to compute.",
                                      "viewColor");
}

tlp::DoubleAlgorithm::DoubleAlgorithm(const tlp::PluginContext *context)
    : tlp::TemplateAlgorithm<DoubleProperty>(context) {
  addOutParameter<tlp::DoubleProperty>(
      "result", "This parameter indicates the property to compute.", "viewMetric");
}

tlp::IntegerAlgorithm::IntegerAlgorithm(const tlp::PluginContext *context)
    : tlp::TemplateAlgorithm<IntegerProperty>(context) {
  addOutParameter<tlp::IntegerProperty>("result",
                                        "This parameter indicates the property to compute.");
}

tlp::LayoutAlgorithm::LayoutAlgorithm(const tlp::PluginContext *context)
    : tlp::TemplateAlgorithm<LayoutProperty>(context) {
  addOutParameter<tlp::LayoutProperty>(
      "result", "This parameter indicates the property to compute.", "viewLayout");
}

tlp::SizeAlgorithm::SizeAlgorithm(const tlp::PluginContext *context)
    : tlp::TemplateAlgorithm<SizeProperty>(context) {
  addOutParameter<tlp::SizeProperty>("result", "This parameter indicates the property to compute.",
                                     "viewSize");
}

tlp::StringAlgorithm::StringAlgorithm(const tlp::PluginContext *context)
    : tlp::TemplateAlgorithm<StringProperty>(context) {
  addOutParameter<tlp::StringProperty>(
      "result", "This parameter indicates the property to compute.", "viewLabel");
}
