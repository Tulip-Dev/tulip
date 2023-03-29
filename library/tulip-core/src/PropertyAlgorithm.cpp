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

#include <tulip/PropertyAlgorithm.h>
#include <tulip/Graph.h>
#include <tulip/BooleanProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/LayoutProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/StringProperty.h>

#define ADD_RESULT_PARAMETER(TYPE, DEFVAL, INOUT)                                                  \
  parameters.template add<TYPE>("result", "This parameter indicates the property to compute.",     \
                                DEFVAL, true, INOUT ? INOUT_PARAM : OUT_PARAM, "")

tlp::BooleanAlgorithm::BooleanAlgorithm(const tlp::PluginContext *context, bool needInOutResult)
    : tlp::TypedPropertyAlgorithm<BooleanProperty>(context) {
  ADD_RESULT_PARAMETER(tlp::BooleanProperty, "viewSelection", needInOutResult);
}

tlp::ColorAlgorithm::ColorAlgorithm(const tlp::PluginContext *context, bool needInOutResult)
    : tlp::TypedPropertyAlgorithm<ColorProperty>(context) {
  ADD_RESULT_PARAMETER(tlp::ColorProperty, "viewColor", needInOutResult);
}

tlp::DoubleAlgorithm::DoubleAlgorithm(const tlp::PluginContext *context, bool needInOutResult)
    : tlp::TypedPropertyAlgorithm<DoubleProperty>(context) {
  ADD_RESULT_PARAMETER(tlp::DoubleProperty, "viewMetric", needInOutResult);
}

tlp::IntegerAlgorithm::IntegerAlgorithm(const tlp::PluginContext *context, bool needInOutResult)
    : tlp::TypedPropertyAlgorithm<IntegerProperty>(context) {
  ADD_RESULT_PARAMETER(tlp::IntegerProperty, "", needInOutResult);
}

tlp::LayoutAlgorithm::LayoutAlgorithm(const tlp::PluginContext *context, bool needInOutResult)
    : tlp::TypedPropertyAlgorithm<LayoutProperty>(context) {
  ADD_RESULT_PARAMETER(tlp::LayoutProperty, "viewLayout", needInOutResult);
}

tlp::SizeAlgorithm::SizeAlgorithm(const tlp::PluginContext *context, bool needInOutResult)
    : tlp::TypedPropertyAlgorithm<SizeProperty>(context) {
  ADD_RESULT_PARAMETER(tlp::SizeProperty, "viewSize", needInOutResult);
}

tlp::StringAlgorithm::StringAlgorithm(const tlp::PluginContext *context, bool needInOutResult)
    : tlp::TypedPropertyAlgorithm<StringProperty>(context) {
  ADD_RESULT_PARAMETER(tlp::StringProperty, "viewLabel", needInOutResult);
}
