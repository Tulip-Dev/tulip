//-*-c++-*-

#include "tulip/StringAlgorithm.h"
#include "tulip/SuperGraph.h"
#include "tulip/String.h"
StringAlgorithm::StringAlgorithm (const PropertyContext & context):Property<StringType , StringType>(context) {
  stringResult = (String *)context.propertyProxy;
}

