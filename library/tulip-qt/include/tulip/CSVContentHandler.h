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
#ifndef CSVCONTENTHANDLER_H_
#define CSVCONTENTHANDLER_H_
namespace tlp {
/**
 * @brief Interface to inherit to get and treat data from csv files with CSVParser object.
 */
class CSVContentHandler {
public:
  ~CSVContentHandler() {

  }
  /**
   * Function called at the beginning of the file parsing.
   */
  virtual void begin() = 0;
  /**
   * Function called for each tokens found in the file.
   * @param row The row position of the token.
   * @param column The column position of the token.
   * @param token The token.
   */
  virtual void token(unsigned int row, unsigned int column, const std::string& token) = 0;
  /**
   * Function called at the end of the parsing.
   * @param rowNumber the number of row read in the file.
   * @param columnNumber The column number for the line with the greatest column number.
   */
  virtual void end(unsigned int rowNumber, unsigned int columnNumber) = 0;
};
}
#endif /* CSVCONTENTHANDLER_H_ */
