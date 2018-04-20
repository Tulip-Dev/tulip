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

#include <QTextCodec>

#include <tulip/CSVParser.h>
#include <tulip/TlpTools.h>
#include <tulip/TlpQtTools.h>
#include <tulip/PluginProgress.h>

#include <fstream>
#include <cassert>
#include <regex>

using namespace std;
using namespace tlp;

// const string defaultRejectedChars = " \r\n";
// const string spaceChars = " \t";
CSVSimpleParser::CSVSimpleParser(const string &fileName, const QString &separator,
                                 const bool mergesep, char textDelimiter, char decimalMark,
                                 const string &fileEncoding, unsigned int firstLine,
                                 unsigned int lastLine)
    : _fileName(fileName), _separator(separator), _textDelimiter(textDelimiter),
      _decimalMark(decimalMark), _fileEncoding(fileEncoding), _firstLine(firstLine),
      _lastLine(lastLine), _mergesep(mergesep) {}

CSVSimpleParser::~CSVSimpleParser() {}

string CSVSimpleParser::convertStringEncoding(const std::string &toConvert, QTextCodec *encoder) {
  return QStringToTlpString(encoder->toUnicode(toConvert.c_str()));
}

bool CSVSimpleParser::parse(CSVContentHandler *handler, PluginProgress *progress,
                            bool firstLineOnly) {
  if (!handler) {
    return false;
  }

  bool result = handler->begin();

  if (!result)
    return result;

  istream *csvFile = tlp::getInputFileStream(_fileName.c_str(), ifstream::in | ifstream::binary);

  if (*csvFile) {
    // Real row number used to
    unsigned int row = 0;
    // Read row number
    unsigned int columnMax = 0;

    csvFile->seekg(0, std::ios_base::end);
    // get position = file size
    unsigned long fileSize = csvFile->tellg(), readSize = 0;
    // reset position
    csvFile->seekg(0, std::ios_base::beg);
    string line;
    vector<string> tokens;

    unsigned int displayProgressEachLineNumber = 200;

    QTextCodec *codec = QTextCodec::codecForName(_fileEncoding.c_str());

    if (codec == nullptr) {
      qWarning() << __PRETTY_FUNCTION__ << ":" << __LINE__
                 << " Cannot found the convertion codec to convert from " << _fileEncoding
                 << " string will be treated as utf8.";
      codec = QTextCodec::codecForName("UTF-8");
    }

    if (progress) {
      progress->progress(0, 100);
    }

    // change locale if needed
    char *prevLocale = setlocale(LC_NUMERIC, nullptr);

    if (decimalMark() == ',')
      setlocale(LC_NUMERIC, "fr_FR");

    while (multiplatformgetline(*csvFile, line) && row <= _lastLine) {

      if (progress) {
        readSize += line.size();

        if (progress->state() != TLP_CONTINUE) {
          break;
        }

        // Each displayProgressEachLineNumber display progression
        if (row % displayProgressEachLineNumber == 0) {
          // compute progression in function of read size and file size.
          progress->progress(readSize, fileSize);
        }
      }

      if (!line.empty() && row >= _firstLine) {
        // Correct the encoding of the line.
        line = convertStringEncoding(line, codec);
        tokens.clear();
        tokenize(line, tokens, _separator, _mergesep, _textDelimiter, 0);

        result = handler->line(row, tokens);

        if (!result)
          break;

        columnMax = max(columnMax, unsigned(tokens.size()));

        // If user want to stop break the import process.
        if (progress) {
          if (progress->state() != TLP_CONTINUE) {
            break;
          }
        }
      }

      ++row;

      if (firstLineOnly && row > _firstLine)
        break;
    }

    delete csvFile;
    // reset locale
    setlocale(LC_NUMERIC, prevLocale);

    return result ? handler->end(row, columnMax) : false;
  } else {
    delete csvFile;
    return false;
  }
}

bool CSVSimpleParser::multiplatformgetline(istream &is, string &str) {
  // nothing new to read.
  if (is.eof())
    return false;

  str.clear();
  str.reserve(2048);
  char c;
  bool tdlm = false;

  while (is.get(c)) {
    if (c == _textDelimiter) {
      tdlm = !tdlm;
      str.push_back(c);
      continue;
    }

    // Carriage return Windows and mac
    if (c == '\r') {
      // Check if the next character is \n and remove it.
      if (is.get(c) && c != '\n') {
        is.unget();
      }

      if (!tdlm)
        break;
    } else if (c == '\n' && !tdlm) {
      break;
    }

    // Push the character
    str.push_back(c);
  }

  // End of line reading.
  return true;
}

void CSVSimpleParser::tokenize(const string &str, vector<string> &tokens, const QString &delimiters,
                               const bool mergedelim, const char textDelim, unsigned int) {
  auto delim = QStringToTlpString(delimiters);
  auto textDel = string(1, textDelim);

  // first step tokenize
  std::regex rgx(delim + string("(?=(?:[^" + textDel + "]*" + textDel + "[^" + textDel + "]*" +
                                textDel + ")*[^" + textDel + "]*$)"));
  std::sregex_token_iterator end;
  for (std::sregex_token_iterator iter(str.cbegin(), str.cend(), rgx, -1); iter != end; ++iter) {
    // remove unneeded textDelim if any at begin and end of the token (so match 1st or 2nd case of
    // re)
    regex re("(" + textDel + "((?:[^" + textDel + "]|" + textDel + textDel + ")*)\"|([^" + delim +
             textDel + "\\n\\r]*))");
    string token(*iter);
    std::smatch cm;
    std::regex_match(token, cm, re);
    string tok;
    if (cm[2].length() == 0)
      tok = cm[1].str();
    else
      tok = cm[2].str();
    // remove any double delimiter
    string tok_final(regex_replace(tok, regex(textDel + textDel), textDel));
    tokens.push_back(tok_final);
    // replace successive empty elements by only one empty elements
    if (mergedelim) {
      auto end = std::unique(tokens.begin(), tokens.end(), [](const string &l, const string &r) {
        return l.empty() && r.empty();
      });
      tokens = std::move(vector<string>(tokens.begin(), end));
    }
  }
}

CSVInvertMatrixParser::CSVInvertMatrixParser(CSVParser *parser) : parser(parser) {}

CSVInvertMatrixParser::~CSVInvertMatrixParser() {
  delete parser;
}

bool CSVInvertMatrixParser::parse(CSVContentHandler *handler, PluginProgress *progress, bool) {
  this->handler = handler;
  return parser->parse(this, progress);
}

bool CSVInvertMatrixParser::begin() {
  maxLineSize = 0;
  return true;
}

bool CSVInvertMatrixParser::line(unsigned int, const std::vector<std::string> &lineTokens) {
  maxLineSize = max(maxLineSize, uint(lineTokens.size()));
  columns.push_back(lineTokens);
  return true;
}

bool CSVInvertMatrixParser::end(unsigned int, unsigned int) {
  if (!handler->begin())
    return false;

  vector<string> tokens(columns.size());

  // Fill the line with
  for (unsigned int line = 0; line < maxLineSize; ++line) {
    for (unsigned int i = 0; i < columns.size(); ++i) {
      // Check if the column is large enough
      tokens[i] = columns[i].size() > line ? columns[i][line] : string();
    }

    if (!handler->line(line, tokens))
      return false;
  }

  return handler->end(maxLineSize, columns.size());
}
