/**
 Author: David Auber
 Email : auber@labri.fr
 Last modification : 20/08/2001
 This program is free software; you can redistribute it and/or modify  *
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/

#include <assert.h>
#include "LabelBooleanProperty.h"
#include <tulip/BooleanProperty.h>
#include <tulip/MethodFactory.h>
#include <qinputdialog.h>
#include <qstring.h>

BOOLEANPLUGIN(LabelSelection,"Label Selection","David Auber","20/01/2003","Alpha","1.0");

using namespace std;
using namespace tlp;



namespace
{

	const char * paramHelp[] =
	{
		// searchStrings
		HTML_HELP_OPEN() \
		HTML_HELP_DEF( "type", "vector<string>" ) \
		HTML_HELP_BODY() \
		"This parameter defines the set of strings to search." \
		HTML_HELP_CLOSE(),

		// searchType
		HTML_HELP_OPEN() \
		HTML_HELP_DEF( "type", "boolean" ) \
		HTML_HELP_DEF( "values", "true/false" ) \
		HTML_HELP_DEF( "default", "false" ) \
		HTML_HELP_BODY() \
		"This parameter defines the search mode to use." \
		"<it>true</it> enables to select the nodes with label matching at least one string in the search set.<br>" \
		"<it>false</it> reverses the comparaison test to enable all non-matching nodes to be selected." \
		HTML_HELP_CLOSE(),

		// searchLabels
		HTML_HELP_OPEN() \
		HTML_HELP_DEF( "type", "String" ) \
		HTML_HELP_BODY() \
		"This parameter defines the String property used as the node's label." \
		HTML_HELP_CLOSE(),
	};

}



LabelSelection::LabelSelection(const PropertyContext &context):BooleanAlgorithm(context) {
  addParameter<vector<string> *> ("searchStrings",paramHelp[0]);
  addParameter<bool>("searchType",paramHelp[1],"false");
  addParameter<String*>("searchLabel",paramHelp[2],"viewLabel");
  searchStrings=0;
  stringProxy=0;
  searchType=false;
}

LabelSelection::~LabelSelection() {
}

bool LabelSelection::getNodeValue(node n) {
  bool result=searchType;
  string currentString=stringProxy->getNodeValue(n);
  for (int i=0;i<searchStrings->size();++i) {
    if (!searchType) {
      if (currentString.find((*searchStrings)[i],0)!=string::npos) {
	result=true;
	break;
      }
    }
    else
      if (currentString.find((*searchStrings)[i],0)==string::npos) {
	result=false;
	break;
      }
  }
  return result;
}

bool LabelSelection::check(string &errMsg) {
  searchType=false;
  searchStrings=0;
  stringProxy=0;
  if (dataSet!=0) {
    dataSet->get("searchStrings",searchStrings);
    dataSet->get("SearchType",searchType);
    dataSet->get("searchLabel",stringProxy);
  }
  if (stringProxy==0) {
    stringProxy=graph->getProperty<StringProperty>("viewLabel");
  }
  if (searchStrings==0) {
    bool ok=true;
    while (ok) {
      QString text = QInputDialog::getText("Label Selection", "Search string:", QLineEdit::Normal, QString::null, &ok );
      if ( ok && !text.isEmpty() ) {
	searchStringsDefault.push_back(text.ascii());
      }
      else
	ok=false;
    }
    searchStrings=&searchStringsDefault;
  }
  vector<string>::iterator it;
  if (searchStrings->size()==0) {
    errMsg="You must give at least one regular expression";
    return false;
  }
  return true;
}
