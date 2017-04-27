#ifndef INC_bibcommandlexer_hpp_
#define INC_bibcommandlexer_hpp_

#include <antlr/config.hpp>
/* $ANTLR 2.7.6 (20060903): "bibcommand.g" -> "bibcommandlexer.hpp"$ */
#include <antlr/CommonToken.hpp>
#include <antlr/InputBuffer.hpp>
#include <antlr/BitSet.hpp>
#include "bibcommandparserTokenTypes.hpp"
#include <antlr/CharScanner.hpp>
#line 19 "bibcommand.g"

    // Will be included before the class definitions
    // in parser.hpp and lexer.hpp
    #include "parsedbibfile.h"
    #include "base.h"
    #include <iostream>
    #include <list>

    class bibcommandparser;

#line 23 "bibcommandlexer.hpp"
class CUSTOM_API bibcommandlexer : public ANTLR_USE_NAMESPACE(antlr)CharScanner, public bibcommandparserTokenTypes
{
#line 281 "bibcommand.g"

    // Will be included before lexer methods declarations
    // *inside* class definition in lexer.hpp
    private:
      ParsedBibFile* pbf_;
      bool           braceStartsValue_;
    public:
      inline void setBraceStartsValue(bool b)
          {
              braceStartsValue_ = b;
          }
      inline void setParsedBibFile(ParsedBibFile* pbf)
          {
              pbf_ = pbf;
          }
#line 27 "bibcommandlexer.hpp"
private:
	void initLiterals();
public:
	bool getCaseSensitiveLiterals() const
	{
		return true;
	}
public:
	bibcommandlexer(ANTLR_USE_NAMESPACE(std)istream& in);
	bibcommandlexer(ANTLR_USE_NAMESPACE(antlr)InputBuffer& ib);
	bibcommandlexer(const ANTLR_USE_NAMESPACE(antlr)LexerSharedInputState& state);
	ANTLR_USE_NAMESPACE(antlr)RefToken nextToken();
	public: void mPOUND(bool _createToken);
	public: void mLPARENTHESIS(bool _createToken);
	public: void mRPARENTHESIS(bool _createToken);
	public: void mRBRACE(bool _createToken);
	public: void mCOMA(bool _createToken);
	public: void mEQUAL(bool _createToken);
	public: void mNAME(bool _createToken);
	public: void mNUMBER(bool _createToken);
	protected: void mESC(bool _createToken);
	protected: void mESC_WITH_QUOTE(bool _createToken);
	protected: void mSTRING_INTERNAL_WITH_QUOTES(bool _createToken);
	protected: void mSTRING_INTERNAL(bool _createToken);
	public: void mSTRING(bool _createToken);
	protected: void mLBRACE(bool _createToken);
	protected: void mVALUE_INTERNAL(bool _createToken);
	protected: void mVALUE(bool _createToken);
	public: void mLBRACE_OR_VALUE(bool _createToken);
	public: void mWS(bool _createToken);
	protected: void mTYPE(bool _createToken);
	protected: void mSTRINGDEF(bool _createToken);
	protected: void mPREAMBLE(bool _createToken);
	public: void mSTRINGDEF_OR_PREAMBLE_OR_TYPE(bool _createToken);
private:
	
	static const unsigned long _tokenSet_0_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_0;
	static const unsigned long _tokenSet_1_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_1;
	static const unsigned long _tokenSet_2_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_2;
	static const unsigned long _tokenSet_3_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_3;
	static const unsigned long _tokenSet_4_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_4;
};

#endif /*INC_bibcommandlexer_hpp_*/
