#ifndef PROPERTYALGORITHM_H
#define PROPERTYALGORITHM_H

#include <tulip/PluginContext.h>
#include <tulip/Graph.h>
#include <tulip/Algorithm.h>
#include <tulip/TemplateAlgorithm.h>

namespace tlp {
class BooleanProperty;
/// Interface for selection plug-ins
class TLP_SCOPE BooleanAlgorithm : public TemplateAlgorithm<tlp::BooleanProperty> {
protected:
  ///
  BooleanAlgorithm (const tlp::PluginContext*);
  virtual std::string category() const {
    return "BooleanAlgorithm";
  }
};

class ColorProperty;
/// Interface for color plug-ins
class TLP_SCOPE ColorAlgorithm : public TemplateAlgorithm<tlp::ColorProperty> {
protected:
  ///
  ColorAlgorithm (const tlp::PluginContext*);
  virtual std::string category() const {
    return "ColorAlgorithm";
  }
};

class DoubleProperty;
/// Interface for metric plug-ins
class TLP_SCOPE DoubleAlgorithm : public TemplateAlgorithm<tlp::DoubleProperty> {
protected:
  ///
  DoubleAlgorithm (const tlp::PluginContext*);
  virtual std::string category() const {
    return "DoubleAlgorithm";
  }
};

class IntegerProperty;
/// Interface for int plug-ins
class TLP_SCOPE IntegerAlgorithm : public TemplateAlgorithm<tlp::IntegerProperty> {
protected:
  ///
  IntegerAlgorithm (const tlp::PluginContext*);
  virtual std::string category() const {
    return "IntegerAlgorithm";
  }
};

class LayoutProperty;
/// Interface for layout plug-ins
class TLP_SCOPE LayoutAlgorithm : public TemplateAlgorithm<tlp::LayoutProperty> {
protected:
  ///
  LayoutAlgorithm (const tlp::PluginContext*);
  virtual std::string category() const {
    return "LayoutAlgorithm";
  }
};

class SizeProperty;
/// Interface for size plug-ins
class TLP_SCOPE SizeAlgorithm : public TemplateAlgorithm<tlp::SizeProperty> {
protected:
  ///
  SizeAlgorithm (const tlp::PluginContext*);
  virtual std::string category() const {
    return "SizeAlgorithm";
  }
};

class StringProperty;
/// Interface for string plug-ins
class TLP_SCOPE StringAlgorithm : public TemplateAlgorithm<tlp::StringProperty> {
protected:
  ///
  StringAlgorithm (const tlp::PluginContext*);
  virtual std::string category() const {
    return "StringAlgorithm";
  }
};
}

#endif // PROPERTYALGORITHM_H
