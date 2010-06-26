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
// +-------------------------------------------------------------------------+
// | Tulip Software                                                          |
// | Author:  Morgan Mathiaut                                                |
// +-------------------------------------------------------------------------+
// | License:                                                                |
// |                                                                         |
// | Tulip is free software; you can redistribute it and/or modify           |
// | it under the terms of the GNU General Public License as published by    |
// | the Free Software Foundation; either version 2 of the License, or       |
// | (at your option) any later version.                                     |
// |                                                                         |
// | Tulip is distributed in the hope that it will be useful,                |
// | but WITHOUT ANY WARRANTY; without even the implied warranty of          |
// | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the            |
// | GNU General Public License for more details.                            |
// |                                                                         |
// | You should have received a copy of the GNU General Public License       |
// | along with QSimpleSheet; if not, write to the Free Software             |
// | Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA|
// |                                                                         |
// +-------------------------------------------------------------------------+

// Based on

// +-------------------------------------------------------------------------+
// | QSimpleSheet simple spreadsheet                                         |
// | Copyright (C) 2008 Shahab <shahab.sh.70@gmail.com>                      |
// +-------------------------------------------------------------------------+
// | License:                                                                |
// |                                                                         |
// | QSimpleSheet is free software; you can redistribute it and/or modify    |
// | it under the terms of the GNU General Public License as published by    |
// | the Free Software Foundation; either version 2 of the License, or       |
// | (at your option) any later version.                                     |
// |                                                                         |
// | QSimpleSheet is distributed in the hope that it will be useful,         |
// | but WITHOUT ANY WARRANTY; without even the implied warranty of          |
// | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the            |
// | GNU General Public License for more details.                            |
// |                                                                         |
// | You should have received a copy of the GNU General Public License       |
// | along with QSimpleSheet; if not, write to the Free Software             |
// | Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA|
// |                                                                         |
// +-------------------------------------------------------------------------+
// | Author:  Shahab Shirazi <shahab.sh.70@gmail.com>                        |
// +-------------------------------------------------------------------------+

#ifndef Tulip_SPREADCALCULATOR_H
#define Tulip_SPREADCALCULATOR_H

#include <cmath>
#include <cassert>
#include <vector>
#include <QtCore/QString>

#include <tulip/tulipconf.h>

#include "SpreadValue.h"

namespace tlp {

  class SpreadTable;

  class SpreadCalculator {
  public:
    static SpreadCalculator* calculator();
    //returns true if the string is a formula (a formula should start with '=')
    static bool isFormula(const QString&);
    //receives a Table pointer, row and column number of the cell which is calculating
    //returns the answer of the formula
    QString calculate(SpreadTable*, QString &);
    QString calculate(SpreadTable*, int, int);
    static QString modifyPosition(const QString &,int, int, int, int);
      
  private:
    //receives two string (formula, errorMessage) and an iterator then:
    //returns -1 if there is no syntax error,
    //otherwise returns the position of error (-2 for unknown position)
    //and puts the error message in errorMessage string
    static int checkSyntax(const QString&, QString&, int);
    //receives a string (formula) and an iterator then
    //evaluates all sentences 
    SpreadValue calculateSentences(const QString&, int&);
    //evaluates just a sentence
    SpreadValue calculateASentence(const QString&, int&);
    //evaluates a factor. a factor can be a number, a function, a paranthesis, a variable,...
    SpreadValue evaluateFactor(const QString&, int&);
    //receives a string (function) and returns the answer of the function
    SpreadValue evaluateFunction(const QString&);
    //it receives two strings (function, errorMessage) then:
    //returns -1 if there is no syntax error
    //otherwise returns the position of error (-2 for unknown position)
    //and puts the error message in errorMessage string
    static int checkFunctionSyntax(const QString&, QString&);
    //receives a string (formula) and deletes all spaces
    static void deleteSpaces(QString&);
    //receives a string (formula) and an iterator, then ignores spaces
    static void ignoreSpaces(const QString&, int&);
    //receives a string (formula) and an iterator, then:
    //returns true if it is a variable (like "A1")
    static bool isVariable(const QString&, int&);
    //receives a string (formula) and an iterator, then:
    //returns true if it is a variable (like "A1:B1")
    static bool isRange(const QString&, int&);
    //receives a string (formula) and an iterator, then:
    //returns true if it is a digit (like "123")
    static bool isNumber(const QString&, int&);
    //receives a string (formula) and an iterator, then:
    //returns true if it is a vector (like "(12.5,12,26)")
    static bool isVector(const QString&, int&);
    //receives a string (formula) and an iterator, then:
    //returns true if it is a function (it doesn't check the syntax of the function)
    static bool isFunction(const QString&, int&);
    //receives a string (formula) and an iterator, then:
    //returns true if it is an operator (like "+", '-', "*", '/')
    static bool isOperator(const QString&, int&);
    //receives a string (formula) and an iterator, then:
    //returns true if it is an open parenthesis (like "(" )
    static bool isOpenParenthesis(const QString&, int&);
    //receives a string (formula) and an iterator, then:
    //returns true if it is a close parenthesis (like ")" )
    static bool isCloseParenthesis(const QString&, int&);
    //receives two strings (formula, errorMessage), then:
    //returns 0 if number of '(' characters are equal to number of ')' characters
    //or returns the position of error
    //or returns -1 for parenthesis missing
    static int countParenthesis(const QString&);
    static QString getNextArgumentOfFunction(const QString&, int&);
    //receives a string (location of cell) then
    //returns the value of cell the cell in table.
    SpreadValue valueOfCell(const QString&);
    QList<double> extractData(const QStringList&, const QList<double>&);
      
    double degToRad(double angle)
    { return 180*angle/M_PI;}
    double radToDeg(double angle)
    { return M_PI*angle/180;}
      
    SpreadCalculator()
      { table = 0; currentRow = currentColumn = 0; }
      
    static SpreadCalculator *_calculator;
    SpreadTable *table;
    int currentRow;
    int currentColumn;
      
      
    ///Functions:
    void func_sin(const QList<double> &,QList<double> &);
    void func_cos(const QList<double> &,QList<double> &);
    void func_tan(const QList<double> &,QList<double> &);
    void func_cot(const QList<double> &,QList<double> &);
    void func_fabs(const QList<double> &,QList<double> &);
    void func_log(const QList<double> &,QList<double> &);
    void func_log10(const QList<double> &,QList<double> &);
    void func_sqrt(const QList<double> &,QList<double> &);
    void func_pow(const QList<double> &,const QList<double> &,QList<double> &);
    double func_row();
    double func_column();
    double func_pi();
    void func_sum(const QList<QList<double> > &,QList<double> &);
    void func_product(const QList<QList<double> > &,QList<double> &);
    void func_min(const QList<QList<double> > &,QList<double> &);
    void func_max(const QList<QList<double> > &,QList<double> &);
    void func_average(const QList<QList<double> > &,QList<double> &);
    void func_median(const QList<QList<double> > &,QList<double> &);
    void func_mode(const QList<QList<double> > &,QList<double> &);
  };

}

#endif

