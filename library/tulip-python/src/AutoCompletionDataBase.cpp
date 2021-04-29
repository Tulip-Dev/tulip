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

#include "tulip/PythonInterpreter.h"
#include "tulip/AutoCompletionDataBase.h"
#include "tulip/APIDataBase.h"

#include <QTextStream>
#include <QRegExp>
#include <QStringList>

#include <tulip/StringCollection.h>
#include <tulip/PluginLister.h>
#include <tulip/PropertyAlgorithm.h>
#include <tulip/ImportModule.h>
#include <tulip/ExportModule.h>
#include <tulip/Glyph.h>
#include <tulip/Interactor.h>
#include <tulip/View.h>
#include <tulip/Perspective.h>
#include <tulip/EdgeExtremityGlyph.h>
#include <tulip/TlpQtTools.h>

using namespace std;
using namespace tlp;

static char sepChar[] = {' ', '\t', '=', '(', '[', '{', ',', '*', '+', '/', '^', '-', 0};

AutoCompletionDataBase::AutoCompletionDataBase(APIDataBase *apiDb)
    : _graph(nullptr), _apiDb(apiDb) {
  _iteratorType["tlp.IteratorNode"] = "tlp.node";
  _iteratorType["tlp.NodeMapIterator"] = "tlp.node";
  _iteratorType["tlp.IteratorEdge"] = "tlp.edge";
  _iteratorType["tlp.EdgeMapIterator"] = "tlp.edge";
  _iteratorType["tlp.IteratorGraph"] = "tlp.Graph";
  _iteratorType["tlp.IteratorString"] = "string";
}

static bool tlpPluginExists(const QString &pluginName) {

  return PluginLister::pluginExists(QStringToTlpString(pluginName));
}

static QString getPythonTypeName(const QString &cppTypeName) {
  if (cppTypeName == "b") {
    return "boolean";
  } else if (cppTypeName == "i") {
    return "integer";
  } else if (cppTypeName == "d") {
    return "float";
  } else if (cppTypeName == typeid(string).name()) {
    return "string";
  } else {
    QString typeName = demangleTlpClassName(QStringToTlpString(cppTypeName).c_str()).c_str();
    typeName.replace("*", "");
    return "tlp." + typeName;
  }
}

static QSet<QString> getParametersListForPlugin(const QString &pluginName,
                                                const QString &prefix = "") {
  QSet<QString> ret;

  if (PluginLister::pluginExists(QStringToTlpString(pluginName))) {

    const ParameterDescriptionList &parameters =
        PluginLister::getPluginParameters(QStringToTlpString(pluginName));

    for (const ParameterDescription &pd : parameters.getParameters()) {
      QString param = tlpStringToQString(pd.getName());
      // remove the special prefixes for files parameters used internally by the Tulip GUI
      param = param.replace("anyfile::", "");
      param = param.replace("file::", "");
      param = param.replace("dir::", "");
      QString paramName = "\"" + param + "\" (" + getPythonTypeName(pd.getTypeName().c_str()) + ")";
      paramName.replace("\n", "\\n");

      if (paramName.startsWith(prefix))
        ret.insert(paramName);

      paramName = "'" + param + "' (" + getPythonTypeName(pd.getTypeName().c_str()) + ")";
      paramName.replace("\n", "\\n");

      if (paramName.startsWith(prefix))
        ret.insert(paramName);
    }
  }

  return ret;
}

static QSet<QString> getStringCollectionEntriesForPlugin(const QString &pluginName,
                                                         const QString &strCollectionName,
                                                         const QString &prefix = "") {
  QSet<QString> ret;

  if (PluginLister::pluginExists(QStringToTlpString(pluginName))) {
    const ParameterDescriptionList &parameters =
        PluginLister::getPluginParameters(QStringToTlpString(pluginName));
    DataSet dataSet;
    parameters.buildDefaultDataSet(dataSet);
    StringCollection sc;
    dataSet.get(QStringToTlpString(strCollectionName), sc);

    for (size_t i = 0; i < sc.size(); ++i) {
      QString entry = "\"" + tlpStringToQString(sc[i]) + "\"";

      if (entry.startsWith(prefix))
        ret.insert(entry);
    }
  }

  return ret;
}

static QString getPythonTypeNameForGraphProperty(Graph *graph, const QString &propName) {
  if (graph->existLocalProperty(QStringToTlpString(propName))) {
    PropertyInterface *prop = graph->getProperty(QStringToTlpString(propName));

    if (prop->getTypename() == "bool") {
      return "tlp.BooleanProperty";
    } else if (prop->getTypename() == "vector<bool>") {
      return "tlp.BooleanVectorProperty";
    } else if (prop->getTypename() == "int") {
      return "tlp.IntegerProperty";
    } else if (prop->getTypename() == "vector<int>") {
      return "tlp.IntegerVectorProperty";
    } else if (prop->getTypename() == "double") {
      return "tlp.DoubleProperty";
    } else if (prop->getTypename() == "vector<double>") {
      return "tlp.DoubleVectorProperty";
    } else if (prop->getTypename() == "color") {
      return "tlp.ColorProperty";
    } else if (prop->getTypename() == "vector<color>") {
      return "tlp.ColorVectorProperty";
    } else if (prop->getTypename() == "layout") {
      return "tlp.LayoutProperty";
    } else if (prop->getTypename() == "vector<coord>") {
      return "tlp.CoordVectorProperty";
    } else if (prop->getTypename() == "size") {
      return "tlp.SizeProperty";
    } else if (prop->getTypename() == "vector<size>") {
      return "tlp.SizeVectorProperty";
    } else if (prop->getTypename() == "string") {
      return "tlp.StringProperty";
    } else if (prop->getTypename() == "vector<string>") {
      return "tlp.StringVectorProperty";
    } else if (prop->getTypename() == "graph") {
      return "tlp.GraphProperty";
    }
  }

  QString ret = "";
  for (Graph *sg : graph->subGraphs()) {
    ret = getPythonTypeNameForGraphProperty(sg, propName);

    if (!ret.isEmpty()) {
      return ret;
    }
  }

  return ret;
}

