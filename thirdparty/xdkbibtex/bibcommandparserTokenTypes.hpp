#ifndef INC_bibcommandparserTokenTypes_hpp_
#define INC_bibcommandparserTokenTypes_hpp_

/* $ANTLR 2.7.6 (20060903): "bibcommand.g" -> "bibcommandparserTokenTypes.hpp"$ */

#ifndef CUSTOM_API
# define CUSTOM_API
#endif

#ifdef __cplusplus
struct CUSTOM_API bibcommandparserTokenTypes {
#endif
	enum {
		EOF_ = 1,
		STRINGDEF = 4,
		LPARENTHESIS = 5,
		EQUAL = 6,
		RPARENTHESIS = 7,
		LBRACE = 8,
		RBRACE = 9,
		PREAMBLE = 10,
		TYPE = 11,
		COMA = 12,
		POUND = 13,
		NAME = 14,
		STRING = 15,
		VALUE = 16,
		NUMBER = 17,
		ESC = 18,
		ESC_WITH_QUOTE = 19,
		STRING_INTERNAL_WITH_QUOTES = 20,
		STRING_INTERNAL = 21,
		VALUE_INTERNAL = 22,
		LBRACE_OR_VALUE = 23,
		WS = 24,
		STRINGDEF_OR_PREAMBLE_OR_TYPE = 25,
		NULL_TREE_LOOKAHEAD = 3
	};
#ifdef __cplusplus
};
#endif
#endif /*INC_bibcommandparserTokenTypes_hpp_*/
