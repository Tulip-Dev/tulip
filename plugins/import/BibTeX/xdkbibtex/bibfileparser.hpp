#ifndef INC_bibfileparser_hpp_
#define INC_bibfileparser_hpp_

#include <antlr/config.hpp>
/* $ANTLR 2.7.6 (20060903): "bibfile.g" -> "bibfileparser.hpp"$ */
#include <antlr/TokenStream.hpp>
#include <antlr/TokenBuffer.hpp>
#include "bibfileparserTokenTypes.hpp"
#include <antlr/LLkParser.hpp>

#line 20 "bibfile.g"

    // Will be included before the class definitions
    // in parser.hpp and lexer.hpp
    #include "parsedbibfile.h"

#line 18 "bibfileparser.hpp"
class CUSTOM_API bibfileparser : public ANTLR_USE_NAMESPACE(antlr)LLkParser, public bibfileparserTokenTypes
{
#line 50 "bibfile.g"

    // Will be included before lexer methods declarations
    // *inside* class definition in parser.hpp
    private:
      ParsedBibFile* pbf_;
      int            mark_;
    public:
      inline void setParsedBibFile(ParsedBibFile* pbf)
          {
              pbf_ = pbf;
          }
#line 22 "bibfileparser.hpp"
public:
	void initializeASTFactory( ANTLR_USE_NAMESPACE(antlr)ASTFactory& factory );
protected:
	bibfileparser(ANTLR_USE_NAMESPACE(antlr)TokenBuffer& tokenBuf, int k);
public:
	bibfileparser(ANTLR_USE_NAMESPACE(antlr)TokenBuffer& tokenBuf);
protected:
	bibfileparser(ANTLR_USE_NAMESPACE(antlr)TokenStream& lexer, int k);
public:
	bibfileparser(ANTLR_USE_NAMESPACE(antlr)TokenStream& lexer);
	bibfileparser(const ANTLR_USE_NAMESPACE(antlr)ParserSharedInputState& state);
	int getNumTokens() const
	{
		return bibfileparser::NUM_TOKENS;
	}
	const char* getTokenName( int type ) const
	{
		if( type > getNumTokens() ) return 0;
		return bibfileparser::tokenNames[type];
	}
	const char* const* getTokenNames() const
	{
		return bibfileparser::tokenNames;
	}
	public: void parse();
	public: void comment();
	public: void bibcommand();
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
	static const int NUM_TOKENS = 8;
#else
	enum {
		NUM_TOKENS = 8
	};
#endif
	
};

#endif /*INC_bibfileparser_hpp_*/
