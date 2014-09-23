/*
 *
 * This file is part of Tulip (www.tulip-software.org)
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
///@cond DOXYGEN_HIDDEN
#ifndef CSVCONTENTHANDLER_H_
#define CSVCONTENTHANDLER_H_

#include <string>
#include <vector>

#include <tulip/tulipconf.h>

namespace tlp {
/**
 * @brief Interface to inherit to get and treat data from csv files with CSVParser object.
 */
class TLP_QT_SCOPE CSVContentHandler {
public:
  virtual ~CSVContentHandler() {

  }
  /**
   * Function called at the beginning of the file parsing.
   */
  virtual void begin() = 0;

  /**
   * Function called for each line in the file.
   * @param row The number of the row.
   * @param lineTokens The tokens.
   */
  virtual void line(unsigned int row,const std::vector<std::string>& lineTokens) = 0;

  /**
   * Function called at the end of the parsing.
   * @param rowNumber the number of row read in the file.
   * @param columnNumber The column number for the line with the greatest column number.
   */
  virtual void end(unsigned int rowNumber, unsigned int columnNumber) = 0;
};
}
#endif /* CSVCONTENTHANDLER_H_ */
///@endcond
