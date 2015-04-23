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

#include <tulip/PropertyAlgorithm.h>

#include <tulip/BooleanProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/LayoutProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/StringProperty.h>

tlp::BooleanAlgorithm::BooleanAlgorithm (const tlp::PluginContext* context):tlp::TemplateAlgorithm<BooleanProperty>(context) {
  addOutParameter
  <tlp::BooleanProperty>("result",
                         HTML_HELP_OPEN()        \
                         HTML_HELP_DEF( "type", "BooleanProperty" )  \
                         HTML_HELP_DEF( "default", "\"viewSelection\"" ) \
                         HTML_HELP_BODY()        \
                         "This parameter indicates the property to compute." \
                         HTML_HELP_CLOSE(),
                         "viewSelection");
}

tlp::ColorAlgorithm::ColorAlgorithm (const tlp::PluginContext* context):tlp::TemplateAlgorithm<ColorProperty>(context) {
  addOutParameter
  <tlp::ColorProperty>("result",
                       HTML_HELP_OPEN()        \
                       HTML_HELP_DEF( "type", "ColorProperty" )  \
                       HTML_HELP_DEF( "default", "\"viewColor\"" ) \
                       HTML_HELP_BODY()        \
                       "This parameter indicates the property to compute." \
                       HTML_HELP_CLOSE(),
                       "viewColor");
}

tlp::DoubleAlgorithm::DoubleAlgorithm (const tlp::PluginContext* context):tlp::TemplateAlgorithm<DoubleProperty>(context) {
  addOutParameter
  <tlp::DoubleProperty>("result",
                        HTML_HELP_OPEN()       \
                        HTML_HELP_DEF( "type", "DoubleProperty" )  \
                        HTML_HELP_DEF( "default", "\"viewMetric\"" ) \
                        HTML_HELP_BODY()       \
                        "This parameter indicates the property to compute." \
                        HTML_HELP_CLOSE(),
                        "viewMetric");
}

tlp::IntegerAlgorithm::IntegerAlgorithm (const tlp::PluginContext* context):tlp::TemplateAlgorithm<IntegerProperty>(context) {
}

tlp::LayoutAlgorithm::LayoutAlgorithm (const tlp::PluginContext* context):tlp::TemplateAlgorithm<LayoutProperty>(context) {
  addOutParameter
  <tlp::LayoutProperty>("result",
                        HTML_HELP_OPEN()       \
                        HTML_HELP_DEF( "type", "LayoutProperty" )  \
                        HTML_HELP_DEF( "default", "\"viewLayout\"" ) \
                        HTML_HELP_BODY()       \
                        "This parameter indicates the property to compute." \
                        HTML_HELP_CLOSE(),
                        "viewLayout");
}

tlp::SizeAlgorithm::SizeAlgorithm (const tlp::PluginContext* context):tlp::TemplateAlgorithm<SizeProperty>(context) {
  addOutParameter
  <tlp::SizeProperty>("result",
                      HTML_HELP_OPEN()         \
                      HTML_HELP_DEF( "type", "SizeProperty" )    \
                      HTML_HELP_DEF( "default", "\"viewSize\"" ) \
                      HTML_HELP_BODY()         \
                      "This parameter indicates the property to compute." \
                      HTML_HELP_CLOSE(),
                      "viewSize");
}

tlp::StringAlgorithm::StringAlgorithm (const tlp::PluginContext* context):tlp::TemplateAlgorithm<StringProperty>(context) {
  addOutParameter
  <tlp::StringProperty>("result",
                        HTML_HELP_OPEN()       \
                        HTML_HELP_DEF( "type", "StringProperty" )  \
                        HTML_HELP_DEF( "default", "\"viewLabel\"" )  \
                        HTML_HELP_BODY()       \
                        "This parameter indicates the property to compute." \
                        HTML_HELP_CLOSE(),
                        "viewLabel");
}
