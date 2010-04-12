/*
 * CSVDataLoader.h
 *
 *  Created on: 16 févr. 2010
 *      Author: Jonathan Dubois
 *       Email: jonathan.dubois@labri.fr
 *
 *	This program is free software; you can redistribute it and/or modify  *
 *  it under the terms of the GNU Lesser General Public License as published by  
 *  the Free Software Foundation; either version 2 of the License, or     
 *  (at your option) any later version.
 */

#ifndef CSVDATALOADER_H_
#define CSVDATALOADER_H_
#include <tulip/tulipconf.h>
#include <string>
#include <vector>
#include <limits.h>
namespace tlp {
/*
 * Parse a csv file and store data in double string vector.
 */
class TLP_QT_SCOPE CSVParser {
public:
  CSVParser();
  virtual ~CSVParser();

  void setRemoveQuotes(bool removeQuotes) {
    this->removeQuotes = removeQuotes;
  }
  virtual const std::vector<std::vector<std::string> >
  & parse(const std::string& fileName, const std::string& separator, unsigned int numberOfRows = UINT_MAX,
      unsigned int numberOfCol = UINT_MAX);
  const std::vector<std::vector<std::string> >& getResult() const {
    return tokens;
  }
  bool resultIsEmpty() const {
    return tokens.empty();
  }

  std::vector<std::string> extractRow(unsigned int rowNumber, unsigned int colBegin = 0, unsigned int colNumber = 0);
  std::vector<std::string> extractColumn(unsigned int colNumber, unsigned int rowBegin = 0, unsigned int rowNumber = 0);

  unsigned int rowNumber() const {
    return tokens.size();
  }

  unsigned int colNumber() const {
    if (!tokens.empty()) {
      return tokens[0].size();
    }
    else {
      return 0;
    }
  }

protected:
  virtual std::string treatToken(const std::string& token, int row, int column);

private:


  void tokenize(const std::string& str, std::vector<std::string>& tokens, const std::string& delimiters, unsigned int numberOfCol);
  std::string removeQuotesIfAny(const std::string &s);
  std::vector<std::vector<std::string> > tokens;
  bool removeQuotes;
};
}
#endif /* CSVDATALOADER_H_ */
