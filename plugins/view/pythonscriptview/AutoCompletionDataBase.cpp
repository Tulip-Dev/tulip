/**
 *
 * This file is part of Tulip (www.tulip-software.org)
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

#include <QtCore/QTextStream>
#include <QtCore/QRegExp>
#include <QtCore/QStringList>

#include <iostream>

#include "AutoCompletionDataBase.h"
#include "PythonInterpreter.h"

using namespace std;

AutoCompletionDataBase::AutoCompletionDataBase(APIDataBase *apiDb) : apiDb(apiDb) {
	iteratorType["tlp.IteratorNode"] = "tlp.node";
	iteratorType["tlp.NodeMapIterator"] = "tlp.node";
	iteratorType["tlp.IteratorEdge"] = "tlp.edge";
	iteratorType["tlp.EdgeMapIterator"] = "tlp.edge";
	iteratorType["tlp.IteratorGraph"] = "tlp.Graph";
	iteratorType["tlp.IteratorString"] = "string";
}

void AutoCompletionDataBase::analyseCurrentScriptCode(const QString &code) {

	globalAutoCompletionList.clear();
	functionAutoCompletionList.clear();

	varToType.clear();
	varToType["global"] = QHash<QString, QString>();
	varToType["global"]["graph"] = "tlp.Graph";
	varToType["global"]["tlp"] = "tlp";

	globalAutoCompletionList.insert("tlp");

	QString codeCp(code);
	QTextStream in(&codeCp);
	QRegExp importRegexp("^import[ ]+[A-Za-z_][A-Za-z0-9_]*.*$");
	QRegExp globalVarRegexp("^global[ ]+[A-Za-z_][A-Za-z0-9_]*.*$");
	QRegExp varAssignRegexp("^[a-zA-Z_][a-zA-Z0-9_]*[ ]*=[ ]*.*$");
	QRegExp forRegexp("^for[ ]+[a-zA-Z_][a-zA-Z0-9_]*[ ]+in[ ]+.*:$");

	QRegExp funcRegexp("^def[ ]+[A-Za-z_][A-Za-z0-9_]+(.*):$");
	QRegExp classRegexp("^class[ ]+[A-Za-z_][A-Za-z0-9_]*.*:$");

	QRegExp methodCallRegexp("[A-Za-z_][A-Za-z0-9_]*\\.[A-Za-z_][A-Za-z0-9_]*\\(.*\\)");

	QString currentClassName = "";
	QString currentFunctionName = "global";

	while (!in.atEnd()) {
		QString origLine = in.readLine();
		QString line = origLine.trimmed();
		if (origLine == "" || origLine.startsWith("#"))
			continue;
		if (!(origLine.startsWith("\t") || origLine.startsWith(" "))) {
			currentClassName = "";
			currentFunctionName = "global";
		}

		QString fullName = currentFunctionName;
		if (currentClassName != "") {
			fullName = currentClassName + "." + fullName;
		}

		if (methodCallRegexp.indexIn(line) != -1) {
			QString expr = line.mid(methodCallRegexp.indexIn(line), line.indexOf('(', methodCallRegexp.indexIn(line)) - methodCallRegexp.indexIn(line));
			QStringList parts = expr.split(".");
			QString varName = parts.at(0);
			QVector<QString> types = apiDb->findTypesContainingDictEntry(parts.at(1));
			if (types.size() == 1) {
				if (varToType.find(fullName) == varToType.end()) {
					varToType[fullName] = QHash<QString, QString>();
				}
				if (varToType[fullName].find(varName) == varToType[fullName].end()) {
					varToType[fullName][varName] = types.at(0);
				}
			}
		}

		if (varAssignRegexp.indexIn(line) != -1) {
			QString varName = line.mid(0, line.indexOf('=')).trimmed();
			QString expr = line.mid(line.indexOf('=')+1).trimmed();

			QString type = findTypeForExpr(expr, fullName);

			if (type == "") {
				if (expr.length() > 1 && expr[0] == '\"' && expr[expr.length() - 1] == '\"') {
					type = "string";
				} else if (expr.length() > 1 && expr[0] == '[' && expr[expr.length() - 1] == ']') {
					type = "list";
				} else if (expr.length() > 1 && expr[0] == '{' && expr[expr.length() - 1] == '}') {
					type = "dict";
				}
			}

			if (type != "") {
				if (varToType.find(fullName) == varToType.end()) {
					varToType[fullName] = QHash<QString, QString>();
				}
				varToType[fullName][varName] = type;
			}

			if (currentFunctionName == "") {
				globalAutoCompletionList.insert(varName);
			} else {
				functionAutoCompletionList[fullName].insert(varName);
			}
		}

		if (forRegexp.indexIn(line) != -1) {
			QString varName = line.mid(4).trimmed();
			varName = varName.mid(0, varName.indexOf(QRegExp("\\bin\\b"))).trimmed();

			QString expr = line.mid(line.indexOf(QRegExp("\\bin\\b"))+3).trimmed();
			expr = expr.mid(0, expr.indexOf(":")).trimmed();

			QString type = findTypeForExpr(expr, fullName);

			if (iteratorType.find(type) != iteratorType.end()) {
				type = iteratorType[type];
				if (type != "") {
					if (varToType.find(fullName) == varToType.end()) {
						varToType[fullName] = QHash<QString, QString>();
					}
					varToType[fullName][varName] = type;
				}

				if (currentFunctionName == "") {
					globalAutoCompletionList.insert(varName);
				} else {
					functionAutoCompletionList[fullName].insert(varName);
				}
			}
		}

		if (funcRegexp.indexIn(line) != -1) {
			QString funcName = line.mid(0, line.indexOf('('));
			funcName = funcName.mid(4, line.indexOf('(')).trimmed();
			currentFunctionName = funcName;
			globalAutoCompletionList.insert(funcName);
			QString fullName = currentFunctionName;
			if (currentClassName != "") {
				fullName = currentClassName + "." + fullName;
			}
			if (functionAutoCompletionList.find(fullName) == functionAutoCompletionList.end()) {
				functionAutoCompletionList[fullName] = QSet<QString>();
			}
			QString params = line.mid(line.indexOf('(')+1, line.indexOf(')') - line.indexOf('(') - 1);
			if (params.indexOf(",") == -1) {
				functionAutoCompletionList[fullName].insert(params.trimmed());
			} else {
				QStringList pList = params.split(",");
				foreach(QString param, pList) {
					functionAutoCompletionList[fullName].insert(param.trimmed());
				}
			}
		}

		if (globalVarRegexp.indexIn(line) != -1) {
			QString varName = line.mid(7).trimmed();
			globalAutoCompletionList.insert(varName);
		}

		if (importRegexp.indexIn(line) != -1) {
			QString varName = line.mid(7).trimmed();
			globalAutoCompletionList.insert(varName);
		}

		if (classRegexp.indexIn(line) != -1) {
			QString className = line;
			if (className.indexOf('(') != -1)
				className = className.mid(6, className.indexOf('(')-6);
			else
				className = className.mid(6, className.indexOf(':')-6);
			currentClassName = "";
			globalAutoCompletionList.insert(className.trimmed());
		}

	}
}

QString AutoCompletionDataBase::findTypeForExpr(const QString &expr, const QString &funcName) {
	QString currentType = "";

	if (expr.indexOf('(') != -1 && expr.indexOf(')') != -1) {
		QString funcName = expr.mid(0, expr.indexOf('('));
		if (!funcName.startsWith("tlp.")) {
			funcName = "tlp." + funcName;
		}
		currentType = apiDb->getReturnTypeForMethodOrFunction(funcName);
	}

	if (currentType == "" && expr.indexOf(".") != -1) {
		QStringList parts = expr.split(".");
		int i = 0;
		foreach(QString p, parts) {
			if (i==0) {
				if (varToType.find(funcName) != varToType.end()) {
					if (varToType[funcName].find(p) != varToType[funcName].end()) {
						currentType = varToType[funcName][p];
						++i;
						continue;
					}
				}
				if (apiDb->getDictContentForType(p).count() > 0) {
					currentType = p;
				} else if (varToType["global"].find(p) != varToType["global"].end()) {
					currentType = varToType["global"][p];
				}
				
				if (currentType == "") {
					currentType = PythonInterpreter::getInstance()->getVariableType(p.toStdString()).c_str();
				}
				
			} else {
				if (p.indexOf('(') != -1 && p.indexOf(')') != -1) {
					QString func = p.mid(0, p.indexOf('('));
					currentType = apiDb->getReturnTypeForMethodOrFunction(currentType + "." + func);
				}
			}
			++i;

			if (currentType == "")
				break;

		}
	} else {
		if (varToType[funcName].find(expr) != varToType[funcName].end()) {
			currentType = varToType[funcName][expr];
		} else if (varToType["global"].find(expr) != varToType["global"].end()) {
			currentType = varToType["global"][expr];
		}
		if (currentType == "") {
			currentType = PythonInterpreter::getInstance()->getVariableType(expr.toStdString()).c_str();
		}
	}

	return currentType;
}

QSet<QString> AutoCompletionDataBase::getAutoCompletionListForContext(const QString &context, const QString &editedFunction) {

	QSet<QString> ret;

	QString cleanContext = context;
	if (cleanContext.lastIndexOf(' ') != -1) {
		cleanContext = cleanContext.mid(cleanContext.lastIndexOf(' ')+1);
	}
	if (cleanContext.lastIndexOf('=') != -1) {
		cleanContext = cleanContext.mid(cleanContext.lastIndexOf('=')+1);
	}

	if (cleanContext.indexOf('.') == -1) {

		foreach(QString s, globalAutoCompletionList) {
			if (s.startsWith(cleanContext)) {
				ret.insert(s);
			}
		}

		if (functionAutoCompletionList.find(editedFunction) != functionAutoCompletionList.end()) {
			foreach(QString s, functionAutoCompletionList[editedFunction]) {
				if (s.startsWith(cleanContext)) {
					ret.insert(s);
				}
			}
		}

	} else {

		QString type = findTypeForExpr(cleanContext.mid(0, cleanContext.lastIndexOf('.')+1), editedFunction);
		QString prefix = cleanContext.mid(cleanContext.lastIndexOf('.')+1);
		QSet<QString> dict;
		if (type != "") {
			ret = apiDb->getDictContentForType(type, prefix);
		} else {
			ret = apiDb->getAllDictEntriesStartingWithPrefix(prefix);
		}
	}

	return ret;
}
