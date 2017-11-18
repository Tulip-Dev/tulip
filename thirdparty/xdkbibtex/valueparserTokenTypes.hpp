#ifndef INC_valueparserTokenTypes_hpp_
#define INC_valueparserTokenTypes_hpp_

/* $ANTLR 2.7.6 (20060903): "value.g" -> "valueparserTokenTypes.hpp"$ */

#ifndef CUSTOM_API
# define CUSTOM_API
#endif

#ifdef __cplusplus
struct CUSTOM_API valueparserTokenTypes {
#endif
	enum {
		EOF_ = 1,
		WS = 4,
		LETTER = 5,
		LBRACE = 6,
		RBRACE = 7,
		TOKEN = 8,
		NULL_TREE_LOOKAHEAD = 3
	};
#ifdef __cplusplus
};
#endif
#endif /*INC_valueparserTokenTypes_hpp_*/
