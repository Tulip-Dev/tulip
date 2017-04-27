#ifndef INC_bibfileparserTokenTypes_hpp_
#define INC_bibfileparserTokenTypes_hpp_

/* $ANTLR 2.7.6 (20060903): "bibfile.g" -> "bibfileparserTokenTypes.hpp"$ */

#ifndef CUSTOM_API
# define CUSTOM_API
#endif

#ifdef __cplusplus
struct CUSTOM_API bibfileparserTokenTypes {
#endif
	enum {
		EOF_ = 1,
		COMMENT = 4,
		ARROBAS = 5,
		ARROBASCOMMENT = 6,
		ARROBAS_OR_COMMENT = 7,
		NULL_TREE_LOOKAHEAD = 3
	};
#ifdef __cplusplus
};
#endif
#endif /*INC_bibfileparserTokenTypes_hpp_*/