static QString getPythonTypeNameForPropertyType(const QString &propertyType, const bool nodes) {
  if (propertyType == "tlp.BooleanProperty") {
    return "boolean";
  } else if (propertyType == "tlp.BooleanVectorProperty") {
    return "list-of-boolean";
  } else if (propertyType == "tlp.LayoutProperty") {
    if (nodes) {
      return "tlp.Coord";
    } else {
      return "list-of-tlp.Coord";
    }
  } else if (propertyType == "tlp.CoordVectorProperty") {
    return "list-of-tlp.Coord";
  } else if (propertyType == "tlp.SizeProperty") {
    return "tlp.Size";
  } else if (propertyType == "tlp.SizeVectorProperty") {
    return "list-of-tlp.Size";
  } else if (propertyType == "tlp.ColorProperty") {
    return "tlp.Color";
  } else if (propertyType == "tlp.ColorVectorProperty") {
    return "list-of-tlp.Color";
  } else if (propertyType == "tlp.DoubleProperty") {
    return "float";
  } else if (propertyType == "tlp.DoubleVectorProperty") {
    return "list-of-float";
  } else if (propertyType == "tlp.IntegerProperty") {
    return "integer";
  } else if (propertyType == "tlp.IntegerVectorProperty") {
    return "list-of-integer";
  } else if (propertyType == "tlp.GraphProperty") {
    if (nodes) {
      return "tlp.Graph";
    } else {
      return "list-of-tlp.edge";
    }
  } else if (propertyType == "tlp.StringProperty") {
    return "string";
  } else if (propertyType == "tlp.StringVectorProperty") {
    return "list-of-string";
  }

  return "";
}

