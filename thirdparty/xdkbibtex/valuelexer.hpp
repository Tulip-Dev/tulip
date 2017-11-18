#ifndef INC_valuelexer_hpp_
#define INC_valuelexer_hpp_

#include <antlr/config.hpp>
/* $ANTLR 2.7.6 (20060903): "value.g" -> "valuelexer.hpp"$ */
#include <antlr/CommonToken.hpp>
#include <antlr/InputBuffer.hpp>
#include <antlr/BitSet.hpp>
#include "valueparserTokenTypes.hpp"
#include <antlr/CharScanner.hpp>
#line 19 "value.g"

    // Will be included before the class definitions
    // in parser.hpp and lexer.hpp
    #include <xdkbibtex/value.h>
    #include <stack>
    #include <stdexcept>

#line 20 "valuelexer.hpp"
class CUSTOM_API valuelexer : public ANTLR_USE_NAMESPACE(antlr)CharScanner, public valueparserTokenTypes
{
#line 187 "value.g"

    // Will be included before lexer methods declarations
    // *inside* class definition in lexer.hpp
#line 24 "valuelexer.hpp"
private:
	void initLiterals();
public:
	bool getCaseSensitiveLiterals() const
	{
		return true;
	}
public:
	valuelexer(ANTLR_USE_NAMESPACE(std)istream& in);
	valuelexer(ANTLR_USE_NAMESPACE(antlr)InputBuffer& ib);
	valuelexer(const ANTLR_USE_NAMESPACE(antlr)LexerSharedInputState& state);
	ANTLR_USE_NAMESPACE(antlr)RefToken nextToken();
	public: void mLBRACE(bool _createToken);
	public: void mRBRACE(bool _createToken);
	public: void mTOKEN(bool _createToken);
	public: void mLETTER(bool _createToken);
	public: void mWS(bool _createToken);
private:
	
	static const unsigned long _tokenSet_0_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_0;
	static const unsigned long _tokenSet_1_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_1;
	static const unsigned long _tokenSet_2_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_2;
};

#endif /*INC_valuelexer_hpp_*/
