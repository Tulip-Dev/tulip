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

  if (dataSet != NULL)
    dataSet->get("result", result);
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

  if (dataSet != NULL)
    dataSet->get("result", result);
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

  if (dataSet != NULL)
    dataSet->get("result", result);
}

tlp::IntegerAlgorithm::IntegerAlgorithm (const tlp::PluginContext* context):tlp::TemplateAlgorithm<IntegerProperty>(context) {
  if (dataSet != NULL)
    dataSet->get("result", result);
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

  if (dataSet != NULL)
    dataSet->get("result", result);
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

  if (dataSet != NULL)
    dataSet->get("result", result);
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

  if (dataSet != NULL)
    dataSet->get("result", result);
}
