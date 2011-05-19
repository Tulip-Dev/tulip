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

#ifndef CSVDATALOADER_H_
#define CSVDATALOADER_H_
#include <tulip/tulipconf.h>
#include <string>
#include <vector>
#include <limits.h>

#include <QtCore/QTextCodec>
#include <QtCore/QString>
#include "tulip/CSVContentHandler.h"
namespace tlp {

    class PluginProgress;


    /*
    * @brief Interface for CSV data parser.
    *
    * Send the found tokens to the CSVContentHandler interface.    
    */
    class TLP_QT_SCOPE CSVParser {
    public:        
        virtual ~CSVParser(){}

        /**
          * @brief Parse the data and send the tokens found to the CSVContentHandler.
          *
          * Notify the progression of the parsing with the progress object.
          **/
        virtual bool parse(CSVContentHandler * handler,tlp::PluginProgress* progress=NULL) = 0;
    };

 /*
 * @brief Parse a csv data and send each tokens to the given CSVContentHandler object.
 *
 * Parse a csv data and send each tokens to the given CSVContentHandler object. Get each line of the file and parse them.
 * Send the found tokens to the CSVContentHandler interface.
 * \code
 * CSVParser parser(fileName,";","\"","UTF-8",true);
 * \/\/Automatically remove quotes. 
 * CSVContentHandler * handler ;
 * parser.parse(handler);
 * \endcode
 */
    class TLP_QT_SCOPE CSVSimpleParser : public CSVParser {
    public:
        CSVSimpleParser(const std::string& fileName,const std::string& separator=std::string(";"),char='"',const std::string& fileEncoding=std::string("UTF-8"));
        virtual ~CSVSimpleParser();

        bool parse(CSVContentHandler * handler,tlp::PluginProgress* progress=NULL);

    protected:
        virtual std::string treatToken(const std::string& token, int row, int column);

    private:
        void tokenize(const std::string& str, std::vector<std::string>& tokens,
                      const std::string& delimiters,char textDelimiter, unsigned int numberOfCol);
        inline std::string convertStringEncoding(const std::string& toConvert,QTextCodec* encoder){
           return std::string(encoder->toUnicode ( toConvert.c_str() ).toUtf8().data());
        }

        /**
          * @brief Function to extract a line from a istream. Can handle Linux,Mac and Windows end of line pattern.
          **/
        bool multiplatformgetline ( std::istream& is, std::string& str );

        std::string removeQuotesIfAny(const std::string &s,const std::string& rejectedChars);
        std::string fileName;
        std::string separator;
        char textDelimiter;
        std::string fileEncoding;        

    };

    /**
      *@brief CSV parser used to treat rows as columns.
      **/
    class TLP_QT_SCOPE CSVInvertMatrixParser : public tlp::CSVParser , public tlp::CSVContentHandler{
    public:
        CSVInvertMatrixParser(CSVParser* parser);
        virtual ~CSVInvertMatrixParser();

        bool parse(CSVContentHandler *handler, tlp::PluginProgress *progress=NULL);

        void begin();

        void token(unsigned int row, unsigned int column, const std::string &token);

        void end(unsigned int rowNumber, unsigned int columnNumber);
    private:
        CSVParser *parser;
        CSVContentHandler *handler;
    };
}
#endif /* CSVDATALOADER_H_ */
