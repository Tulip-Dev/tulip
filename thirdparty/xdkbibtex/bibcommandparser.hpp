#ifndef INC_bibcommandparser_hpp_
#define INC_bibcommandparser_hpp_

#include <antlr/config.hpp>
/* $ANTLR 2.7.6 (20060903): "bibcommand.g" -> "bibcommandparser.hpp"$ */
#include <antlr/TokenStream.hpp>
#include <antlr/TokenBuffer.hpp>
#include "bibcommandparserTokenTypes.hpp"
#include <antlr/LLkParser.hpp>

#line 19 "bibcommand.g"

    // Will be included before the class definitions
    // in parser.hpp and lexer.hpp
    #include "parsedbibfile.h"
    #include "base.h"
    #include <iostream>
    #include <list>

    class bibcommandparser;

#line 23 "bibcommandparser.hpp"
class CUSTOM_API bibcommandparser : public ANTLR_USE_NAMESPACE(antlr)LLkParser, public bibcommandparserTokenTypes
{
#line 74 "bibcommand.g"

    // Will be included before parser methods declarations
    // *inside* class definition in parser.hpp
    private:
      ParsedBibFile* pbf_;
      xdkbib::Entry* currentEntry_;
      std::list<xdkbib::ValuePart> currentValueParts_;
    public:
      inline void setParsedBibFile(ParsedBibFile* pbf)
          {
              pbf_ = pbf;
          }
    protected:
      void setBraceStartsValue(bool b);
      void addPart(xdkbib::ValuePartType t,const std::string& v);
#line 27 "bibcommandparser.hpp"
public:
	void initializeASTFactory( ANTLR_USE_NAMESPACE(antlr)ASTFactory& factory );
protected:
	bibcommandparser(ANTLR_USE_NAMESPACE(antlr)TokenBuffer& tokenBuf, int k);
public:
	bibcommandparser(ANTLR_USE_NAMESPACE(antlr)TokenBuffer& tokenBuf);
protected:
	bibcommandparser(ANTLR_USE_NAMESPACE(antlr)TokenStream& lexer, int k);
public:
	bibcommandparser(ANTLR_USE_NAMESPACE(antlr)TokenStream& lexer);
	bibcommandparser(const ANTLR_USE_NAMESPACE(antlr)ParserSharedInputState& state);
	int getNumTokens() const
	{
		return bibcommandparser::NUM_TOKENS;
	}
	const char* getTokenName( int type ) const
	{
		if( type > getNumTokens() ) return 0;
		return bibcommandparser::tokenNames[type];
	}
	const char* const* getTokenNames() const
	{
		return bibcommandparser::tokenNames;
	}
	public: void parse();
	public: void entry();
	public: void stringdef();
	public: void preamble();
	public: std::string  id();
	public: void fieldValue();
	public: std::string  key();
	public: void fields();
	public: void field();
	public: void fieldValuePart();
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
	static const int NUM_TOKENS = 26;
#else
	enum {
		NUM_TOKENS = 26
	};
#endif
	
};

#endif /*INC_bibcommandparser_hpp_*/
