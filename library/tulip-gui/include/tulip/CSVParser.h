/*
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

  virtual char decimalMark() const {
    return '.';
  }
  /**
   * @brief Parse the data and send the tokens found to the CSVContentHandler.
   *
   * Notify the progression of the parsing with the progress object.
   **/
  virtual bool parse(CSVContentHandler *handler, tlp::PluginProgress *progress = nullptr,
                     bool firstLineOnly = false) = 0;
};

/**
 * @brief Parse a csv data and send each tokens to the given CSVContentHandler object.
 *
 * Parse a csv data and send each tokens to the given CSVContentHandler object. Get each line of the
 *file in the given range and parse them. This object skip empty lines.
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
   * @param textDelimiter If a token is sourrounded by this character we ignore all the separators
   *found in this token. Useful if a token contains the separator.
   * @param firstLine The number of the first line to read. The first line is 0.
   * @param lastLine The number of the last line to read.
   **/
  CSVSimpleParser(const std::string &fileName, const QString &separator = ";",
                  const bool mergesep = false, char textDelimiter = '"', char delimiterMark = '.',
                  const std::string &fileEncoding = std::string("UTF-8"),
                  unsigned int firstLine = 0, unsigned int lastLine = UINT_MAX);

  ~CSVSimpleParser() override;

  inline std::string fileName() const {
    return _fileName;
  }
  inline void setFileName(const std::string &fileName) {
    _fileName = fileName;
  }

  inline char textDelimiter() const {
    return _textDelimiter;
  }

  char decimalMark() const override {
    return _decimalMark;
  }

  inline void setTextDelimiter(char delimiter) {
    _textDelimiter = delimiter;
  }

  inline std::string fileEncoding() const {
    return _fileEncoding;
  }

  inline void setFileEncoding(const std::string &fileEncoding) {
    _fileEncoding = fileEncoding;
  }

  bool parse(CSVContentHandler *handler, tlp::PluginProgress *progress = nullptr,
             bool firstLineOnly = false) override;

protected:
  virtual std::string treatToken(const std::string &token, int row, int column);

private:
  void tokenize(const std::string &str, std::vector<std::string> &tokens, const QString &delimiters,
                const bool mergedelim, char _textDelimiter, unsigned int numberOfCol);
  std::string convertStringEncoding(const std::string &toConvert, QTextCodec *encoder);

  /**
   * @brief Function to extract a line from a istream. Can handle Linux,Mac and Windows end of line
   *pattern.
   **/
  bool multiplatformgetline(std::istream &is, std::string &str);

  std::string removeQuotesIfAny(std::string &s);
  std::string _fileName;
  QString _separator;
  char _textDelimiter;
  char _decimalMark;
  std::string _fileEncoding;
  unsigned int _firstLine;
  unsigned int _lastLine;
  bool _mergesep;
};

/**
 *@brief CSV parser used to invert the token matrix in order to treat rows as columns.
 **/
class TLP_QT_SCOPE CSVInvertMatrixParser : public tlp::CSVParser, public tlp::CSVContentHandler {
public:
  CSVInvertMatrixParser(CSVParser *parser);
  ~CSVInvertMatrixParser() override;

  char decimalMark() const override {
    return parser->decimalMark();
  }

  bool parse(CSVContentHandler *handler, tlp::PluginProgress *progress = nullptr,
             bool firstLineOnly = false) override;

  bool begin() override;
  bool line(unsigned int row, const std::vector<std::string> &lineTokens) override;
  bool end(unsigned int rowNumber, unsigned int columnNumber) override;

private:
  CSVParser *parser;
  CSVContentHandler *handler;
  std::vector<std::vector<std::string>> columns;
  unsigned int maxLineSize;
};
} // namespace tlp
#endif /* CSVDATALOADER_H_ */
///@endcond
