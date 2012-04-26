#include "tulip/TulipMimes.h"

#include <tulip/TulipMetaTypes.h>
#include <tulip/ForEach.h>
#include <tulip/Perspective.h>

using namespace tlp;

AlgorithmMimeType::AlgorithmMimeType(QString algorithmName, const DataSet &data): _algorithm(algorithmName), _params(data) {
}

template<typename PROP>
void asLocal(QVariant var, DataSet& data, Graph* g) {
  if (var.userType() == qMetaTypeId<PROP*>()) {
    PROP* local = g->getLocalProperty<PROP>(var.value<PROP*>()->getName());
    data.set("result",local);
  }
}
void copyToLocal(DataSet& data, Graph* g) {
  if (!data.exist("result"))
    return;

  DataType* d = data.getData("result");
  QVariant var = TulipMetaTypes::dataTypeToQvariant(d,"");
  asLocal<DoubleProperty>(var,data,g);
  asLocal<IntegerProperty>(var,data,g);
  asLocal<LayoutProperty>(var,data,g);
  asLocal<SizeProperty>(var,data,g);
  asLocal<ColorProperty>(var,data,g);
  asLocal<BooleanProperty>(var,data,g);
  asLocal<StringProperty>(var,data,g);
}
#define CHECK_PROP(T)     if (v->getTypeName().compare(typeid(T*).name()) == 0) {\
T* prop = *((T**)v->value); \
dataSet.set(k,g->getProperty<T>(prop->getName()));\
}
void AlgorithmMimeType::run(Graph* g) const {
  if (g == NULL) {
    qCritical() << _algorithm << trUtf8(": No graph selected");
    return;
  }

  Observable::holdObservers();
  DataSet dataSet = _params;
  std::pair<std::string,tlp::DataType*> entry;
  forEach(entry, dataSet.getValues()) {
    std::string k = entry.first;
    DataType* v = dataSet.getData(k);
    CHECK_PROP(DoubleProperty)
    CHECK_PROP(IntegerProperty)
    CHECK_PROP(LayoutProperty)
    CHECK_PROP(SizeProperty)
    CHECK_PROP(ColorProperty)
    CHECK_PROP(BooleanProperty)
    CHECK_PROP(StringProperty)
    CHECK_PROP(GraphProperty)
  }
  copyToLocal(dataSet,g);
  std::string errorMessage;
  PluginProgress* progress = Perspective::instance()->progress();
  bool result = g->applyAlgorithm(_algorithm.toStdString(),errorMessage,&dataSet,progress);
  delete progress;

  if (!result)
    qCritical() << _algorithm << ": " << errorMessage.c_str();

  Observable::unholdObservers();
}
