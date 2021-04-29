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
#include <locale>

using namespace std;
using namespace tlp;

const string defaultRejectedChars = " \r\n";
const string spaceChars = " \t";
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
                 << " Cannot found the conversion codec to convert from " << _fileEncoding
                 << " string will be treated as utf8.";
      codec = QTextCodec::codecForName("UTF-8");
    }

    if (progress) {
      progress->progress(0, 100);
    }

    // change locale if needed
    std::locale prevLocale;

    if (decimalMark() == ',') {
      std::locale loc = std::locale().combine<std::numpunct<char>>(std::locale("fr_FR.UTF8"));
      std::locale::global(loc);
    }

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
        unsigned int column = 0;

        for (column = 0; column < tokens.size(); ++column) {
          tokens[column] = treatToken(tokens[column], row, column);
        }

        result = handler->line(row, tokens);

        if (!result)
          break;

        columnMax = max(columnMax, column);

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
    std::locale::global(prevLocale);

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
        c = '\r';
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
                               const bool mergedelim, char textDelim, unsigned int) {
  // Skip delimiters at beginning.
  string::size_type lastPos = 0;
  string::size_type pos = 0;
  bool quit = false;

  auto delim = QStringToTlpString(delimiters);

  while (!quit) {
    // Don't search tokens in chars surrounded by text delimiters.
    assert(pos != string::npos);
    assert(pos < str.size());

    while (pos < str.length() && ((str[pos] != delim[0]) || (str.find(delim, pos) != pos))) {
      if (str[pos] == textDelim) {
        do {
          pos += 1;
          // go the the next text delimiter .
          pos = str.find_first_of(textDelim, pos);
        }
        // continue until a single textDelim
        while (pos != string::npos && str[++pos] == textDelim);
      } else
        pos += 1;
    }

    // if merge delimiter, skip the next char if it is a delimiter
    if (mergedelim) {
      while ((pos < str.length() - delim.size()) && (str.substr(pos + 1, delim.length()) == delim))
        pos += delim.length();
    }

    // Extracting tokens.
    assert(lastPos != string::npos);
    // Compute the extracted char number
    size_t nbExtractedChars = pos - lastPos;

    try {
      tokens.push_back(str.substr(lastPos, nbExtractedChars));
    } catch (...) {
      // An error occur quit the line parsing.
      break;
    }

    // Go to the begin of the next token.
    if (pos + 1 < str.size()) {
      // Skip the delimiter.
      ++pos;
      assert(pos > lastPos);
      // Store the begin position of the next token
      lastPos = pos;
    } else {
      // End of line found quit
      quit = true;
    }
  }
}

string CSVSimpleParser::treatToken(const string &token, int, int) {
  string currentToken = token;
  // erase space chars at the beginning/end of the value
  // and replace multiple occurrences of space chars by a blank
  string::size_type beginPos = currentToken.find_first_of(spaceChars);

  while (beginPos != string::npos) {
    string::size_type endPos = currentToken.find_first_not_of(spaceChars, beginPos);

    if (beginPos == 0) {
      // erase space chars at the beginning
      if (endPos != string::npos)
        currentToken.erase(beginPos, endPos - beginPos);
      else
        // only space chars in currentToken
        currentToken.clear();

      beginPos = currentToken.find_first_of(spaceChars);
    } else {
      if (endPos == string::npos) {
        // erase space chars at the end
        currentToken.erase(beginPos);
        break;
      }

      // replace multiple space chars
      if (endPos - beginPos > 1)
        currentToken.replace(beginPos, endPos - beginPos, 1, ' ');

      beginPos = currentToken.find_first_of(spaceChars, beginPos + 1);
    }
  }
  if (currentToken == "\"\"")
    return std::string();

  // Treat string to remove special characters from its beginning and its end.
  // and non needed "
  return removeQuotesIfAny(currentToken);
}

string CSVSimpleParser::removeQuotesIfAny(string &s) {
  // remove special chars at the beginning and end
  string::size_type pos = s.find_first_not_of(defaultRejectedChars);
  if (pos && pos != string::npos)
    s.erase(0, pos);
  pos = s.find_last_not_of(defaultRejectedChars);
  if (pos != string::npos && pos < s.size() - 1)
    s.erase(pos + 1);

  if (s[0] == _textDelimiter) {
    s.erase(0, 1);
    // treat " in " delimited string
    if (_textDelimiter == '"') {
      pos = 0;
      while ((pos = s.find("\"\"", pos)) != std::string::npos) {
        // replace double " by "
        s.replace(pos, 2, "\"");
        pos += 1;
      }
    }
    if (s[s.size() - 1] == _textDelimiter)
      s.erase(s.size() - 1, 1);
  }
  return s;
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
