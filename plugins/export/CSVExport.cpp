/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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
#include "CSVExport.h"

#include <tulip/StringCollection.h>
#include <tulip/PropertiesCollection.h>
#include <tulip/Graph.h>
#include <tulip/StringProperty.h>
#include <tulip/BooleanProperty.h>

#include <regex>

PLUGIN(CsvExport)

using namespace tlp;
using namespace std;

static const char *paramHelp[] = {
    // the type of element to export
    "This parameter enables to choose the type of graph elements to export",
    // selection
    "This parameter indicates the property used to restrict export to selected nodes only.",
    // export ids of graph nodes
    "This parameter indicates if the internal ids of the nodes and/or "
    " the ids of the nodes at the extremities of each edge have to be exported, according the type of elements to export.",
    // exported properties
    "This parameter indicates the properties to be exported. Default indicates only the user "
    "defined properties",
    // the field separator
    "This parameter indicates the field separator (sequence of one or more characters used to "
    "specify the boundary between two consecutive fields).",
    // the field separator custom value
    "This parameter allows to indicate a custom field separator. The 'Field separator' parameter "
    "must be set to 'Custom'",
    // the text delimiter
    "This parameter indicates the text delimiter (sequence of one or more characters used to "
    "specify the boundary of value of type text).",
    // the decimal mark
    "This parameter indicates the character used to separate the integer part from the fractional "
    "part of a number written in decimal form.",
};

#define ELT_TYPE "type of elements"
#define ELT_TYPES "nodes;edges;both"
#define NODE_TYPE 0
#define EDGE_TYPE 1
#define BOTH_TYPES 2

#define EXPORT_SELECTION "export selection"

#define EXPORT_ID "export nodes ids"

#define EXPORTED_PROPERTIES "exported properties"

#define FIELD_SEPARATOR "field separator"
#define FIELD_SEPARATORS " \\; ; , ;Tab;Space;Custom"
#define CUSTOM_SEPARATOR 4
#define COMMA_SEPARATOR 1
#define TAB_SEPARATOR 2
#define SPACE_SEPARATOR 3
#define SEMICOLON_SEPARATOR 0
#define FIELD_SEPARATOR_CUSTOM "custom separator"
#define CUSTOM_MARK ";"

#define STRING_DELIMITER "string delimiter"
#define STRING_DELIMITERS " \" ; ' "
#define DBL_QUOTE_DELIMITER 0
#define QUOTE_DELIMITER 1
#define DECIMAL_MARK "decimal mark"
#define DECIMAL_MARKS " . ; , "

//================================================================================
CsvExport::CsvExport(const PluginContext *context) : ExportModule(context, {"csv"}) {
  addInParameter<StringCollection>(ELT_TYPE, paramHelp[0], ELT_TYPES);
  addInParameter<BooleanProperty>("selection", paramHelp[1], "", false);
  addInParameter<bool>(EXPORT_ID, paramHelp[2], "false");
  addInParameter<PropertiesCollection>(EXPORTED_PROPERTIES, paramHelp[3],
                                       "the user defined properties");
  addInParameter<StringCollection>(FIELD_SEPARATOR, paramHelp[4], FIELD_SEPARATORS);
  addInParameter<string>(FIELD_SEPARATOR_CUSTOM, paramHelp[5], CUSTOM_MARK);
  addInParameter<StringCollection>(STRING_DELIMITER, paramHelp[6], STRING_DELIMITERS);
  addInParameter<StringCollection>(DECIMAL_MARK, paramHelp[7], DECIMAL_MARKS);
}

//================================================================================
// define a special facet to force the output
// of a comma as decimal mark
struct decimal_comma : std::numpunct<char> {
  char do_decimal_point() const override {
    return ',';
  }
};

void CsvExport::exportString(std::ostream &os, const std::string &s) {
  // do not forget to escape quotes in data
  std::string delim(1, stringDelimiter);
  std::string delim2(2, stringDelimiter);
  os << stringDelimiter << std::regex_replace(s, std::regex(delim), delim2) << stringDelimiter;
}

