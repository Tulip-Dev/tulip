#ifndef PROPERTYALGORITHM_H
#define PROPERTYALGORITHM_H

#include <tulip/Algorithm.h>
#include <tulip/TemplateAlgorithm.h>

namespace tlp {
class PluginContext;

class BooleanProperty;
/// Interface for selection plug-ins
class TLP_SCOPE BooleanAlgorithm : public TemplateAlgorithm<tlp::BooleanProperty> {
protected:
  ///
  BooleanAlgorithm (const tlp::PluginContext*);
  virtual std::string category() const {
    return QObject::trUtf8("Filtering").toStdString();
  }
};

class ColorProperty;
/// Interface for color plug-ins
class TLP_SCOPE ColorAlgorithm : public TemplateAlgorithm<tlp::ColorProperty> {
protected:
  ///
  ColorAlgorithm (const tlp::PluginContext*);
  virtual std::string category() const {
    return QObject::trUtf8("Coloring").toStdString();
  }
};

class DoubleProperty;
/// Interface for metric plug-ins
class TLP_SCOPE DoubleAlgorithm : public TemplateAlgorithm<tlp::DoubleProperty> {
protected:
  ///
  DoubleAlgorithm (const tlp::PluginContext*);
  virtual std::string category() const {
    return QObject::trUtf8("Metric").toStdString();
  }
};

class IntegerProperty;
/// Interface for int plug-ins
class TLP_SCOPE IntegerAlgorithm : public TemplateAlgorithm<tlp::IntegerProperty> {
protected:
  ///
  IntegerAlgorithm (const tlp::PluginContext*);
  virtual std::string category() const {
    return QObject::trUtf8("Metric").toStdString();
  }
};

class LayoutProperty;
/// Interface for layout plug-ins
class TLP_SCOPE LayoutAlgorithm : public TemplateAlgorithm<tlp::LayoutProperty> {
protected:
  ///
  LayoutAlgorithm (const tlp::PluginContext*);
  virtual std::string category() const {
    return QObject::trUtf8("Layout").toStdString();
  }
};

class SizeProperty;
/// Interface for size plug-ins
class TLP_SCOPE SizeAlgorithm : public TemplateAlgorithm<tlp::SizeProperty> {
protected:
  ///
  SizeAlgorithm (const tlp::PluginContext*);
  virtual std::string category() const {
    return QObject::trUtf8("Resizing").toStdString();
  }
};

class StringProperty;
/// Interface for string plug-ins
class TLP_SCOPE StringAlgorithm : public TemplateAlgorithm<tlp::StringProperty> {
protected:
  ///
  StringAlgorithm (const tlp::PluginContext*);
  virtual std::string category() const {
    return QObject::trUtf8("Labeling").toStdString();
  }
};
}

#endif // PROPERTYALGORITHM_H
