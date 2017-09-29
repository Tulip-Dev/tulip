#ifndef INC_valueparser_hpp_
#define INC_valueparser_hpp_

#include <antlr/config.hpp>
/* $ANTLR 2.7.6 (20060903): "value.g" -> "valueparser.hpp"$ */
#include <antlr/TokenStream.hpp>
#include <antlr/TokenBuffer.hpp>
#include "valueparserTokenTypes.hpp"
#include <antlr/LLkParser.hpp>

#line 19 "value.g"

    // Will be included before the class definitions
    // in parser.hpp and lexer.hpp
    #include <xdkbibtex/value.h>
    #include <stack>
    #include <stdexcept>

#line 20 "valueparser.hpp"
class CUSTOM_API valueparser : public ANTLR_USE_NAMESPACE(antlr)LLkParser, public valueparserTokenTypes
{
#line 70 "value.g"

    // Will be included before lexer methods declarations
    // *inside* class definition in lexer.hpp
    private:
      std::stack<xdkbib::Text*> textStack_;
      xdkbib::Text              splitText_;
      xdkbib::Word*             splitWord_;
    public:
      void setText(xdkbib::Text* t)
          {
              textStack_.push(t);
              splitWord_ = NULL;
          }
      
#if __cplusplus < 201103L
      void setSplitWord(const std::string s) throw(std::invalid_argument);
#else
      void setSplitWord(const std::string s);
#endif

#line 24 "valueparser.hpp"
public:
	void initializeASTFactory( ANTLR_USE_NAMESPACE(antlr)ASTFactory& factory );
protected:
	valueparser(ANTLR_USE_NAMESPACE(antlr)TokenBuffer& tokenBuf, int k);
public:
	valueparser(ANTLR_USE_NAMESPACE(antlr)TokenBuffer& tokenBuf);
protected:
	valueparser(ANTLR_USE_NAMESPACE(antlr)TokenStream& lexer, int k);
public:
	valueparser(ANTLR_USE_NAMESPACE(antlr)TokenStream& lexer);
	valueparser(const ANTLR_USE_NAMESPACE(antlr)ParserSharedInputState& state);
	int getNumTokens() const
	{
		return valueparser::NUM_TOKENS;
	}
	const char* getTokenName( int type ) const
	{
		if( type > getNumTokens() ) return 0;
		return valueparser::tokenNames[type];
	}
	const char* const* getTokenNames() const
	{
		return valueparser::tokenNames;
	}
	public: void parse();
	public: void text();
	public: void word();
	public: xdkbib::Letter*  letter();
public:
	ANTLR_USE_NAMESPACE(antlr)RefAST getAST()
	{
		return returnAST;
	}
	
protected:
	ANTLR_USE_NAMESPACE(antlr)RefAST returnAST;
private:
	static const char* tokenNames[];
#ifndef NO_STATIC_CONSTS
	static const int NUM_TOKENS = 9;
#else
	enum {
		NUM_TOKENS = 9
	};
#endif
	
	static const unsigned long _tokenSet_0_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_0;
};

#endif /*INC_valueparser_hpp_*/