bool CsvExport::exportGraph(std::ostream &os) {
  // initialize parameters with default values
  // only nodes are exported
  StringCollection eltTypes(ELT_TYPES);
  int eltType = 0;
  bool first = true;
  eltTypes.setCurrent(0);
  // export all
  bool exportSelection = false;
  // ids are not exported
  bool exportId = false;
  // export properties
  PropertiesCollection propsCollection(graph);
  // field separator is Custom
  StringCollection fieldSeparators(FIELD_SEPARATORS);
  fieldSeparators.setCurrent(0);
  // custom field separator is ;
  string fieldSeparatorCustom(CUSTOM_MARK);
  // string delimiter is "
  StringCollection stringDelimiters(STRING_DELIMITERS);
  stringDelimiters.setCurrent(0);
  // decimal mark is .
  StringCollection decimalMarks(DECIMAL_MARKS);
  decimalMarks.setCurrent(0);

  // get chosen values of plugin parameters
  if (dataSet != nullptr) {
    if (dataSet->get(ELT_TYPE, eltTypes))
      eltType = eltTypes.getCurrent();

    // this parameter is no longer needed
    // because the presence of the "selection" property is now sufficient
    // to indicate if the export is restricted to the selection
    // but it is there for compatibility reason
    // and then force the use of "viewSelection"
    // as default value of the former parameter "export selection property"
    dataSet->get(EXPORT_SELECTION, exportSelection);
    dataSet->get(EXPORT_ID, exportId);

    dataSet->get(EXPORTED_PROPERTIES, propsCollection);
    dataSet->get(FIELD_SEPARATOR_CUSTOM, fieldSeparatorCustom);

    if (dataSet->get(FIELD_SEPARATOR, fieldSeparators)) {
      switch (fieldSeparators.getCurrent()) {
      case COMMA_SEPARATOR:
        fieldSeparator = ',';
        break;

      case TAB_SEPARATOR:
        fieldSeparator = '\t';
        break;

      case SPACE_SEPARATOR:
        fieldSeparator = ' ';
        break;

      case SEMICOLON_SEPARATOR:
        fieldSeparator = ';';
        break;

      default:
        fieldSeparator = fieldSeparatorCustom;
      }
    }

    if (dataSet->get(STRING_DELIMITER, stringDelimiters))
      stringDelimiter = stringDelimiters.getCurrent() == DBL_QUOTE_DELIMITER ? '"' : '\'';

    if (dataSet->get(DECIMAL_MARK, decimalMarks))
      decimalMark = decimalMarks.getCurrent() ? ',' : '.';
  }

  // export names of fields
  // export ids if needed
  if (exportId) {
    if (eltType != EDGE_TYPE) {
      exportString(os, string("node id"));
    }

    if (eltType == BOTH_TYPES) {
      os << fieldSeparator;
    }

    if (eltType != NODE_TYPE) {
      exportString(os, string("src id"));
      os << fieldSeparator;
      exportString(os, string("tgt id"));
    }

    first = false;
  }

  // exported properties
  // use vectors for further access to exported properties
  vector<PropertyInterface *> props;
  props.reserve(propsCollection.getSelected().size());

  vector<bool> propIsString;
  unsigned int nbProps = 0;

  // do nothing if no properties selected and ids not exported
  if (propsCollection.emptySelected() && !exportId) {
    if (pluginProgress != nullptr)
      pluginProgress->setError("Nothing to export. Export cancelled.");
    else
      tlp::warning() << "Nothing to export. Export cancelled." << std::endl;
    return false;
  }

  for (auto &propName : propsCollection.getSelected()) {
    auto prop = graph->getProperty(propName);
    ++nbProps;
    props.push_back(prop);
    propIsString.push_back(dynamic_cast<tlp::StringProperty *>(prop));

    if (!first)
      os << fieldSeparator;
    else
      first = false;

    exportString(os, propName);
  }
  os << endl;

  // export nodes
  BooleanProperty *prop = nullptr;

  if (exportSelection) {
    prop = graph->getProperty<BooleanProperty>("viewSelection");
    dataSet->get("export selection property", prop);
  } else
    dataSet->get("selection", prop);

  // get global locale
  std::locale prevLocale;

  // change decimal point of global locale if needed
  if (decimalMark == ',')
    std::locale::global(std::locale(prevLocale, new decimal_comma));

  if (eltType != EDGE_TYPE) {
    Iterator<node> *it = prop ? prop->getNodesEqualTo(true, graph) : graph->getNodes();

    for (auto n : it) {

      if (exportId) {
        os << n;

        if (eltType == BOTH_TYPES)
          os << fieldSeparator << fieldSeparator;

        if (nbProps > 0)
          os << fieldSeparator;
      }

      for (unsigned int i = 0; i < nbProps; ++i) {
        PropertyInterface *prop = props[i];
        string value = prop->getNodeStringValue(n);

        if (!value.empty()) {
          if (propIsString[i])
            exportString(os, value);
          else
            os << value;
        }

        if (i != nbProps - 1)
          os << fieldSeparator;
      }

      os << endl;
    }
  }

  // export edges
  if (eltType != NODE_TYPE) {
    Iterator<edge> *it = prop ? prop->getEdgesEqualTo(true, graph) : graph->getEdges();

    for (auto e : it) {

      if (exportId) {
        if (eltType == BOTH_TYPES)
          os << fieldSeparator;

        auto ends = graph->ends(e);
        os << ends.first << fieldSeparator << ends.second.id;

        if (nbProps > 0)
          os << fieldSeparator;
      }

      for (unsigned int i = 0; i < nbProps; ++i) {
        PropertyInterface *prop = props[i];
        string value = prop->getEdgeStringValue(e);

        if (!value.empty()) {
          if (propIsString[i])
            exportString(os, value);
          else
            os << value;
        }

        if (i != nbProps - 1)
          os << fieldSeparator;
      }

      os << endl;
    }
  }

  // restore global locale
  std::locale::global(prevLocale);

  return true;
}