void AutoCompletionDataBase::analyseCurrentScriptCode(const QString &code, const int currentLine,
                                                      const bool interactiveSession,
                                                      const QString &moduleName) {

  _globalAutoCompletionList.clear();
  _functionAutoCompletionList.clear();
  _pluginParametersDataSet.clear();
  _varToPluginName.clear();

  _classContents.clear();
  _classBases.clear();

  _varToType.clear();
  _varToType["global"]["graph"] = "tlp.Graph";
  _varToType["global"]["tlp"] = "tlp";

  _classAttributeToType.clear();
  _classAttributeToType["tlp.Algorithm"]["self.graph"] = "tlp.Graph";
  _classAttributeToType["tlp.Algorithm"]["self.dataSet"] = "tlp.DataSet";
  _classAttributeToType["tlp.Algorithm"]["self.pluginProgress"] = "tlp.PluginProgress";
  _classAttributeToType["tlp.ImportModule"]["self.graph"] = "tlp.Graph";
  _classAttributeToType["tlp.ImportModule"]["self.dataSet"] = "tlp.DataSet";
  _classAttributeToType["tlp.ImportModule"]["self.pluginProgress"] = "tlp.PluginProgress";
  _classAttributeToType["tlp.ExportModule"]["self.graph"] = "tlp.Graph";
  _classAttributeToType["tlp.ExportModule"]["self.dataSet"] = "tlp.DataSet";
  _classAttributeToType["tlp.ExportModule"]["self.pluginProgress"] = "tlp.PluginProgress";
  _classAttributeToType["tlp.BooleanAlgorithm"]["self.result"] = "tlp.BooleanProperty";
  _classAttributeToType["tlp.ColorAlgorithm"]["self.result"] = "tlp.ColorProperty";
  _classAttributeToType["tlp.DoubleAlgorithm"]["self.result"] = "tlp.DoubleProperty";
  _classAttributeToType["tlp.IntegerAlgorithm"]["self.result"] = "tlp.IntegerProperty";
  _classAttributeToType["tlp.LayoutAlgorithm"]["self.result"] = "tlp.LayoutProperty";
  _classAttributeToType["tlp.SizeAlgorithm"]["self.result"] = "tlp.SizeProperty";
  _classAttributeToType["tlp.StringAlgorithm"]["self.result"] = "tlp.StringProperty";

  QSet<QString> types = _apiDb->getTypesList();

  for (const QString &type : types) {
    _globalAutoCompletionList.insert(type);

    if (type.indexOf(".") != -1) {
      QStringList types = type.split(".");

      for (const QString &entry : types) {
        _globalAutoCompletionList.insert(entry);
      }
    }
  }

  QVector<QString> importedModules = PythonInterpreter::getInstance()->getImportedModulesList();

  for (int i = 0; i < importedModules.size(); ++i) {
    QString moduleName = importedModules[i];
    _globalAutoCompletionList.insert(moduleName);

    if (moduleName.indexOf(".") != -1) {
      _apiDb->addApiEntry(moduleName);
    }
  }

  QString builtinModName = "__builtin__";

  if (PythonInterpreter::getInstance()->getPythonVersion() >= 3.0) {
    builtinModName = "builtins";
  }

  if (PythonInterpreter::getInstance()->runString(QString("import ") + builtinModName)) {
    QVector<QString> builtinDictContent =
        PythonInterpreter::getInstance()->getObjectDictEntries(builtinModName);

    for (int i = 0; i < builtinDictContent.size(); ++i) {
      _globalAutoCompletionList.insert(builtinDictContent[i]);
    }
  }

  int i = 0;

  while (PythonInterpreter::pythonKeywords[i]) {
    _globalAutoCompletionList.insert(PythonInterpreter::pythonKeywords[i++]);
  }

  QString codeCp(code);

  QTextStream in(&codeCp);

  QRegExp importRegexp("^import[ \t]+[A-Za-z_][A-Za-z0-9_]*.*$");
  QRegExp globalVarRegexp("^global[ \t]+[A-Za-z_][A-Za-z0-9_]*.*$");
  QRegExp varAssignRegexp("^[a-zA-Z_][a-zA-Z0-9_.]*[ \t]*=[ \t]*.*$");
  QRegExp forRegexp("^for[ \t]+[a-zA-Z_][a-zA-Z0-9_]*[ \t]+in[ \t]+.*:$");
  QRegExp funcRegexp("^def[ \t]+[A-Za-z_][A-Za-z0-9_]+\\(.*\\)[ \t]*:$");
  QRegExp classRegexp("^class[ \t]+[A-Za-z_][A-Za-z0-9_]*.*:$");
  QRegExp methodCallRegexp("[A-Za-z_][A-Za-z0-9_]*\\.[A-Za-z_][A-Za-z0-9_]*\\(.*\\)");
  QRegExp pluginDataSetRegexp(
      "^[a-zA-Z_][a-zA-Z0-9_]*[ \t]*=[ \t]*tlp\\.getDefaultPluginParameters\\(.*\\).*$");
  QRegExp graphPropRegexp("\\w+\\[\".+\"\\]");
  QRegExp graphPropAccessRegexp("\\w+\\[\".+\"\\]\\[.+\\]");
  QRegExp graphPropAccessRegexp2("\\w+\\[.+\\]");

  QString currentClassName = "";
  QString currentFunctionName = "global";

  int ln = -1;

  while (!in.atEnd()) {
    ++ln;
    QString origLine = in.readLine();
    QString line = origLine.trimmed();

    if (interactiveSession) {
      if (!(line.startsWith(">>> ") || line.startsWith("... "))) {
        continue;
      } else {
        line = line.replace(0, 4, "");
      }
    }

    if (line.isEmpty() || line.startsWith("#"))
      continue;

    if (!(origLine.startsWith("\t") || origLine.startsWith(" "))) {
      currentClassName = "";
      currentFunctionName = "global";
    }

    QString fullName = currentFunctionName;

    if (!currentClassName.isEmpty()) {
      fullName = currentClassName + "." + fullName;
    }

    if (ln < currentLine && pluginDataSetRegexp.indexIn(line) != -1) {
      QString varName = line.mid(0, line.indexOf('=')).trimmed();
      QString pattern = "getDefaultPluginParameters(";
      int pos = line.indexOf(pattern);

      if (pos != -1) {
        pos += pattern.length();

        if (line.length() > pos && (line.at(pos) == '"' || line.at(pos) == '\'')) {
          QChar stringDelim = line.at(pos++);
          int pos2 = line.indexOf(stringDelim, pos);

          if (pos2 != -1) {
            QString pluginName = line.mid(pos, pos2 - pos);

            if (tlpPluginExists(pluginName)) {
              if (_pluginParametersDataSet.find(fullName) == _pluginParametersDataSet.end()) {
                _pluginParametersDataSet[fullName] = QHash<QString, QSet<QString>>();
                _varToPluginName[fullName] = QHash<QString, QString>();
              }

              _varToPluginName[fullName][varName] = pluginName;
              _pluginParametersDataSet[fullName][varName] = getParametersListForPlugin(pluginName);

              for (const QString &param : _pluginParametersDataSet[fullName][varName]) {
                QString name = param.mid(0, param.indexOf("(") - 1);
                QString type =
                    param.mid(param.indexOf("(") + 1, param.indexOf(")") - param.indexOf("(") - 1);
                QString dataSetVarName = varName + "[" + name + "]";

                if (_varToType.find(fullName) == _varToType.end()) {
                  _varToType[fullName] = QHash<QString, QString>();
                }

                _varToType[fullName][dataSetVarName] = type;
              }
            }
          }
        }
      }
    }

    if (methodCallRegexp.indexIn(line) != -1) {
      QString expr = line.mid(methodCallRegexp.indexIn(line),
                              line.indexOf('(', methodCallRegexp.indexIn(line)) -
                                  methodCallRegexp.indexIn(line));
      QStringList parts = expr.split(".");
      QString varName = parts.at(0);
      QString funcName = parts.at(1);
      QVector<QString> types = _apiDb->findTypesContainingDictEntry(funcName);

      if (types.size() == 1) {
        if (_varToType.find(fullName) == _varToType.end()) {
          _varToType[fullName] = QHash<QString, QString>();
        }

        if (_varToType[fullName].find(varName) == _varToType[fullName].end()) {
          _varToType[fullName][varName] = types.at(0);
        }
      }

      _globalAutoCompletionList.insert(funcName);
    }

    if (ln < currentLine && varAssignRegexp.indexIn(line) != -1) {
      QString varName = line.mid(0, line.indexOf('=')).trimmed();
      QString expr = line.mid(line.indexOf('=') + 1).trimmed();

      QString selfPattern = "self.";
      int pos = varName.indexOf(selfPattern);

      if (!currentClassName.isEmpty()) {
        if (pos != -1) {
          QString classEntry = varName.mid(pos + selfPattern.size());

          if (_classContents.find(currentClassName) == _classContents.end()) {
            _classContents[currentClassName] = QSet<QString>();
          }

          _classContents[currentClassName].insert(classEntry);
        }
      }

      QString type = "";

      if (expr.length() > 1 && expr[0] == '\"' && expr[expr.length() - 1] == '\"') {
        type = "string";
      } else if (expr.length() > 1 && expr[0] == '[' && expr[expr.length() - 1] == ']') {
        type = "list";
      } else if (expr.length() > 1 && expr[0] == '{' && expr[expr.length() - 1] == '}') {
        type = "dict";
      }

      if (type.isEmpty()) {
        type = findTypeForExpr(expr, fullName);
      }

      if (!type.isEmpty()) {
        if (currentClassName.isEmpty()) {
          if (_varToType.find(fullName) == _varToType.end()) {
            _varToType[fullName] = QHash<QString, QString>();
          }

          _varToType[fullName][varName] = type;
        } else {
          if (_classAttributeToType.find(currentClassName) == _classAttributeToType.end()) {
            _classAttributeToType[currentClassName] = QHash<QString, QString>();
          }

          _classAttributeToType[currentClassName][varName] = type;
        }
      }

      if (currentFunctionName == "global") {
        _globalAutoCompletionList.insert(varName);

        if (!moduleName.isEmpty()) {
          _apiDb->addApiEntry(moduleName + "." + varName);
        }
      } else {
        _functionAutoCompletionList[fullName].insert(varName);
      }

      continue;
    }

    if (ln < currentLine && forRegexp.indexIn(line) != -1) {
      QString varName = line.mid(4).trimmed();
      varName = varName.mid(0, varName.indexOf(QRegExp("\\bin\\b"))).trimmed();

      if (currentFunctionName == "global") {
        _globalAutoCompletionList.insert(varName);
      } else {
        _functionAutoCompletionList[fullName].insert(varName);
      }

      QString expr = line.mid(line.indexOf(QRegExp("\\bin\\b")) + 3).trimmed();
      expr = expr.mid(0, expr.indexOf(":")).trimmed();

      QString type = findTypeForExpr(expr, fullName);

      if (_iteratorType.find(type) != _iteratorType.end()) {
        type = _iteratorType[type];
      }

      else if (type.startsWith("list-of-")) {
        type = type.mid(8);

        if (type.startsWith("std_set") || type.startsWith("std_vector") ||
            type.startsWith("std_list")) {
          type = "list";
        }
      }

      if (!type.isEmpty()) {
        if (_varToType.find(fullName) == _varToType.end()) {
          _varToType[fullName] = QHash<QString, QString>();
        }

        _varToType[fullName][varName] = type;
      }

      continue;
    }

    if (funcRegexp.indexIn(line) != -1) {
      QString funcName = line.mid(0, line.indexOf('('));
      funcName = funcName.mid(4, line.indexOf('(')).trimmed();
      currentFunctionName = funcName;
      _globalAutoCompletionList.insert(funcName);
      QString fullName = currentFunctionName;

      if (!currentClassName.isEmpty()) {
        fullName = currentClassName + "." + fullName;

        if (_varToType.find(fullName) == _varToType.end()) {
          _varToType[fullName] = QHash<QString, QString>();
        }

        _varToType[fullName]["self"] = currentClassName;

        if (_classContents.find(currentClassName) == _classContents.end()) {
          _classContents[currentClassName] = QSet<QString>();
        }

        _classContents[currentClassName].insert(currentFunctionName);
      }

      if (!moduleName.isEmpty()) {
        QString withParams = line.mid(4, line.lastIndexOf(')') - 3);

        if (!currentClassName.isEmpty()) {
          QString withParamsFull = currentClassName + "." + withParams;

          if (!withParams.startsWith("__")) {
            withParamsFull = withParamsFull.replace(QRegExp("[ \t]*self[ \t]*,[ \t]*"), "");
            withParamsFull = withParamsFull.replace(QRegExp("[ \t]*self[ \t]*"), "");
          } else if (withParams.startsWith("__init__")) {
            _apiDb->addApiEntry(moduleName + "." + withParamsFull);
            withParamsFull =
                withParamsFull.replace(QRegExp(".__init__\\([ \t]*self[ \t]*,[ \t]*"), "(");
            withParamsFull = withParamsFull.replace(QRegExp(".__init__\\([ \t]*self[ \t]*"), "(");
          }

          withParams = withParamsFull;
        }

        _apiDb->addApiEntry(moduleName + "." + withParams);
      }

      if (_functionAutoCompletionList.find(fullName) == _functionAutoCompletionList.end()) {
        _functionAutoCompletionList[fullName] = QSet<QString>();
      }

      QString params = line.mid(line.indexOf('(') + 1, line.indexOf(')') - line.indexOf('(') - 1);

      if (params.indexOf(",") == -1) {
        QString paramClean = params;

        if (params.indexOf("=") != -1) {
          paramClean = params.mid(0, params.indexOf("="));
        }

        _functionAutoCompletionList[fullName].insert(paramClean.trimmed());
      } else {
        QStringList pList = params.split(",");

        for (const QString &param : pList) {
          QString paramClean = param;

          if (param.indexOf("=") != -1) {
            paramClean = param.mid(0, param.indexOf("="));
          }

          _functionAutoCompletionList[fullName].insert(paramClean.trimmed());
        }
      }

      continue;
    }

    if (globalVarRegexp.indexIn(line) != -1) {
      QString varName = line.mid(7).trimmed();
      _globalAutoCompletionList.insert(varName);
      continue;
    }

    if (importRegexp.indexIn(line) != -1) {
      QString varName = line.mid(7).trimmed();
      _globalAutoCompletionList.insert(varName);
      continue;
    }

    if (classRegexp.indexIn(line) != -1) {
      QString className = line;
      int pos = className.indexOf('(');

      if (pos != -1) {
        className = className.mid(6, pos - 6);
        int pos2 = line.indexOf(')', pos + 1);

        if (pos2 != -1) {
          QString cType = line.mid(pos + 1, pos2 - pos - 1);

          if (_classBases.find(className) == _classBases.end()) {
            _classBases[className] = QSet<QString>();
          }

          if (_apiDb->typeExists(cType)) {
            _classBases[className].insert(cType);
          } else if (!_apiDb->getFullTypeName(cType).isEmpty()) {
            _classBases[className].insert(_apiDb->getFullTypeName(cType));
          } else {
            _classBases[className].insert(cType);
          }
        }
      } else {
        className = className.mid(6, className.indexOf(':') - 6);

        if (_classBases.find(className) == _classBases.end()) {
          _classBases[className] = QSet<QString>();
        }
      }

      currentClassName = className;
      _globalAutoCompletionList.insert(className.trimmed());

      if (!moduleName.isEmpty()) {
        _apiDb->addApiEntry(moduleName + "." + className);
      }

      continue;
    }

    if (_graph && graphPropRegexp.indexIn(line) != -1) {
      QString expr = line.mid(graphPropRegexp.indexIn(line), graphPropRegexp.matchedLength());
      QString varName = expr.mid(0, expr.indexOf("["));
      QString propName =
          expr.mid(expr.indexOf("\"") + 1, expr.lastIndexOf("\"") - expr.indexOf("\"") - 1);

      if (findTypeForExpr(varName, fullName) == "tlp.Graph") {
        QString type = getPythonTypeNameForGraphProperty(_graph->getRoot(), propName);

        if (!type.isEmpty()) {
          if (_varToType.find(fullName) == _varToType.end()) {
            _varToType[fullName] = QHash<QString, QString>();
          }

          _varToType[fullName][expr] = type;
        }
      }
    }

    if (_graph && graphPropAccessRegexp.indexIn(line) != -1) {
      QString expr =
          line.mid(graphPropAccessRegexp.indexIn(line), graphPropAccessRegexp.matchedLength());
      int pos = expr.indexOf("[");
      QString varName = expr.mid(0, pos);
      QString propName =
          expr.mid(expr.indexOf("\"") + 1, expr.lastIndexOf("\"") - expr.indexOf("\"") - 1);
      pos = expr.indexOf("[", pos + 1);
      QString varName2 = expr.mid(pos + 1, expr.lastIndexOf("]") - pos - 1);

      if (findTypeForExpr(varName, fullName) == "tlp.Graph") {
        QString type = getPythonTypeNameForGraphProperty(_graph->getRoot(), propName);

        if (!type.isEmpty()) {
          QString type2 = findTypeForExpr(varName2, fullName);
          QString type3 = "";

          if (type2 == "tlp.node") {
            type3 = getPythonTypeNameForPropertyType(type, true);
          } else if (type2 == "tlp.edge") {
            type3 = getPythonTypeNameForPropertyType(type, false);
          }

          if (!type3.isEmpty()) {
            if (_varToType.find(fullName) == _varToType.end()) {
              _varToType[fullName] = QHash<QString, QString>();
            }

            _varToType[fullName][expr] = type3;
          }
        }
      }
    }

    if (graphPropAccessRegexp2.indexIn(line) != -1) {
      QString expr =
          line.mid(graphPropAccessRegexp2.indexIn(line), graphPropAccessRegexp2.matchedLength());
      int pos = expr.indexOf("[");
      QString varName = expr.mid(0, pos);
      QString varName2 = expr.mid(pos + 1, expr.lastIndexOf("]") - pos - 1);
      QString type = findTypeForExpr(varName, fullName);

      if (!type.isEmpty()) {
        QString type2 = findTypeForExpr(varName2, fullName);
        QString type3 = "";

        if (type2 == "tlp.node") {
          type3 = getPythonTypeNameForPropertyType(type, true);
        } else if (type2 == "tlp.edge") {
          type3 = getPythonTypeNameForPropertyType(type, false);
        }

        if (!type3.isEmpty()) {
          if (_varToType.find(fullName) == _varToType.end()) {
            _varToType[fullName] = QHash<QString, QString>();
          }

          _varToType[fullName][expr] = type3;
        }
      }
    }
  }
}

