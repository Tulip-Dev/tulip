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
#include "SpreadCalculator.h"

#include <iostream> //NOTE: for debugging
#include <stdexcept>
using std::runtime_error;

#include <tulip/ForEach.h>

#include "SpreadTable.h"

using namespace std;

namespace tlp {

SpreadCalculator* SpreadCalculator::_calculator = 0;

SpreadCalculator* SpreadCalculator::calculator()
{
	if (!_calculator)
		_calculator = new SpreadCalculator;
	return _calculator;
}

bool SpreadCalculator::isFormula(const QString &formula)
{
	if (formula.isEmpty())
		return false;
	else if (formula[0] != '=')
		return false;
	else
		return true;
}

QString SpreadCalculator::calculate(SpreadTable*, QString &formula) {
	if (!isFormula(formula))
		return formula;
	formula = formula.toUpper();
	formula.append(QChar(QChar::Null));

	int iterator = 1; //first character is '='
	QString errorMessage;
	int errorPosition;
	errorPosition = checkSyntax(formula, errorMessage, iterator);
	if (errorPosition >= 0) {
		cout << (errorMessage + " at position " + QString::number(errorPosition+1)).toStdString() << endl;
		return errorMessage + " at position " + QString::number(errorPosition+1);
	}else if (errorPosition == -2){ //unknown error position*
		cout << errorMessage.toStdString() << endl;
		return errorMessage;
	}

	deleteSpaces(formula);
	QString answer;
	try
	{
		answer = calculateSentences(formula, iterator).toString();
		cout << answer.toStdString() << endl;
	}
	catch (runtime_error &error)
	{
		answer = error.what();
		cout << answer.toStdString() << endl;
	}
	return answer;
}

QString SpreadCalculator::calculate(SpreadTable *t, int r, int c)
{
	table = t;
	currentRow = r;
	currentColumn = c;

	QString formula = table -> getItemText(currentRow, currentColumn, Qt::EditRole);
	return calculate(t,formula);
}

SpreadValue SpreadCalculator::calculateSentences(const QString &formula, int &iterator)
{
	cout << formula.toStdString() << endl;
	SpreadValue sumOfSentences;
	QChar sentencesOperator = '+';
	SpreadValue currentSentence;

	while(1)
	{
		currentSentence = calculateASentence(formula, iterator);
		cout << "currentSentence :" << currentSentence.toString().toStdString() << endl;
		sumOfSentences=SpreadValue::computeOpp2Value(sentencesOperator,sumOfSentences,currentSentence);
		/*if (sentencesOperator == '+')
	  sumOfSentences += currentSentence;
	else  // sentencesOperator == '-'
	sumOfSentences -= currentSentence;*/

		if (formula[iterator] == QChar::Null)
			return sumOfSentences;
		if (formula[iterator] == ')')
		{
			iterator++;
			return sumOfSentences;
		}

		sentencesOperator = formula[iterator++];
	}
}

SpreadValue SpreadCalculator::calculateASentence(const QString &formula, int &iterator)
{
	SpreadValue answer;
	SpreadValue operand;
	QChar currentSentenceOperator;

	answer = evaluateFactor(formula, iterator);
	cout << "calculateASentence answer : " << answer.toString().toStdString() << endl;
	while (formula[iterator] != QChar::Null)
	{
		currentSentenceOperator = formula[iterator];
		cout << "currentSentenceOperator : " << QString(formula[iterator]).toStdString() << endl;
		if (currentSentenceOperator != '*' && currentSentenceOperator != '/')
			return answer;

		iterator++;
		operand = evaluateFactor(formula, iterator);

		if (currentSentenceOperator == '*')
			answer=SpreadValue::computeOpp2Value('*',answer,operand);
		else //currentSentenceOperator == '/'
		{
			/*if (operand == 0)
	      throw runtime_error("Error: Divide by zero");
	      else*/
			answer=SpreadValue::computeOpp2Value('/',answer,operand);
		}
	}

	return answer;
}

SpreadValue SpreadCalculator::evaluateFactor(const QString &formula, int &iterator)
{
	int lastIterator = iterator;
	if (isNumber(formula, iterator) || isVector(formula, iterator))
		return SpreadValue(formula.mid(lastIterator, iterator-lastIterator));

	else if (isVariable(formula, iterator))
		return valueOfCell(formula.mid(lastIterator, iterator-lastIterator));

	else if (isRange(formula, iterator))
		return valueOfCell(formula.mid(lastIterator,
				formula.indexOf(':', lastIterator)-lastIterator));

	else if (isOpenParenthesis(formula, iterator))
		return calculateSentences(formula, iterator);

	else if (isFunction(formula, iterator))
		return evaluateFunction(formula.mid(lastIterator,
				iterator-lastIterator)+QChar(QChar::Null));

	else //should not happen
		throw runtime_error("Unknown factor");
}

SpreadValue SpreadCalculator::evaluateFunction(const QString &function)
{
	QString functionName;
	QStringList stringArgs;
	QList<SpreadValue> valueArgs;
	int iterator = 0;

	while(function[iterator] != '(')
		functionName += function[iterator++];
	iterator++; //ignore '('

	while (function[iterator] != QChar::Null){
		QString curArgument = getNextArgumentOfFunction(function, iterator);
		if (!curArgument.isEmpty())
			stringArgs.append(curArgument);
	}
	for (int i=0; i<stringArgs.size(); i++){
		int j=0;
		valueArgs.append(calculateSentences(stringArgs[i]+QChar(QChar::Null), j));
	}

	if(functionName == "ROW" || functionName == "COLUMN" || functionName == "PI") {
		if (functionName == "ROW")
			return func_row();
		else if (functionName == "COLUMN")
			return func_column();
		else if (functionName == "PI")
			return func_pi();
	}

	if(functionName == "SIN" || functionName == "COS" || functionName == "TAN" || functionName == "COT" || functionName == "FABS" || functionName == "LOG" || functionName == "LOG10" || functionName == "SQRT" || functionName == "POW") {
		QList<double> input;
		QList<double> output;
		SpreadValue::value2List(valueArgs[0],input);

		if (functionName == "SIN")
			func_sin(input,output);
		else if (functionName == "COS")
			func_cos(input,output);
		else if (functionName == "TAN")
			func_tan(input,output);
		else if (functionName == "COT")
			func_cot(input,output);
		else if (functionName == "FABS")
			func_fabs(input,output);
		else if (functionName == "LOG")
			func_log(input,output);
		else if (functionName == "LOG10")
			func_log10(input,output);
		else if (functionName == "SQRT")
			func_sqrt(input,output);

		SpreadValue answer;
		SpreadValue::list2Value(output,0,valueArgs[0].valueType,answer);
		return answer;
	}

	if(functionName == "POW") {
		QList<double> input1;
		QList<double> input2;
		QList<double> output;

		SpreadValue::value2List(valueArgs[0],input1);
		SpreadValue::value2List(valueArgs[1],input2);

		func_pow(input1,input2,output);

		SpreadValue answer;
		SpreadValue::list2Value(output,0,valueArgs[0].valueType,answer);
		return answer;
	}

	if(functionName == "SUM" || functionName == "PRODUCT" || functionName == "MIN" || functionName == "MAX" || functionName == "AVERAGE" || functionName == "MEDIAN") {
		QList<QList<double> > input;
		QList<double> output;

		SpreadValue::valueList2List(valueArgs,input);

		if (functionName == "SUM")
			func_sum(input,output);
		else if (functionName == "PRODUCT")
			func_product(input,output);
		else if (functionName == "MIN")
			func_min(input,output);
		else if (functionName == "MAX")
			func_max(input,output);
		else if (functionName == "AVERAGE")
			func_average(input,output);
		else if (functionName == "MEDIAN")
			func_median(input,output);

		SpreadValue answer;
		SpreadValue::list2Value(output,0,valueArgs[0].valueType,answer);
		return answer;
	}

	/*if (functionName == "SIN")
      answer = func_sin(doubleArgs[0]);
    else if (functionName == "COS")
      answer = func_cos(doubleArgs[0]);
    else if (functionName == "TAN")
      answer = func_tan(doubleArgs[0]);
    else if (functionName == "COT")
      answer = func_cot(doubleArgs[0]);
    else if (functionName == "FABS")
      answer = func_fabs(doubleArgs[0]);
    else if (functionName == "LOG")
      answer = func_log(doubleArgs[0]);
    else if (functionName == "LOG10")
      answer = func_log10(doubleArgs[0]);
    else if (functionName == "SQRT")
      answer = func_sqrt(doubleArgs[0]);
    else if (functionName == "POW")
      answer = func_pow(doubleArgs[0], doubleArgs[1]);
    else if (functionName == "ROW")
      answer = func_row();
    else if (functionName == "COLUMN")
      answer = func_column();
    else if (functionName == "PI")
      answer = func_pi();
    else if (functionName == "SUM")
      answer = func_sum(stringArgs, doubleArgs);
    else if (functionName == "PRODUCT")
      answer = func_product(stringArgs, doubleArgs);
    else if (functionName == "MIN")
      answer = func_min(stringArgs, doubleArgs);
    else if (functionName == "MAX")
      answer = func_max(stringArgs, doubleArgs);
    else if (functionName == "AVERAGE")
      answer = func_average(stringArgs, doubleArgs);
    else if (functionName == "MEDIAN")
      answer = func_median(stringArgs, doubleArgs);
    else if (functionName == "MODE")
      answer = func_mode(stringArgs, doubleArgs);

    else //should not happen
    throw runtime_error("unknown function "+functionName.toStdString());*/

	return SpreadValue();
}

SpreadValue SpreadCalculator::valueOfCell(const QString &locationOfOtherCell)
{
	int otherCellRow, otherCellColumn;
	SpreadTable::decodePosition(locationOfOtherCell, otherCellRow, otherCellColumn);
	QString otherCellFormula = table -> getItemText(otherCellRow, otherCellColumn, Qt::EditRole);
	otherCellFormula.append(QChar(QChar::Null));

	//if the formula of cell "A1" is using the value of cell "A2" and
	//the formula of cell "A2" is using the value of cell "A1",
	//an infinite loop will occur... So:
	if (isFormula(otherCellFormula))
	{
		QString currentCellLocation;
		SpreadTable::encodePosition(currentCellLocation, currentRow, currentColumn);
		int i = otherCellFormula.indexOf(currentCellLocation, 0, Qt::CaseInsensitive);
		if (i != -1 && !otherCellFormula[i-1].isLetter()
				&& !otherCellFormula[i+currentCellLocation.length()].isDigit())
		{
			throw runtime_error("Error: This formula makes an infinite loop");
		}
	}

	//QString::toDouble() returns 0 if conversion fails
	return SpreadValue(table->getItemText(otherCellRow, otherCellColumn));
	//return table -> getItemText(otherCellRow, otherCellColumn).toDouble();
}

int SpreadCalculator::checkSyntax(const QString &formula, QString &errorMessage, int iterator)
{
	int parenthesis = countParenthesis(formula);
	if (parenthesis == -1)
	{
		errorMessage = "Syntax error: Missing parenthesis";
		return -2; //unknown position
	}
	else if (parenthesis > 0)
	{
		errorMessage = "Syntax error";
		return parenthesis;
	}

	while (1)
	{
		ignoreSpaces(formula, iterator);
		if(!isVector(formula, iterator)) {
			while(isOpenParenthesis(formula, iterator))
				ignoreSpaces(formula, iterator);

			if (formula[iterator] == QChar::Null
					|| (!isVariable(formula, iterator)
							&& !isNumber(formula, iterator)
							&& !isVector(formula, iterator)
							&& !isRange(formula, iterator))) {
				int lastIterator = iterator;
				if (isFunction(formula, iterator)) {
					QString function = formula.mid(lastIterator, iterator-lastIterator)
				+ QChar(QChar::Null);
					int errorPosition = checkFunctionSyntax(function, errorMessage);
					if (errorPosition != -1)
						return errorPosition == -2 ? -2 : errorPosition+lastIterator;
				} else {
					errorMessage = "Syntax error";
					return iterator;
				}
			}

		}
		ignoreSpaces(formula, iterator);
		while(isCloseParenthesis(formula, iterator))
			ignoreSpaces(formula, iterator);

		if (formula[iterator] == QChar::Null)
			break;
		if (!isOperator(formula, iterator))
		{
			errorMessage = "Syntax error";
			return iterator;
		}
	}

	return -1; //successful
}

QString SpreadCalculator::modifyPosition(const QString &formula,int baseRow, int baseColumn, int newRow, int newColumn) {
	QString result;
	int i=0;
	for(int iterator=0;iterator<formula.size();) {
		if(isVariable(formula,iterator)) {
			int x,y;
			QString newPosition;
			SpreadTable::decodePosition(formula.mid(i,iterator-i), x, y);
			x+=newRow-baseRow;
			y+=newColumn-baseColumn;
			SpreadTable::encodePosition(newPosition, x, y);
			result.push_back(newPosition);
		}else{
			result.push_back(formula[i]);
			iterator++;
		}
		i=iterator;
	}
	return result;
}

int SpreadCalculator::checkFunctionSyntax(const QString &function, QString &errorMessage)
{
	QString functionName;
	QStringList arguments;
	int iterator = 0;
	while(function[iterator] != '(')
		functionName += function[iterator++];
	ignoreSpaces(function, iterator);
	iterator++; //ignore '('

	while (function[iterator] != QChar::Null)
	{
		int lastIterator = iterator;
		arguments.append(getNextArgumentOfFunction(function, iterator));
		if (arguments.last().isEmpty())
		{
			if (arguments.size() > 1)
			{
				errorMessage = "Syntax error";
				return iterator;
			}
			else
				arguments.clear();
		}
		else
		{
			int errorPosition = checkSyntax(arguments.last()+QChar(QChar::Null), errorMessage, 0);
			if (errorPosition != -1)
				return errorPosition == -2 ? -2 : errorPosition+lastIterator;
		}
	}

	int argumentCount = arguments.size();
	bool rightArguments = false;
	if (functionName == "SIN"
			|| functionName == "COS"
					|| functionName == "TAN"
							|| functionName == "COT"
									|| functionName == "FABS"
											|| functionName == "LOG"
													|| functionName == "LOG10"
															|| functionName == "SQRT")
		rightArguments = argumentCount == 1;

	else if (functionName == "POW")
		rightArguments = argumentCount == 2;

	else if (functionName == "ROW" || functionName == "COLUMN" || functionName == "PI")
		rightArguments = argumentCount == 0;

	else if (functionName == "SUM"
			|| functionName == "PRODUCT"
					|| functionName == "MIN"
							|| functionName == "MAX"
									|| functionName == "AVERAGE"
											|| functionName == "MEDIAN"
													|| functionName == "MODE")
		rightArguments = argumentCount > 0;

	if (rightArguments)
		return -1; //successful
	else
	{
		errorMessage = "Syntax Error: Unknown function " + functionName;
		return 0;
	}
}

QString SpreadCalculator::getNextArgumentOfFunction(const QString &function, int &iterator)
{
	QString argument;
	int parenthesisCount = 1;
	while (function[iterator] != QChar::Null)
	{
		QChar curChar = function[iterator++];
		if (curChar == '(')
			parenthesisCount++;
		else if (curChar == ')')
		{
			parenthesisCount--;
			if (parenthesisCount == 0)
				return argument;
		}
		else if (curChar == ',' && parenthesisCount == 1)
			return argument;
		argument += curChar;
	}

	return argument; //should not reach here
}

void SpreadCalculator::deleteSpaces(QString &formula)
{
	formula.remove(QRegExp("\\s"));
}

void SpreadCalculator::ignoreSpaces(const QString &formula, int &iterator)
{
	while (1)
	{
		if (formula[iterator].isSpace())
			iterator++;
		else
			break;
	}
}

bool SpreadCalculator::isVariable(const QString &formula, int &iterator)
{
	// a variable starts with (at least) one letter
	// and continues with (at least) one digit (e.g "EF23")
	// but if it ends with ')' it's a function
	// and if it ends with ':' it's a range
	int i = iterator;
	int lastI = iterator;

	while(formula[i].isLetter())
		i++;
	if (i == lastI)
		return false;
	lastI = i;
	while(formula[i].isDigit())
		i++;
	if (i==lastI)
		return false;

	ignoreSpaces(formula, i);
	lastI = i;
	if (formula[i] == '(' || formula[i] == ':')
			return false; //it is a function or a range, not a variable

	iterator = lastI;
	return true;
}

bool SpreadCalculator::isRange(const QString &formula, int &iterator)
{
	// A range starts with one variable, then ':', then another variable (e.g. "A1:B1"
	int i = iterator;
	int lastI = iterator;

	while(formula[i].isLetter())
		i++;
	if (i == lastI)
		return false;
	lastI = i;
	while(formula[i].isDigit())
		i++;
	if (i==lastI)
		return false;
	ignoreSpaces(formula, i);
	if (formula[i++] != ':')
		return false;
	ignoreSpaces(formula, i);

	lastI = i;
	while(formula[i].isLetter())
		i++;
	if (i == lastI)
		return false;
	lastI = i;
	while(formula[i].isDigit())
		i++;
	if (i==lastI)
		return false;

	iterator = i;
	return true;
}

bool SpreadCalculator::isNumber(const QString &formula, int &iterator)
{
	//valid number formats:
	//"123", "123.123", "123E+123", "123E-123"
	//it can start with '+' or '-' (or no sign)
	int count = 0;
	int i=iterator;
	int pointPosition = -1;

	if (formula[i] == '-' || formula[i] == '+')
		i++;

	for (;; i++)
	{
		if (formula[i].isDigit())
			count++;
		else if (formula[i] == '.')
		{
			if (pointPosition != -1)
				return false;
			else
				pointPosition = count;
		}
		else
			break;
	}

	if (count==0 || pointPosition==0 || pointPosition==count)
		return false;

	iterator = i;
	if (formula[i] == 'E' && (formula[i+1] == '-' || formula[i+1] == '+'))
	{
		count = 0;
		i+=2;
		while(formula[i].isDigit())
		{
			count++;
			i++;
		}
		if (count)
			iterator = i;
	}

	return true;
}

bool SpreadCalculator::isVector(const QString &formula, int &iterator)
{
	//valid number formats:
	//"123", "123.123", "123E+123", "123E-123"
	//it can start with '+' or '-' (or no sign)
	int count=0;
	int i=iterator;
	int pointPosition = -1;

	if(formula[i] != '(' )
		return false;

	i++;

	for (;; i++) {
		if (formula[i].isDigit()) {
			count++;
		} else if (formula[i] == '.') {
			if (pointPosition != -1)
				return false;
			else
				pointPosition = count;
		} else if (formula[i] == ',') {
			pointPosition=-1;
		} else
			break;
	}

	if (count==0 || pointPosition==0 || pointPosition==count)
		return false;

	iterator = i;

	if(formula[i]!=')')
		return false;

	iterator++;

	return true;
}

bool SpreadCalculator::isFunction(const QString &formula, int &iterator)
{
	int count = 0;
	int i=iterator;

	//first character should be a letter
	if (!formula[i].isLetter())
		return false;

	//letter or number
	for (; formula[i].isLetter() || formula[i].isDigit(); i++)
		count++;
	if (!count)
		return false;

	ignoreSpaces(formula, i);
	if (formula[i++] != '(')
		return false;

	int parenthesisCount = 1;
	for (; parenthesisCount != 0; i++) {
		if (formula[i] == '(')
			parenthesisCount++;
		else if (formula[i] == ')')
			parenthesisCount--;
	}

	iterator = i;
	return true;
}

bool SpreadCalculator::isOperator(const QString &formula, int &iterator)
{
	int i = iterator;
	if (formula[i] == '+'
			|| formula[i] == '-'
					|| formula[i] == '*'
							|| formula[i] == '/')
	{
		iterator++;
		return 1;
	}
	else
		return 0;
}

bool SpreadCalculator::isOpenParenthesis(const QString &formula, int &iterator)
{
	if (formula[iterator] == '(') {
		iterator++;
		return true;
	}
	else
		return false;
}

bool SpreadCalculator::isCloseParenthesis(const QString &formula, int &iterator)
{
	if (formula[iterator] == ')')
	{
		iterator++;
		return true;
	}
	else
		return false;
}

int SpreadCalculator::countParenthesis(const QString &formula)
{
	int parenthesisCount = 0;
	for (int i=0; formula[i] != QChar::Null; i++)
	{
		if (formula[i] == '(')
			parenthesisCount++;
		else if (formula[i] == ')')
			parenthesisCount--;

		if (parenthesisCount < 0)
			return i;
	}

	if (parenthesisCount > 0)
		return -1; //missing parenthesis
	else
		return 0;
}

QList<double> SpreadCalculator::extractData(const QStringList &stringArgs, const QList<double> &doubleArgs)
{
	QList<double> returnList;
	for (int i=0; i<stringArgs.size(); i++)
	{
		int iterator = 0;
		if (isRange(stringArgs[i]+QChar(QChar::Null), iterator))
		{
			if (iterator == stringArgs[i].size()) //if there is only a range in the argument
			{
				QTableWidgetSelectionRange range;
				SpreadTable::decodeRange(stringArgs[i], range);
				for (int row=range.topRow(); row<=range.bottomRow(); row++)
					for (int column=range.leftColumn(); column<=range.rightColumn(); column++)
						returnList.append(table -> getItemText(row, column).toDouble());
				continue;
			}
		}

		returnList.append(doubleArgs[i]);
	}
	return returnList;
}


/// Functions

void SpreadCalculator::func_sin(const QList<double> &arg,QList<double> &output) {
	foreach(double elem,arg) {
		output.append(std::sin(radToDeg(elem)));
	}
}

void SpreadCalculator::func_cos(const QList<double> &arg,QList<double> &output) {
	foreach(double elem,arg) {
		output.append(std::cos(radToDeg(elem)));
	}
}

void SpreadCalculator::func_tan(const QList<double> &arg,QList<double> &output) {
	foreach(double elem,arg) {
		output.append(std::tan(radToDeg(elem)));
	}
}

void SpreadCalculator::func_cot(const QList<double> &arg,QList<double> &output) {
	foreach(double elem,arg) {
		output.append(1/std::tan(radToDeg(elem)));
	}
}

void SpreadCalculator::func_fabs(const QList<double> &arg,QList<double> &output) {
	foreach(double elem,arg) {
		output.append(fabs(elem));
	}
}

void SpreadCalculator::func_log(const QList<double> &arg,QList<double> &output) {
	foreach(double elem,arg) {
		output.append(std::log(elem));
	}
}

void SpreadCalculator::func_log10(const QList<double> &arg,QList<double> &output) {
	foreach(double elem,arg) {
		output.append(std::log10(elem));
	}
}

void SpreadCalculator::func_sqrt(const QList<double> &arg,QList<double> &output) {
	foreach(double elem,arg) {
		output.append(std::sqrt(elem));
	}
}

void SpreadCalculator::func_pow(const QList<double> &arg1,const QList<double> &arg2,QList<double> &output) {
	for(int i=0;i<arg1.size();i++) {
		output.append(std::pow(arg1[i], arg2[i]));
	}
}

double SpreadCalculator::func_row()
{
	return currentRow+1;
}

double SpreadCalculator::func_column()
{
	return currentColumn+1;
}

double SpreadCalculator::func_pi()
{
	return M_PI;
}

void SpreadCalculator::func_sum(const QList<QList<double> > &arg,QList<double> &output)
{
	for(int i=0;i<arg.size();i++) {
		double sum = 0;
		foreach(double elem,arg[i]) {
			sum+=elem;
		}
		output.append(sum);
	}
}

void SpreadCalculator::func_product(const QList<QList<double> > &arg,QList<double> &output)
{
	for(int i=0;i<arg.size();i++) {
		double product = 1;
		foreach(double elem,arg[i]) {
			product*=elem;
		}
		output.append(product);
	}
}

void SpreadCalculator::func_min(const QList<QList<double> > &arg,QList<double> &output)
{
	for(int i=0;i<arg.size();i++) {
		double min = arg[i][0];
		foreach(double elem,arg[i]) {
			if(elem<min)
				min=elem;
		}
		output.append(min);
	}
}

void SpreadCalculator::func_max(const QList<QList<double> > &arg,QList<double> &output)
{
	for(int i=0;i<arg.size();i++) {
		double max = arg[i][0];
		foreach(double elem,arg[i]) {
			if(elem>max)
				max=elem;
		}
		output.append(max);
	}
}

void SpreadCalculator::func_average(const QList<QList<double> > &arg,QList<double> &output)
{
	for(int i=0;i<arg.size();i++) {
		double sum = 0;
		foreach(double elem,arg[i]) {
			sum+=elem;
		}
		output.append(sum/arg[i].size());
	}
}

void SpreadCalculator::func_median(const QList<QList<double> > &arg,QList<double> &output)
{
	for(int i=0;i<arg.size();i++) {
		QList<double> tmp=arg[i];
		qStableSort(tmp);
		if (tmp.size()%2 != 0)
			output.append(tmp[tmp.size()/2]);
		else
			output.append((tmp[tmp.size()/2]+tmp[tmp.size()/2-1])/2);
	}
}

}
