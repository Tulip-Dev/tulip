//-*-c++-*-

#include "tulip/StringAlgorithm.h"
#include "tulip/Graph.h"
#include "tulip/StringProperty.h"
StringAlgorithm::StringAlgorithm (const PropertyContext & context):PropertyAlgorithm<StringType , StringType>(context) {
  stringResult = (StringProperty *)context.propertyProxy;
}