QString AutoCompletionDataBase::findTypeForExpr(const QString &expr,
                                                const QString &funcName) const {

  QString currentType = "";

  if (expr.indexOf('(') != -1 && expr.indexOf(')') != -1) {
    QString name = expr.mid(0, expr.indexOf('('));

    if (_apiDb->typeExists(name)) {
      currentType = name;
    } else if (!_apiDb->getFullTypeName(name).isEmpty()) {
      currentType = _apiDb->getFullTypeName(name);
    } else if (!_apiDb->getReturnTypeForMethodOrFunction(name).isEmpty()) {
      currentType = _apiDb->getReturnTypeForMethodOrFunction(name);
    }
  }

  if (currentType.isEmpty() && funcName.indexOf(".") != -1) {
    QString className = funcName.mid(0, funcName.indexOf("."));
    currentType = getClassAttributeType(className, expr);
  }

  QString cleanExpr = expr;
  int parenLevel = 0;

  for (int i = 0; i < cleanExpr.length(); ++i) {
    if (cleanExpr[i] == '(') {
      parenLevel += 1;
    } else if (cleanExpr[i] == ')') {
      parenLevel -= 1;
    } else if (cleanExpr[i] == '.') {
      if (parenLevel > 0) {
        cleanExpr[i] = '_';
      }
    }
  }

  if (currentType.isEmpty() && cleanExpr.indexOf(".") != -1) {
    QStringList parts = cleanExpr.split(".");
    int i = 0;

    for (const QString &p : parts) {
      if (i == 0) {
        if (_varToType.find(funcName) != _varToType.end()) {
          if (_varToType[funcName].find(p) != _varToType[funcName].end()) {
            currentType = _varToType[funcName][p];
            ++i;
            continue;
          }
        }

        if (_apiDb->getDictContentForType(p).count() > 0) {
          currentType = p;
        } else if (!_apiDb->getFullTypeName(p).isEmpty() &&
                   _apiDb->getDictContentForType(_apiDb->getFullTypeName(p)).count() > 0) {
          currentType = _apiDb->getFullTypeName(p);
        } else if (_varToType["global"].find(p) != _varToType["global"].end()) {
          currentType = _varToType["global"][p];
        }

        if (currentType.isEmpty()) {
          currentType = PythonInterpreter::getInstance()->getVariableType(p);
        }

      } else {
        if (p.indexOf('(') != -1 && p.indexOf(')') != -1) {
          QString func = p.mid(0, p.indexOf('('));

          currentType = getReturnTypeForMethodOrFunction(currentType, func);

          if (!_apiDb->getFullTypeName(currentType).isEmpty()) {
            currentType = _apiDb->getFullTypeName(currentType);
          }
        } else if (_apiDb->getDictContentForType(currentType + "." + p).count() > 0) {
          currentType = currentType + "." + p;
        } else if (_apiDb->dictEntryExists(currentType, p)) {
          currentType = currentType + "." + p;
        } else {
          currentType = "";
        }
      }

      ++i;

      if (currentType.isEmpty())
        break;
    }
  }

  if (currentType.isEmpty()) {

    if (_varToType.find(funcName) != _varToType.end() &&
        _varToType[funcName].find(expr) != _varToType[funcName].end()) {
      currentType = _varToType[funcName][expr];
    } else if (_apiDb->typeExists(expr)) {
      currentType = expr;
    } else if (!_apiDb->getFullTypeName(expr).isEmpty()) {
      currentType = _apiDb->getFullTypeName(expr);
    } else if (_varToType["global"].find(expr) != _varToType["global"].end()) {
      currentType = _varToType["global"][expr];
    }

    if (currentType.isEmpty()) {
      currentType = PythonInterpreter::getInstance()->getVariableType(expr);
    }
  }

  return currentType;
}

