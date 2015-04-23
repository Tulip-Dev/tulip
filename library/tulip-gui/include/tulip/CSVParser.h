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

#ifndef CSVDATALOADER_H_
#define CSVDATALOADER_H_

#include <tulip/tulipconf.h>
#include <tulip/CSVContentHandler.h>

#include <vector>
#include <climits>

#include <QString>

class QTextCodec;

namespace tlp {

class PluginProgress;

/*
* @brief Interface for CSV data parser.
*
* Send the found tokens to the CSVContentHandler interface.
*/
class TLP_QT_SCOPE CSVParser {
public:
  virtual ~CSVParser() {}

  /**
    * @brief Parse the data and send the tokens found to the CSVContentHandler.
    *
    * Notify the progression of the parsing with the progress object.
    **/
  virtual bool parse(CSVContentHandler * handler,tlp::PluginProgress* progress=NULL) = 0;
};

/**
* @brief Parse a csv data and send each tokens to the given CSVContentHandler object.
*
* Parse a csv data and send each tokens to the given CSVContentHandler object. Get each line of the file in the given range and parse them. This object skip empty lines.
* Send the found tokens to the CSVContentHandler interface.
* \code
* CSVParser parser(fileName,";","\"","UTF-8",true);
* \/\/Automatically remove quotes.
* CSVContentHandler * handler ;
* parser.parse(handler);
* \endcode
**/
class TLP_QT_SCOPE CSVSimpleParser : public CSVParser {
public:
  /**
    * @brief Construct a csv simple file parser.
    * @param filename The path to the file to import.
    * @param separator The separator to use.
    * @param textDelimiter If a token is sourrounded by this charater we ignore all the separators found in this token. Useful if a token contains the separator.
    * @param firstLine The number of the first line to read. The first line is 0.
    * @param lastLine The number of the last line to read.
    **/
  CSVSimpleParser(const std::string& fileName,const QString &separator=";", const bool mergesep=false, char textDelimiter='"',const std::string& fileEncoding=std::string("UTF-8"),unsigned int firstLine = 0,unsigned int lastLine = UINT_MAX);

  virtual ~CSVSimpleParser();

  inline std::string fileName()const {
    return _fileName;
  }
  inline void setFileName(const std::string& fileName) {
    _fileName = fileName;
  }

  inline char textDelimiter()const {
    return _textDelimiter;
  }

  inline void setTextDelimiter(char delimiter) {
    _textDelimiter = delimiter;
  }

  inline std::string fileEncoding()const {
    return _fileEncoding;
  }

  inline void setFileEncoding(const std::string& fileEncoding) {
    _fileEncoding = fileEncoding;
  }

  bool parse(CSVContentHandler * handler,tlp::PluginProgress* progress=NULL);

protected:
  virtual std::string treatToken(const std::string& token, int row, int column);

private:
  void tokenize(const std::string& str, std::vector<std::string>& tokens,
                const QString &delimiters, const bool mergedelim, char _textDelimiter, unsigned int numberOfCol);
  std::string convertStringEncoding(const std::string& toConvert,QTextCodec* encoder);

  /**
    * @brief Function to extract a line from a istream. Can handle Linux,Mac and Windows end of line pattern.
    **/
  bool multiplatformgetline ( std::istream& is, std::string& str );

  std::string removeQuotesIfAny(const std::string &s,const std::string& rejectedChars);
  std::string _fileName;
  QString _separator;
  char _textDelimiter;
  std::string _fileEncoding;
  unsigned int _firstLine;
  unsigned int _lastLine;
  bool _mergesep;

};

/**
  *@brief CSV parser used to invert the token matrix in order to treat rows as columns.
  **/
class TLP_QT_SCOPE CSVInvertMatrixParser : public tlp::CSVParser , public tlp::CSVContentHandler {
public:
  CSVInvertMatrixParser(CSVParser* parser);
  virtual ~CSVInvertMatrixParser();

  bool parse(CSVContentHandler *handler, tlp::PluginProgress *progress=NULL);

  void begin();
  void line(unsigned int row,const std::vector<std::string>& lineTokens);
  void end(unsigned int rowNumber, unsigned int columnNumber);
private:
  CSVParser *parser;
  CSVContentHandler *handler;
  std::vector<std::vector<std::string> > columns;
  unsigned int maxLineSize;
};
}
#endif /* CSVDATALOADER_H_ */
///@endcond
