#ifndef INC_bibfilelexer_hpp_
#define INC_bibfilelexer_hpp_

#include <antlr/config.hpp>
/* $ANTLR 2.7.6 (20060903): "bibfile.g" -> "bibfilelexer.hpp"$ */
#include <antlr/CommonToken.hpp>
#include <antlr/InputBuffer.hpp>
#include <antlr/BitSet.hpp>
#include "bibfileparserTokenTypes.hpp"
#include <antlr/CharScanner.hpp>
#line 20 "bibfile.g"

    // Will be included before the class definitions
    // in parser.hpp and lexer.hpp
    #include "parsedbibfile.h"

#line 18 "bibfilelexer.hpp"
class CUSTOM_API bibfilelexer : public ANTLR_USE_NAMESPACE(antlr)CharScanner, public bibfileparserTokenTypes
{
#line 103 "bibfile.g"

    // Will be included before lexer methods declarations
    // *inside* class definition in lexer.hpp
    private:
      ParsedBibFile*    pbf_;    
    public:
      inline void setParsedBibFile(ParsedBibFile* pbf)
          {
              pbf_ = pbf;
          }
#line 22 "bibfilelexer.hpp"
private:
	void initLiterals();
public:
	bool getCaseSensitiveLiterals() const
	{
		return true;
	}
public:
	bibfilelexer(ANTLR_USE_NAMESPACE(std)istream& in);
	bibfilelexer(ANTLR_USE_NAMESPACE(antlr)InputBuffer& ib);
	bibfilelexer(const ANTLR_USE_NAMESPACE(antlr)LexerSharedInputState& state);
	ANTLR_USE_NAMESPACE(antlr)RefToken nextToken();
	public: void mCOMMENT(bool _createToken);
	protected: void mARROBAS(bool _createToken);
	protected: void mARROBASCOMMENT(bool _createToken);
	public: void mARROBAS_OR_COMMENT(bool _createToken);
private:
	
	static const unsigned long _tokenSet_0_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_0;
	static const unsigned long _tokenSet_1_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_1;
};

#endif /*INC_bibfilelexer_hpp_*/