static QVector<PropertyInterface *> getAllGraphPropertiesFromRoot(Graph *root) {
  QVector<PropertyInterface *> ret;
  for (const string &prop : root->getLocalProperties()) {
    ret.append(root->getProperty(prop));
  }
  for (Graph *sg : root->subGraphs()) {
    ret += getAllGraphPropertiesFromRoot(sg);
  }
  return ret;
}

static QSet<QString> getAllSubGraphsNamesFromRoot(Graph *root, const QString &prefix) {
  QSet<QString> ret;
  for (Graph *sg : root->subGraphs()) {
    QString sgName = "\"" + tlpStringToQString(sg->getName()) + "\"";

    if (sgName.startsWith(prefix))
      ret.insert(sgName);

    sgName = "'" + tlpStringToQString(sg->getName()) + "'";

    if (sgName.startsWith(prefix))
      ret.insert(sgName);
  }
  for (Graph *sg : root->subGraphs()) {
    ret += getAllSubGraphsNamesFromRoot(sg, prefix);
  }
  return ret;
}

static QSet<QString> getGraphPropertiesList(Graph *graph, const QString &prefix,
                                            const QString &type = "") {
  QSet<QString> ret;
  QVector<PropertyInterface *> properties = getAllGraphPropertiesFromRoot(graph);

  for (auto prop : properties) {
    if (type.isEmpty() || prop->getTypename() == QStringToTlpString(type)) {
      QString qProp = "\"" + tlpStringToQString(prop->getName()) + "\"";

      if (qProp.startsWith(prefix)) {
        ret.insert(qProp);
      }

      qProp = "'" + tlpStringToQString(prop->getName()) + "'";

      if (qProp.startsWith(prefix)) {
        ret.insert(qProp);
      }
    }
  }

  return ret;
}

QSet<QString>
AutoCompletionDataBase::getPluginParametersListIfContext(const QString &context,
                                                         const QString &editedFunction) const {
  QSet<QString> ret;

  if (_pluginParametersDataSet.find(editedFunction) != _pluginParametersDataSet.end()) {
    int pos = context.lastIndexOf("[");
    QString varName = context.mid(0, pos);
    QString strCollecExpr = "] =";

    int pos2 = context.indexOf(strCollecExpr, pos + 1);

    if (pos != -1 && pos2 == -1) {
      QString prefix = context.mid(pos + 1);

      if (_pluginParametersDataSet[editedFunction].find(varName) !=
          _pluginParametersDataSet[editedFunction].end()) {
        for (const QString &param : _pluginParametersDataSet[editedFunction][varName]) {
          if (param.startsWith(prefix)) {
            ret.insert(param);
          }
        }
      }

    } else if (pos != -1 && pos2 != -1) {
      QString entryName = context.mid(pos + 1, pos2 - pos - 1);

      entryName.replace("\"", "");
      entryName.replace("'", "");
      QString prefix = context.mid(pos2 + strCollecExpr.size());

      if (_pluginParametersDataSet[editedFunction].find(varName) !=
          _pluginParametersDataSet[editedFunction].end()) {
        for (const QString &param : _pluginParametersDataSet[editedFunction][varName]) {
          if (param.indexOf(entryName) != -1 && param.indexOf("tlp.StringCollection") != -1) {
            ret = getStringCollectionEntriesForPlugin(_varToPluginName[editedFunction][varName],
                                                      entryName, prefix);
          }
        }
      }
    }
  }

  return ret;
}

QSet<QString>
AutoCompletionDataBase::getSubGraphsListIfContext(const QString &context,
                                                  const QString &editedFunction) const {

  QString cleanContext = context;
  QSet<QString> ret;

  QStringList sgExprs;
  sgExprs << ".getSubGraph("
          << ".getDescendantGraph(";

  for (int j = 0; j < sgExprs.count(); ++j) {
    if (_graph && cleanContext.lastIndexOf(sgExprs[j]) != -1) {
      int i = 0;

      while (sepChar[i]) {
        if (sepChar[i] != '(' && cleanContext.lastIndexOf(sepChar[i]) != -1) {
          cleanContext = cleanContext.mid(cleanContext.lastIndexOf(sepChar[i]) + 1);
        }

        ++i;
      }

      QString expr = cleanContext.mid(0, cleanContext.lastIndexOf(sgExprs[j]));
      QString type = findTypeForExpr(expr, editedFunction);

      if (type == "tlp.Graph") {
        QString prefix = cleanContext.mid(cleanContext.lastIndexOf(sgExprs[j]) + sgExprs[j].size());
        ret = getAllSubGraphsNamesFromRoot(_graph->getRoot(), prefix);
      }

      break;
    }
  }

  return ret;
}

static QSet<QString> getAllGraphsAttributesFromRoot(Graph *rootGraph, const QString &prefix) {
  QSet<QString> ret;

  for (const pair<string, DataType *> &entry : rootGraph->getAttributes().getValues()) {
    QString attrName = "\"" + tlpStringToQString(entry.first) + "\"";

    if (attrName.startsWith(prefix))
      ret.insert(attrName);

    attrName = "'" + tlpStringToQString(entry.first) + "'";

    if (attrName.startsWith(prefix))
      ret.insert(attrName);
  }

  for (Graph *sg : rootGraph->subGraphs()) {
    ret += getAllGraphsAttributesFromRoot(sg, prefix);
  }
  return ret;
}

QSet<QString>
AutoCompletionDataBase::getGraphsAttributesListIfContext(const QString &context,
                                                         const QString &editedFunction) const {

  QString cleanContext = context;
  QSet<QString> ret;

  QString sgExpr = ".getAttribute(";

  if (_graph && cleanContext.lastIndexOf(sgExpr) != -1) {
    int i = 0;

    while (sepChar[i]) {
      if (sepChar[i] != '(' && cleanContext.lastIndexOf(sepChar[i]) != -1) {
        cleanContext = cleanContext.mid(cleanContext.lastIndexOf(sepChar[i]) + 1);
      }

      ++i;
    }

    QString expr = cleanContext.mid(0, cleanContext.lastIndexOf(sgExpr));
    QString type = findTypeForExpr(expr, editedFunction);

    if (type == "tlp.Graph") {
      QString prefix = cleanContext.mid(cleanContext.lastIndexOf(sgExpr) + sgExpr.size());
      ret = getAllGraphsAttributesFromRoot(_graph->getRoot(), prefix);
    }
  }

  return ret;
}

QSet<QString>
AutoCompletionDataBase::getGraphPropertiesListIfContext(const QString &context,
                                                        const QString &editedFunction) const {

  QString cleanContext = context;
  QSet<QString> ret;

  if (_graph && cleanContext.lastIndexOf("[") != -1) {
    int i = 0;

    while (sepChar[i]) {
      if (sepChar[i] != '[' && cleanContext.lastIndexOf(sepChar[i]) != -1) {
        cleanContext = cleanContext.mid(cleanContext.lastIndexOf(sepChar[i]) + 1);
      }

      ++i;
    }

    QString expr = cleanContext.mid(0, cleanContext.lastIndexOf("["));
    QString type = findTypeForExpr(expr, editedFunction);

    if (type == "tlp.Graph") {
      QString prefix = cleanContext.mid(cleanContext.lastIndexOf("[") + 1);
      ret = getGraphPropertiesList(_graph->getRoot(), prefix);
    }
  } else if (_graph && cleanContext.lastIndexOf("(") != -1) {
    int i = 0;

    while (sepChar[i]) {
      if (sepChar[i] != '(' && cleanContext.lastIndexOf(sepChar[i]) != -1) {
        cleanContext = cleanContext.mid(cleanContext.lastIndexOf(sepChar[i]) + 1);
      }

      ++i;
    }

    QString expr = cleanContext.mid(0, cleanContext.lastIndexOf("("));
    QString prefix = cleanContext.mid(cleanContext.lastIndexOf("(") + 1);
    QString type = findTypeForExpr(expr, editedFunction);

    if (type == "tlp.Graph.getBooleanProperty" || type == "tlp.Graph.getLocalBooleanProperty") {
      ret = getGraphPropertiesList(_graph->getRoot(), prefix, "bool");
    }

    if (type == "tlp.Graph.getBooleanVectorProperty" ||
        type == "tlp.Graph.getLocalBooleanVectorProperty") {
      ret = getGraphPropertiesList(_graph->getRoot(), prefix, "vector<bool>");
    }

    if (type == "tlp.Graph.getColorProperty" || type == "tlp.Graph.getLocalColorProperty") {
      ret = getGraphPropertiesList(_graph->getRoot(), prefix, "color");
    }

    if (type == "tlp.Graph.getColorVectorProperty" ||
        type == "tlp.Graph.getLocalColorVectorProperty") {
      ret = getGraphPropertiesList(_graph->getRoot(), prefix, "vector<color>");
    }

    if (type == "tlp.Graph.getDoubleProperty" || type == "tlp.Graph.getLocalDoubleProperty") {
      ret = getGraphPropertiesList(_graph->getRoot(), prefix, "double");
    }

    if (type == "tlp.Graph.getDoubleVectorProperty" ||
        type == "tlp.Graph.getLocalDoubleVectorProperty") {
      ret = getGraphPropertiesList(_graph->getRoot(), prefix, "vector<double>");
    }

    if (type == "tlp.Graph.getGraphProperty" || type == "tlp.Graph.getLocalGraphProperty") {
      ret = getGraphPropertiesList(_graph->getRoot(), prefix, "graph");
    }

    if (type == "tlp.Graph.getIntegerProperty" || type == "tlp.Graph.getLocalIntegerProperty") {
      ret = getGraphPropertiesList(_graph->getRoot(), prefix, "int");
    }

    if (type == "tlp.Graph.getIntegerVectorProperty" ||
        type == "tlp.Graph.getLocalIntegerVectorProperty") {
      ret = getGraphPropertiesList(_graph->getRoot(), prefix, "vector<int>");
    }

    if (type == "tlp.Graph.getLayoutProperty" || type == "tlp.Graph.getLocalLayoutProperty") {
      ret = getGraphPropertiesList(_graph->getRoot(), prefix, "layout");
    }

    if (type == "tlp.Graph.getCoordVectorProperty" ||
        type == "tlp.Graph.getLocalCoordVectorProperty") {
      ret = getGraphPropertiesList(_graph->getRoot(), prefix, "vector<coord>");
    }

    if (type == "tlp.Graph.getSizeProperty" || type == "tlp.Graph.getLocalSizeProperty") {
      ret = getGraphPropertiesList(_graph->getRoot(), prefix, "size");
    }

    if (type == "tlp.Graph.getSizeVectorProperty" ||
        type == "tlp.Graph.getLocalSizeVectorProperty") {
      ret = getGraphPropertiesList(_graph->getRoot(), prefix, "vector<size>");
    }

    if (type == "tlp.Graph.getStringProperty" || type == "tlp.Graph.getLocalStringProperty") {
      ret = getGraphPropertiesList(_graph->getRoot(), prefix, "string");
    }

    if (type == "tlp.Graph.getStringVectorProperty" ||
        type == "tlp.Graph.getLocalStringVectorProperty") {
      ret = getGraphPropertiesList(_graph->getRoot(), prefix, "vector<string>");
    }
  }

  return ret;
}

static QSet<QString> getAlgorithmPluginsListOfType(const QString &type, const QString &prefix) {
  QSet<QString> ret;
  list<string> pluginNames = PluginLister::availablePlugins();

  for (list<string>::iterator it = pluginNames.begin(); it != pluginNames.end(); ++it) {
    Plugin *plugin = PluginLister::getPluginObject(*it);

    if (plugin->category() != GLYPH_CATEGORY && plugin->category() != EEGLYPH_CATEGORY &&
        plugin->category() != INTERACTOR_CATEGORY && plugin->category() != VIEW_CATEGORY &&
        plugin->category() != PERSPECTIVE_CATEGORY) {

      if (type.isEmpty() || plugin->category() == QStringToTlpString(type)) {
        QString pluginName = "\"" + tlpStringToQString(*it) + "\"";

        if (pluginName.startsWith(prefix)) {
          ret.insert(pluginName);
        }

        pluginName = "'" + tlpStringToQString(*it) + "'";

        if (pluginName.startsWith(prefix)) {
          ret.insert(pluginName);
        }
      }
    }

    delete plugin;
  }

  return ret;
}

static QSet<QString> tryAlgorithmContext(const QString &context, const QString &algoContext,
                                         const QString &algoType = "") {
  QSet<QString> ret;

  if (context.indexOf(algoContext) != -1) {
    int pos1 = context.indexOf(algoContext);
    int pos2 = pos1 + algoContext.length();

    if (context.indexOf(",", pos2) == -1) {
      QString prefix = context.mid(pos2);
      ret = getAlgorithmPluginsListOfType(algoType, prefix);
    }
  }

  return ret;
}

static QSet<QString> getTulipAlgorithmListIfContext(const QString &context) {
  QSet<QString> ret;
  ret = tryAlgorithmContext(context, ".applyAlgorithm(", ALGORITHM_CATEGORY.c_str());

  if (!ret.empty()) {
    return ret;
  }

  ret = tryAlgorithmContext(context, ".importGraph(", IMPORT_CATEGORY.c_str());

  if (!ret.empty()) {
    return ret;
  }

  ret = tryAlgorithmContext(context, ".exportGraph(", EXPORT_CATEGORY.c_str());

  if (!ret.empty()) {
    return ret;
  }

  ret = tryAlgorithmContext(context, ".getDefaultPluginParameters(");

  if (!ret.empty()) {
    return ret;
  }

  ret =
      tryAlgorithmContext(context, ".computeBooleanProperty(", BOOLEAN_ALGORITHM_CATEGORY.c_str());

  if (!ret.empty()) {
    return ret;
  }

  ret = tryAlgorithmContext(context, ".applyBooleanAlgorithm(", BOOLEAN_ALGORITHM_CATEGORY.c_str());

  if (!ret.empty()) {
    return ret;
  }

  ret = tryAlgorithmContext(context, ".computeColorProperty(", COLOR_ALGORITHM_CATEGORY.c_str());

  if (!ret.empty()) {
    return ret;
  }

  ret = tryAlgorithmContext(context, ".applyColorAlgorithm(", COLOR_ALGORITHM_CATEGORY.c_str());

  if (!ret.empty()) {
    return ret;
  }

  ret = tryAlgorithmContext(context, ".computeDoubleProperty(", DOUBLE_ALGORITHM_CATEGORY.c_str());

  if (!ret.empty()) {
    return ret;
  }

  ret = tryAlgorithmContext(context, ".applyDoubleAlgorithm(", DOUBLE_ALGORITHM_CATEGORY.c_str());

  if (!ret.empty()) {
    return ret;
  }

  ret =
      tryAlgorithmContext(context, ".computeIntegerProperty(", INTEGER_ALGORITHM_CATEGORY.c_str());

  if (!ret.empty()) {
    return ret;
  }

  ret = tryAlgorithmContext(context, ".applyIntegerAlgorithm(", INTEGER_ALGORITHM_CATEGORY.c_str());

  if (!ret.empty()) {
    return ret;
  }

  ret = tryAlgorithmContext(context, ".computeLayoutProperty(", LAYOUT_ALGORITHM_CATEGORY.c_str());

  if (!ret.empty()) {
    return ret;
  }

  ret = tryAlgorithmContext(context, ".applyLayoutAlgorithm(", LAYOUT_ALGORITHM_CATEGORY.c_str());

  if (!ret.empty()) {
    return ret;
  }

  ret = tryAlgorithmContext(context, ".computeSizeProperty(", SIZE_ALGORITHM_CATEGORY.c_str());

  if (!ret.empty()) {
    return ret;
  }

  ret = tryAlgorithmContext(context, ".applySizeAlgorithm(", SIZE_ALGORITHM_CATEGORY.c_str());

  if (!ret.empty()) {
    return ret;
  }

  ret = tryAlgorithmContext(context, ".computeStringProperty(", STRING_ALGORITHM_CATEGORY.c_str());

  if (!ret.empty()) {
    return ret;
  }

  ret = tryAlgorithmContext(context, ".applyStringAlgorithm(", STRING_ALGORITHM_CATEGORY.c_str());

  return ret;
}

QSet<QString> AutoCompletionDataBase::getAllDictForType(const QString &type, const QString &prefix,
                                                        const bool root) const {
  QSet<QString> ret;
  ret = _apiDb->getDictContentForType(type, prefix);

  if (!root) {
    for (const QString &entry : ret) {
      if (entry[0].isUpper()) {
        ret.remove(entry);
      }
    }
  }

  QVector<QString> baseTypes = PythonInterpreter::getInstance()->getBaseTypesForType(type);

  for (int i = 0; i < baseTypes.size(); ++i) {
    QString baseType = baseTypes[i];
    baseType.replace("_tulipgui", "tlpgui");
    baseType.replace("_tulip", "tlp");

    if (baseType != type)
      ret += getAllDictForType(baseType, prefix, false);
  }

  if (_classContents.find(type) != _classContents.end()) {
    for (const QString &entry : _classContents[type]) {
      if (entry.toLower().startsWith(prefix.toLower()))
        ret.insert(entry);
    }
  }

  if (_classBases.find(type) != _classBases.end()) {
    for (const QString &baseType : _classBases[type]) {
      if (baseType != type)
        ret += getAllDictForType(baseType, prefix, false);
    }
  }

  return ret;
}

QSet<QString> AutoCompletionDataBase::getAutoCompletionListForContext(const QString &context,
                                                                      const QString &editedFunction,
                                                                      bool dotContext) {

  _lastFoundType = "";

  QSet<QString> ret;

  QString cleanContext = context;

  if (cleanContext.startsWith(">>> ") || cleanContext.startsWith("... ")) {
    cleanContext.replace(0, 4, "");
  }

  ret = getTulipAlgorithmListIfContext(cleanContext);

  if (!ret.empty()) {
    return ret;
  }

  ret = getGraphPropertiesListIfContext(cleanContext, editedFunction);

  if (!ret.empty()) {
    return ret;
  }

  ret = getPluginParametersListIfContext(cleanContext, editedFunction);

  if (!ret.empty()) {
    return ret;
  }

  ret = getSubGraphsListIfContext(cleanContext, editedFunction);

  if (!ret.empty()) {
    return ret;
  }

  ret = getGraphsAttributesListIfContext(cleanContext, editedFunction);

  if (!ret.empty()) {
    return ret;
  }

  int i = 0;

  while (sepChar[i]) {
    int pos = cleanContext.lastIndexOf(sepChar[i]);

    if (sepChar[i] == '(' && pos != -1 && cleanContext.indexOf(')', pos + 1) != -1) {
      ++i;
      continue;
    }

    if (pos != -1) {
      if ((!(sepChar[i] == '[' && pos < cleanContext.size() - 1 && cleanContext[pos + 1] == '"')) &&
          (!(sepChar[i] == '[' && cleanContext.indexOf(']', pos + 1) != -1))) {
        int nbQuotes = 0;
        int nbDblQuotes = 0;

        for (int j = pos - 1; j >= 0; --j) {
          if (cleanContext[j] == '\'') {
            ++nbQuotes;
          }

          if (cleanContext[j] == '"') {
            ++nbDblQuotes;
          }
        }

        if (nbQuotes % 2 == 0 && nbDblQuotes % 2 == 0) {
          cleanContext = cleanContext.mid(cleanContext.lastIndexOf(sepChar[i]) + 1);
        }
      }
    }

    ++i;
  }

  if (cleanContext.indexOf('.') == -1) {

    for (const QString &s : _globalAutoCompletionList) {
      if (s.toLower().startsWith(cleanContext.toLower())) {
        ret.insert(s);
      }
    }

    if (_functionAutoCompletionList.find(editedFunction) != _functionAutoCompletionList.end()) {
      for (const QString &s : _functionAutoCompletionList[editedFunction]) {
        if (s.toLower().startsWith(cleanContext.toLower())) {
          ret.insert(s);
        }
      }
    }

  } else {

    QString expr = cleanContext.mid(0, cleanContext.lastIndexOf('.'));
    QString type = findTypeForExpr(expr, editedFunction);
    QString prefix = cleanContext.mid(cleanContext.lastIndexOf('.') + 1);

    if (type.startsWith("list")) {
      type = "list";
    } else if (type.startsWith("dict")) {
      type = "dict";
    }

    if (!type.isEmpty()) {
      _lastFoundType = type;
      ret = getAllDictForType(type, prefix);

      if (type != "tlp" &&
          ((_varToType.find(editedFunction) != _varToType.end() &&
            _varToType[editedFunction].find(expr) != _varToType[editedFunction].end()) ||
           (!_apiDb->typeExists(expr) && _apiDb->getFullTypeName(expr).isEmpty()))) {
        for (const QString &entry : ret) {
          if (entry[0].isUpper()) {
            ret.remove(entry);
          }
        }
      }
    } else if (!dotContext) {
      ret = _apiDb->getAllDictEntriesStartingWithPrefix(prefix);

      for (const QString &s : _globalAutoCompletionList) {
        if (s.toLower().startsWith(prefix.toLower())) {
          ret.insert(s);
        }
      }
    }
  }

  return ret;
}

QVector<QVector<QString>>
AutoCompletionDataBase::getParamTypesForMethodOrFunction(const QString &type,
                                                         const QString &funcName) const {
  QString fullName = type + "." + funcName;
  QVector<QVector<QString>> ret = _apiDb->getParamTypesForMethodOrFunction(fullName);
  QVector<QString> baseTypes = PythonInterpreter::getInstance()->getBaseTypesForType(type);

  for (int i = 0; i < baseTypes.size(); ++i) {
    QString baseType = baseTypes[i];
    baseType.replace("_tulipgui", "tlpgui");
    baseType.replace("_tulip", "tlp");

    if (baseType != type)
      ret += getParamTypesForMethodOrFunction(baseType, funcName);
  }

  if (_classBases.find(type) != _classBases.end()) {
    for (const QString &baseType : _classBases[type]) {
      if (baseType != type)
        ret += getParamTypesForMethodOrFunction(baseType, funcName);
    }
  }

  return ret;
}

QString AutoCompletionDataBase::getReturnTypeForMethodOrFunction(const QString &type,
                                                                 const QString &funcName) const {
  QString fullName = type + "." + funcName;
  QString ret = _apiDb->getReturnTypeForMethodOrFunction(fullName);

  if (ret.isEmpty()) {
    QVector<QString> baseTypes = PythonInterpreter::getInstance()->getBaseTypesForType(type);

    for (int i = 0; i < baseTypes.size(); ++i) {
      QString baseType = baseTypes[i];
      baseType.replace("_tulipgui", "tlpgui");
      baseType.replace("_tulip", "tlp");

      if (baseType != type)
        ret = getReturnTypeForMethodOrFunction(baseType, funcName);

      if (!ret.isEmpty()) {
        break;
      }
    }
  }

  if (ret.isEmpty()) {
    if (_classBases.find(type) != _classBases.end()) {
      for (const QString &baseType : _classBases[type]) {
        if (baseType != type)
          ret = getReturnTypeForMethodOrFunction(baseType, funcName);

        if (!ret.isEmpty()) {
          break;
        }
      }
    }
  }

  return ret;
}

QString AutoCompletionDataBase::getClassAttributeType(const QString &className,
                                                      const QString &classAttribute) const {
  if (_classAttributeToType.find(className) != _classAttributeToType.end()) {
    if (_classAttributeToType[className].find(classAttribute) !=
        _classAttributeToType[className].end()) {
      return _classAttributeToType[className][classAttribute];
    }
  }

  if (_classBases.find(className) != _classBases.end()) {
    for (const QString &baseType : _classBases[className]) {
      if (baseType != className) {
        QString ret = getClassAttributeType(baseType, classAttribute);

        if (!ret.isEmpty()) {
          return ret;
        }
      }
    }
  }

  return "";
}
