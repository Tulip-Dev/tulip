
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 19 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "sip.h"


#define MAX_NESTED_IF       10
#define MAX_NESTED_SCOPE    10

#define inMainModule()      (currentSpec->module == currentModule || currentModule->container != NULL)


static sipSpec *currentSpec;            /* The current spec being parsed. */
static stringList *neededQualifiers;    /* The list of required qualifiers. */
static stringList *excludedQualifiers;  /* The list of excluded qualifiers. */
static moduleDef *currentModule;        /* The current module being parsed. */
static mappedTypeDef *currentMappedType;    /* The current mapped type. */
static enumDef *currentEnum;            /* The current enum being parsed. */
static int sectionFlags;                /* The current section flags. */
static int currentOverIsVirt;           /* Set if the overload is virtual. */
static int currentCtorIsExplicit;       /* Set if the ctor is explicit. */
static int currentIsStatic;             /* Set if the current is static. */
static int currentIsSignal;             /* Set if the current is Q_SIGNAL. */
static int currentIsSlot;               /* Set if the current is Q_SLOT. */
static int currentIsTemplate;           /* Set if the current is a template. */
static char *previousFile;              /* The file just parsed. */
static parserContext currentContext;    /* The current context. */
static int skipStackPtr;                /* The skip stack pointer. */
static int skipStack[MAX_NESTED_IF];    /* Stack of skip flags. */
static classDef *scopeStack[MAX_NESTED_SCOPE];  /* The scope stack. */
static int sectFlagsStack[MAX_NESTED_SCOPE];    /* The section flags stack. */
static int currentScopeIdx;             /* The scope stack index. */
static int currentTimelineOrder;        /* The current timeline order. */
static classList *currentSupers;        /* The current super-class list. */
static KwArgs defaultKwArgs;            /* The default keyword arguments support. */
static int makeProtPublic;              /* Treat protected items as public. */
static int parsingCSignature;           /* An explicit C/C++ signature is being parsed. */


static const char *getPythonName(moduleDef *mod, optFlags *optflgs,
        const char *cname);
static classDef *findClass(sipSpec *pt, ifaceFileType iftype,
        apiVersionRangeDef *api_range, scopedNameDef *fqname);
static classDef *findClassWithInterface(sipSpec *pt, ifaceFileDef *iff);
static classDef *newClass(sipSpec *pt, ifaceFileType iftype,
        apiVersionRangeDef *api_range, scopedNameDef *snd);
static void finishClass(sipSpec *, moduleDef *, classDef *, optFlags *);
static exceptionDef *findException(sipSpec *pt, scopedNameDef *fqname, int new);
static mappedTypeDef *newMappedType(sipSpec *,argDef *, optFlags *);
static enumDef *newEnum(sipSpec *pt, moduleDef *mod, mappedTypeDef *mt_scope,
        char *name, optFlags *of, int flags);
static void instantiateClassTemplate(sipSpec *pt, moduleDef *mod, classDef *scope, scopedNameDef *fqname, classTmplDef *tcd, templateDef *td);
static void newTypedef(sipSpec *, moduleDef *, char *, argDef *, optFlags *);
static void newVar(sipSpec *pt, moduleDef *mod, char *name, int isstatic,
        argDef *type, optFlags *of, codeBlock *acode, codeBlock *gcode,
        codeBlock *scode, int section);
static void newCtor(moduleDef *, char *, int, signatureDef *, optFlags *,
        codeBlock *, throwArgs *, signatureDef *, int, codeBlock *);
static void newFunction(sipSpec *, moduleDef *, classDef *, mappedTypeDef *,
        int, int, int, int, int, char *, signatureDef *, int, int, optFlags *,
        codeBlock *, codeBlock *, throwArgs *, signatureDef *, codeBlock *);
static optFlag *findOptFlag(optFlags *flgs, const char *name);
static optFlag *getOptFlag(optFlags *flgs, const char *name, flagType ft);
static memberDef *findFunction(sipSpec *, moduleDef *, classDef *,
        mappedTypeDef *, const char *, int, int, int);
static void checkAttributes(sipSpec *, moduleDef *, classDef *,
        mappedTypeDef *, const char *, int);
static void newModule(FILE *fp, char *filename);
static moduleDef *allocModule();
static void parseFile(FILE *fp, char *name, moduleDef *prevmod, int optional);
static void handleEOF(void);
static void handleEOM(void);
static qualDef *findQualifier(const char *name);
static const char *getInt(const char *cp, int *ip);
static scopedNameDef *text2scopedName(ifaceFileDef *scope, char *text);
static scopedNameDef *scopeScopedName(ifaceFileDef *scope,
        scopedNameDef *name);
static void pushScope(classDef *);
static void popScope(void);
static classDef *currentScope(void);
static void newQualifier(moduleDef *, int, int, const char *, qualType);
static qualDef *allocQualifier(moduleDef *, int, int, const char *, qualType);
static void newImport(char *filename);
static int timePeriod(const char *lname, const char *uname);
static int platOrFeature(char *,int);
static int isNeeded(qualDef *);
static int notSkipping(void);
static void getHooks(optFlags *,char **,char **);
static int getTransfer(optFlags *optflgs);
static int getReleaseGIL(optFlags *optflgs);
static int getHoldGIL(optFlags *optflgs);
static int getDeprecated(optFlags *optflgs);
static int getAllowNone(optFlags *optflgs);
static const char *getDocType(optFlags *optflgs);
static const char *getDocValue(optFlags *optflgs);
static void templateSignature(signatureDef *sd, int result, classTmplDef *tcd, templateDef *td, classDef *ncd);
static void templateType(argDef *ad, classTmplDef *tcd, templateDef *td, classDef *ncd);
static int search_back(const char *end, const char *start, const char *target);
static char *type2string(argDef *ad);
static char *scopedNameToString(scopedNameDef *name);
static void addUsedFromCode(sipSpec *pt, ifaceFileList **used, const char *sname);
static int sameName(scopedNameDef *snd, const char *sname);
static int stringFind(stringList *sl, const char *s);
static void setModuleName(sipSpec *pt, moduleDef *mod, const char *fullname);
static int foundInScope(scopedNameDef *fq_name, scopedNameDef *rel_name);
static void defineClass(scopedNameDef *snd, classList *supers, optFlags *of);
static classDef *completeClass(scopedNameDef *snd, optFlags *of, int has_def);
static memberDef *instantiateTemplateMethods(memberDef *tmd, moduleDef *mod);
static void instantiateTemplateEnums(sipSpec *pt, classTmplDef *tcd,
        templateDef *td, classDef *cd, ifaceFileList **used,
        scopedNameDef *type_names, scopedNameDef *type_values);
static void instantiateTemplateVars(sipSpec *pt, classTmplDef *tcd,
        templateDef *td, classDef *cd, ifaceFileList **used,
        scopedNameDef *type_names, scopedNameDef *type_values);
static overDef *instantiateTemplateOverloads(sipSpec *pt, overDef *tod,
        memberDef *tmethods, memberDef *methods, classTmplDef *tcd,
        templateDef *td, classDef *cd, ifaceFileList **used,
        scopedNameDef *type_names, scopedNameDef *type_values);
static void resolveAnyTypedef(sipSpec *pt, argDef *ad);
static void addVariable(sipSpec *pt, varDef *vd);
static void applyTypeFlags(moduleDef *mod, argDef *ad, optFlags *flags);
static argType convertEncoding(const char *encoding);
static apiVersionRangeDef *getAPIRange(optFlags *optflgs);
static apiVersionRangeDef *convertAPIRange(moduleDef *mod, nameDef *name,
        int from, int to);
static char *convertFeaturedString(char *fs);
static scopedNameDef *text2scopePart(char *text);
static KwArgs keywordArgs(moduleDef *mod, optFlags *optflgs, signatureDef *sd);
static char *strip(char *s);
static int isEnabledFeature(const char *name);
static void addProperty(sipSpec *pt, moduleDef *mod, classDef *cd,
        const char *name, const char *get, const char *set,
        codeBlock *docstring);
static moduleDef *configureModule(sipSpec *pt, moduleDef *module,
        const char *filename, const char *name, int version, int c_module,
        KwArgs kwargs, int use_arg_names, codeBlock *docstring);
static void addAutoPyName(moduleDef *mod, const char *remove_leading);
static KwArgs convertKwArgs(const char *kwargs);
static void checkAnnos(optFlags *annos, const char *valid[]);
static void checkNoAnnos(optFlags *annos, const char *msg);


/* Line 189 of yacc.c  */
#line 218 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     TK_API = 258,
     TK_AUTOPYNAME = 259,
     TK_DEFENCODING = 260,
     TK_PLUGIN = 261,
     TK_DOCSTRING = 262,
     TK_DOC = 263,
     TK_EXPORTEDDOC = 264,
     TK_EXTRACT = 265,
     TK_MAKEFILE = 266,
     TK_ACCESSCODE = 267,
     TK_GETCODE = 268,
     TK_SETCODE = 269,
     TK_PREINITCODE = 270,
     TK_INITCODE = 271,
     TK_POSTINITCODE = 272,
     TK_UNITCODE = 273,
     TK_UNITPOSTINCLUDECODE = 274,
     TK_MODCODE = 275,
     TK_TYPECODE = 276,
     TK_PREPYCODE = 277,
     TK_COPYING = 278,
     TK_MAPPEDTYPE = 279,
     TK_CODELINE = 280,
     TK_IF = 281,
     TK_END = 282,
     TK_NAME_VALUE = 283,
     TK_PATH_VALUE = 284,
     TK_STRING_VALUE = 285,
     TK_VIRTUALCATCHERCODE = 286,
     TK_TRAVERSECODE = 287,
     TK_CLEARCODE = 288,
     TK_GETBUFFERCODE = 289,
     TK_RELEASEBUFFERCODE = 290,
     TK_READBUFFERCODE = 291,
     TK_WRITEBUFFERCODE = 292,
     TK_SEGCOUNTCODE = 293,
     TK_CHARBUFFERCODE = 294,
     TK_PICKLECODE = 295,
     TK_METHODCODE = 296,
     TK_FROMTYPE = 297,
     TK_TOTYPE = 298,
     TK_TOSUBCLASS = 299,
     TK_INCLUDE = 300,
     TK_OPTINCLUDE = 301,
     TK_IMPORT = 302,
     TK_EXPHEADERCODE = 303,
     TK_MODHEADERCODE = 304,
     TK_TYPEHEADERCODE = 305,
     TK_MODULE = 306,
     TK_CMODULE = 307,
     TK_CONSMODULE = 308,
     TK_COMPOMODULE = 309,
     TK_CLASS = 310,
     TK_STRUCT = 311,
     TK_PUBLIC = 312,
     TK_PROTECTED = 313,
     TK_PRIVATE = 314,
     TK_SIGNALS = 315,
     TK_SIGNAL_METHOD = 316,
     TK_SLOTS = 317,
     TK_SLOT_METHOD = 318,
     TK_BOOL = 319,
     TK_SHORT = 320,
     TK_INT = 321,
     TK_LONG = 322,
     TK_FLOAT = 323,
     TK_DOUBLE = 324,
     TK_CHAR = 325,
     TK_WCHAR_T = 326,
     TK_VOID = 327,
     TK_PYOBJECT = 328,
     TK_PYTUPLE = 329,
     TK_PYLIST = 330,
     TK_PYDICT = 331,
     TK_PYCALLABLE = 332,
     TK_PYSLICE = 333,
     TK_PYTYPE = 334,
     TK_VIRTUAL = 335,
     TK_ENUM = 336,
     TK_SIGNED = 337,
     TK_UNSIGNED = 338,
     TK_SCOPE = 339,
     TK_LOGICAL_OR = 340,
     TK_CONST = 341,
     TK_STATIC = 342,
     TK_SIPSIGNAL = 343,
     TK_SIPSLOT = 344,
     TK_SIPANYSLOT = 345,
     TK_SIPRXCON = 346,
     TK_SIPRXDIS = 347,
     TK_SIPSLOTCON = 348,
     TK_SIPSLOTDIS = 349,
     TK_SIPSSIZET = 350,
     TK_NUMBER_VALUE = 351,
     TK_REAL_VALUE = 352,
     TK_TYPEDEF = 353,
     TK_NAMESPACE = 354,
     TK_TIMELINE = 355,
     TK_PLATFORMS = 356,
     TK_FEATURE = 357,
     TK_LICENSE = 358,
     TK_QCHAR_VALUE = 359,
     TK_TRUE_VALUE = 360,
     TK_FALSE_VALUE = 361,
     TK_NULL_VALUE = 362,
     TK_OPERATOR = 363,
     TK_THROW = 364,
     TK_QOBJECT = 365,
     TK_EXCEPTION = 366,
     TK_RAISECODE = 367,
     TK_EXPLICIT = 368,
     TK_TEMPLATE = 369,
     TK_ELLIPSIS = 370,
     TK_DEFMETATYPE = 371,
     TK_DEFSUPERTYPE = 372,
     TK_PROPERTY = 373,
     TK_GET = 374,
     TK_ID = 375,
     TK_KWARGS = 376,
     TK_LANGUAGE = 377,
     TK_LICENSEE = 378,
     TK_NAME = 379,
     TK_OPTIONAL = 380,
     TK_ORDER = 381,
     TK_REMOVELEADING = 382,
     TK_SET = 383,
     TK_SIGNATURE = 384,
     TK_TIMESTAMP = 385,
     TK_TYPE = 386,
     TK_USEARGNAMES = 387,
     TK_VERSION = 388
   };
#endif
/* Tokens.  */
#define TK_API 258
#define TK_AUTOPYNAME 259
#define TK_DEFENCODING 260
#define TK_PLUGIN 261
#define TK_DOCSTRING 262
#define TK_DOC 263
#define TK_EXPORTEDDOC 264
#define TK_EXTRACT 265
#define TK_MAKEFILE 266
#define TK_ACCESSCODE 267
#define TK_GETCODE 268
#define TK_SETCODE 269
#define TK_PREINITCODE 270
#define TK_INITCODE 271
#define TK_POSTINITCODE 272
#define TK_UNITCODE 273
#define TK_UNITPOSTINCLUDECODE 274
#define TK_MODCODE 275
#define TK_TYPECODE 276
#define TK_PREPYCODE 277
#define TK_COPYING 278
#define TK_MAPPEDTYPE 279
#define TK_CODELINE 280
#define TK_IF 281
#define TK_END 282
#define TK_NAME_VALUE 283
#define TK_PATH_VALUE 284
#define TK_STRING_VALUE 285
#define TK_VIRTUALCATCHERCODE 286
#define TK_TRAVERSECODE 287
#define TK_CLEARCODE 288
#define TK_GETBUFFERCODE 289
#define TK_RELEASEBUFFERCODE 290
#define TK_READBUFFERCODE 291
#define TK_WRITEBUFFERCODE 292
#define TK_SEGCOUNTCODE 293
#define TK_CHARBUFFERCODE 294
#define TK_PICKLECODE 295
#define TK_METHODCODE 296
#define TK_FROMTYPE 297
#define TK_TOTYPE 298
#define TK_TOSUBCLASS 299
#define TK_INCLUDE 300
#define TK_OPTINCLUDE 301
#define TK_IMPORT 302
#define TK_EXPHEADERCODE 303
#define TK_MODHEADERCODE 304
#define TK_TYPEHEADERCODE 305
#define TK_MODULE 306
#define TK_CMODULE 307
#define TK_CONSMODULE 308
#define TK_COMPOMODULE 309
#define TK_CLASS 310
#define TK_STRUCT 311
#define TK_PUBLIC 312
#define TK_PROTECTED 313
#define TK_PRIVATE 314
#define TK_SIGNALS 315
#define TK_SIGNAL_METHOD 316
#define TK_SLOTS 317
#define TK_SLOT_METHOD 318
#define TK_BOOL 319
#define TK_SHORT 320
#define TK_INT 321
#define TK_LONG 322
#define TK_FLOAT 323
#define TK_DOUBLE 324
#define TK_CHAR 325
#define TK_WCHAR_T 326
#define TK_VOID 327
#define TK_PYOBJECT 328
#define TK_PYTUPLE 329
#define TK_PYLIST 330
#define TK_PYDICT 331
#define TK_PYCALLABLE 332
#define TK_PYSLICE 333
#define TK_PYTYPE 334
#define TK_VIRTUAL 335
#define TK_ENUM 336
#define TK_SIGNED 337
#define TK_UNSIGNED 338
#define TK_SCOPE 339
#define TK_LOGICAL_OR 340
#define TK_CONST 341
#define TK_STATIC 342
#define TK_SIPSIGNAL 343
#define TK_SIPSLOT 344
#define TK_SIPANYSLOT 345
#define TK_SIPRXCON 346
#define TK_SIPRXDIS 347
#define TK_SIPSLOTCON 348
#define TK_SIPSLOTDIS 349
#define TK_SIPSSIZET 350
#define TK_NUMBER_VALUE 351
#define TK_REAL_VALUE 352
#define TK_TYPEDEF 353
#define TK_NAMESPACE 354
#define TK_TIMELINE 355
#define TK_PLATFORMS 356
#define TK_FEATURE 357
#define TK_LICENSE 358
#define TK_QCHAR_VALUE 359
#define TK_TRUE_VALUE 360
#define TK_FALSE_VALUE 361
#define TK_NULL_VALUE 362
#define TK_OPERATOR 363
#define TK_THROW 364
#define TK_QOBJECT 365
#define TK_EXCEPTION 366
#define TK_RAISECODE 367
#define TK_EXPLICIT 368
#define TK_TEMPLATE 369
#define TK_ELLIPSIS 370
#define TK_DEFMETATYPE 371
#define TK_DEFSUPERTYPE 372
#define TK_PROPERTY 373
#define TK_GET 374
#define TK_ID 375
#define TK_KWARGS 376
#define TK_LANGUAGE 377
#define TK_LICENSEE 378
#define TK_NAME 379
#define TK_OPTIONAL 380
#define TK_ORDER 381
#define TK_REMOVELEADING 382
#define TK_SET 383
#define TK_SIGNATURE 384
#define TK_TIMESTAMP 385
#define TK_TYPE 386
#define TK_USEARGNAMES 387
#define TK_VERSION 388




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 163 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"

    char            qchar;
    char            *text;
    long            number;
    double          real;
    argDef          memArg;
    signatureDef    signature;
    signatureDef    *optsignature;
    throwArgs       *throwlist;
    codeBlock       *codeb;
    valueDef        value;
    valueDef        *valp;
    optFlags        optflags;
    optFlag         flag;
    scopedNameDef   *scpvalp;
    fcallDef        fcall;
    int             boolean;
    exceptionDef    exceptionbase;
    classDef        *klass;
    apiCfg          api;
    autoPyNameCfg   autopyname;
    compModuleCfg   compmodule;
    consModuleCfg   consmodule;
    defEncodingCfg  defencoding;
    defMetatypeCfg  defmetatype;
    defSupertypeCfg defsupertype;
    exceptionCfg    exception;
    extractCfg      extract;
    featureCfg      feature;
    licenseCfg      license;
    importCfg       import;
    includeCfg      include;
    moduleCfg       module;
    pluginCfg       plugin;
    propertyCfg     property;
    variableCfg     variable;



/* Line 214 of yacc.c  */
#line 560 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 572 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1535

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  156
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  220
/* YYNRULES -- Number of rules.  */
#define YYNRULES  516
/* YYNRULES -- Number of states.  */
#define YYNSTATES  903

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   388

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   141,     2,     2,     2,   154,   146,     2,
     134,   135,   144,   143,   136,   142,     2,   145,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   151,   140,
     149,   137,   150,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   152,     2,   153,   155,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   138,   147,   139,   148,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     8,     9,    12,    14,    16,    18,
      20,    22,    24,    26,    28,    30,    32,    34,    36,    38,
      40,    42,    44,    46,    48,    50,    52,    54,    56,    58,
      60,    62,    64,    66,    68,    70,    72,    74,    76,    78,
      80,    82,    84,    86,    88,    90,    92,    94,    96,    98,
     100,   103,   105,   109,   111,   115,   119,   122,   124,   128,
     130,   134,   138,   141,   144,   148,   150,   154,   158,   162,
     168,   169,   173,   178,   180,   183,   185,   187,   189,   191,
     194,   195,   201,   202,   209,   214,   216,   219,   221,   223,
     225,   227,   230,   233,   235,   237,   251,   252,   258,   259,
     263,   265,   268,   269,   275,   277,   280,   282,   285,   287,
     291,   293,   297,   301,   302,   308,   310,   313,   315,   320,
     322,   325,   329,   334,   336,   340,   342,   346,   347,   349,
     353,   355,   359,   363,   367,   371,   375,   378,   380,   384,
     386,   390,   394,   397,   399,   403,   405,   409,   413,   417,
     419,   423,   425,   429,   433,   434,   439,   441,   444,   446,
     448,   450,   454,   456,   460,   462,   466,   470,   471,   476,
     478,   481,   483,   485,   487,   491,   495,   498,   502,   504,
     508,   512,   516,   520,   524,   528,   529,   534,   536,   539,
     541,   543,   545,   547,   549,   551,   552,   554,   557,   559,
     563,   565,   569,   573,   577,   580,   583,   585,   589,   591,
     595,   599,   600,   603,   604,   607,   608,   611,   614,   617,
     620,   623,   626,   629,   632,   635,   638,   641,   644,   647,
     650,   653,   656,   659,   662,   665,   668,   671,   674,   677,
     680,   683,   687,   689,   693,   697,   701,   703,   707,   709,
     713,   717,   721,   726,   729,   731,   734,   735,   744,   745,
     747,   748,   750,   751,   753,   755,   758,   760,   762,   767,
     768,   770,   771,   774,   775,   778,   780,   784,   786,   788,
     790,   792,   794,   796,   797,   799,   801,   803,   805,   808,
     810,   814,   816,   818,   820,   822,   827,   829,   831,   833,
     835,   837,   839,   840,   842,   846,   852,   864,   865,   866,
     875,   876,   880,   885,   886,   887,   896,   897,   900,   902,
     906,   908,   909,   913,   915,   918,   920,   922,   924,   926,
     928,   930,   932,   934,   936,   938,   940,   942,   944,   946,
     948,   950,   952,   954,   956,   958,   960,   962,   964,   966,
     969,   972,   976,   980,   984,   987,   991,   995,   997,  1001,
    1005,  1009,  1013,  1014,  1019,  1021,  1024,  1026,  1028,  1030,
    1031,  1033,  1045,  1046,  1050,  1052,  1063,  1064,  1065,  1072,
    1073,  1074,  1082,  1083,  1085,  1100,  1108,  1123,  1137,  1139,
    1141,  1143,  1145,  1147,  1149,  1151,  1153,  1156,  1159,  1162,
    1165,  1168,  1171,  1174,  1177,  1180,  1183,  1187,  1191,  1193,
    1196,  1199,  1201,  1204,  1207,  1210,  1212,  1215,  1216,  1218,
    1219,  1222,  1223,  1227,  1229,  1233,  1235,  1239,  1241,  1247,
    1249,  1251,  1254,  1255,  1257,  1258,  1261,  1262,  1265,  1267,
    1268,  1270,  1274,  1279,  1284,  1289,  1293,  1297,  1304,  1311,
    1315,  1318,  1319,  1323,  1324,  1328,  1330,  1331,  1335,  1337,
    1339,  1341,  1342,  1346,  1348,  1357,  1358,  1362,  1364,  1367,
    1369,  1371,  1374,  1377,  1380,  1385,  1389,  1393,  1394,  1396,
    1397,  1400,  1402,  1407,  1410,  1413,  1415,  1417,  1420,  1422,
    1424,  1427,  1430,  1434,  1436,  1438,  1440,  1443,  1446,  1448,
    1450,  1452,  1454,  1456,  1458,  1460,  1462,  1464,  1466,  1468,
    1470,  1472,  1476,  1477,  1482,  1483,  1485
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     157,     0,    -1,   158,    -1,   157,   158,    -1,    -1,   159,
     160,    -1,   234,    -1,   220,    -1,   227,    -1,   166,    -1,
     255,    -1,   243,    -1,   247,    -1,   248,    -1,   170,    -1,
     200,    -1,   192,    -1,   196,    -1,   208,    -1,   162,    -1,
     212,    -1,   216,    -1,   256,    -1,   257,    -1,   268,    -1,
     270,    -1,   271,    -1,   272,    -1,   273,    -1,   274,    -1,
     275,    -1,   276,    -1,   277,    -1,   282,    -1,   286,    -1,
     180,    -1,   182,    -1,   161,    -1,   204,    -1,   207,    -1,
     188,    -1,   309,    -1,   315,    -1,   312,    -1,   174,    -1,
     308,    -1,   289,    -1,   341,    -1,   364,    -1,   258,    -1,
       5,   163,    -1,    30,    -1,   134,   164,   135,    -1,   165,
      -1,   164,   136,   165,    -1,   124,   137,    30,    -1,     6,
     167,    -1,    28,    -1,   134,   168,   135,    -1,   169,    -1,
     168,   136,   169,    -1,   124,   137,    28,    -1,     3,   171,
      -1,    28,    96,    -1,   134,   172,   135,    -1,   173,    -1,
     172,   136,   173,    -1,   124,   137,    28,    -1,   133,   137,
      96,    -1,   111,   303,   175,   345,   176,    -1,    -1,   134,
     303,   135,    -1,   138,   177,   139,   140,    -1,   178,    -1,
     177,   178,    -1,   204,    -1,   207,    -1,   179,    -1,   258,
      -1,   112,   287,    -1,    -1,    24,   372,   345,   181,   184,
      -1,    -1,   314,    24,   372,   345,   183,   184,    -1,   138,
     185,   139,   140,    -1,   186,    -1,   185,   186,    -1,   204,
      -1,   207,    -1,   258,    -1,   269,    -1,    42,   287,    -1,
      43,   287,    -1,   289,    -1,   187,    -1,    87,   368,    28,
     134,   353,   135,   343,   374,   345,   338,   140,   350,   351,
      -1,    -1,    99,    28,   189,   190,   140,    -1,    -1,   138,
     191,   139,    -1,   161,    -1,   191,   161,    -1,    -1,   101,
     193,   138,   194,   139,    -1,   195,    -1,   194,   195,    -1,
      28,    -1,   102,   197,    -1,    28,    -1,   134,   198,   135,
      -1,   199,    -1,   198,   136,   199,    -1,   124,   137,    28,
      -1,    -1,   100,   201,   138,   202,   139,    -1,   203,    -1,
     202,   203,    -1,    28,    -1,    26,   134,   206,   135,    -1,
      28,    -1,   141,    28,    -1,   205,    85,    28,    -1,   205,
      85,   141,    28,    -1,   205,    -1,   292,   142,   292,    -1,
      27,    -1,   103,   209,   345,    -1,    -1,    30,    -1,   134,
     210,   135,    -1,   211,    -1,   210,   136,   211,    -1,   131,
     137,    30,    -1,   123,   137,    30,    -1,   129,   137,    30,
      -1,   130,   137,    30,    -1,   116,   213,    -1,   241,    -1,
     134,   214,   135,    -1,   215,    -1,   214,   136,   215,    -1,
     124,   137,   241,    -1,   117,   217,    -1,   241,    -1,   134,
     218,   135,    -1,   219,    -1,   218,   136,   219,    -1,   124,
     137,   241,    -1,    53,   221,   224,    -1,   241,    -1,   134,
     222,   135,    -1,   223,    -1,   222,   136,   223,    -1,   124,
     137,   241,    -1,    -1,   138,   225,   139,   140,    -1,   226,
      -1,   225,   226,    -1,   204,    -1,   207,    -1,   349,    -1,
      54,   228,   231,    -1,   241,    -1,   134,   229,   135,    -1,
     230,    -1,   229,   136,   230,    -1,   124,   137,   241,    -1,
      -1,   138,   232,   139,   140,    -1,   233,    -1,   232,   233,
      -1,   204,    -1,   207,    -1,   349,    -1,    51,   235,   238,
      -1,    52,   241,   242,    -1,   241,   242,    -1,   134,   236,
     135,    -1,   237,    -1,   236,   136,   237,    -1,   121,   137,
      30,    -1,   122,   137,    30,    -1,   124,   137,   241,    -1,
     132,   137,   305,    -1,   133,   137,    96,    -1,    -1,   138,
     239,   139,   140,    -1,   240,    -1,   239,   240,    -1,   204,
      -1,   207,    -1,   278,    -1,   349,    -1,    28,    -1,    29,
      -1,    -1,    96,    -1,    45,   244,    -1,    29,    -1,   134,
     245,   135,    -1,   246,    -1,   245,   136,   246,    -1,   124,
     137,    29,    -1,   125,   137,   305,    -1,    46,    29,    -1,
      47,   249,    -1,    29,    -1,   134,   250,   135,    -1,   251,
      -1,   250,   136,   251,    -1,   124,   137,    29,    -1,    -1,
      12,   287,    -1,    -1,    13,   287,    -1,    -1,    14,   287,
      -1,    23,   287,    -1,    48,   287,    -1,    49,   287,    -1,
      50,   287,    -1,    32,   287,    -1,    33,   287,    -1,    34,
     287,    -1,    35,   287,    -1,    36,   287,    -1,    37,   287,
      -1,    38,   287,    -1,    39,   287,    -1,    40,   287,    -1,
      20,   287,    -1,    21,   287,    -1,    15,   287,    -1,    16,
     287,    -1,    17,   287,    -1,    18,   287,    -1,    19,   287,
      -1,    22,   287,    -1,     8,   287,    -1,     9,   287,    -1,
       4,   279,    -1,   134,   280,   135,    -1,   281,    -1,   280,
     136,   281,    -1,   127,   137,    30,    -1,    10,   283,   287,
      -1,    28,    -1,   134,   284,   135,    -1,   285,    -1,   284,
     136,   285,    -1,   120,   137,    28,    -1,   126,   137,    96,
      -1,    11,    29,   291,   287,    -1,   288,    27,    -1,    25,
      -1,   288,    25,    -1,    -1,    81,   292,   345,   290,   138,
     293,   139,   140,    -1,    -1,    29,    -1,    -1,    28,    -1,
      -1,   294,    -1,   295,    -1,   294,   295,    -1,   204,    -1,
     207,    -1,    28,   297,   345,   296,    -1,    -1,   136,    -1,
      -1,   137,   302,    -1,    -1,   137,   299,    -1,   302,    -1,
     299,   300,   302,    -1,   142,    -1,   143,    -1,   144,    -1,
     145,    -1,   146,    -1,   147,    -1,    -1,   141,    -1,   148,
      -1,   142,    -1,   143,    -1,   301,   306,    -1,   304,    -1,
     303,    84,   304,    -1,    28,    -1,   105,    -1,   106,    -1,
     303,    -1,   372,   134,   307,   135,    -1,    97,    -1,    96,
      -1,   305,    -1,   107,    -1,    30,    -1,   104,    -1,    -1,
     299,    -1,   307,   136,   299,    -1,    98,   368,    28,   345,
     140,    -1,    98,   368,   134,   371,    28,   135,   134,   373,
     135,   345,   140,    -1,    -1,    -1,    56,   303,   310,   318,
     345,   311,   321,   140,    -1,    -1,   314,   313,   315,    -1,
     114,   149,   373,   150,    -1,    -1,    -1,    55,   303,   316,
     318,   345,   317,   321,   140,    -1,    -1,   151,   319,    -1,
     320,    -1,   319,   136,   320,    -1,   303,    -1,    -1,   138,
     322,   139,    -1,   323,    -1,   322,   323,    -1,   204,    -1,
     207,    -1,   188,    -1,   309,    -1,   315,    -1,   174,    -1,
     308,    -1,   289,    -1,   324,    -1,   349,    -1,   269,    -1,
     258,    -1,   259,    -1,   260,    -1,   261,    -1,   262,    -1,
     263,    -1,   264,    -1,   265,    -1,   266,    -1,   267,    -1,
     333,    -1,   332,    -1,   356,    -1,    44,   287,    -1,    43,
     287,    -1,    57,   331,   151,    -1,    58,   331,   151,    -1,
      59,   331,   151,    -1,    60,   151,    -1,   118,   325,   328,
      -1,   134,   326,   135,    -1,   327,    -1,   326,   136,   327,
      -1,   119,   137,    28,    -1,   124,   137,    28,    -1,   128,
     137,    28,    -1,    -1,   138,   329,   139,   140,    -1,   330,
      -1,   329,   330,    -1,   204,    -1,   207,    -1,   349,    -1,
      -1,    62,    -1,   340,   148,    28,   134,   135,   374,   344,
     345,   140,   351,   352,    -1,    -1,   113,   334,   335,    -1,
     335,    -1,    28,   134,   353,   135,   374,   345,   336,   140,
     350,   351,    -1,    -1,    -1,   152,   337,   134,   353,   135,
     153,    -1,    -1,    -1,   152,   339,   368,   134,   353,   135,
     153,    -1,    -1,    80,    -1,   368,    28,   134,   353,   135,
     343,   374,   344,   345,   338,   140,   350,   351,   352,    -1,
     368,   108,   137,   134,   368,   135,   140,    -1,   368,   108,
     342,   134,   353,   135,   343,   374,   344,   345,   338,   140,
     351,   352,    -1,   108,   368,   134,   353,   135,   343,   374,
     344,   345,   338,   140,   351,   352,    -1,   143,    -1,   142,
      -1,   144,    -1,   145,    -1,   154,    -1,   146,    -1,   147,
      -1,   155,    -1,   149,   149,    -1,   150,   150,    -1,   143,
     137,    -1,   142,   137,    -1,   144,   137,    -1,   145,   137,
      -1,   154,   137,    -1,   146,   137,    -1,   147,   137,    -1,
     155,   137,    -1,   149,   149,   137,    -1,   150,   150,   137,
      -1,   148,    -1,   134,   135,    -1,   152,   153,    -1,   149,
      -1,   149,   137,    -1,   137,   137,    -1,   141,   137,    -1,
     150,    -1,   150,   137,    -1,    -1,    86,    -1,    -1,   137,
      96,    -1,    -1,   145,   346,   145,    -1,   347,    -1,   346,
     136,   347,    -1,    28,    -1,    28,   137,   348,    -1,   241,
      -1,    28,   151,   242,   142,   242,    -1,    30,    -1,    96,
      -1,     7,   287,    -1,    -1,   349,    -1,    -1,    41,   287,
      -1,    -1,    31,   287,    -1,   354,    -1,    -1,   355,    -1,
     354,   136,   355,    -1,    88,   292,   345,   298,    -1,    89,
     292,   345,   298,    -1,    90,   292,   345,   298,    -1,    91,
     292,   345,    -1,    92,   292,   345,    -1,    93,   134,   353,
     135,   292,   345,    -1,    94,   134,   353,   135,   292,   345,
      -1,   110,   292,   345,    -1,   369,   298,    -1,    -1,    61,
     357,   359,    -1,    -1,    63,   358,   359,    -1,   359,    -1,
      -1,    87,   360,   361,    -1,   361,    -1,   362,    -1,   364,
      -1,    -1,    80,   363,   341,    -1,   341,    -1,   368,    28,
     345,   365,   140,   252,   253,   254,    -1,    -1,   138,   366,
     139,    -1,   367,    -1,   366,   367,    -1,   204,    -1,   207,
      -1,    12,   287,    -1,    13,   287,    -1,    14,   287,    -1,
      86,   372,   371,   370,    -1,   372,   371,   370,    -1,   368,
     292,   345,    -1,    -1,   146,    -1,    -1,   371,   144,    -1,
     303,    -1,   303,   149,   373,   150,    -1,    56,   303,    -1,
      83,    65,    -1,    65,    -1,    83,    -1,    83,    66,    -1,
      66,    -1,    67,    -1,    83,    67,    -1,    67,    67,    -1,
      83,    67,    67,    -1,    68,    -1,    69,    -1,    64,    -1,
      82,    70,    -1,    83,    70,    -1,    70,    -1,    71,    -1,
      72,    -1,    73,    -1,    74,    -1,    75,    -1,    76,    -1,
      77,    -1,    78,    -1,    79,    -1,    95,    -1,   115,    -1,
     368,    -1,   373,   136,   368,    -1,    -1,   109,   134,   375,
     135,    -1,    -1,   303,    -1,   375,   136,   303,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   480,   480,   481,   484,   484,   503,   504,   505,   506,
     507,   508,   509,   510,   511,   512,   513,   514,   515,   516,
     517,   518,   519,   520,   521,   522,   523,   524,   525,   526,
     527,   528,   529,   530,   531,   532,   533,   534,   537,   538,
     539,   540,   541,   542,   543,   544,   545,   546,   547,   548,
     561,   570,   575,   580,   581,   591,   598,   606,   611,   616,
     617,   627,   634,   660,   668,   673,   674,   685,   691,   699,
     749,   753,   827,   832,   833,   844,   847,   850,   862,   876,
     881,   881,   900,   900,   959,   973,   974,   977,   978,   979,
     983,   987,   996,  1005,  1006,  1009,  1023,  1023,  1060,  1061,
    1064,  1065,  1068,  1068,  1097,  1098,  1101,  1106,  1113,  1118,
    1123,  1124,  1134,  1141,  1141,  1167,  1168,  1171,  1177,  1190,
    1193,  1196,  1199,  1204,  1205,  1210,  1216,  1253,  1261,  1267,
    1272,  1273,  1286,  1294,  1302,  1310,  1320,  1331,  1336,  1341,
    1342,  1352,  1359,  1370,  1375,  1380,  1381,  1391,  1398,  1416,
    1421,  1426,  1427,  1437,  1444,  1448,  1453,  1454,  1464,  1467,
    1470,  1484,  1502,  1507,  1512,  1513,  1523,  1530,  1534,  1539,
    1540,  1550,  1553,  1556,  1570,  1580,  1590,  1602,  1607,  1608,
    1622,  1631,  1646,  1655,  1664,  1678,  1682,  1687,  1688,  1698,
    1701,  1704,  1707,  1721,  1722,  1738,  1741,  1744,  1753,  1759,
    1764,  1765,  1776,  1782,  1790,  1798,  1804,  1809,  1814,  1815,
    1825,  1832,  1835,  1840,  1843,  1848,  1851,  1856,  1862,  1868,
    1874,  1879,  1884,  1889,  1894,  1899,  1904,  1909,  1914,  1919,
    1924,  1930,  1935,  1941,  1947,  1953,  1959,  1965,  1970,  1976,
    1982,  1988,  1993,  1994,  2004,  2011,  2020,  2026,  2031,  2032,
    2043,  2049,  2060,  2065,  2068,  2069,  2080,  2080,  2099,  2102,
    2107,  2110,  2115,  2116,  2119,  2120,  2123,  2124,  2125,  2161,
    2162,  2165,  2166,  2169,  2172,  2177,  2178,  2196,  2199,  2202,
    2205,  2208,  2211,  2216,  2219,  2222,  2225,  2228,  2233,  2251,
    2252,  2260,  2265,  2268,  2273,  2282,  2292,  2296,  2300,  2304,
    2308,  2312,  2318,  2323,  2329,  2347,  2364,  2399,  2405,  2399,
    2438,  2438,  2464,  2469,  2475,  2469,  2505,  2506,  2509,  2510,
    2513,  2552,  2555,  2560,  2561,  2564,  2565,  2566,  2567,  2568,
    2569,  2570,  2571,  2572,  2573,  2583,  2587,  2591,  2602,  2613,
    2624,  2635,  2646,  2657,  2668,  2679,  2690,  2691,  2692,  2693,
    2704,  2715,  2722,  2729,  2736,  2745,  2758,  2763,  2764,  2776,
    2783,  2790,  2799,  2803,  2808,  2809,  2819,  2822,  2825,  2839,
    2842,  2847,  2909,  2909,  2910,  2913,  2956,  2959,  2959,  2970,
    2973,  2973,  2985,  2988,  2993,  3011,  3031,  3067,  3148,  3149,
    3150,  3151,  3152,  3153,  3154,  3155,  3156,  3157,  3158,  3159,
    3160,  3161,  3162,  3163,  3164,  3165,  3166,  3167,  3168,  3169,
    3170,  3171,  3172,  3173,  3174,  3175,  3176,  3179,  3182,  3187,
    3190,  3198,  3201,  3207,  3211,  3223,  3227,  3233,  3237,  3260,
    3264,  3270,  3275,  3278,  3281,  3284,  3289,  3292,  3297,  3345,
    3350,  3356,  3383,  3394,  3405,  3416,  3434,  3444,  3460,  3476,
    3484,  3491,  3491,  3492,  3492,  3493,  3497,  3497,  3498,  3502,
    3503,  3507,  3507,  3508,  3511,  3563,  3569,  3574,  3575,  3587,
    3590,  3593,  3607,  3621,  3637,  3642,  3656,  3759,  3762,  3770,
    3773,  3778,  3786,  3797,  3812,  3816,  3820,  3824,  3828,  3832,
    3836,  3840,  3844,  3848,  3852,  3856,  3860,  3864,  3868,  3872,
    3876,  3880,  3884,  3888,  3892,  3896,  3900,  3904,  3908,  3912,
    3918,  3924,  3940,  3943,  3951,  3957,  3964
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TK_API", "TK_AUTOPYNAME",
  "TK_DEFENCODING", "TK_PLUGIN", "TK_DOCSTRING", "TK_DOC",
  "TK_EXPORTEDDOC", "TK_EXTRACT", "TK_MAKEFILE", "TK_ACCESSCODE",
  "TK_GETCODE", "TK_SETCODE", "TK_PREINITCODE", "TK_INITCODE",
  "TK_POSTINITCODE", "TK_UNITCODE", "TK_UNITPOSTINCLUDECODE", "TK_MODCODE",
  "TK_TYPECODE", "TK_PREPYCODE", "TK_COPYING", "TK_MAPPEDTYPE",
  "TK_CODELINE", "TK_IF", "TK_END", "TK_NAME_VALUE", "TK_PATH_VALUE",
  "TK_STRING_VALUE", "TK_VIRTUALCATCHERCODE", "TK_TRAVERSECODE",
  "TK_CLEARCODE", "TK_GETBUFFERCODE", "TK_RELEASEBUFFERCODE",
  "TK_READBUFFERCODE", "TK_WRITEBUFFERCODE", "TK_SEGCOUNTCODE",
  "TK_CHARBUFFERCODE", "TK_PICKLECODE", "TK_METHODCODE", "TK_FROMTYPE",
  "TK_TOTYPE", "TK_TOSUBCLASS", "TK_INCLUDE", "TK_OPTINCLUDE", "TK_IMPORT",
  "TK_EXPHEADERCODE", "TK_MODHEADERCODE", "TK_TYPEHEADERCODE", "TK_MODULE",
  "TK_CMODULE", "TK_CONSMODULE", "TK_COMPOMODULE", "TK_CLASS", "TK_STRUCT",
  "TK_PUBLIC", "TK_PROTECTED", "TK_PRIVATE", "TK_SIGNALS",
  "TK_SIGNAL_METHOD", "TK_SLOTS", "TK_SLOT_METHOD", "TK_BOOL", "TK_SHORT",
  "TK_INT", "TK_LONG", "TK_FLOAT", "TK_DOUBLE", "TK_CHAR", "TK_WCHAR_T",
  "TK_VOID", "TK_PYOBJECT", "TK_PYTUPLE", "TK_PYLIST", "TK_PYDICT",
  "TK_PYCALLABLE", "TK_PYSLICE", "TK_PYTYPE", "TK_VIRTUAL", "TK_ENUM",
  "TK_SIGNED", "TK_UNSIGNED", "TK_SCOPE", "TK_LOGICAL_OR", "TK_CONST",
  "TK_STATIC", "TK_SIPSIGNAL", "TK_SIPSLOT", "TK_SIPANYSLOT",
  "TK_SIPRXCON", "TK_SIPRXDIS", "TK_SIPSLOTCON", "TK_SIPSLOTDIS",
  "TK_SIPSSIZET", "TK_NUMBER_VALUE", "TK_REAL_VALUE", "TK_TYPEDEF",
  "TK_NAMESPACE", "TK_TIMELINE", "TK_PLATFORMS", "TK_FEATURE",
  "TK_LICENSE", "TK_QCHAR_VALUE", "TK_TRUE_VALUE", "TK_FALSE_VALUE",
  "TK_NULL_VALUE", "TK_OPERATOR", "TK_THROW", "TK_QOBJECT", "TK_EXCEPTION",
  "TK_RAISECODE", "TK_EXPLICIT", "TK_TEMPLATE", "TK_ELLIPSIS",
  "TK_DEFMETATYPE", "TK_DEFSUPERTYPE", "TK_PROPERTY", "TK_GET", "TK_ID",
  "TK_KWARGS", "TK_LANGUAGE", "TK_LICENSEE", "TK_NAME", "TK_OPTIONAL",
  "TK_ORDER", "TK_REMOVELEADING", "TK_SET", "TK_SIGNATURE", "TK_TIMESTAMP",
  "TK_TYPE", "TK_USEARGNAMES", "TK_VERSION", "'('", "')'", "','", "'='",
  "'{'", "'}'", "';'", "'!'", "'-'", "'+'", "'*'", "'/'", "'&'", "'|'",
  "'~'", "'<'", "'>'", "':'", "'['", "']'", "'%'", "'^'", "$accept",
  "specification", "statement", "$@1", "modstatement", "nsstatement",
  "defencoding", "defencoding_args", "defencoding_arg_list",
  "defencoding_arg", "plugin", "plugin_args", "plugin_arg_list",
  "plugin_arg", "api", "api_args", "api_arg_list", "api_arg", "exception",
  "baseexception", "exception_body", "exception_body_directives",
  "exception_body_directive", "raisecode", "mappedtype", "$@2",
  "mappedtypetmpl", "$@3", "mtdefinition", "mtbody", "mtline",
  "mtfunction", "namespace", "$@4", "optnsbody", "nsbody", "platforms",
  "$@5", "platformlist", "platform", "feature", "feature_args",
  "feature_arg_list", "feature_arg", "timeline", "$@6", "qualifierlist",
  "qualifiername", "ifstart", "oredqualifiers", "qualifiers", "ifend",
  "license", "license_args", "license_arg_list", "license_arg",
  "defmetatype", "defmetatype_args", "defmetatype_arg_list",
  "defmetatype_arg", "defsupertype", "defsupertype_args",
  "defsupertype_arg_list", "defsupertype_arg", "consmodule",
  "consmodule_args", "consmodule_arg_list", "consmodule_arg",
  "consmodule_body", "consmodule_body_directives",
  "consmodule_body_directive", "compmodule", "compmodule_args",
  "compmodule_arg_list", "compmodule_arg", "compmodule_body",
  "compmodule_body_directives", "compmodule_body_directive", "module",
  "module_args", "module_arg_list", "module_arg", "module_body",
  "module_body_directives", "module_body_directive", "dottedname",
  "optnumber", "include", "include_args", "include_arg_list",
  "include_arg", "optinclude", "import", "import_args", "import_arg_list",
  "import_arg", "optaccesscode", "optgetcode", "optsetcode", "copying",
  "exphdrcode", "modhdrcode", "typehdrcode", "travcode", "clearcode",
  "getbufcode", "releasebufcode", "readbufcode", "writebufcode",
  "segcountcode", "charbufcode", "picklecode", "modcode", "typecode",
  "preinitcode", "initcode", "postinitcode", "unitcode", "unitpostinccode",
  "prepycode", "doc", "exporteddoc", "autopyname", "autopyname_args",
  "autopyname_arg_list", "autopyname_arg", "extract", "extract_args",
  "extract_arg_list", "extract_arg", "makefile", "codeblock", "codelines",
  "enum", "$@7", "optfilename", "optname", "optenumbody", "enumbody",
  "enumline", "optcomma", "optenumassign", "optassign", "expr", "binop",
  "optunop", "value", "scopedname", "scopepart", "bool_value",
  "simplevalue", "exprlist", "typedef", "struct", "$@8", "$@9",
  "classtmpl", "$@10", "template", "class", "$@11", "$@12", "superclasses",
  "superlist", "superclass", "optclassbody", "classbody", "classline",
  "property", "property_args", "property_arg_list", "property_arg",
  "property_body", "property_body_directives", "property_body_directive",
  "optslot", "dtor", "ctor", "$@13", "simplector", "optctorsig", "$@14",
  "optsig", "$@15", "optvirtual", "function", "operatorname", "optconst",
  "optabstract", "optflags", "flaglist", "flag", "flagvalue", "docstring",
  "optdocstring", "methodcode", "virtualcatchercode", "arglist",
  "rawarglist", "argvalue", "varmember", "$@16", "$@17", "simple_varmem",
  "$@18", "varmem", "member", "$@19", "variable", "variable_body",
  "variable_body_directives", "variable_body_directive", "cpptype",
  "argtype", "optref", "deref", "basetype", "cpptypelist", "optexceptions",
  "exceptionlist", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,    40,    41,    44,    61,   123,   125,
      59,    33,    45,    43,    42,    47,    38,   124,   126,    60,
      62,    58,    91,    93,    37,    94
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   156,   157,   157,   159,   158,   160,   160,   160,   160,
     160,   160,   160,   160,   160,   160,   160,   160,   160,   160,
     160,   160,   160,   160,   160,   160,   160,   160,   160,   160,
     160,   160,   160,   160,   160,   160,   160,   160,   161,   161,
     161,   161,   161,   161,   161,   161,   161,   161,   161,   161,
     162,   163,   163,   164,   164,   165,   166,   167,   167,   168,
     168,   169,   170,   171,   171,   172,   172,   173,   173,   174,
     175,   175,   176,   177,   177,   178,   178,   178,   178,   179,
     181,   180,   183,   182,   184,   185,   185,   186,   186,   186,
     186,   186,   186,   186,   186,   187,   189,   188,   190,   190,
     191,   191,   193,   192,   194,   194,   195,   196,   197,   197,
     198,   198,   199,   201,   200,   202,   202,   203,   204,   205,
     205,   205,   205,   206,   206,   207,   208,   209,   209,   209,
     210,   210,   211,   211,   211,   211,   212,   213,   213,   214,
     214,   215,   216,   217,   217,   218,   218,   219,   220,   221,
     221,   222,   222,   223,   224,   224,   225,   225,   226,   226,
     226,   227,   228,   228,   229,   229,   230,   231,   231,   232,
     232,   233,   233,   233,   234,   234,   235,   235,   236,   236,
     237,   237,   237,   237,   237,   238,   238,   239,   239,   240,
     240,   240,   240,   241,   241,   242,   242,   243,   244,   244,
     245,   245,   246,   246,   247,   248,   249,   249,   250,   250,
     251,   252,   252,   253,   253,   254,   254,   255,   256,   257,
     258,   259,   260,   261,   262,   263,   264,   265,   266,   267,
     268,   269,   270,   271,   272,   273,   274,   275,   276,   277,
     278,   279,   280,   280,   281,   282,   283,   283,   284,   284,
     285,   285,   286,   287,   288,   288,   290,   289,   291,   291,
     292,   292,   293,   293,   294,   294,   295,   295,   295,   296,
     296,   297,   297,   298,   298,   299,   299,   300,   300,   300,
     300,   300,   300,   301,   301,   301,   301,   301,   302,   303,
     303,   304,   305,   305,   306,   306,   306,   306,   306,   306,
     306,   306,   307,   307,   307,   308,   308,   310,   311,   309,
     313,   312,   314,   316,   317,   315,   318,   318,   319,   319,
     320,   321,   321,   322,   322,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   324,   325,   326,   326,   327,
     327,   327,   328,   328,   329,   329,   330,   330,   330,   331,
     331,   332,   334,   333,   333,   335,   336,   337,   336,   338,
     339,   338,   340,   340,   341,   341,   341,   341,   342,   342,
     342,   342,   342,   342,   342,   342,   342,   342,   342,   342,
     342,   342,   342,   342,   342,   342,   342,   342,   342,   342,
     342,   342,   342,   342,   342,   342,   342,   343,   343,   344,
     344,   345,   345,   346,   346,   347,   347,   348,   348,   348,
     348,   349,   350,   350,   351,   351,   352,   352,   353,   354,
     354,   354,   355,   355,   355,   355,   355,   355,   355,   355,
     355,   357,   356,   358,   356,   356,   360,   359,   359,   361,
     361,   363,   362,   362,   364,   365,   365,   366,   366,   367,
     367,   367,   367,   367,   368,   368,   369,   370,   370,   371,
     371,   372,   372,   372,   372,   372,   372,   372,   372,   372,
     372,   372,   372,   372,   372,   372,   372,   372,   372,   372,
     372,   372,   372,   372,   372,   372,   372,   372,   372,   372,
     373,   373,   374,   374,   375,   375,   375
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     0,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     1,     3,     1,     3,     3,     2,     1,     3,     1,
       3,     3,     2,     2,     3,     1,     3,     3,     3,     5,
       0,     3,     4,     1,     2,     1,     1,     1,     1,     2,
       0,     5,     0,     6,     4,     1,     2,     1,     1,     1,
       1,     2,     2,     1,     1,    13,     0,     5,     0,     3,
       1,     2,     0,     5,     1,     2,     1,     2,     1,     3,
       1,     3,     3,     0,     5,     1,     2,     1,     4,     1,
       2,     3,     4,     1,     3,     1,     3,     0,     1,     3,
       1,     3,     3,     3,     3,     3,     2,     1,     3,     1,
       3,     3,     2,     1,     3,     1,     3,     3,     3,     1,
       3,     1,     3,     3,     0,     4,     1,     2,     1,     1,
       1,     3,     1,     3,     1,     3,     3,     0,     4,     1,
       2,     1,     1,     1,     3,     3,     2,     3,     1,     3,
       3,     3,     3,     3,     3,     0,     4,     1,     2,     1,
       1,     1,     1,     1,     1,     0,     1,     2,     1,     3,
       1,     3,     3,     3,     2,     2,     1,     3,     1,     3,
       3,     0,     2,     0,     2,     0,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     3,     1,     3,     3,     3,     1,     3,     1,     3,
       3,     3,     4,     2,     1,     2,     0,     8,     0,     1,
       0,     1,     0,     1,     1,     2,     1,     1,     4,     0,
       1,     0,     2,     0,     2,     1,     3,     1,     1,     1,
       1,     1,     1,     0,     1,     1,     1,     1,     2,     1,
       3,     1,     1,     1,     1,     4,     1,     1,     1,     1,
       1,     1,     0,     1,     3,     5,    11,     0,     0,     8,
       0,     3,     4,     0,     0,     8,     0,     2,     1,     3,
       1,     0,     3,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       2,     3,     3,     3,     2,     3,     3,     1,     3,     3,
       3,     3,     0,     4,     1,     2,     1,     1,     1,     0,
       1,    11,     0,     3,     1,    10,     0,     0,     6,     0,
       0,     7,     0,     1,    14,     7,    14,    13,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     3,     3,     1,     2,
       2,     1,     2,     2,     2,     1,     2,     0,     1,     0,
       2,     0,     3,     1,     3,     1,     3,     1,     5,     1,
       1,     2,     0,     1,     0,     2,     0,     2,     1,     0,
       1,     3,     4,     4,     4,     3,     3,     6,     6,     3,
       2,     0,     3,     0,     3,     1,     0,     3,     1,     1,
       1,     0,     3,     1,     8,     0,     3,     1,     2,     1,
       1,     2,     2,     2,     4,     3,     3,     0,     1,     0,
       2,     1,     4,     2,     2,     1,     1,     2,     1,     1,
       2,     2,     3,     1,     1,     1,     2,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     0,     4,     0,     1,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       4,     4,     2,     0,     1,     3,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   125,   291,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   495,   485,   488,
     489,   493,   494,   498,   499,   500,   501,   502,   503,   504,
     505,   506,   507,   260,     0,   486,     0,   508,     0,     0,
     113,   102,     0,   127,     0,     0,     0,   509,     0,     0,
       5,    37,    19,     9,    14,    44,    35,    36,    40,    16,
      17,    15,    38,    39,    18,    20,    21,     7,     8,     6,
      11,    12,    13,    10,    22,    23,    49,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    46,   481,
     289,    45,    41,    43,   310,    42,    47,    48,     0,   479,
       0,     0,    62,    51,     0,    50,    57,     0,    56,   254,
     238,     0,   239,   246,     0,     0,   258,   232,   233,   234,
     235,   236,   230,   237,   217,     0,   421,   260,   198,     0,
     197,   204,   206,     0,   205,   218,   219,   220,   193,   194,
       0,   185,   195,   195,     0,   154,   149,     0,   167,   162,
     313,   307,   491,   261,   421,   496,   484,   487,   490,   497,
     479,     0,    96,     0,     0,   108,     0,   107,   128,     0,
     421,     0,    70,     0,     0,   136,   137,     0,   142,   143,
       0,     0,     0,     0,   421,     0,   477,    63,     0,     0,
       0,    65,     0,     0,    53,     0,     0,    59,   255,   253,
       0,     0,     0,   248,   245,   259,     0,   483,     0,    80,
     119,     0,   123,     0,     0,     0,     0,     0,   200,     0,
       0,   208,     0,     0,     0,     0,     0,     0,   178,     0,
     174,   196,   176,   175,     0,     0,   151,     0,   148,     0,
       0,   164,     0,   161,   316,   316,   256,   492,   477,   421,
     479,    98,     0,     0,     0,     0,   110,     0,     0,     0,
       0,     0,   130,   126,   439,     0,   421,   510,     0,     0,
       0,   139,     0,     0,   145,   290,     0,   421,   311,   439,
     465,     0,     0,     0,   389,   388,   390,   391,   393,   394,
     408,   411,   415,     0,   392,   395,     0,   480,   478,   475,
       0,     0,    64,     0,     0,    52,     0,     0,    58,     0,
       0,     0,   247,     0,   252,   425,     0,   423,     0,   120,
       0,   118,   260,     0,     0,   199,     0,     0,   207,     0,
       0,     0,     0,     0,     0,   177,     0,     0,     0,   189,
     190,     0,   187,   191,   192,     0,   150,     0,   158,   159,
       0,   156,   160,     0,   163,     0,   171,   172,     0,   169,
     173,     0,   421,   421,     0,   474,     0,     0,     0,     0,
     117,     0,   115,   106,     0,   104,     0,   109,     0,     0,
       0,     0,     0,   129,     0,   260,   260,   260,   260,   260,
       0,     0,   260,     0,   438,   440,   260,   273,     0,     0,
       0,   312,     0,   138,     0,     0,   144,     0,   482,    82,
       0,     0,     0,   409,     0,   413,   414,   399,   398,   400,
     401,   403,   404,   412,   396,   416,   397,   410,   402,   405,
     439,    67,    68,    66,    55,    54,    61,    60,   250,   251,
     249,     0,     0,   422,     0,    81,   121,     0,   124,   202,
     292,   293,   203,   201,   210,   209,   180,   181,   182,   183,
     184,   179,     0,   240,   431,     0,   188,   153,   152,     0,
     157,   166,   165,     0,   170,   320,   317,   318,   314,   308,
     262,   305,     0,   100,     0,   310,    97,   114,   116,   103,
     105,   112,   111,   133,   134,   135,   132,   131,   421,   421,
     421,   421,   421,   439,   439,   421,   417,     0,   421,   283,
     450,    71,     0,    69,   511,   141,   140,   147,   146,     0,
     417,     0,     0,     0,   469,   470,     0,   467,   211,     0,
     406,   407,     0,   193,   429,   430,   427,   426,   424,     0,
       0,     0,     0,     0,    85,    94,    87,    88,    89,    90,
      93,   122,     0,     0,   242,   186,   155,   168,     0,   321,
     321,   271,   266,   267,     0,   263,   264,     0,    99,   101,
     273,   273,   273,   445,   446,     0,     0,   449,   418,   512,
     441,   476,   284,   286,   287,   285,   274,     0,   275,     0,
       0,    73,    77,    75,    76,    78,    83,   512,   471,   472,
     473,   466,   468,     0,   213,     0,   417,   195,   231,    91,
      92,     0,     0,    86,     0,   241,     0,   319,   382,     0,
       0,   283,   421,     0,   265,     0,   442,   443,   444,   260,
     260,     0,   419,   277,   278,   279,   280,   281,   282,   283,
     300,   297,   296,   301,   299,   294,   298,   288,     0,    79,
       0,    74,   419,   212,     0,   215,   385,   512,     0,     0,
      84,   244,   243,   291,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   369,   369,   369,     0,   451,
     453,   461,   456,   372,     0,   330,   327,   325,   326,   336,
     337,   338,   339,   340,   341,   342,   343,   344,   345,   335,
     332,   331,   328,   329,   382,   323,   333,   347,   346,   374,
       0,   463,   334,   348,   455,   458,   459,   460,   315,   309,
     272,   269,   257,     0,   421,   421,   514,     0,   421,   276,
     283,    72,   421,   214,     0,   464,   419,   195,   439,   439,
     221,   222,   223,   224,   225,   226,   227,   228,   229,   350,
     349,   370,     0,     0,     0,   354,     0,     0,     0,     0,
       0,     0,   362,   322,   324,     0,   270,   268,   421,   447,
     448,   515,     0,   420,   379,   303,     0,   379,   216,   421,
     428,     0,     0,   351,   352,   353,   461,   452,   454,   462,
       0,   457,     0,   373,     0,     0,     0,     0,   357,     0,
     355,     0,     0,   513,     0,   380,     0,   295,   283,     0,
     379,   417,   512,     0,     0,     0,     0,   356,     0,   366,
     367,     0,   364,   368,     0,   306,   516,     0,   434,   304,
     432,     0,   512,   421,   359,   360,   361,   358,     0,   365,
     512,     0,     0,   436,   433,   434,   434,   421,   376,   363,
     419,   439,   435,     0,   387,   436,   436,   379,   377,     0,
     421,     0,   437,   384,   386,     0,     0,   432,     0,     0,
     432,   439,   434,   434,   381,   434,     0,   375,   436,    95,
       0,   371,   378
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,     3,    70,    71,    72,   125,   213,   214,
      73,   128,   216,   217,    74,   122,   210,   211,    75,   286,
     533,   610,   611,   612,    76,   338,    77,   539,   465,   563,
     564,   565,    78,   271,   389,   504,    79,   184,   394,   395,
      80,   187,   275,   276,    81,   183,   391,   392,    82,   232,
     233,    83,    84,   190,   281,   282,    85,   195,   290,   291,
      86,   198,   293,   294,    87,   165,   255,   256,   258,   370,
     371,    88,   168,   260,   261,   263,   378,   379,    89,   161,
     247,   248,   250,   361,   362,   162,   252,    90,   150,   237,
     238,    91,    92,   154,   240,   241,   624,   675,   755,    93,
      94,    95,    96,   710,   711,   712,   713,   714,   715,   716,
     717,   718,    97,   569,    98,    99,   100,   101,   102,   103,
     104,   105,   363,   483,   573,   574,   106,   135,   222,   223,
     107,   130,   131,   108,   384,   226,   174,   584,   585,   586,
     787,   642,   530,   606,   659,   607,   608,   109,   110,   472,
     667,   796,   111,   112,   265,   580,   113,   203,   505,   115,
     264,   579,   382,   496,   497,   639,   724,   725,   726,   782,
     817,   818,   820,   841,   842,   772,   727,   728,   780,   729,
     879,   886,   826,   847,   730,   731,   316,   599,   748,   229,
     336,   337,   557,   864,   865,   863,   874,   413,   414,   415,
     733,   776,   777,   734,   779,   735,   736,   778,   737,   432,
     546,   547,   416,   417,   319,   206,   119,   288,   652,   792
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -668
static const yytype_int16 yypact[] =
{
    -668,   104,  -668,   935,  -668,  -668,    52,    41,    66,    89,
      89,    71,   100,    89,    89,    89,    89,    89,    89,    89,
      89,   500,     3,  -668,  -668,    32,   175,    50,    89,    89,
      89,    58,   216,    62,    74,   116,   116,  -668,  -668,  -668,
     107,  -668,  -668,  -668,  -668,  -668,  -668,  -668,  -668,  -668,
    -668,  -668,  -668,   169,   149,   266,   500,  -668,  1420,   224,
    -668,  -668,    83,    54,  1420,   116,   117,  -668,    78,    80,
    -668,  -668,  -668,  -668,  -668,  -668,  -668,  -668,  -668,  -668,
    -668,  -668,  -668,  -668,  -668,  -668,  -668,  -668,  -668,  -668,
    -668,  -668,  -668,  -668,  -668,  -668,  -668,  -668,  -668,  -668,
    -668,  -668,  -668,  -668,  -668,  -668,  -668,  -668,  -668,    44,
    -668,  -668,  -668,  -668,   256,  -668,  -668,  -668,    73,  -668,
     180,    56,  -668,  -668,   162,  -668,  -668,   166,  -668,  -668,
    -668,   293,  -668,  -668,   148,    89,   275,  -668,  -668,  -668,
    -668,  -668,  -668,  -668,  -668,   116,   150,     6,  -668,   185,
    -668,  -668,  -668,   223,  -668,  -668,  -668,  -668,  -668,  -668,
     205,   196,   261,   261,   227,   254,  -668,   241,   278,  -668,
     337,    16,  -668,  -668,   150,  -668,  -668,  -668,   306,  -668,
    -668,    87,  -668,   284,   285,  -668,   300,  -668,  -668,   168,
     150,   291,    81,  1420,   302,  -668,  -668,   304,  -668,  -668,
     116,  1420,   500,   374,     1,   504,   184,  -668,   294,   297,
     208,  -668,   298,   210,  -668,   299,   214,  -668,  -668,  -668,
     301,   303,   218,  -668,  -668,  -668,    89,   337,   402,  -668,
     305,   405,   352,   308,   307,   309,   311,   220,  -668,   313,
     232,  -668,   315,   316,   317,   318,   319,   235,  -668,   126,
    -668,  -668,  -668,  -668,   320,   246,  -668,   215,  -668,   321,
     248,  -668,   215,  -668,   288,   288,  -668,  -668,   184,   150,
    -668,   322,   413,   416,   324,   252,  -668,   325,   326,   327,
     330,   255,  -668,  -668,  1152,   116,   150,  -668,    27,   331,
     259,  -668,   332,   262,  -668,  -668,   115,   150,  -668,  1152,
     333,   335,   120,   336,   338,   339,   340,   341,   342,   343,
    -668,    99,    96,   329,   349,   350,   354,  -668,  -668,  -668,
     431,   376,  -668,    56,   444,  -668,   162,   461,  -668,   166,
     462,   395,  -668,   148,  -668,   355,    90,  -668,   356,  -668,
      15,  -668,   169,   464,   295,  -668,   185,   466,  -668,   223,
     468,   469,   216,   295,   400,  -668,   205,   366,    89,  -668,
    -668,    33,  -668,  -668,  -668,   216,  -668,   227,  -668,  -668,
      40,  -668,  -668,   216,  -668,   241,  -668,  -668,    43,  -668,
    -668,   116,   150,   150,   363,  -668,   364,    11,  1096,   365,
    -668,    29,  -668,  -668,    30,  -668,   478,  -668,   300,   477,
     479,   480,   481,  -668,   168,   169,   169,   169,   169,   169,
     380,   381,   169,   373,   383,  -668,   169,   379,    35,   382,
    1420,  -668,   216,  -668,   302,   216,  -668,   304,  -668,  -668,
     386,   289,   377,  -668,  1420,  -668,  -668,  -668,  -668,  -668,
    -668,  -668,  -668,  -668,   385,  -668,   387,  -668,  -668,  -668,
    1152,  -668,  -668,  -668,  -668,  -668,  -668,  -668,  -668,  -668,
    -668,   200,   402,  -668,   213,  -668,  -668,   495,  -668,  -668,
    -668,  -668,  -668,  -668,  -668,  -668,  -668,  -668,  -668,  -668,
    -668,  -668,   403,  -668,  -668,   389,  -668,  -668,  -668,   394,
    -668,  -668,  -668,   396,  -668,   337,   391,  -668,  -668,  -668,
     296,  -668,   404,  -668,   683,  -668,  -668,  -668,  -668,  -668,
    -668,  -668,  -668,  -668,  -668,  -668,  -668,  -668,   150,   150,
     150,   150,   150,  1152,  1152,   150,   449,  1152,   150,   129,
    -668,  -668,    91,  -668,  -668,  -668,  -668,  -668,  -668,   356,
     449,    89,    89,    89,  -668,  -668,    51,  -668,   526,   406,
    -668,  -668,   407,   392,  -668,  -668,  -668,  -668,  -668,    89,
      89,    89,  1420,    55,  -668,  -668,  -668,  -668,  -668,  -668,
    -668,  -668,   408,   268,  -668,  -668,  -668,  -668,   116,   409,
     409,   411,  -668,  -668,   401,   296,  -668,   410,  -668,  -668,
     379,   379,   379,  -668,  -668,   417,   418,  -668,  -668,   437,
    -668,  -668,  -668,  -668,  -668,  -668,   233,  1209,  -668,    89,
      46,  -668,  -668,  -668,  -668,  -668,  -668,   437,  -668,  -668,
    -668,  -668,  -668,    89,   541,   419,   449,   261,  -668,  -668,
    -668,   529,   421,  -668,   528,  -668,   403,  -668,  1032,   422,
     423,   129,   150,   440,  -668,  1420,  -668,  -668,  -668,   169,
     169,   447,   450,  -668,  -668,  -668,  -668,  -668,  -668,   129,
    -668,  -668,  -668,  -668,  -668,    64,  -668,  -668,   454,  -668,
     451,  -668,   450,  -668,    89,   576,  -668,   437,   452,   467,
    -668,  -668,  -668,   470,    89,    89,    89,    89,    89,    89,
      89,    89,    89,    89,    89,   540,   540,   540,   455,  -668,
    -668,   457,  -668,  -668,   473,  -668,  -668,  -668,  -668,  -668,
    -668,  -668,  -668,  -668,  -668,  -668,  -668,  -668,  -668,  -668,
    -668,  -668,  -668,  -668,   821,  -668,  -668,  -668,  -668,  -668,
     463,  -668,  -668,  -668,  -668,  -668,  -668,  -668,  -668,  -668,
    -668,   474,  -668,   270,   150,   150,   116,   507,   150,  -668,
     171,  -668,   150,  -668,    89,  -668,   450,   261,  1152,  1152,
    -668,  -668,  -668,  -668,  -668,  -668,  -668,  -668,  -668,  -668,
    -668,  -668,   458,   465,   471,  -668,  1261,  1261,  1367,  1314,
     584,   145,   483,  -668,  -668,   590,  -668,  -668,   150,  -668,
    -668,   337,   272,  -668,   472,   233,   274,   472,  -668,   150,
    -668,   484,   488,  -668,  -668,  -668,  -668,  -668,  -668,  -668,
     103,  -668,   470,  -668,   489,   490,   491,   277,  -668,   215,
    -668,   497,   485,  -668,   116,  -668,   492,  -668,   129,   493,
     472,   449,   437,   501,   606,   609,   611,  -668,   145,  -668,
    -668,    48,  -668,  -668,   509,  -668,   337,  1420,   601,   233,
     648,   517,   437,   150,  -668,  -668,  -668,  -668,   520,  -668,
     437,   527,    89,   631,  -668,   601,   601,   150,   511,  -668,
     450,  1152,  -668,    89,  -668,   631,   631,   472,  -668,   525,
     150,   531,  -668,  -668,  -668,   530,   533,   648,   532,   515,
     648,  1152,   601,   601,  -668,   601,   534,  -668,   631,  -668,
     518,  -668,  -668
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -668,  -668,   672,  -668,  -668,  -340,  -668,  -668,  -668,   359,
    -668,  -668,  -668,   357,  -668,  -668,  -668,   367,  -570,  -668,
    -668,  -668,    77,  -668,  -668,  -668,  -668,  -668,   152,  -668,
     125,  -668,  -553,  -668,  -668,  -668,  -668,  -668,  -668,   310,
    -668,  -668,  -668,   314,  -668,  -668,  -668,   312,  -221,  -668,
    -668,  -211,  -668,  -668,  -668,   292,  -668,  -668,  -668,   265,
    -668,  -668,  -668,   271,  -668,  -668,  -668,   328,  -668,  -668,
     323,  -668,  -668,  -668,   344,  -668,  -668,   346,  -668,  -668,
    -668,   345,  -668,  -668,   353,    20,  -161,  -668,  -668,  -668,
     351,  -668,  -668,  -668,  -668,   358,  -668,  -668,  -668,  -668,
    -668,  -668,  -437,  -668,  -668,  -668,  -668,  -668,  -668,  -668,
    -668,  -668,  -668,  -522,  -668,  -668,  -668,  -668,  -668,  -668,
    -668,  -668,  -668,  -668,  -668,    63,  -668,  -668,  -668,   369,
    -668,   -10,  -668,  -431,  -668,  -668,  -124,  -668,  -668,   121,
    -668,  -668,  -251,  -667,  -668,  -668,  -520,   -23,   505,  -329,
    -668,  -668,  -518,  -515,  -668,  -668,  -668,  -668,   689,  -187,
    -668,  -668,   435,  -668,   135,   136,  -668,    -9,  -668,  -668,
    -668,  -121,  -668,  -668,  -123,  -282,  -668,  -668,  -668,   -60,
    -668,  -668,  -652,  -668,  -668,     8,  -668,  -514,  -643,  -159,
    -668,   263,  -668,  -227,  -582,  -634,  -638,  -274,  -668,   199,
    -668,  -668,  -668,  -357,  -668,   -52,  -668,  -668,    14,  -668,
    -668,   182,    -2,  -668,   475,   -87,    -7,  -180,  -585,  -668
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -484
static const yytype_int16 yytable[] =
{
     132,   118,   253,   137,   138,   139,   140,   141,   142,   143,
     144,   116,   170,   171,   146,   266,   298,   117,   155,   156,
     157,   296,   364,   234,   479,   430,   617,   568,   359,   752,
     372,   283,   672,   570,   230,   380,   368,   357,   360,   502,
     358,   376,   192,   466,  -483,   300,   369,   358,   503,   180,
     358,   377,   163,   166,   169,   358,   181,   390,   393,    22,
      23,   148,   191,   541,   542,   543,    22,    23,   705,    22,
      23,   123,    22,    23,    22,    23,   559,    22,    23,   152,
     120,    22,    23,   795,   188,   706,   158,   159,   196,   199,
     158,   159,   756,   268,   126,   615,    30,   560,   561,   133,
     200,   204,   158,   159,     4,    30,   158,   159,   158,   159,
     386,   185,   677,   799,   129,   269,   719,    22,    23,   200,
     721,   740,   227,   722,  -483,   224,   568,   419,   200,   136,
     357,   833,   570,   358,   364,   299,    53,   147,   429,   749,
     359,    30,   562,   372,    24,   829,   228,   231,   200,   368,
     360,   380,    22,    23,   705,   317,   467,   376,   609,   369,
    -483,   849,  -483,   420,   589,   200,   149,   377,   507,   509,
     531,   706,   485,   615,   172,   124,   552,   421,   851,   489,
     208,   205,   493,   387,   153,   670,   121,   858,   189,   209,
     621,   287,   160,   201,   632,   297,   164,   173,  -481,   287,
     127,   709,   719,   609,   151,   134,   721,   720,   167,   722,
     544,   205,   194,   201,   197,   285,   334,   186,   468,   175,
     545,   270,   358,   498,   499,   885,   462,   880,   553,   159,
     554,   875,   876,   445,   559,   463,   443,   883,   884,    22,
      23,    22,    23,   566,   158,   159,   446,   853,   444,   595,
     596,   420,   182,   567,   434,   560,   561,   435,   897,   898,
     901,   899,   418,    30,   814,   428,   193,   867,   220,   815,
     602,   603,   604,   816,   221,   870,   207,   605,   666,   582,
     202,   518,   519,   520,   521,   522,   212,   709,   525,   583,
     215,   277,   528,   720,    53,   228,   555,   278,   279,   280,
     562,   541,   542,   543,   225,   892,  -302,  -302,   895,   235,
     236,   613,   602,   603,   604,    22,    23,   852,   218,   605,
     219,   614,    22,    23,   581,   544,   242,   243,   317,   244,
     318,   176,   177,   178,   249,   545,   179,   245,   246,   646,
     647,   648,   566,   322,   323,   325,   326,   239,   484,   328,
     329,   254,   567,   332,   333,   345,   346,   251,   495,   590,
     591,   592,   593,   594,   582,   259,   597,   348,   349,   601,
     355,   356,   478,   267,   583,   653,   654,   655,   656,   657,
     658,   366,   367,   374,   375,   487,   118,   397,   398,   613,
     403,   404,   257,   491,   423,   424,   116,   426,   427,   614,
     470,   471,   117,   635,   636,   788,   420,   823,   824,   827,
     828,   732,   837,   838,   773,   774,   262,   707,   534,   807,
     808,   200,   272,   273,   274,   284,   289,   708,   292,    35,
     335,   320,   549,   339,   321,   324,   327,   340,   330,   381,
     331,   390,   535,   341,   393,   537,   343,  -261,   344,   342,
     347,   723,   350,   351,   352,   353,   354,   365,   373,   451,
     388,   396,   399,   400,   401,   743,   678,   402,   422,   425,
     433,   431,   452,   436,   454,   437,   438,   439,   440,   441,
     442,   556,   447,   741,   801,   802,   448,   449,   450,   456,
     458,   459,   461,   469,   464,   474,   480,   732,   476,   477,
     482,   500,   118,   707,   501,   506,   511,   513,   526,   514,
     515,   516,   116,   708,   523,   524,   529,   548,   117,   527,
     532,   540,   550,   571,   551,   744,   745,   578,    24,   575,
     572,   618,   619,   620,   576,   598,   577,   723,   623,   587,
     643,   625,   626,   627,   645,   634,   651,   638,   641,   628,
     629,   630,   649,   650,   674,   495,   145,   679,   681,   676,
     631,   680,   738,   739,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
     742,   746,    54,    55,   665,   789,   790,   747,   750,   794,
     754,   751,   843,   797,   757,    57,   800,   881,   839,   669,
     668,   758,   771,   793,   759,  -383,   775,   781,   840,   803,
     786,   785,   812,   673,   843,    67,   804,   896,   821,   831,
     839,   819,   805,   832,   825,   845,   834,   835,   836,   822,
     840,   844,   848,   850,   854,   299,   118,   855,   301,   856,
     830,   302,   862,   287,   860,   303,   304,   305,   306,   307,
     308,   309,   310,   311,   312,   358,   313,   866,   314,   315,
     869,   871,   873,   878,   753,   887,   889,   891,   894,   900,
     890,   902,   893,     5,   760,   761,   762,   763,   764,   765,
     766,   767,   768,   769,   770,   455,   457,   671,   633,   536,
     453,   616,   114,   490,   868,   488,   517,   473,   538,   682,
     383,   481,   460,   508,   510,   295,   644,   475,   877,    22,
      23,    24,   512,   637,   486,   784,   640,   857,   859,   492,
     813,   888,   118,   791,   494,   558,   600,   811,   622,     0,
       0,     0,     0,    30,     0,     0,     0,     0,    35,    36,
       0,     0,     0,   385,   798,     0,     0,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,     0,    53,    54,    55,     0,     0,    56,
       0,     0,     0,     0,   118,   118,   810,   118,    57,     0,
       0,    58,    59,     0,     0,     0,   809,     0,     0,     0,
       0,    64,     0,     0,    65,     0,     0,    66,    67,     0,
       0,   846,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   588,     0,     0,     0,     0,     0,   358,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   559,     0,     0,   861,     0,    22,    23,   683,
       0,     0,   872,   684,   685,   686,   687,   688,   689,   690,
     691,   692,     0,   882,   693,   694,     0,     0,     0,     0,
       0,    30,     0,     0,     0,     0,    35,    36,   695,   696,
     697,   698,   699,     0,   700,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,   701,    53,    54,    55,     0,     0,    56,   702,     0,
       0,     0,     0,     0,     0,     0,    57,     0,     0,    58,
      59,     0,     0,     0,     0,     0,     0,     0,     0,    64,
       0,     0,    65,     0,   703,     0,    67,     0,     6,   704,
       7,     8,     0,     9,    10,    11,    12,     0,     0,     0,
      13,    14,    15,    16,    17,    18,     0,    19,    20,    21,
     783,    22,    23,    24,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,     0,     0,     0,     0,     0,     0,     0,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,     0,    53,    54,    55,     0,
       0,    56,     0,     0,     0,     0,     0,     0,     0,     0,
      57,     0,     0,    58,    59,    60,    61,    62,    63,   358,
       0,     0,     0,    64,     0,     0,    65,     0,     0,    66,
      67,    68,    69,   559,     0,     0,     0,     0,    22,    23,
     683,     0,     0,     0,   684,   685,   686,   687,   688,   689,
     690,   691,   692,     0,     0,   693,   694,     0,     0,     0,
       0,     0,    30,     0,     0,     0,     0,    35,    36,   695,
     696,   697,   698,   699,     0,   700,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,   701,    53,    54,    55,     0,     0,    56,   702,
       0,     0,    22,    23,    24,     0,     0,    57,     0,     0,
      58,    59,     0,     0,     0,     0,     0,     0,     0,     0,
      64,     0,     0,    65,     0,   703,    30,    67,     0,     0,
     704,    35,    36,     0,     0,     0,     0,     0,     0,     0,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,     0,    53,    54,    55,
      24,     0,    56,     0,     0,     0,     0,     0,     0,     0,
       0,    57,     0,     0,    58,    59,     0,     0,     0,     0,
       0,     0,     0,     0,    64,     0,     0,    65,   145,     0,
      66,    67,     0,     0,     0,     0,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,     0,     0,    54,    55,     0,    24,    56,   660,
     405,   406,   407,   408,   409,   410,   411,    57,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   412,     0,     0,   145,     0,    67,     0,     0,
       0,     0,     0,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    24,
       0,    54,    55,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,   661,   662,     0,     0,     0,
       0,     0,     0,   663,   470,   471,   664,   145,     0,     0,
       0,     0,     0,     0,    67,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,   806,    24,    54,    55,     0,     0,    56,   702,     0,
       0,     0,     0,     0,     0,     0,    57,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    64,
     145,     0,     0,     0,     0,     0,    67,     0,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,   806,    24,    54,    55,     0,     0,
      56,     0,     0,     0,     0,     0,     0,     0,     0,    57,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    64,   145,     0,     0,     0,     0,     0,    67,
       0,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,     0,    24,    54,
      55,     0,     0,    56,     0,     0,     0,     0,     0,     0,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    64,   145,     0,     0,     0,
       0,     0,    67,     0,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
       0,     0,    54,    55,     0,     0,    56,     0,     0,     0,
       0,     0,     0,     0,     0,    57,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    67
};

static const yytype_int16 yycheck[] =
{
      10,     3,   163,    13,    14,    15,    16,    17,    18,    19,
      20,     3,    35,    36,    21,   174,   203,     3,    28,    29,
      30,   201,   249,   147,   353,   299,   540,   464,   249,   672,
     257,   190,   617,   464,    28,   262,   257,     4,   249,    28,
       7,   262,    65,    28,    28,   204,   257,     7,   388,    56,
       7,   262,    32,    33,    34,     7,    58,    28,    28,    26,
      27,    29,    64,    12,    13,    14,    26,    27,   638,    26,
      27,    30,    26,    27,    26,    27,    21,    26,    27,    29,
      28,    26,    27,   750,    30,   638,    28,    29,    68,    69,
      28,    29,   677,   180,    28,   532,    50,    42,    43,    28,
      84,    28,    28,    29,     0,    50,    28,    29,    28,    29,
     269,    28,   626,   756,    25,    28,   638,    26,    27,    84,
     638,   641,   145,   638,   108,   135,   563,   286,    84,    29,
       4,    28,   563,     7,   361,   134,    81,   134,   297,   659,
     361,    50,    87,   370,    28,   797,   145,   141,    84,   370,
     361,   378,    26,    27,   724,   144,   141,   378,   112,   370,
     144,   828,   146,   136,   504,    84,   134,   378,   139,   139,
     135,   724,   139,   610,    67,   134,   450,   150,   830,   139,
     124,   108,   139,   270,   134,   139,   134,   139,   134,   133,
     139,   193,   134,   149,   139,   202,   134,    28,   134,   201,
     134,   638,   724,   112,    29,   134,   724,   638,   134,   724,
     431,   108,   134,   149,   134,   134,   226,   134,   342,    70,
     431,   134,     7,   382,   383,   877,   136,   870,    28,    29,
      30,   865,   866,   137,    21,   145,   137,   875,   876,    26,
      27,    26,    27,   464,    28,    29,   150,   832,   149,   523,
     524,   136,    28,   464,   134,    42,    43,   137,   892,   893,
     898,   895,   285,    50,   119,   150,   149,   852,   120,   124,
     141,   142,   143,   128,   126,   860,    96,   148,   607,   500,
      24,   405,   406,   407,   408,   409,   124,   724,   412,   500,
     124,   123,   416,   724,    81,   145,    96,   129,   130,   131,
      87,    12,    13,    14,    29,   887,   135,   136,   890,   124,
     125,   532,   141,   142,   143,    26,    27,   831,    25,   148,
      27,   532,    26,    27,    28,   546,   121,   122,   144,   124,
     146,    65,    66,    67,   138,   546,    70,   132,   133,   590,
     591,   592,   563,   135,   136,   135,   136,   124,   358,   135,
     136,   124,   563,   135,   136,   135,   136,    96,   381,   518,
     519,   520,   521,   522,   585,   124,   525,   135,   136,   528,
     135,   136,   352,    67,   585,   142,   143,   144,   145,   146,
     147,   135,   136,   135,   136,   365,   388,   135,   136,   610,
     135,   136,   138,   373,   135,   136,   388,   135,   136,   610,
     105,   106,   388,   135,   136,   135,   136,   135,   136,   135,
     136,   638,   135,   136,   696,   697,   138,   638,   420,   776,
     777,    84,   138,   138,   124,   134,   124,   638,   124,    55,
      28,   137,   434,    28,   137,   137,   137,    85,   137,   151,
     137,    28,   422,   135,    28,   425,   137,   142,   137,   142,
     137,   638,   137,   137,   137,   137,   137,   137,   137,    28,
     138,   137,   137,   137,   137,   645,   627,   137,   137,   137,
     135,   138,    96,   137,    30,   137,   137,   137,   137,   137,
     137,   461,   153,   642,   758,   759,   137,   137,   134,    28,
      28,    96,   137,    29,   138,    29,    96,   724,    30,    30,
     134,   138,   504,   724,   140,   140,    28,    30,   135,    30,
      30,    30,   504,   724,   134,   134,   137,   140,   504,   136,
     138,   135,   137,    28,   137,   649,   650,   136,    28,   140,
     127,   541,   542,   543,   140,    86,   140,   724,    12,   135,
     139,   135,   135,   151,   134,   137,   109,   138,   137,   559,
     560,   561,   135,   135,    13,   578,    56,    28,    30,   140,
     562,   140,   140,   140,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
     140,   134,    82,    83,   607,   744,   745,   137,   134,   748,
      14,   140,   819,   752,   142,    95,   757,   871,   819,   609,
     607,   134,    62,    96,   134,   148,   151,   134,   819,   151,
     136,   148,    28,   623,   841,   115,   151,   891,    28,   135,
     841,   138,   151,   135,   152,   140,   137,   137,   137,   788,
     841,   134,   140,   140,    28,   134,   638,    28,   134,    28,
     799,   137,    41,   645,   135,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,     7,   152,   140,   154,   155,
     140,   134,    31,   152,   674,   140,   135,   134,   153,   135,
     140,   153,   140,     1,   684,   685,   686,   687,   688,   689,
     690,   691,   692,   693,   694,   326,   329,   610,   563,   424,
     323,   539,     3,   370,   853,   367,   404,   346,   427,   636,
     265,   356,   333,   391,   394,   200,   585,   349,   867,    26,
      27,    28,   398,   578,   361,   724,   580,   838,   841,   375,
     780,   880,   724,   746,   378,   462,   527,   779,   546,    -1,
      -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,    55,    56,
      -1,    -1,    -1,   268,   754,    -1,    -1,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    -1,    81,    82,    83,    -1,    -1,    86,
      -1,    -1,    -1,    -1,   776,   777,   778,   779,    95,    -1,
      -1,    98,    99,    -1,    -1,    -1,   778,    -1,    -1,    -1,
      -1,   108,    -1,    -1,   111,    -1,    -1,   114,   115,    -1,
      -1,   824,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,     7,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    21,    -1,    -1,   847,    -1,    26,    27,    28,
      -1,    -1,   862,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    -1,   873,    43,    44,    -1,    -1,    -1,    -1,
      -1,    50,    -1,    -1,    -1,    -1,    55,    56,    57,    58,
      59,    60,    61,    -1,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    -1,    -1,    86,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,    -1,    98,
      99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,
      -1,    -1,   111,    -1,   113,    -1,   115,    -1,     3,   118,
       5,     6,    -1,     8,     9,    10,    11,    -1,    -1,    -1,
      15,    16,    17,    18,    19,    20,    -1,    22,    23,    24,
     139,    26,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    -1,    81,    82,    83,    -1,
      -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    -1,    -1,    98,    99,   100,   101,   102,   103,     7,
      -1,    -1,    -1,   108,    -1,    -1,   111,    -1,    -1,   114,
     115,   116,   117,    21,    -1,    -1,    -1,    -1,    26,    27,
      28,    -1,    -1,    -1,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    44,    -1,    -1,    -1,
      -1,    -1,    50,    -1,    -1,    -1,    -1,    55,    56,    57,
      58,    59,    60,    61,    -1,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    -1,    -1,    86,    87,
      -1,    -1,    26,    27,    28,    -1,    -1,    95,    -1,    -1,
      98,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     108,    -1,    -1,   111,    -1,   113,    50,   115,    -1,    -1,
     118,    55,    56,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    -1,    81,    82,    83,
      28,    -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    -1,    -1,    98,    99,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   108,    -1,    -1,   111,    56,    -1,
     114,   115,    -1,    -1,    -1,    -1,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    -1,    -1,    82,    83,    -1,    28,    86,    30,
      88,    89,    90,    91,    92,    93,    94,    95,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   110,    -1,    -1,    56,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    28,
      -1,    82,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    95,    96,    97,    -1,    -1,    -1,
      -1,    -1,    -1,   104,   105,   106,   107,    56,    -1,    -1,
      -1,    -1,    -1,    -1,   115,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    28,    82,    83,    -1,    -1,    86,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,
      56,    -1,    -1,    -1,    -1,    -1,   115,    -1,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    28,    82,    83,    -1,    -1,
      86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   108,    56,    -1,    -1,    -1,    -1,    -1,   115,
      -1,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    -1,    28,    82,
      83,    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   108,    56,    -1,    -1,    -1,
      -1,    -1,   115,    -1,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      -1,    -1,    82,    83,    -1,    -1,    86,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   115
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   157,   158,   159,     0,   158,     3,     5,     6,     8,
       9,    10,    11,    15,    16,    17,    18,    19,    20,    22,
      23,    24,    26,    27,    28,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    81,    82,    83,    86,    95,    98,    99,
     100,   101,   102,   103,   108,   111,   114,   115,   116,   117,
     160,   161,   162,   166,   170,   174,   180,   182,   188,   192,
     196,   200,   204,   207,   208,   212,   216,   220,   227,   234,
     243,   247,   248,   255,   256,   257,   258,   268,   270,   271,
     272,   273,   274,   275,   276,   277,   282,   286,   289,   303,
     304,   308,   309,   312,   314,   315,   341,   364,   368,   372,
      28,   134,   171,    30,   134,   163,    28,   134,   167,    25,
     287,   288,   287,    28,   134,   283,    29,   287,   287,   287,
     287,   287,   287,   287,   287,    56,   372,   134,    29,   134,
     244,    29,    29,   134,   249,   287,   287,   287,    28,    29,
     134,   235,   241,   241,   134,   221,   241,   134,   228,   241,
     303,   303,    67,    28,   292,    70,    65,    66,    67,    70,
     372,   368,    28,   201,   193,    28,   134,   197,    30,   134,
     209,   368,   303,   149,   134,   213,   241,   134,   217,   241,
      84,   149,    24,   313,    28,   108,   371,    96,   124,   133,
     172,   173,   124,   164,   165,   124,   168,   169,    25,    27,
     120,   126,   284,   285,   287,    29,   291,   303,   145,   345,
      28,   141,   205,   206,   292,   124,   125,   245,   246,   124,
     250,   251,   121,   122,   124,   132,   133,   236,   237,   138,
     238,    96,   242,   242,   124,   222,   223,   138,   224,   124,
     229,   230,   138,   231,   316,   310,   345,    67,   371,    28,
     134,   189,   138,   138,   124,   198,   199,   123,   129,   130,
     131,   210,   211,   345,   134,   134,   175,   368,   373,   124,
     214,   215,   124,   218,   219,   304,   373,   372,   315,   134,
     345,   134,   137,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   152,   154,   155,   342,   144,   146,   370,
     137,   137,   135,   136,   137,   135,   136,   137,   135,   136,
     137,   137,   135,   136,   287,    28,   346,   347,   181,    28,
      85,   135,   142,   137,   137,   135,   136,   137,   135,   136,
     137,   137,   137,   137,   137,   135,   136,     4,     7,   204,
     207,   239,   240,   278,   349,   137,   135,   136,   204,   207,
     225,   226,   349,   137,   135,   136,   204,   207,   232,   233,
     349,   151,   318,   318,   290,   370,   345,   371,   138,   190,
      28,   202,   203,    28,   194,   195,   137,   135,   136,   137,
     137,   137,   137,   135,   136,    88,    89,    90,    91,    92,
      93,    94,   110,   353,   354,   355,   368,   369,   303,   345,
     136,   150,   137,   135,   136,   137,   135,   136,   150,   345,
     353,   138,   365,   135,   134,   137,   137,   137,   137,   137,
     137,   137,   137,   137,   149,   137,   150,   153,   137,   137,
     134,    28,    96,   173,    30,   165,    28,   169,    28,    96,
     285,   137,   136,   145,   138,   184,    28,   141,   292,    29,
     105,   106,   305,   246,    29,   251,    30,    30,   241,   305,
      96,   237,   134,   279,   287,   139,   240,   241,   223,   139,
     226,   241,   230,   139,   233,   303,   319,   320,   345,   345,
     138,   140,    28,   161,   191,   314,   140,   139,   203,   139,
     195,    28,   199,    30,    30,    30,    30,   211,   292,   292,
     292,   292,   292,   134,   134,   292,   135,   136,   292,   137,
     298,   135,   138,   176,   368,   241,   215,   241,   219,   183,
     135,    12,    13,    14,   204,   207,   366,   367,   140,   368,
     137,   137,   353,    28,    30,    96,   241,   348,   347,    21,
      42,    43,    87,   185,   186,   187,   204,   207,   258,   269,
     289,    28,   127,   280,   281,   140,   140,   140,   136,   317,
     311,    28,   204,   207,   293,   294,   295,   135,   139,   161,
     345,   345,   345,   345,   345,   353,   353,   345,    86,   343,
     355,   345,   141,   142,   143,   148,   299,   301,   302,   112,
     177,   178,   179,   204,   207,   258,   184,   343,   287,   287,
     287,   139,   367,    12,   252,   135,   135,   151,   287,   287,
     287,   368,   139,   186,   137,   135,   136,   320,   138,   321,
     321,   137,   297,   139,   295,   134,   298,   298,   298,   135,
     135,   109,   374,   142,   143,   144,   145,   146,   147,   300,
      30,    96,    97,   104,   107,   303,   305,   306,   372,   287,
     139,   178,   374,   287,    13,   253,   140,   343,   242,    28,
     140,    30,   281,    28,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    43,    44,    57,    58,    59,    60,    61,
      63,    80,    87,   113,   118,   174,   188,   204,   207,   258,
     259,   260,   261,   262,   263,   264,   265,   266,   267,   269,
     289,   308,   309,   315,   322,   323,   324,   332,   333,   335,
     340,   341,   349,   356,   359,   361,   362,   364,   140,   140,
     302,   345,   140,   373,   292,   292,   134,   137,   344,   302,
     134,   140,   344,   287,    14,   254,   374,   142,   134,   134,
     287,   287,   287,   287,   287,   287,   287,   287,   287,   287,
     287,    62,   331,   331,   331,   151,   357,   358,   363,   360,
     334,   134,   325,   139,   323,   148,   136,   296,   135,   345,
     345,   303,   375,    96,   345,   299,   307,   345,   287,   344,
     242,   353,   353,   151,   151,   151,    80,   359,   359,   341,
     368,   361,    28,   335,   119,   124,   128,   326,   327,   138,
     328,    28,   345,   135,   136,   152,   338,   135,   136,   338,
     345,   135,   135,    28,   137,   137,   137,   135,   136,   204,
     207,   329,   330,   349,   134,   140,   303,   339,   140,   299,
     140,   338,   343,   374,    28,    28,    28,   327,   139,   330,
     135,   368,    41,   351,   349,   350,   140,   374,   345,   140,
     374,   134,   287,    31,   352,   351,   351,   345,   152,   336,
     344,   353,   287,   352,   352,   338,   337,   140,   345,   135,
     140,   134,   350,   140,   153,   350,   353,   351,   351,   351,
     135,   352,   153
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 4:

/* Line 1455 of yacc.c  */
#line 484 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            /*
             * We don't do these in parserEOF() because the parser is reading
             * ahead and that would be too early.
             */

            if (previousFile != NULL)
            {
                handleEOF();

                if (currentContext.prevmod != NULL)
                    handleEOM();

                free(previousFile);
                previousFile = NULL;
            }
    }
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 548 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
            {
                classDef *scope = currentScope();

                if (scope == NULL)
                    yyerror("%TypeHeaderCode can only be used in a namespace, class or mapped type");

                appendCodeBlock(&scope->iff->hdrcode, (yyvsp[(1) - (1)].codeb));
            }
        }
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 561 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
            {
                if ((currentModule->encoding = convertEncoding((yyvsp[(2) - (2)].defencoding).name)) == no_type)
                    yyerror("The %DefaultEncoding name must be one of \"ASCII\", \"Latin-1\", \"UTF-8\" or \"None\"");
            }
        }
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 570 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            resetLexerState();

            (yyval.defencoding).name = (yyvsp[(1) - (1)].text);
        }
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 575 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.defencoding) = (yyvsp[(2) - (3)].defencoding);
        }
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 581 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.defencoding) = (yyvsp[(1) - (3)].defencoding);

            switch ((yyvsp[(3) - (3)].defencoding).token)
            {
            case TK_NAME: (yyval.defencoding).name = (yyvsp[(3) - (3)].defencoding).name; break;
            }
        }
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 591 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.defencoding).token = TK_NAME;

            (yyval.defencoding).name = (yyvsp[(3) - (3)].text);
        }
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 598 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            /* Note that %Plugin is internal in SIP v4. */

            if (notSkipping())
                appendString(&currentSpec->plugins, (yyvsp[(2) - (2)].plugin).name);
        }
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 606 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            resetLexerState();

            (yyval.plugin).name = (yyvsp[(1) - (1)].text);
        }
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 611 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.plugin) = (yyvsp[(2) - (3)].plugin);
        }
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 617 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.plugin) = (yyvsp[(1) - (3)].plugin);

            switch ((yyvsp[(3) - (3)].plugin).token)
            {
            case TK_NAME: (yyval.plugin).name = (yyvsp[(3) - (3)].plugin).name; break;
            }
        }
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 627 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.plugin).token = TK_NAME;

            (yyval.plugin).name = (yyvsp[(3) - (3)].text);
        }
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 634 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
            {
                apiVersionRangeDef *avd;

                if (findAPI(currentSpec, (yyvsp[(2) - (2)].api).name) != NULL)
                    yyerror("The API name in the %API directive has already been defined");

                if ((yyvsp[(2) - (2)].api).version < 1)
                    yyerror("The version number in the %API directive must be greater than or equal to 1");

                avd = sipMalloc(sizeof (apiVersionRangeDef));

                avd->api_name = cacheName(currentSpec, (yyvsp[(2) - (2)].api).name);
                avd->from = (yyvsp[(2) - (2)].api).version;
                avd->to = -1;

                avd->next = currentModule->api_versions;
                currentModule->api_versions = avd;

                if (inMainModule())
                    setIsUsedName(avd->api_name);
            }
        }
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 660 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            resetLexerState();

            deprecated("%API name and version number should be specified using the 'name' and 'version' arguments");

            (yyval.api).name = (yyvsp[(1) - (2)].text);
            (yyval.api).version = (yyvsp[(2) - (2)].number);
        }
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 668 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.api) = (yyvsp[(2) - (3)].api);
        }
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 674 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.api) = (yyvsp[(1) - (3)].api);

            switch ((yyvsp[(3) - (3)].api).token)
            {
            case TK_NAME: (yyval.api).name = (yyvsp[(3) - (3)].api).name; break;
            case TK_VERSION: (yyval.api).version = (yyvsp[(3) - (3)].api).version; break;
            }
        }
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 685 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.api).token = TK_NAME;

            (yyval.api).name = (yyvsp[(3) - (3)].text);
            (yyval.api).version = 0;
        }
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 691 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.api).token = TK_VERSION;

            (yyval.api).name = NULL;
            (yyval.api).version = (yyvsp[(3) - (3)].number);
        }
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 699 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
            {
                static const char *annos[] = {
                    "Default",
                    "PyName",
                    NULL
                };

                exceptionDef *xd;
                const char *pyname;

                checkAnnos(&(yyvsp[(4) - (5)].optflags), annos);

                if (currentSpec->genc)
                    yyerror("%Exception not allowed in a C module");

                if ((yyvsp[(5) - (5)].exception).raise_code == NULL)
                    yyerror("%Exception must have a %RaiseCode sub-directive");

                pyname = getPythonName(currentModule, &(yyvsp[(4) - (5)].optflags), scopedNameTail((yyvsp[(2) - (5)].scpvalp)));

                checkAttributes(currentSpec, currentModule, NULL, NULL,
                        pyname, FALSE);

                xd = findException(currentSpec, (yyvsp[(2) - (5)].scpvalp), TRUE);

                if (xd->cd != NULL)
                    yyerror("%Exception name has already been seen as a class name - it must be defined before being used");

                if (xd->iff->module != NULL)
                    yyerror("The %Exception has already been defined");

                /* Complete the definition. */
                xd->iff->module = currentModule;
                xd->iff->hdrcode = (yyvsp[(5) - (5)].exception).type_header_code;
                xd->pyname = pyname;
                xd->bibase = (yyvsp[(3) - (5)].exceptionbase).bibase;
                xd->base = (yyvsp[(3) - (5)].exceptionbase).base;
                xd->raisecode = (yyvsp[(5) - (5)].exception).raise_code;

                if (getOptFlag(&(yyvsp[(4) - (5)].optflags), "Default", bool_flag) != NULL)
                    currentModule->defexception = xd;

                if (xd->bibase != NULL || xd->base != NULL)
                    xd->exceptionnr = currentModule->nrexceptions++;
            }
        }
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 749 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.exceptionbase).bibase = NULL;
            (yyval.exceptionbase).base = NULL;
        }
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 753 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            exceptionDef *xd;

            (yyval.exceptionbase).bibase = NULL;
            (yyval.exceptionbase).base = NULL;

            /* See if it is a defined exception. */
            for (xd = currentSpec->exceptions; xd != NULL; xd = xd->next)
                if (compareScopedNames(xd->iff->fqcname, (yyvsp[(2) - (3)].scpvalp)) == 0)
                {
                    (yyval.exceptionbase).base = xd;
                    break;
                }

            if (xd == NULL && (yyvsp[(2) - (3)].scpvalp)->next == NULL && strncmp((yyvsp[(2) - (3)].scpvalp)->name, "SIP_", 4) == 0)
            {
                /* See if it is a builtin exception. */

                static char *builtins[] = {
                    "Exception",
                    "StopIteration",
                    "StandardError",
                    "ArithmeticError",
                    "LookupError",
                    "AssertionError",
                    "AttributeError",
                    "EOFError",
                    "FloatingPointError",
                    "EnvironmentError",
                    "IOError",
                    "OSError",
                    "ImportError",
                    "IndexError",
                    "KeyError",
                    "KeyboardInterrupt",
                    "MemoryError",
                    "NameError",
                    "OverflowError",
                    "RuntimeError",
                    "NotImplementedError",
                    "SyntaxError",
                    "IndentationError",
                    "TabError",
                    "ReferenceError",
                    "SystemError",
                    "SystemExit",
                    "TypeError",
                    "UnboundLocalError",
                    "UnicodeError",
                    "UnicodeEncodeError",
                    "UnicodeDecodeError",
                    "UnicodeTranslateError",
                    "ValueError",
                    "ZeroDivisionError",
                    "WindowsError",
                    "VMSError",
                    NULL
                };

                char **cp;

                for (cp = builtins; *cp != NULL; ++cp)
                    if (strcmp((yyvsp[(2) - (3)].scpvalp)->name + 4, *cp) == 0)
                    {
                        (yyval.exceptionbase).bibase = *cp;
                        break;
                    }
            }

            if ((yyval.exceptionbase).bibase == NULL && (yyval.exceptionbase).base == NULL)
                yyerror("Unknown exception base type");
        }
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 827 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.exception) = (yyvsp[(2) - (4)].exception);
        }
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 833 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.exception) = (yyvsp[(1) - (2)].exception);

            switch ((yyvsp[(2) - (2)].exception).token)
            {
            case TK_RAISECODE: (yyval.exception).raise_code = (yyvsp[(2) - (2)].exception).raise_code; break;
            case TK_TYPEHEADERCODE: (yyval.exception).type_header_code = (yyvsp[(2) - (2)].exception).type_header_code; break;
            }
        }
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 844 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.exception).token = TK_IF;
        }
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 847 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.exception).token = TK_END;
        }
    break;

  case 77:

/* Line 1455 of yacc.c  */
#line 850 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
            {
                (yyval.exception).token = TK_RAISECODE;
                (yyval.exception).raise_code = (yyvsp[(1) - (1)].codeb);
            }
            else
            {
                (yyval.exception).token = 0;
                (yyval.exception).raise_code = NULL;
            }
        }
    break;

  case 78:

/* Line 1455 of yacc.c  */
#line 862 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
            {
                (yyval.exception).token = TK_TYPEHEADERCODE;
                (yyval.exception).type_header_code = (yyvsp[(1) - (1)].codeb);
            }
            else
            {
                (yyval.exception).token = 0;
                (yyval.exception).type_header_code = NULL;
            }
        }
    break;

  case 79:

/* Line 1455 of yacc.c  */
#line 876 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.codeb) = (yyvsp[(2) - (2)].codeb);
        }
    break;

  case 80:

/* Line 1455 of yacc.c  */
#line 881 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
            {
                static const char *annos[] = {
                    "AllowNone",
                    "API",
                    "DocType",
                    "NoRelease",
                    "PyName",
                    NULL
                };

                checkAnnos(&(yyvsp[(3) - (3)].optflags), annos);

                currentMappedType = newMappedType(currentSpec, &(yyvsp[(2) - (3)].memArg), &(yyvsp[(3) - (3)].optflags));
            }
        }
    break;

  case 82:

/* Line 1455 of yacc.c  */
#line 900 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
            {
                static const char *annos[] = {
                    "DocType",
                    NULL
                };

                int a;
                mappedTypeTmplDef *mtt;
                ifaceFileDef *iff;

                checkAnnos(&(yyvsp[(4) - (4)].optflags), annos);

                if (currentSpec->genc)
                    yyerror("%MappedType templates not allowed in a C module");

                /*
                 * Check the template arguments are basic types or simple
                 * names.
                 */
                for (a = 0; a < (yyvsp[(1) - (4)].signature).nrArgs; ++a)
                {
                    argDef *ad = &(yyvsp[(1) - (4)].signature).args[a];

                    if (ad->atype == defined_type && ad->u.snd->next != NULL)
                        yyerror("%MappedType template arguments must be simple names");
                }

                if ((yyvsp[(3) - (4)].memArg).atype != template_type)
                    yyerror("%MappedType template must map a template type");

                /* Check a template hasn't already been provided. */
                for (mtt = currentSpec->mappedtypetemplates; mtt != NULL; mtt = mtt->next)
                    if (compareScopedNames(mtt->mt->type.u.td->fqname, (yyvsp[(3) - (4)].memArg).u.td->fqname) == 0 && sameTemplateSignature(&mtt->mt->type.u.td->types, &(yyvsp[(3) - (4)].memArg).u.td->types, TRUE))
                        yyerror("%MappedType template for this type has already been defined");

                (yyvsp[(3) - (4)].memArg).nrderefs = 0;
                (yyvsp[(3) - (4)].memArg).argflags = 0;

                mtt = sipMalloc(sizeof (mappedTypeTmplDef));

                mtt->sig = (yyvsp[(1) - (4)].signature);
                mtt->mt = allocMappedType(currentSpec, &(yyvsp[(3) - (4)].memArg));
                mtt->mt->doctype = getDocType(&(yyvsp[(4) - (4)].optflags));
                mtt->next = currentSpec->mappedtypetemplates;

                currentSpec->mappedtypetemplates = mtt;

                currentMappedType = mtt->mt;

                /* Create a dummy interface file. */
                iff = sipMalloc(sizeof (ifaceFileDef));
                iff->hdrcode = NULL;
                mtt->mt->iff = iff;
            }
        }
    break;

  case 84:

/* Line 1455 of yacc.c  */
#line 959 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
            {
                if (currentMappedType->convfromcode == NULL)
                    yyerror("%MappedType must have a %ConvertFromTypeCode directive");

                if (currentMappedType->convtocode == NULL)
                    yyerror("%MappedType must have a %ConvertToTypeCode directive");

                currentMappedType = NULL;
            }
        }
    break;

  case 89:

/* Line 1455 of yacc.c  */
#line 979 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
                appendCodeBlock(&currentMappedType->iff->hdrcode, (yyvsp[(1) - (1)].codeb));
        }
    break;

  case 90:

/* Line 1455 of yacc.c  */
#line 983 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
                appendCodeBlock(&currentMappedType->typecode, (yyvsp[(1) - (1)].codeb));
        }
    break;

  case 91:

/* Line 1455 of yacc.c  */
#line 987 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
            {
                if (currentMappedType -> convfromcode != NULL)
                    yyerror("%MappedType has more than one %ConvertFromTypeCode directive");

                currentMappedType -> convfromcode = (yyvsp[(2) - (2)].codeb);
            }
        }
    break;

  case 92:

/* Line 1455 of yacc.c  */
#line 996 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
            {
                if (currentMappedType -> convtocode != NULL)
                    yyerror("%MappedType has more than one %ConvertToTypeCode directive");

                currentMappedType -> convtocode = (yyvsp[(2) - (2)].codeb);
            }
        }
    break;

  case 95:

/* Line 1455 of yacc.c  */
#line 1009 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
            {
                applyTypeFlags(currentModule, &(yyvsp[(2) - (13)].memArg), &(yyvsp[(9) - (13)].optflags));

                (yyvsp[(5) - (13)].signature).result = (yyvsp[(2) - (13)].memArg);

                newFunction(currentSpec, currentModule, NULL,
                        currentMappedType, 0, TRUE, FALSE, FALSE, FALSE, (yyvsp[(3) - (13)].text),
                        &(yyvsp[(5) - (13)].signature), (yyvsp[(7) - (13)].number), FALSE, &(yyvsp[(9) - (13)].optflags), (yyvsp[(13) - (13)].codeb), NULL, (yyvsp[(8) - (13)].throwlist), (yyvsp[(10) - (13)].optsignature), (yyvsp[(12) - (13)].codeb));
            }
        }
    break;

  case 96:

/* Line 1455 of yacc.c  */
#line 1023 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (currentSpec -> genc)
                yyerror("namespace definition not allowed in a C module");

            if (notSkipping())
            {
                classDef *ns, *c_scope;
                ifaceFileDef *scope;

                if ((c_scope = currentScope()) != NULL)
                    scope = c_scope->iff;
                else
                    scope = NULL;

                ns = newClass(currentSpec, namespace_iface, NULL,
                        text2scopedName(scope, (yyvsp[(2) - (2)].text)));

                pushScope(ns);

                sectionFlags = 0;
            }
        }
    break;

  case 97:

/* Line 1455 of yacc.c  */
#line 1044 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
            {
                if (inMainModule())
                {
                    classDef *ns = currentScope();

                    setIsUsedName(ns->iff->name);
                    setIsUsedName(ns->pyname);
                }

                popScope();
            }
        }
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 1068 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
            {
                qualDef *qd;

                for (qd = currentModule->qualifiers; qd != NULL; qd = qd->next)
                    if (qd->qtype == platform_qualifier)
                        yyerror("%Platforms has already been defined for this module");
            }
        }
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 1078 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
            {
                qualDef *qd;
                int nrneeded;

                /* Check that exactly one platform in the set was requested. */
                nrneeded = 0;

                for (qd = currentModule->qualifiers; qd != NULL; qd = qd->next)
                    if (qd->qtype == platform_qualifier && isNeeded(qd))
                        ++nrneeded;

                if (nrneeded > 1)
                    yyerror("No more than one of these %Platforms must be specified with the -t flag");
            }
        }
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 1101 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            newQualifier(currentModule,-1,-1,(yyvsp[(1) - (1)].text),platform_qualifier);
        }
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 1106 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
                newQualifier(currentModule, -1, -1, (yyvsp[(2) - (2)].feature).name,
                        feature_qualifier);
        }
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 1113 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            resetLexerState();

            (yyval.feature).name = (yyvsp[(1) - (1)].text);
        }
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 1118 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.feature) = (yyvsp[(2) - (3)].feature);
        }
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 1124 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.feature) = (yyvsp[(1) - (3)].feature);

            switch ((yyvsp[(3) - (3)].feature).token)
            {
            case TK_NAME: (yyval.feature).name = (yyvsp[(3) - (3)].feature).name; break;
            }
        }
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 1134 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.feature).token = TK_NAME;

            (yyval.feature).name = (yyvsp[(3) - (3)].text);
        }
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 1141 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            currentTimelineOrder = 0;
        }
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 1144 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
            {
                qualDef *qd;
                int nrneeded;

                /*
                 * Check that exactly one time slot in the set was requested.
                 */
                nrneeded = 0;

                for (qd = currentModule->qualifiers; qd != NULL; qd = qd->next)
                    if (qd->qtype == time_qualifier && isNeeded(qd))
                        ++nrneeded;

                if (nrneeded > 1)
                    yyerror("At most one of this %Timeline must be specified with the -t flag");

                currentModule->nrtimelines++;
            }
        }
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 1171 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            newQualifier(currentModule, currentModule->nrtimelines,
                    currentTimelineOrder++, (yyvsp[(1) - (1)].text), time_qualifier);
        }
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 1177 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (skipStackPtr >= MAX_NESTED_IF)
                yyerror("Internal error: increase the value of MAX_NESTED_IF");

            /* Nested %Ifs are implicit logical ands. */

            if (skipStackPtr > 0)
                (yyvsp[(3) - (4)].boolean) = ((yyvsp[(3) - (4)].boolean) && skipStack[skipStackPtr - 1]);

            skipStack[skipStackPtr++] = (yyvsp[(3) - (4)].boolean);
        }
    break;

  case 119:

/* Line 1455 of yacc.c  */
#line 1190 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.boolean) = platOrFeature((yyvsp[(1) - (1)].text),FALSE);
        }
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 1193 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.boolean) = platOrFeature((yyvsp[(2) - (2)].text),TRUE);
        }
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 1196 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.boolean) = (platOrFeature((yyvsp[(3) - (3)].text),FALSE) || (yyvsp[(1) - (3)].boolean));
        }
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 1199 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.boolean) = (platOrFeature((yyvsp[(4) - (4)].text),TRUE) || (yyvsp[(1) - (4)].boolean));
        }
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 1205 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.boolean) = timePeriod((yyvsp[(1) - (3)].text), (yyvsp[(3) - (3)].text));
        }
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 1210 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (skipStackPtr-- <= 0)
                yyerror("Too many %End directives");
        }
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 1216 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            optFlag *of;

            if ((yyvsp[(3) - (3)].optflags).nrFlags != 0)
                deprecated("%License annotations are deprecated, use arguments instead");

            if ((yyvsp[(2) - (3)].license).type == NULL)
                if ((of = getOptFlag(&(yyvsp[(3) - (3)].optflags), "Type", string_flag)) != NULL)
                    (yyvsp[(2) - (3)].license).type = of->fvalue.sval;

            if ((yyvsp[(2) - (3)].license).licensee == NULL)
                if ((of = getOptFlag(&(yyvsp[(3) - (3)].optflags), "Licensee", string_flag)) != NULL)
                    (yyvsp[(2) - (3)].license).licensee = of->fvalue.sval;

            if ((yyvsp[(2) - (3)].license).signature == NULL)
                if ((of = getOptFlag(&(yyvsp[(3) - (3)].optflags), "Signature", string_flag)) != NULL)
                    (yyvsp[(2) - (3)].license).signature = of->fvalue.sval;

            if ((yyvsp[(2) - (3)].license).timestamp == NULL)
                if ((of = getOptFlag(&(yyvsp[(3) - (3)].optflags), "Timestamp", string_flag)) != NULL)
                    (yyvsp[(2) - (3)].license).timestamp = of->fvalue.sval;

            if ((yyvsp[(2) - (3)].license).type == NULL)
                yyerror("%License must have a 'type' argument");

            if (notSkipping())
            {
                currentModule->license = sipMalloc(sizeof (licenseDef));

                currentModule->license->type = (yyvsp[(2) - (3)].license).type;
                currentModule->license->licensee = (yyvsp[(2) - (3)].license).licensee;
                currentModule->license->sig = (yyvsp[(2) - (3)].license).signature;
                currentModule->license->timestamp = (yyvsp[(2) - (3)].license).timestamp;
            }
        }
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 1253 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            resetLexerState();

            (yyval.license).type = NULL;
            (yyval.license).licensee = NULL;
            (yyval.license).signature = NULL;
            (yyval.license).timestamp = NULL;
        }
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 1261 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.license).type = (yyvsp[(1) - (1)].text);
            (yyval.license).licensee = NULL;
            (yyval.license).signature = NULL;
            (yyval.license).timestamp = NULL;
        }
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 1267 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.license) = (yyvsp[(2) - (3)].license);
        }
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 1273 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.license) = (yyvsp[(1) - (3)].license);

            switch ((yyvsp[(3) - (3)].license).token)
            {
            case TK_TYPE: (yyval.license).type = (yyvsp[(3) - (3)].license).type; break;
            case TK_LICENSEE: (yyval.license).licensee = (yyvsp[(3) - (3)].license).licensee; break;
            case TK_SIGNATURE: (yyval.license).signature = (yyvsp[(3) - (3)].license).signature; break;
            case TK_TIMESTAMP: (yyval.license).timestamp = (yyvsp[(3) - (3)].license).timestamp; break;
            }
        }
    break;

  case 132:

/* Line 1455 of yacc.c  */
#line 1286 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.license).token = TK_NAME;

            (yyval.license).type = (yyvsp[(3) - (3)].text);
            (yyval.license).licensee = NULL;
            (yyval.license).signature = NULL;
            (yyval.license).timestamp = NULL;
        }
    break;

  case 133:

/* Line 1455 of yacc.c  */
#line 1294 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.license).token = TK_LICENSEE;

            (yyval.license).type = NULL;
            (yyval.license).licensee = (yyvsp[(3) - (3)].text);
            (yyval.license).signature = NULL;
            (yyval.license).timestamp = NULL;
        }
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 1302 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.license).token = TK_SIGNATURE;

            (yyval.license).type = NULL;
            (yyval.license).licensee = NULL;
            (yyval.license).signature = (yyvsp[(3) - (3)].text);
            (yyval.license).timestamp = NULL;
        }
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 1310 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.license).token = TK_TIMESTAMP;

            (yyval.license).type = NULL;
            (yyval.license).licensee = NULL;
            (yyval.license).signature = NULL;
            (yyval.license).timestamp = (yyvsp[(3) - (3)].text);
        }
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 1320 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
            {
                if (currentModule->defmetatype != NULL)
                    yyerror("%DefaultMetatype has already been defined for this module");

                currentModule->defmetatype = cacheName(currentSpec, (yyvsp[(2) - (2)].defmetatype).name);
            }
        }
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 1331 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            resetLexerState();

            (yyval.defmetatype).name = (yyvsp[(1) - (1)].text);
        }
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 1336 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.defmetatype) = (yyvsp[(2) - (3)].defmetatype);
        }
    break;

  case 140:

/* Line 1455 of yacc.c  */
#line 1342 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.defmetatype) = (yyvsp[(1) - (3)].defmetatype);

            switch ((yyvsp[(3) - (3)].defmetatype).token)
            {
            case TK_NAME: (yyval.defmetatype).name = (yyvsp[(3) - (3)].defmetatype).name; break;
            }
        }
    break;

  case 141:

/* Line 1455 of yacc.c  */
#line 1352 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.defmetatype).token = TK_NAME;

            (yyval.defmetatype).name = (yyvsp[(3) - (3)].text);
        }
    break;

  case 142:

/* Line 1455 of yacc.c  */
#line 1359 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
            {
                if (currentModule->defsupertype != NULL)
                    yyerror("%DefaultSupertype has already been defined for this module");

                currentModule->defsupertype = cacheName(currentSpec, (yyvsp[(2) - (2)].defsupertype).name);
            }
        }
    break;

  case 143:

/* Line 1455 of yacc.c  */
#line 1370 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            resetLexerState();

            (yyval.defsupertype).name = (yyvsp[(1) - (1)].text);
        }
    break;

  case 144:

/* Line 1455 of yacc.c  */
#line 1375 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.defsupertype) = (yyvsp[(2) - (3)].defsupertype);
        }
    break;

  case 146:

/* Line 1455 of yacc.c  */
#line 1381 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.defsupertype) = (yyvsp[(1) - (3)].defsupertype);

            switch ((yyvsp[(3) - (3)].defsupertype).token)
            {
            case TK_NAME: (yyval.defsupertype).name = (yyvsp[(3) - (3)].defsupertype).name; break;
            }
        }
    break;

  case 147:

/* Line 1455 of yacc.c  */
#line 1391 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.defsupertype).token = TK_NAME;

            (yyval.defsupertype).name = (yyvsp[(3) - (3)].text);
        }
    break;

  case 148:

/* Line 1455 of yacc.c  */
#line 1398 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
            {
                /* Make sure this is the first mention of a module. */
                if (currentSpec->module != currentModule)
                    yyerror("A %ConsolidatedModule cannot be %Imported");

                if (currentModule->fullname != NULL)
                    yyerror("%ConsolidatedModule must appear before any %Module or %CModule directive");

                setModuleName(currentSpec, currentModule, (yyvsp[(2) - (3)].consmodule).name);
                currentModule->docstring = (yyvsp[(3) - (3)].consmodule).docstring;

                setIsConsolidated(currentModule);
            }
        }
    break;

  case 149:

/* Line 1455 of yacc.c  */
#line 1416 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            resetLexerState();

            (yyval.consmodule).name = (yyvsp[(1) - (1)].text);
        }
    break;

  case 150:

/* Line 1455 of yacc.c  */
#line 1421 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.consmodule) = (yyvsp[(2) - (3)].consmodule);
        }
    break;

  case 152:

/* Line 1455 of yacc.c  */
#line 1427 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.consmodule) = (yyvsp[(1) - (3)].consmodule);

            switch ((yyvsp[(3) - (3)].consmodule).token)
            {
            case TK_NAME: (yyval.consmodule).name = (yyvsp[(3) - (3)].consmodule).name; break;
            }
        }
    break;

  case 153:

/* Line 1455 of yacc.c  */
#line 1437 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.consmodule).token = TK_NAME;

            (yyval.consmodule).name = (yyvsp[(3) - (3)].text);
        }
    break;

  case 154:

/* Line 1455 of yacc.c  */
#line 1444 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.consmodule).token = 0;
            (yyval.consmodule).docstring = NULL;
        }
    break;

  case 155:

/* Line 1455 of yacc.c  */
#line 1448 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.consmodule) = (yyvsp[(2) - (4)].consmodule);
        }
    break;

  case 157:

/* Line 1455 of yacc.c  */
#line 1454 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.consmodule) = (yyvsp[(1) - (2)].consmodule);

            switch ((yyvsp[(2) - (2)].consmodule).token)
            {
            case TK_DOCSTRING: (yyval.consmodule).docstring = (yyvsp[(2) - (2)].consmodule).docstring; break;
            }
        }
    break;

  case 158:

/* Line 1455 of yacc.c  */
#line 1464 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.consmodule).token = TK_IF;
        }
    break;

  case 159:

/* Line 1455 of yacc.c  */
#line 1467 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.consmodule).token = TK_END;
        }
    break;

  case 160:

/* Line 1455 of yacc.c  */
#line 1470 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
            {
                (yyval.consmodule).token = TK_DOCSTRING;
                (yyval.consmodule).docstring = (yyvsp[(1) - (1)].codeb);
            }
            else
            {
                (yyval.consmodule).token = 0;
                (yyval.consmodule).docstring = NULL;
            }
        }
    break;

  case 161:

/* Line 1455 of yacc.c  */
#line 1484 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
            {
                /* Make sure this is the first mention of a module. */
                if (currentSpec->module != currentModule)
                    yyerror("A %CompositeModule cannot be %Imported");

                if (currentModule->fullname != NULL)
                    yyerror("%CompositeModule must appear before any %Module directive");

                setModuleName(currentSpec, currentModule, (yyvsp[(2) - (3)].compmodule).name);
                currentModule->docstring = (yyvsp[(3) - (3)].compmodule).docstring;

                setIsComposite(currentModule);
            }
        }
    break;

  case 162:

/* Line 1455 of yacc.c  */
#line 1502 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            resetLexerState();

            (yyval.compmodule).name = (yyvsp[(1) - (1)].text);
        }
    break;

  case 163:

/* Line 1455 of yacc.c  */
#line 1507 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.compmodule) = (yyvsp[(2) - (3)].compmodule);
        }
    break;

  case 165:

/* Line 1455 of yacc.c  */
#line 1513 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.compmodule) = (yyvsp[(1) - (3)].compmodule);

            switch ((yyvsp[(3) - (3)].compmodule).token)
            {
            case TK_NAME: (yyval.compmodule).name = (yyvsp[(3) - (3)].compmodule).name; break;
            }
        }
    break;

  case 166:

/* Line 1455 of yacc.c  */
#line 1523 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.compmodule).token = TK_NAME;

            (yyval.compmodule).name = (yyvsp[(3) - (3)].text);
        }
    break;

  case 167:

/* Line 1455 of yacc.c  */
#line 1530 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.compmodule).token = 0;
            (yyval.compmodule).docstring = NULL;
        }
    break;

  case 168:

/* Line 1455 of yacc.c  */
#line 1534 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.compmodule) = (yyvsp[(2) - (4)].compmodule);
        }
    break;

  case 170:

/* Line 1455 of yacc.c  */
#line 1540 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.compmodule) = (yyvsp[(1) - (2)].compmodule);

            switch ((yyvsp[(2) - (2)].compmodule).token)
            {
            case TK_DOCSTRING: (yyval.compmodule).docstring = (yyvsp[(2) - (2)].compmodule).docstring; break;
            }
        }
    break;

  case 171:

/* Line 1455 of yacc.c  */
#line 1550 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.compmodule).token = TK_IF;
        }
    break;

  case 172:

/* Line 1455 of yacc.c  */
#line 1553 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.compmodule).token = TK_END;
        }
    break;

  case 173:

/* Line 1455 of yacc.c  */
#line 1556 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
            {
                (yyval.compmodule).token = TK_DOCSTRING;
                (yyval.compmodule).docstring = (yyvsp[(1) - (1)].codeb);
            }
            else
            {
                (yyval.compmodule).token = 0;
                (yyval.compmodule).docstring = NULL;
            }
        }
    break;

  case 174:

/* Line 1455 of yacc.c  */
#line 1570 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if ((yyvsp[(2) - (3)].module).name == NULL)
                yyerror("%Module must have a 'name' argument");

            if (notSkipping())
                currentModule = configureModule(currentSpec, currentModule,
                        currentContext.filename, (yyvsp[(2) - (3)].module).name, (yyvsp[(2) - (3)].module).version,
                        (yyvsp[(2) - (3)].module).c_module, (yyvsp[(2) - (3)].module).kwargs, (yyvsp[(2) - (3)].module).use_arg_names,
                        (yyvsp[(3) - (3)].module).docstring);
        }
    break;

  case 175:

/* Line 1455 of yacc.c  */
#line 1580 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            deprecated("%CModule is deprecated, use %Module and the 'language' argument instead");

            if (notSkipping())
                currentModule = configureModule(currentSpec, currentModule,
                        currentContext.filename, (yyvsp[(2) - (3)].text), (yyvsp[(3) - (3)].number), TRUE, defaultKwArgs,
                        FALSE, NULL);
        }
    break;

  case 176:

/* Line 1455 of yacc.c  */
#line 1590 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            resetLexerState();

            if ((yyvsp[(2) - (2)].number) >= 0)
                deprecated("%Module version number should be specified using the 'version' argument");

            (yyval.module).c_module = FALSE;
            (yyval.module).kwargs = defaultKwArgs;
            (yyval.module).name = (yyvsp[(1) - (2)].text);
            (yyval.module).use_arg_names = FALSE;
            (yyval.module).version = (yyvsp[(2) - (2)].number);
        }
    break;

  case 177:

/* Line 1455 of yacc.c  */
#line 1602 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.module) = (yyvsp[(2) - (3)].module);
        }
    break;

  case 179:

/* Line 1455 of yacc.c  */
#line 1608 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.module) = (yyvsp[(1) - (3)].module);

            switch ((yyvsp[(3) - (3)].module).token)
            {
            case TK_KWARGS: (yyval.module).kwargs = (yyvsp[(3) - (3)].module).kwargs; break;
            case TK_LANGUAGE: (yyval.module).c_module = (yyvsp[(3) - (3)].module).c_module; break;
            case TK_NAME: (yyval.module).name = (yyvsp[(3) - (3)].module).name; break;
            case TK_USEARGNAMES: (yyval.module).use_arg_names = (yyvsp[(3) - (3)].module).use_arg_names; break;
            case TK_VERSION: (yyval.module).version = (yyvsp[(3) - (3)].module).version; break;
            }
        }
    break;

  case 180:

/* Line 1455 of yacc.c  */
#line 1622 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.module).token = TK_KWARGS;

            (yyval.module).c_module = FALSE;
            (yyval.module).kwargs = convertKwArgs((yyvsp[(3) - (3)].text));
            (yyval.module).name = NULL;
            (yyval.module).use_arg_names = FALSE;
            (yyval.module).version = -1;
        }
    break;

  case 181:

/* Line 1455 of yacc.c  */
#line 1631 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.module).token = TK_LANGUAGE;

            if (strcmp((yyvsp[(3) - (3)].text), "C++") == 0)
                (yyval.module).c_module = FALSE;
            else if (strcmp((yyvsp[(3) - (3)].text), "C") == 0)
                (yyval.module).c_module = TRUE;
            else
                yyerror("%Module 'language' argument must be either \"C++\" or \"C\"");

            (yyval.module).kwargs = defaultKwArgs;
            (yyval.module).name = NULL;
            (yyval.module).use_arg_names = FALSE;
            (yyval.module).version = -1;
        }
    break;

  case 182:

/* Line 1455 of yacc.c  */
#line 1646 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.module).token = TK_NAME;

            (yyval.module).c_module = FALSE;
            (yyval.module).kwargs = defaultKwArgs;
            (yyval.module).name = (yyvsp[(3) - (3)].text);
            (yyval.module).use_arg_names = FALSE;
            (yyval.module).version = -1;
        }
    break;

  case 183:

/* Line 1455 of yacc.c  */
#line 1655 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.module).token = TK_USEARGNAMES;

            (yyval.module).c_module = FALSE;
            (yyval.module).kwargs = defaultKwArgs;
            (yyval.module).name = NULL;
            (yyval.module).use_arg_names = (yyvsp[(3) - (3)].boolean);
            (yyval.module).version = -1;
        }
    break;

  case 184:

/* Line 1455 of yacc.c  */
#line 1664 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if ((yyvsp[(3) - (3)].number) < 0)
                yyerror("%Module 'version' argument cannot be negative");

            (yyval.module).token = TK_VERSION;

            (yyval.module).c_module = FALSE;
            (yyval.module).kwargs = defaultKwArgs;
            (yyval.module).name = NULL;
            (yyval.module).use_arg_names = FALSE;
            (yyval.module).version = (yyvsp[(3) - (3)].number);
        }
    break;

  case 185:

/* Line 1455 of yacc.c  */
#line 1678 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.module).token = 0;
            (yyval.module).docstring = NULL;
        }
    break;

  case 186:

/* Line 1455 of yacc.c  */
#line 1682 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.module) = (yyvsp[(2) - (4)].module);
        }
    break;

  case 188:

/* Line 1455 of yacc.c  */
#line 1688 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.module) = (yyvsp[(1) - (2)].module);

            switch ((yyvsp[(2) - (2)].module).token)
            {
            case TK_DOCSTRING: (yyval.module).docstring = (yyvsp[(2) - (2)].module).docstring; break;
            }
        }
    break;

  case 189:

/* Line 1455 of yacc.c  */
#line 1698 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.module).token = TK_IF;
        }
    break;

  case 190:

/* Line 1455 of yacc.c  */
#line 1701 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.module).token = TK_END;
        }
    break;

  case 191:

/* Line 1455 of yacc.c  */
#line 1704 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.module).token = TK_AUTOPYNAME;
        }
    break;

  case 192:

/* Line 1455 of yacc.c  */
#line 1707 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
            {
                (yyval.module).token = TK_DOCSTRING;
                (yyval.module).docstring = (yyvsp[(1) - (1)].codeb);
            }
            else
            {
                (yyval.module).token = 0;
                (yyval.module).docstring = NULL;
            }
        }
    break;

  case 194:

/* Line 1455 of yacc.c  */
#line 1722 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            /*
             * The grammar design is a bit broken and this is the easiest way
             * to allow periods in names.
             */

            char *cp;

            for (cp = (yyvsp[(1) - (1)].text); *cp != '\0'; ++cp)
                if (*cp != '.' && *cp != '_' && !isalnum(*cp))
                    yyerror("Invalid character in name");

            (yyval.text) = (yyvsp[(1) - (1)].text);
        }
    break;

  case 195:

/* Line 1455 of yacc.c  */
#line 1738 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.number) = -1;
        }
    break;

  case 197:

/* Line 1455 of yacc.c  */
#line 1744 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if ((yyvsp[(2) - (2)].include).name == NULL)
                yyerror("%Include must have a 'name' argument");

            if (notSkipping())
                parseFile(NULL, (yyvsp[(2) - (2)].include).name, NULL, (yyvsp[(2) - (2)].include).optional);
        }
    break;

  case 198:

/* Line 1455 of yacc.c  */
#line 1753 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            resetLexerState();

            (yyval.include).name = (yyvsp[(1) - (1)].text);
            (yyval.include).optional = FALSE;
        }
    break;

  case 199:

/* Line 1455 of yacc.c  */
#line 1759 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.include) = (yyvsp[(2) - (3)].include);
        }
    break;

  case 201:

/* Line 1455 of yacc.c  */
#line 1765 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.include) = (yyvsp[(1) - (3)].include);

            switch ((yyvsp[(3) - (3)].include).token)
            {
            case TK_NAME: (yyval.include).name = (yyvsp[(3) - (3)].include).name; break;
            case TK_OPTIONAL: (yyval.include).optional = (yyvsp[(3) - (3)].include).optional; break;
            }
        }
    break;

  case 202:

/* Line 1455 of yacc.c  */
#line 1776 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.include).token = TK_NAME;

            (yyval.include).name = (yyvsp[(3) - (3)].text);
            (yyval.include).optional = FALSE;
        }
    break;

  case 203:

/* Line 1455 of yacc.c  */
#line 1782 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.include).token = TK_OPTIONAL;

            (yyval.include).name = NULL;
            (yyval.include).optional = (yyvsp[(3) - (3)].boolean);
        }
    break;

  case 204:

/* Line 1455 of yacc.c  */
#line 1790 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            deprecated("%OptionalInclude is deprecated, use %Include and the 'optional' argument instead");

            if (notSkipping)
                parseFile(NULL, (yyvsp[(2) - (2)].text), NULL, TRUE);
        }
    break;

  case 205:

/* Line 1455 of yacc.c  */
#line 1798 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
                newImport((yyvsp[(2) - (2)].import).name);
        }
    break;

  case 206:

/* Line 1455 of yacc.c  */
#line 1804 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            resetLexerState();

            (yyval.import).name = (yyvsp[(1) - (1)].text);
        }
    break;

  case 207:

/* Line 1455 of yacc.c  */
#line 1809 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.import) = (yyvsp[(2) - (3)].import);
        }
    break;

  case 209:

/* Line 1455 of yacc.c  */
#line 1815 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.import) = (yyvsp[(1) - (3)].import);

            switch ((yyvsp[(3) - (3)].import).token)
            {
            case TK_NAME: (yyval.import).name = (yyvsp[(3) - (3)].import).name; break;
            }
        }
    break;

  case 210:

/* Line 1455 of yacc.c  */
#line 1825 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.import).token = TK_NAME;

            (yyval.import).name = (yyvsp[(3) - (3)].text);
        }
    break;

  case 211:

/* Line 1455 of yacc.c  */
#line 1832 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.codeb) = NULL;
        }
    break;

  case 212:

/* Line 1455 of yacc.c  */
#line 1835 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.codeb) = (yyvsp[(2) - (2)].codeb);
        }
    break;

  case 213:

/* Line 1455 of yacc.c  */
#line 1840 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.codeb) = NULL;
        }
    break;

  case 214:

/* Line 1455 of yacc.c  */
#line 1843 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.codeb) = (yyvsp[(2) - (2)].codeb);
        }
    break;

  case 215:

/* Line 1455 of yacc.c  */
#line 1848 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.codeb) = NULL;
        }
    break;

  case 216:

/* Line 1455 of yacc.c  */
#line 1851 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.codeb) = (yyvsp[(2) - (2)].codeb);
        }
    break;

  case 217:

/* Line 1455 of yacc.c  */
#line 1856 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
                appendCodeBlock(&currentModule->copying, (yyvsp[(2) - (2)].codeb));
        }
    break;

  case 218:

/* Line 1455 of yacc.c  */
#line 1862 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
                appendCodeBlock(&currentSpec->exphdrcode, (yyvsp[(2) - (2)].codeb));
        }
    break;

  case 219:

/* Line 1455 of yacc.c  */
#line 1868 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
                appendCodeBlock(&currentModule->hdrcode, (yyvsp[(2) - (2)].codeb));
        }
    break;

  case 220:

/* Line 1455 of yacc.c  */
#line 1874 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.codeb) = (yyvsp[(2) - (2)].codeb);
        }
    break;

  case 221:

/* Line 1455 of yacc.c  */
#line 1879 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.codeb) = (yyvsp[(2) - (2)].codeb);
        }
    break;

  case 222:

/* Line 1455 of yacc.c  */
#line 1884 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.codeb) = (yyvsp[(2) - (2)].codeb);
        }
    break;

  case 223:

/* Line 1455 of yacc.c  */
#line 1889 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.codeb) = (yyvsp[(2) - (2)].codeb);
        }
    break;

  case 224:

/* Line 1455 of yacc.c  */
#line 1894 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.codeb) = (yyvsp[(2) - (2)].codeb);
        }
    break;

  case 225:

/* Line 1455 of yacc.c  */
#line 1899 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.codeb) = (yyvsp[(2) - (2)].codeb);
        }
    break;

  case 226:

/* Line 1455 of yacc.c  */
#line 1904 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.codeb) = (yyvsp[(2) - (2)].codeb);
        }
    break;

  case 227:

/* Line 1455 of yacc.c  */
#line 1909 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.codeb) = (yyvsp[(2) - (2)].codeb);
        }
    break;

  case 228:

/* Line 1455 of yacc.c  */
#line 1914 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.codeb) = (yyvsp[(2) - (2)].codeb);
        }
    break;

  case 229:

/* Line 1455 of yacc.c  */
#line 1919 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.codeb) = (yyvsp[(2) - (2)].codeb);
        }
    break;

  case 230:

/* Line 1455 of yacc.c  */
#line 1924 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
                appendCodeBlock(&currentModule->cppcode, (yyvsp[(2) - (2)].codeb));
        }
    break;

  case 231:

/* Line 1455 of yacc.c  */
#line 1930 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.codeb) = (yyvsp[(2) - (2)].codeb);
        }
    break;

  case 232:

/* Line 1455 of yacc.c  */
#line 1935 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
                appendCodeBlock(&currentModule->preinitcode, (yyvsp[(2) - (2)].codeb));
        }
    break;

  case 233:

/* Line 1455 of yacc.c  */
#line 1941 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
                appendCodeBlock(&currentModule->initcode, (yyvsp[(2) - (2)].codeb));
        }
    break;

  case 234:

/* Line 1455 of yacc.c  */
#line 1947 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
                appendCodeBlock(&currentModule->postinitcode, (yyvsp[(2) - (2)].codeb));
        }
    break;

  case 235:

/* Line 1455 of yacc.c  */
#line 1953 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
                appendCodeBlock(&currentModule->unitcode, (yyvsp[(2) - (2)].codeb));
        }
    break;

  case 236:

/* Line 1455 of yacc.c  */
#line 1959 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
                appendCodeBlock(&currentModule->unitpostinccode, (yyvsp[(2) - (2)].codeb));
        }
    break;

  case 237:

/* Line 1455 of yacc.c  */
#line 1965 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            /* Deprecated. */
        }
    break;

  case 238:

/* Line 1455 of yacc.c  */
#line 1970 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping() && inMainModule())
                appendCodeBlock(&currentSpec->docs, (yyvsp[(2) - (2)].codeb));
        }
    break;

  case 239:

/* Line 1455 of yacc.c  */
#line 1976 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
                appendCodeBlock(&currentSpec->docs, (yyvsp[(2) - (2)].codeb));
        }
    break;

  case 240:

/* Line 1455 of yacc.c  */
#line 1982 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
                addAutoPyName(currentModule, (yyvsp[(2) - (2)].autopyname).remove_leading);
        }
    break;

  case 241:

/* Line 1455 of yacc.c  */
#line 1988 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.autopyname) = (yyvsp[(2) - (3)].autopyname);
        }
    break;

  case 243:

/* Line 1455 of yacc.c  */
#line 1994 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.autopyname) = (yyvsp[(1) - (3)].autopyname);

            switch ((yyvsp[(3) - (3)].autopyname).token)
            {
            case TK_REMOVELEADING: (yyval.autopyname).remove_leading = (yyvsp[(3) - (3)].autopyname).remove_leading; break;
            }
        }
    break;

  case 244:

/* Line 1455 of yacc.c  */
#line 2004 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.autopyname).token = TK_REMOVELEADING;

            (yyval.autopyname).remove_leading = (yyvsp[(3) - (3)].text);
        }
    break;

  case 245:

/* Line 1455 of yacc.c  */
#line 2011 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if ((yyvsp[(2) - (3)].extract).id == NULL)
                yyerror("%Extract must have an 'id' argument");

            if (notSkipping())
                addExtractPart(currentSpec, (yyvsp[(2) - (3)].extract).id, (yyvsp[(2) - (3)].extract).order, (yyvsp[(3) - (3)].codeb));
        }
    break;

  case 246:

/* Line 1455 of yacc.c  */
#line 2020 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            resetLexerState();

            (yyval.extract).id = (yyvsp[(1) - (1)].text);
            (yyval.extract).order = -1;
        }
    break;

  case 247:

/* Line 1455 of yacc.c  */
#line 2026 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.extract) = (yyvsp[(2) - (3)].extract);
        }
    break;

  case 249:

/* Line 1455 of yacc.c  */
#line 2032 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.extract) = (yyvsp[(1) - (3)].extract);

            switch ((yyvsp[(3) - (3)].extract).token)
            {
            case TK_ID: (yyval.extract).id = (yyvsp[(3) - (3)].extract).id; break;
            case TK_ORDER: (yyval.extract).order = (yyvsp[(3) - (3)].extract).order; break;
            }
        }
    break;

  case 250:

/* Line 1455 of yacc.c  */
#line 2043 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.extract).token = TK_ID;

            (yyval.extract).id = (yyvsp[(3) - (3)].text);
            (yyval.extract).order = -1;
        }
    break;

  case 251:

/* Line 1455 of yacc.c  */
#line 2049 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.extract).token = TK_ORDER;

            if ((yyvsp[(3) - (3)].number) < 0)
                yyerror("The 'order' of an %Extract directive must not be negative");

            (yyval.extract).id = NULL;
            (yyval.extract).order = (yyvsp[(3) - (3)].number);
        }
    break;

  case 252:

/* Line 1455 of yacc.c  */
#line 2060 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            /* Deprecated. */
        }
    break;

  case 255:

/* Line 1455 of yacc.c  */
#line 2069 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.codeb) = (yyvsp[(1) - (2)].codeb);

            append(&(yyval.codeb)->frag, (yyvsp[(2) - (2)].codeb)->frag);

            free((yyvsp[(2) - (2)].codeb)->frag);
            free((char *)(yyvsp[(2) - (2)].codeb)->filename);
            free((yyvsp[(2) - (2)].codeb));
        }
    break;

  case 256:

/* Line 1455 of yacc.c  */
#line 2080 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
            {
                const char *annos[] = {
                    "PyName",
                    NULL
                };

                checkAnnos(&(yyvsp[(3) - (3)].optflags), annos);

                if (sectionFlags != 0 && (sectionFlags & ~(SECT_IS_PUBLIC | SECT_IS_PROT)) != 0)
                    yyerror("Class enums must be in the public or protected sections");

                currentEnum = newEnum(currentSpec, currentModule,
                        currentMappedType, (yyvsp[(2) - (3)].text), &(yyvsp[(3) - (3)].optflags), sectionFlags);
            }
        }
    break;

  case 258:

/* Line 1455 of yacc.c  */
#line 2099 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.text) = NULL;
        }
    break;

  case 259:

/* Line 1455 of yacc.c  */
#line 2102 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.text) = (yyvsp[(1) - (1)].text);
        }
    break;

  case 260:

/* Line 1455 of yacc.c  */
#line 2107 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.text) = NULL;
        }
    break;

  case 261:

/* Line 1455 of yacc.c  */
#line 2110 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.text) = (yyvsp[(1) - (1)].text);
        }
    break;

  case 268:

/* Line 1455 of yacc.c  */
#line 2125 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
            {
                const char *annos[] = {
                    "PyName",
                    NULL
                };

                enumMemberDef *emd, **tail;

                checkAnnos(&(yyvsp[(3) - (4)].optflags), annos);

                /* Note that we don't use the assigned value. */
                emd = sipMalloc(sizeof (enumMemberDef));

                emd -> pyname = cacheName(currentSpec,
                        getPythonName(currentModule, &(yyvsp[(3) - (4)].optflags), (yyvsp[(1) - (4)].text)));
                emd -> cname = (yyvsp[(1) - (4)].text);
                emd -> ed = currentEnum;
                emd -> next = NULL;

                checkAttributes(currentSpec, currentModule, emd->ed->ecd,
                        emd->ed->emtd, emd->pyname->text, FALSE);

                /* Append to preserve the order. */
                for (tail = &currentEnum->members; *tail != NULL; tail = &(*tail)->next)
                    ;

                *tail = emd;

                if (inMainModule())
                    setIsUsedName(emd -> pyname);
            }
        }
    break;

  case 273:

/* Line 1455 of yacc.c  */
#line 2169 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.valp) = NULL;
        }
    break;

  case 274:

/* Line 1455 of yacc.c  */
#line 2172 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.valp) = (yyvsp[(2) - (2)].valp);
        }
    break;

  case 276:

/* Line 1455 of yacc.c  */
#line 2178 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            valueDef *vd;
 
            if ((yyvsp[(1) - (3)].valp) -> vtype == string_value || (yyvsp[(3) - (3)].valp) -> vtype == string_value)
                yyerror("Invalid binary operator for string");
 
            /* Find the last value in the existing expression. */
 
            for (vd = (yyvsp[(1) - (3)].valp); vd -> next != NULL; vd = vd -> next)
                ;
 
            vd -> vbinop = (yyvsp[(2) - (3)].qchar);
            vd -> next = (yyvsp[(3) - (3)].valp);

            (yyval.valp) = (yyvsp[(1) - (3)].valp);
        }
    break;

  case 277:

/* Line 1455 of yacc.c  */
#line 2196 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.qchar) = '-';
        }
    break;

  case 278:

/* Line 1455 of yacc.c  */
#line 2199 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.qchar) = '+';
        }
    break;

  case 279:

/* Line 1455 of yacc.c  */
#line 2202 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.qchar) = '*';
        }
    break;

  case 280:

/* Line 1455 of yacc.c  */
#line 2205 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.qchar) = '/';
        }
    break;

  case 281:

/* Line 1455 of yacc.c  */
#line 2208 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.qchar) = '&';
        }
    break;

  case 282:

/* Line 1455 of yacc.c  */
#line 2211 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.qchar) = '|';
        }
    break;

  case 283:

/* Line 1455 of yacc.c  */
#line 2216 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.qchar) = '\0';
        }
    break;

  case 284:

/* Line 1455 of yacc.c  */
#line 2219 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.qchar) = '!';
        }
    break;

  case 285:

/* Line 1455 of yacc.c  */
#line 2222 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.qchar) = '~';
        }
    break;

  case 286:

/* Line 1455 of yacc.c  */
#line 2225 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.qchar) = '-';
        }
    break;

  case 287:

/* Line 1455 of yacc.c  */
#line 2228 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.qchar) = '+';
        }
    break;

  case 288:

/* Line 1455 of yacc.c  */
#line 2233 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if ((yyvsp[(1) - (2)].qchar) != '\0' && (yyvsp[(2) - (2)].value).vtype == string_value)
                yyerror("Invalid unary operator for string");
 
            /*
             * Convert the value to a simple expression on the
             * heap.
             */
 
            (yyval.valp) = sipMalloc(sizeof (valueDef));
 
            *(yyval.valp) = (yyvsp[(2) - (2)].value);
            (yyval.valp) -> vunop = (yyvsp[(1) - (2)].qchar);
            (yyval.valp) -> vbinop = '\0';
            (yyval.valp) -> next = NULL;
        }
    break;

  case 290:

/* Line 1455 of yacc.c  */
#line 2252 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (currentSpec -> genc)
                yyerror("Scoped names are not allowed in a C module");

            appendScopedName(&(yyvsp[(1) - (3)].scpvalp),(yyvsp[(3) - (3)].scpvalp));
        }
    break;

  case 291:

/* Line 1455 of yacc.c  */
#line 2260 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.scpvalp) = text2scopePart((yyvsp[(1) - (1)].text));
        }
    break;

  case 292:

/* Line 1455 of yacc.c  */
#line 2265 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.boolean) = TRUE;
        }
    break;

  case 293:

/* Line 1455 of yacc.c  */
#line 2268 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.boolean) = FALSE;
        }
    break;

  case 294:

/* Line 1455 of yacc.c  */
#line 2273 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            /*
             * We let the C++ compiler decide if the value is a valid one - no
             * point in building a full C++ parser here.
             */

            (yyval.value).vtype = scoped_value;
            (yyval.value).u.vscp = (yyvsp[(1) - (1)].scpvalp);
        }
    break;

  case 295:

/* Line 1455 of yacc.c  */
#line 2282 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            fcallDef *fcd;

            fcd = sipMalloc(sizeof (fcallDef));
            *fcd = (yyvsp[(3) - (4)].fcall);
            fcd -> type = (yyvsp[(1) - (4)].memArg);

            (yyval.value).vtype = fcall_value;
            (yyval.value).u.fcd = fcd;
        }
    break;

  case 296:

/* Line 1455 of yacc.c  */
#line 2292 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.value).vtype = real_value;
            (yyval.value).u.vreal = (yyvsp[(1) - (1)].real);
        }
    break;

  case 297:

/* Line 1455 of yacc.c  */
#line 2296 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.value).vtype = numeric_value;
            (yyval.value).u.vnum = (yyvsp[(1) - (1)].number);
        }
    break;

  case 298:

/* Line 1455 of yacc.c  */
#line 2300 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.value).vtype = numeric_value;
            (yyval.value).u.vnum = (yyvsp[(1) - (1)].boolean);
        }
    break;

  case 299:

/* Line 1455 of yacc.c  */
#line 2304 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.value).vtype = numeric_value;
            (yyval.value).u.vnum = 0;
        }
    break;

  case 300:

/* Line 1455 of yacc.c  */
#line 2308 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.value).vtype = string_value;
            (yyval.value).u.vstr = (yyvsp[(1) - (1)].text);
        }
    break;

  case 301:

/* Line 1455 of yacc.c  */
#line 2312 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.value).vtype = qchar_value;
            (yyval.value).u.vqchar = (yyvsp[(1) - (1)].qchar);
        }
    break;

  case 302:

/* Line 1455 of yacc.c  */
#line 2318 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            /* No values. */

            (yyval.fcall).nrArgs = 0;
        }
    break;

  case 303:

/* Line 1455 of yacc.c  */
#line 2323 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            /* The single or first expression. */

            (yyval.fcall).args[0] = (yyvsp[(1) - (1)].valp);
            (yyval.fcall).nrArgs = 1;
        }
    break;

  case 304:

/* Line 1455 of yacc.c  */
#line 2329 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            /* Check that it wasn't ...(,expression...). */

            if ((yyval.fcall).nrArgs == 0)
                yyerror("First argument to function call is missing");

            /* Check there is room. */

            if ((yyvsp[(1) - (3)].fcall).nrArgs == MAX_NR_ARGS)
                yyerror("Too many arguments to function call");

            (yyval.fcall) = (yyvsp[(1) - (3)].fcall);

            (yyval.fcall).args[(yyval.fcall).nrArgs] = (yyvsp[(3) - (3)].valp);
            (yyval.fcall).nrArgs++;
        }
    break;

  case 305:

/* Line 1455 of yacc.c  */
#line 2347 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
            {
                const char *annos[] = {
                    "DocType",
                    "Encoding",
                    "NoTypeName",
                    "PyInt",
                    NULL
                };

                checkAnnos(&(yyvsp[(4) - (5)].optflags), annos);

                applyTypeFlags(currentModule, &(yyvsp[(2) - (5)].memArg), &(yyvsp[(4) - (5)].optflags));
                newTypedef(currentSpec, currentModule, (yyvsp[(3) - (5)].text), &(yyvsp[(2) - (5)].memArg), &(yyvsp[(4) - (5)].optflags));
            }
        }
    break;

  case 306:

/* Line 1455 of yacc.c  */
#line 2364 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
            {
                const char *annos[] = {
                    "DocType",
                    "Encoding",
                    "NoTypeName",
                    "PyInt",
                    NULL
                };

                signatureDef *sig;
                argDef ftype;

                checkAnnos(&(yyvsp[(10) - (11)].optflags), annos);

                applyTypeFlags(currentModule, &(yyvsp[(2) - (11)].memArg), &(yyvsp[(10) - (11)].optflags));

                memset(&ftype, 0, sizeof (argDef));

                /* Create the full signature on the heap. */
                sig = sipMalloc(sizeof (signatureDef));
                *sig = (yyvsp[(8) - (11)].signature);
                sig->result = (yyvsp[(2) - (11)].memArg);

                /* Create the full type. */
                ftype.atype = function_type;
                ftype.nrderefs = (yyvsp[(4) - (11)].number);
                ftype.u.sa = sig;

                newTypedef(currentSpec, currentModule, (yyvsp[(5) - (11)].text), &ftype, &(yyvsp[(10) - (11)].optflags));
            }
        }
    break;

  case 307:

/* Line 1455 of yacc.c  */
#line 2399 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (currentSpec -> genc && (yyvsp[(2) - (2)].scpvalp)->next != NULL)
                yyerror("Namespaces not allowed in a C module");

            if (notSkipping())
                currentSupers = NULL;
        }
    break;

  case 308:

/* Line 1455 of yacc.c  */
#line 2405 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
            {
                const char *annos[] = {
                    "Abstract",
                    "AllowNone",
                    "API",
                    "DelayDtor",
                    "Deprecated",
                    "External",
                    "Metatype",
                    "NoDefaultCtors",
                    "PyName",
                    "PyQt4Flags",
                    "PyQt4NoQMetaObject",
                    "Supertype",
                    NULL
                };

                checkAnnos(&(yyvsp[(5) - (5)].optflags), annos);

                if (currentSpec->genc && currentSupers != NULL)
                    yyerror("Super-classes not allowed in a C module struct");

                defineClass((yyvsp[(2) - (5)].scpvalp), currentSupers, &(yyvsp[(5) - (5)].optflags));
                sectionFlags = SECT_IS_PUBLIC;
            }
        }
    break;

  case 309:

/* Line 1455 of yacc.c  */
#line 2432 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
                completeClass((yyvsp[(2) - (8)].scpvalp), &(yyvsp[(5) - (8)].optflags), (yyvsp[(7) - (8)].boolean));
        }
    break;

  case 310:

/* Line 1455 of yacc.c  */
#line 2438 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {currentIsTemplate = TRUE;}
    break;

  case 311:

/* Line 1455 of yacc.c  */
#line 2438 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (currentSpec->genc)
                yyerror("Class templates not allowed in a C module");

            if (notSkipping())
            {
                classTmplDef *tcd;

                /*
                 * Make sure there is room for the extra class name argument.
                 */
                if ((yyvsp[(1) - (3)].signature).nrArgs == MAX_NR_ARGS)
                    yyerror("Internal error - increase the value of MAX_NR_ARGS");

                tcd = sipMalloc(sizeof (classTmplDef));
                tcd->sig = (yyvsp[(1) - (3)].signature);
                tcd->cd = (yyvsp[(3) - (3)].klass);
                tcd->next = currentSpec->classtemplates;

                currentSpec->classtemplates = tcd;
            }

            currentIsTemplate = FALSE;
        }
    break;

  case 312:

/* Line 1455 of yacc.c  */
#line 2464 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.signature) = (yyvsp[(3) - (4)].signature);
        }
    break;

  case 313:

/* Line 1455 of yacc.c  */
#line 2469 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (currentSpec->genc)
                yyerror("Class definition not allowed in a C module");

            if (notSkipping())
                currentSupers = NULL;
        }
    break;

  case 314:

/* Line 1455 of yacc.c  */
#line 2475 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
            {
                const char *annos[] = {
                    "Abstract",
                    "AllowNone",
                    "API",
                    "DelayDtor",
                    "Deprecated",
                    "External",
                    "Metatype",
                    "NoDefaultCtors",
                    "PyName",
                    "PyQt4Flags",
                    "PyQt4NoQMetaObject",
                    "Supertype",
                    NULL
                };

                checkAnnos(&(yyvsp[(5) - (5)].optflags), annos);

                defineClass((yyvsp[(2) - (5)].scpvalp), currentSupers, &(yyvsp[(5) - (5)].optflags));
                sectionFlags = SECT_IS_PRIVATE;
            }
        }
    break;

  case 315:

/* Line 1455 of yacc.c  */
#line 2499 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
                (yyval.klass) = completeClass((yyvsp[(2) - (8)].scpvalp), &(yyvsp[(5) - (8)].optflags), (yyvsp[(7) - (8)].boolean));
        }
    break;

  case 320:

/* Line 1455 of yacc.c  */
#line 2513 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
            {
                argDef ad;
                classDef *super;
                scopedNameDef *snd = (yyvsp[(1) - (1)].scpvalp);

                /*
                 * This is a hack to allow typedef'ed classes to be used before
                 * we have resolved the typedef definitions.  Unlike elsewhere,
                 * we require that the typedef is defined before being used.
                 */
                for (;;)
                {
                    ad.atype = no_type;
                    ad.argflags = 0;
                    ad.nrderefs = 0;
                    ad.original_type = NULL;

                    searchTypedefs(currentSpec, snd, &ad);

                    if (ad.atype != defined_type)
                        break;

                    if (ad.nrderefs != 0 || isConstArg(&ad) || isReference(&ad))
                        break;

                    snd = ad.u.snd;
                }

                if (ad.atype != no_type)
                    yyerror("Super-class list contains an invalid type");

                super = findClass(currentSpec, class_iface, NULL, snd);
                appendToClassList(&currentSupers, super);
            }
        }
    break;

  case 321:

/* Line 1455 of yacc.c  */
#line 2552 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.boolean) = FALSE;
        }
    break;

  case 322:

/* Line 1455 of yacc.c  */
#line 2555 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.boolean) = TRUE;
        }
    break;

  case 334:

/* Line 1455 of yacc.c  */
#line 2573 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
            {
                classDef *scope = currentScope();

                /* Make sure this is before any ctor docstrings. */
                (yyvsp[(1) - (1)].codeb)->next = scope->docstring;
                scope->docstring = (yyvsp[(1) - (1)].codeb);
            }
        }
    break;

  case 335:

/* Line 1455 of yacc.c  */
#line 2583 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
                appendCodeBlock(&currentScope()->cppcode, (yyvsp[(1) - (1)].codeb));
        }
    break;

  case 336:

/* Line 1455 of yacc.c  */
#line 2587 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
                appendCodeBlock(&currentScope()->iff->hdrcode, (yyvsp[(1) - (1)].codeb));
        }
    break;

  case 337:

/* Line 1455 of yacc.c  */
#line 2591 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
            {
                classDef *scope = currentScope();

                if (scope->travcode != NULL)
                    yyerror("%GCTraverseCode already given for class");

                scope->travcode = (yyvsp[(1) - (1)].codeb);
            }
        }
    break;

  case 338:

/* Line 1455 of yacc.c  */
#line 2602 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
            {
                classDef *scope = currentScope();

                if (scope->clearcode != NULL)
                    yyerror("%GCClearCode already given for class");

                scope->clearcode = (yyvsp[(1) - (1)].codeb);
            }
        }
    break;

  case 339:

/* Line 1455 of yacc.c  */
#line 2613 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
            {
                classDef *scope = currentScope();

                if (scope->getbufcode != NULL)
                    yyerror("%BIGetBufferCode already given for class");

                scope->getbufcode = (yyvsp[(1) - (1)].codeb);
            }
        }
    break;

  case 340:

/* Line 1455 of yacc.c  */
#line 2624 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
            {
                classDef *scope = currentScope();

                if (scope->releasebufcode != NULL)
                    yyerror("%BIReleaseBufferCode already given for class");

                scope->releasebufcode = (yyvsp[(1) - (1)].codeb);
            }
        }
    break;

  case 341:

/* Line 1455 of yacc.c  */
#line 2635 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
            {
                classDef *scope = currentScope();

                if (scope->readbufcode != NULL)
                    yyerror("%BIGetReadBufferCode already given for class");

                scope->readbufcode = (yyvsp[(1) - (1)].codeb);
            }
        }
    break;

  case 342:

/* Line 1455 of yacc.c  */
#line 2646 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
            {
                classDef *scope = currentScope();

                if (scope->writebufcode != NULL)
                    yyerror("%BIGetWriteBufferCode already given for class");

                scope->writebufcode = (yyvsp[(1) - (1)].codeb);
            }
        }
    break;

  case 343:

/* Line 1455 of yacc.c  */
#line 2657 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
            {
                classDef *scope = currentScope();

                if (scope->segcountcode != NULL)
                    yyerror("%BIGetSegCountCode already given for class");

                scope->segcountcode = (yyvsp[(1) - (1)].codeb);
            }
        }
    break;

  case 344:

/* Line 1455 of yacc.c  */
#line 2668 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
            {
                classDef *scope = currentScope();

                if (scope->charbufcode != NULL)
                    yyerror("%BIGetCharBufferCode already given for class");

                scope->charbufcode = (yyvsp[(1) - (1)].codeb);
            }
        }
    break;

  case 345:

/* Line 1455 of yacc.c  */
#line 2679 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
            {
                classDef *scope = currentScope();

                if (scope->picklecode != NULL)
                    yyerror("%PickleCode already given for class");

                scope->picklecode = (yyvsp[(1) - (1)].codeb);
            }
        }
    break;

  case 349:

/* Line 1455 of yacc.c  */
#line 2693 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
            {
                classDef *scope = currentScope();

                if (scope->convtosubcode != NULL)
                    yyerror("Class has more than one %ConvertToSubClassCode directive");

                scope->convtosubcode = (yyvsp[(2) - (2)].codeb);
            }
        }
    break;

  case 350:

/* Line 1455 of yacc.c  */
#line 2704 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
            {
                classDef *scope = currentScope();

                if (scope->convtocode != NULL)
                    yyerror("Class has more than one %ConvertToTypeCode directive");

                scope->convtocode = (yyvsp[(2) - (2)].codeb);
            }
        }
    break;

  case 351:

/* Line 1455 of yacc.c  */
#line 2715 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (currentSpec -> genc)
                yyerror("public section not allowed in a C module");

            if (notSkipping())
                sectionFlags = SECT_IS_PUBLIC | (yyvsp[(2) - (3)].number);
        }
    break;

  case 352:

/* Line 1455 of yacc.c  */
#line 2722 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (currentSpec -> genc)
                yyerror("protected section not allowed in a C module");

            if (notSkipping())
                sectionFlags = SECT_IS_PROT | (yyvsp[(2) - (3)].number);
        }
    break;

  case 353:

/* Line 1455 of yacc.c  */
#line 2729 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (currentSpec -> genc)
                yyerror("private section not allowed in a C module");

            if (notSkipping())
                sectionFlags = SECT_IS_PRIVATE | (yyvsp[(2) - (3)].number);
        }
    break;

  case 354:

/* Line 1455 of yacc.c  */
#line 2736 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (currentSpec -> genc)
                yyerror("signals section not allowed in a C module");

            if (notSkipping())
                sectionFlags = SECT_IS_SIGNAL;
        }
    break;

  case 355:

/* Line 1455 of yacc.c  */
#line 2745 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if ((yyvsp[(2) - (3)].property).name == NULL)
                yyerror("A %Property directive must have a 'name' argument");

            if ((yyvsp[(2) - (3)].property).get == NULL)
                yyerror("A %Property directive must have a 'get' argument");

            if (notSkipping())
                addProperty(currentSpec, currentModule, currentScope(),
                        (yyvsp[(2) - (3)].property).name, (yyvsp[(2) - (3)].property).get, (yyvsp[(2) - (3)].property).set, (yyvsp[(3) - (3)].property).docstring);
        }
    break;

  case 356:

/* Line 1455 of yacc.c  */
#line 2758 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.property) = (yyvsp[(2) - (3)].property);
        }
    break;

  case 358:

/* Line 1455 of yacc.c  */
#line 2764 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.property) = (yyvsp[(1) - (3)].property);

            switch ((yyvsp[(3) - (3)].property).token)
            {
            case TK_GET: (yyval.property).get = (yyvsp[(3) - (3)].property).get; break;
            case TK_NAME: (yyval.property).name = (yyvsp[(3) - (3)].property).name; break;
            case TK_SET: (yyval.property).set = (yyvsp[(3) - (3)].property).set; break;
            }
        }
    break;

  case 359:

/* Line 1455 of yacc.c  */
#line 2776 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.property).token = TK_GET;

            (yyval.property).get = (yyvsp[(3) - (3)].text);
            (yyval.property).name = NULL;
            (yyval.property).set = NULL;
        }
    break;

  case 360:

/* Line 1455 of yacc.c  */
#line 2783 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.property).token = TK_NAME;

            (yyval.property).get = NULL;
            (yyval.property).name = (yyvsp[(3) - (3)].text);
            (yyval.property).set = NULL;
        }
    break;

  case 361:

/* Line 1455 of yacc.c  */
#line 2790 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.property).token = TK_SET;

            (yyval.property).get = NULL;
            (yyval.property).name = NULL;
            (yyval.property).set = (yyvsp[(3) - (3)].text);
        }
    break;

  case 362:

/* Line 1455 of yacc.c  */
#line 2799 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.property).token = 0;
            (yyval.property).docstring = NULL;
        }
    break;

  case 363:

/* Line 1455 of yacc.c  */
#line 2803 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.property) = (yyvsp[(2) - (4)].property);
        }
    break;

  case 365:

/* Line 1455 of yacc.c  */
#line 2809 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.property) = (yyvsp[(1) - (2)].property);

            switch ((yyvsp[(2) - (2)].property).token)
            {
            case TK_DOCSTRING: (yyval.property).docstring = (yyvsp[(2) - (2)].property).docstring; break;
            }
        }
    break;

  case 366:

/* Line 1455 of yacc.c  */
#line 2819 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.property).token = TK_IF;
        }
    break;

  case 367:

/* Line 1455 of yacc.c  */
#line 2822 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.property).token = TK_END;
        }
    break;

  case 368:

/* Line 1455 of yacc.c  */
#line 2825 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
            {
                (yyval.property).token = TK_DOCSTRING;
                (yyval.property).docstring = (yyvsp[(1) - (1)].codeb);
            }
            else
            {
                (yyval.property).token = 0;
                (yyval.property).docstring = NULL;
            }
        }
    break;

  case 369:

/* Line 1455 of yacc.c  */
#line 2839 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.number) = 0;
        }
    break;

  case 370:

/* Line 1455 of yacc.c  */
#line 2842 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.number) = SECT_IS_SLOT;
        }
    break;

  case 371:

/* Line 1455 of yacc.c  */
#line 2847 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            /* Note that we allow non-virtual dtors in C modules. */

            if (notSkipping())
            {
                const char *annos[] = {
                    "HoldGIL",
                    "ReleaseGIL",
                    NULL
                };

                classDef *cd = currentScope();

                checkAnnos(&(yyvsp[(8) - (11)].optflags), annos);

                if (strcmp(classBaseName(cd),(yyvsp[(3) - (11)].text)) != 0)
                    yyerror("Destructor doesn't have the same name as its class");

                if (isDtor(cd))
                    yyerror("Destructor has already been defined");

                if (currentSpec -> genc && (yyvsp[(10) - (11)].codeb) == NULL)
                    yyerror("Destructor in C modules must include %MethodCode");

                cd -> dealloccode = (yyvsp[(10) - (11)].codeb);
                cd -> dtorcode = (yyvsp[(11) - (11)].codeb);
                cd -> dtorexceptions = (yyvsp[(6) - (11)].throwlist);

                /*
                 * Note that we don't apply the protected/public hack to dtors
                 * as it (I think) may change the behaviour of the wrapped API.
                 */
                cd->classflags |= sectionFlags;

                if ((yyvsp[(7) - (11)].number))
                {
                    if (!(yyvsp[(1) - (11)].number))
                        yyerror("Abstract destructor must be virtual");

                    setIsAbstractClass(cd);
                }

                /*
                 * The class has a shadow if we have a virtual dtor or some
                 * dtor code.
                 */
                if ((yyvsp[(1) - (11)].number) || (yyvsp[(11) - (11)].codeb) != NULL)
                {
                    if (currentSpec -> genc)
                        yyerror("Virtual destructor or %VirtualCatcherCode not allowed in a C module");

                    setHasShadow(cd);
                }

                if (getReleaseGIL(&(yyvsp[(8) - (11)].optflags)))
                    setIsReleaseGILDtor(cd);
                else if (getHoldGIL(&(yyvsp[(8) - (11)].optflags)))
                    setIsHoldGILDtor(cd);
            }
        }
    break;

  case 372:

/* Line 1455 of yacc.c  */
#line 2909 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {currentCtorIsExplicit = TRUE;}
    break;

  case 375:

/* Line 1455 of yacc.c  */
#line 2913 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            /* Note that we allow ctors in C modules. */

            if (notSkipping())
            {
                const char *annos[] = {
                    "API",
                    "Default",
                    "Deprecated",
                    "HoldGIL",
                    "KeywordArgs",
                    "NoDerived",
                    "PostHook",
                    "PreHook",
                    "ReleaseGIL",
                    "Transfer",
                    NULL
                };

                checkAnnos(&(yyvsp[(6) - (10)].optflags), annos);

                if (currentSpec -> genc)
                {
                    if ((yyvsp[(10) - (10)].codeb) == NULL && (yyvsp[(3) - (10)].signature).nrArgs != 0)
                        yyerror("Constructors with arguments in C modules must include %MethodCode");

                    if (currentCtorIsExplicit)
                        yyerror("Explicit constructors not allowed in a C module");
                }

                if ((sectionFlags & (SECT_IS_PUBLIC | SECT_IS_PROT | SECT_IS_PRIVATE)) == 0)
                    yyerror("Constructor must be in the public, private or protected sections");

                newCtor(currentModule, (yyvsp[(1) - (10)].text), sectionFlags, &(yyvsp[(3) - (10)].signature), &(yyvsp[(6) - (10)].optflags), (yyvsp[(10) - (10)].codeb), (yyvsp[(5) - (10)].throwlist), (yyvsp[(7) - (10)].optsignature),
                        currentCtorIsExplicit, (yyvsp[(9) - (10)].codeb));
            }

            free((yyvsp[(1) - (10)].text));

            currentCtorIsExplicit = FALSE;
        }
    break;

  case 376:

/* Line 1455 of yacc.c  */
#line 2956 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.optsignature) = NULL;
        }
    break;

  case 377:

/* Line 1455 of yacc.c  */
#line 2959 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            parsingCSignature = TRUE;
        }
    break;

  case 378:

/* Line 1455 of yacc.c  */
#line 2961 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.optsignature) = sipMalloc(sizeof (signatureDef));

            *(yyval.optsignature) = (yyvsp[(4) - (6)].signature);

            parsingCSignature = FALSE;
        }
    break;

  case 379:

/* Line 1455 of yacc.c  */
#line 2970 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.optsignature) = NULL;
        }
    break;

  case 380:

/* Line 1455 of yacc.c  */
#line 2973 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            parsingCSignature = TRUE;
        }
    break;

  case 381:

/* Line 1455 of yacc.c  */
#line 2975 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.optsignature) = sipMalloc(sizeof (signatureDef));

            *(yyval.optsignature) = (yyvsp[(5) - (7)].signature);
            (yyval.optsignature)->result = (yyvsp[(3) - (7)].memArg);

            parsingCSignature = FALSE;
        }
    break;

  case 382:

/* Line 1455 of yacc.c  */
#line 2985 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.number) = FALSE;
        }
    break;

  case 383:

/* Line 1455 of yacc.c  */
#line 2988 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.number) = TRUE;
        }
    break;

  case 384:

/* Line 1455 of yacc.c  */
#line 2993 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
            {
                applyTypeFlags(currentModule, &(yyvsp[(1) - (14)].memArg), &(yyvsp[(9) - (14)].optflags));

                (yyvsp[(4) - (14)].signature).result = (yyvsp[(1) - (14)].memArg);

                newFunction(currentSpec, currentModule, currentScope(), NULL,
                        sectionFlags, currentIsStatic, currentIsSignal,
                        currentIsSlot, currentOverIsVirt, (yyvsp[(2) - (14)].text), &(yyvsp[(4) - (14)].signature), (yyvsp[(6) - (14)].number), (yyvsp[(8) - (14)].number), &(yyvsp[(9) - (14)].optflags),
                        (yyvsp[(13) - (14)].codeb), (yyvsp[(14) - (14)].codeb), (yyvsp[(7) - (14)].throwlist), (yyvsp[(10) - (14)].optsignature), (yyvsp[(12) - (14)].codeb));
            }

            currentIsStatic = FALSE;
            currentIsSignal = FALSE;
            currentIsSlot = FALSE;
            currentOverIsVirt = FALSE;
        }
    break;

  case 385:

/* Line 1455 of yacc.c  */
#line 3011 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            /*
             * It looks like an assignment operator (though we don't bother to
             * check the types) so make sure it is private.
             */
            if (notSkipping())
            {
                classDef *cd = currentScope();

                if (cd == NULL || !(sectionFlags & SECT_IS_PRIVATE))
                    yyerror("Assignment operators may only be defined as private");

                setCannotAssign(cd);
            }

            currentIsStatic = FALSE;
            currentIsSignal = FALSE;
            currentIsSlot = FALSE;
            currentOverIsVirt = FALSE;
        }
    break;

  case 386:

/* Line 1455 of yacc.c  */
#line 3031 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
            {
                classDef *cd = currentScope();

                /*
                 * If the scope is a namespace then make sure the operator is
                 * handled as a global.
                 */
                if (cd != NULL && cd->iff->type == namespace_iface)
                    cd = NULL;

                applyTypeFlags(currentModule, &(yyvsp[(1) - (14)].memArg), &(yyvsp[(10) - (14)].optflags));

                /* Handle the unary '+' and '-' operators. */
                if ((cd != NULL && (yyvsp[(5) - (14)].signature).nrArgs == 0) || (cd == NULL && (yyvsp[(5) - (14)].signature).nrArgs == 1))
                {
                    if (strcmp((yyvsp[(3) - (14)].text), "__add__") == 0)
                        (yyvsp[(3) - (14)].text) = "__pos__";
                    else if (strcmp((yyvsp[(3) - (14)].text), "__sub__") == 0)
                        (yyvsp[(3) - (14)].text) = "__neg__";
                }

                (yyvsp[(5) - (14)].signature).result = (yyvsp[(1) - (14)].memArg);

                newFunction(currentSpec, currentModule, cd, NULL,
                        sectionFlags, currentIsStatic, currentIsSignal,
                        currentIsSlot, currentOverIsVirt, (yyvsp[(3) - (14)].text), &(yyvsp[(5) - (14)].signature), (yyvsp[(7) - (14)].number), (yyvsp[(9) - (14)].number),
                        &(yyvsp[(10) - (14)].optflags), (yyvsp[(13) - (14)].codeb), (yyvsp[(14) - (14)].codeb), (yyvsp[(8) - (14)].throwlist), (yyvsp[(11) - (14)].optsignature), NULL);
            }

            currentIsStatic = FALSE;
            currentIsSignal = FALSE;
            currentIsSlot = FALSE;
            currentOverIsVirt = FALSE;
        }
    break;

  case 387:

/* Line 1455 of yacc.c  */
#line 3067 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
            {
                char *sname;
                classDef *scope = currentScope();

                if (scope == NULL || (yyvsp[(4) - (13)].signature).nrArgs != 0)
                    yyerror("Operator casts must be specified in a class and have no arguments");

                applyTypeFlags(currentModule, &(yyvsp[(2) - (13)].memArg), &(yyvsp[(9) - (13)].optflags));

                switch ((yyvsp[(2) - (13)].memArg).atype)
                {
                case defined_type:
                    sname = NULL;
                    break;

                case bool_type:
                case cbool_type:
                case byte_type:
                case sbyte_type:
                case ubyte_type:
                case short_type:
                case ushort_type:
                case int_type:
                case cint_type:
                case uint_type:
                    sname = "__int__";
                    break;

                case long_type:
                case ulong_type:
                case longlong_type:
                case ulonglong_type:
                    sname = "__long__";
                    break;

                case float_type:
                case cfloat_type:
                case double_type:
                case cdouble_type:
                    sname = "__float__";
                    break;

                default:
                    yyerror("Unsupported operator cast");
                }

                if (sname != NULL)
                {
                    (yyvsp[(4) - (13)].signature).result = (yyvsp[(2) - (13)].memArg);

                    newFunction(currentSpec, currentModule, scope, NULL,
                            sectionFlags, currentIsStatic, currentIsSignal,
                            currentIsSlot, currentOverIsVirt, sname, &(yyvsp[(4) - (13)].signature), (yyvsp[(6) - (13)].number),
                            (yyvsp[(8) - (13)].number), &(yyvsp[(9) - (13)].optflags), (yyvsp[(12) - (13)].codeb), (yyvsp[(13) - (13)].codeb), (yyvsp[(7) - (13)].throwlist), (yyvsp[(10) - (13)].optsignature), NULL);
                }
                else
                {
                    argList *al;

                    /* Check it doesn't already exist. */
                    for (al = scope->casts; al != NULL; al = al->next)
                        if (compareScopedNames((yyvsp[(2) - (13)].memArg).u.snd, al->arg.u.snd) == 0)
                            yyerror("This operator cast has already been specified in this class");

                    al = sipMalloc(sizeof (argList));
                    al->arg = (yyvsp[(2) - (13)].memArg);
                    al->next = scope->casts;

                    scope->casts = al;
                }
            }

            currentIsStatic = FALSE;
            currentIsSignal = FALSE;
            currentIsSlot = FALSE;
            currentOverIsVirt = FALSE;
        }
    break;

  case 388:

/* Line 1455 of yacc.c  */
#line 3148 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {(yyval.text) = "__add__";}
    break;

  case 389:

/* Line 1455 of yacc.c  */
#line 3149 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {(yyval.text) = "__sub__";}
    break;

  case 390:

/* Line 1455 of yacc.c  */
#line 3150 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {(yyval.text) = "__mul__";}
    break;

  case 391:

/* Line 1455 of yacc.c  */
#line 3151 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {(yyval.text) = "__div__";}
    break;

  case 392:

/* Line 1455 of yacc.c  */
#line 3152 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {(yyval.text) = "__mod__";}
    break;

  case 393:

/* Line 1455 of yacc.c  */
#line 3153 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {(yyval.text) = "__and__";}
    break;

  case 394:

/* Line 1455 of yacc.c  */
#line 3154 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {(yyval.text) = "__or__";}
    break;

  case 395:

/* Line 1455 of yacc.c  */
#line 3155 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {(yyval.text) = "__xor__";}
    break;

  case 396:

/* Line 1455 of yacc.c  */
#line 3156 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {(yyval.text) = "__lshift__";}
    break;

  case 397:

/* Line 1455 of yacc.c  */
#line 3157 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {(yyval.text) = "__rshift__";}
    break;

  case 398:

/* Line 1455 of yacc.c  */
#line 3158 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {(yyval.text) = "__iadd__";}
    break;

  case 399:

/* Line 1455 of yacc.c  */
#line 3159 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {(yyval.text) = "__isub__";}
    break;

  case 400:

/* Line 1455 of yacc.c  */
#line 3160 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {(yyval.text) = "__imul__";}
    break;

  case 401:

/* Line 1455 of yacc.c  */
#line 3161 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {(yyval.text) = "__idiv__";}
    break;

  case 402:

/* Line 1455 of yacc.c  */
#line 3162 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {(yyval.text) = "__imod__";}
    break;

  case 403:

/* Line 1455 of yacc.c  */
#line 3163 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {(yyval.text) = "__iand__";}
    break;

  case 404:

/* Line 1455 of yacc.c  */
#line 3164 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {(yyval.text) = "__ior__";}
    break;

  case 405:

/* Line 1455 of yacc.c  */
#line 3165 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {(yyval.text) = "__ixor__";}
    break;

  case 406:

/* Line 1455 of yacc.c  */
#line 3166 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {(yyval.text) = "__ilshift__";}
    break;

  case 407:

/* Line 1455 of yacc.c  */
#line 3167 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {(yyval.text) = "__irshift__";}
    break;

  case 408:

/* Line 1455 of yacc.c  */
#line 3168 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {(yyval.text) = "__invert__";}
    break;

  case 409:

/* Line 1455 of yacc.c  */
#line 3169 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {(yyval.text) = "__call__";}
    break;

  case 410:

/* Line 1455 of yacc.c  */
#line 3170 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {(yyval.text) = "__getitem__";}
    break;

  case 411:

/* Line 1455 of yacc.c  */
#line 3171 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {(yyval.text) = "__lt__";}
    break;

  case 412:

/* Line 1455 of yacc.c  */
#line 3172 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {(yyval.text) = "__le__";}
    break;

  case 413:

/* Line 1455 of yacc.c  */
#line 3173 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {(yyval.text) = "__eq__";}
    break;

  case 414:

/* Line 1455 of yacc.c  */
#line 3174 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {(yyval.text) = "__ne__";}
    break;

  case 415:

/* Line 1455 of yacc.c  */
#line 3175 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {(yyval.text) = "__gt__";}
    break;

  case 416:

/* Line 1455 of yacc.c  */
#line 3176 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {(yyval.text) = "__ge__";}
    break;

  case 417:

/* Line 1455 of yacc.c  */
#line 3179 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.number) = FALSE;
        }
    break;

  case 418:

/* Line 1455 of yacc.c  */
#line 3182 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.number) = TRUE;
        }
    break;

  case 419:

/* Line 1455 of yacc.c  */
#line 3187 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.number) = 0;
        }
    break;

  case 420:

/* Line 1455 of yacc.c  */
#line 3190 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if ((yyvsp[(2) - (2)].number) != 0)
                yyerror("Abstract virtual function '= 0' expected");

            (yyval.number) = TRUE;
        }
    break;

  case 421:

/* Line 1455 of yacc.c  */
#line 3198 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.optflags).nrFlags = 0;
        }
    break;

  case 422:

/* Line 1455 of yacc.c  */
#line 3201 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.optflags) = (yyvsp[(2) - (3)].optflags);
        }
    break;

  case 423:

/* Line 1455 of yacc.c  */
#line 3207 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.optflags).flags[0] = (yyvsp[(1) - (1)].flag);
            (yyval.optflags).nrFlags = 1;
        }
    break;

  case 424:

/* Line 1455 of yacc.c  */
#line 3211 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            /* Check there is room. */

            if ((yyvsp[(1) - (3)].optflags).nrFlags == MAX_NR_FLAGS)
                yyerror("Too many optional flags");

            (yyval.optflags) = (yyvsp[(1) - (3)].optflags);

            (yyval.optflags).flags[(yyval.optflags).nrFlags++] = (yyvsp[(3) - (3)].flag);
        }
    break;

  case 425:

/* Line 1455 of yacc.c  */
#line 3223 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.flag).ftype = bool_flag;
            (yyval.flag).fname = (yyvsp[(1) - (1)].text);
        }
    break;

  case 426:

/* Line 1455 of yacc.c  */
#line 3227 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.flag) = (yyvsp[(3) - (3)].flag);
            (yyval.flag).fname = (yyvsp[(1) - (3)].text);
        }
    break;

  case 427:

/* Line 1455 of yacc.c  */
#line 3233 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.flag).ftype = (strchr((yyvsp[(1) - (1)].text), '.') != NULL) ? dotted_name_flag : name_flag;
            (yyval.flag).fvalue.sval = (yyvsp[(1) - (1)].text);
        }
    break;

  case 428:

/* Line 1455 of yacc.c  */
#line 3237 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            apiVersionRangeDef *avd;
            int from, to;

            (yyval.flag).ftype = api_range_flag;

            /* Check that the API is known. */
            if ((avd = findAPI(currentSpec, (yyvsp[(1) - (5)].text))) == NULL)
                yyerror("unknown API name in API annotation");

            if (inMainModule())
                setIsUsedName(avd->api_name);

            /* Unbounded values are represented by 0. */
            if ((from = (yyvsp[(3) - (5)].number)) < 0)
                from = 0;

            if ((to = (yyvsp[(5) - (5)].number)) < 0)
                to = 0;

            (yyval.flag).fvalue.aval = convertAPIRange(currentModule, avd->api_name,
                    from, to);
        }
    break;

  case 429:

/* Line 1455 of yacc.c  */
#line 3260 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.flag).ftype = string_flag;
            (yyval.flag).fvalue.sval = convertFeaturedString((yyvsp[(1) - (1)].text));
        }
    break;

  case 430:

/* Line 1455 of yacc.c  */
#line 3264 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.flag).ftype = integer_flag;
            (yyval.flag).fvalue.ival = (yyvsp[(1) - (1)].number);
        }
    break;

  case 431:

/* Line 1455 of yacc.c  */
#line 3270 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.codeb) = (yyvsp[(2) - (2)].codeb);
        }
    break;

  case 432:

/* Line 1455 of yacc.c  */
#line 3275 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.codeb) = NULL;
        }
    break;

  case 434:

/* Line 1455 of yacc.c  */
#line 3281 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.codeb) = NULL;
        }
    break;

  case 435:

/* Line 1455 of yacc.c  */
#line 3284 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.codeb) = (yyvsp[(2) - (2)].codeb);
        }
    break;

  case 436:

/* Line 1455 of yacc.c  */
#line 3289 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.codeb) = NULL;
        }
    break;

  case 437:

/* Line 1455 of yacc.c  */
#line 3292 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.codeb) = (yyvsp[(2) - (2)].codeb);
        }
    break;

  case 438:

/* Line 1455 of yacc.c  */
#line 3297 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            int a, nrrxcon, nrrxdis, nrslotcon, nrslotdis, nrarray, nrarraysize;

            nrrxcon = nrrxdis = nrslotcon = nrslotdis = nrarray = nrarraysize = 0;

            for (a = 0; a < (yyvsp[(1) - (1)].signature).nrArgs; ++a)
            {
                argDef *ad = &(yyvsp[(1) - (1)].signature).args[a];

                switch (ad -> atype)
                {
                case rxcon_type:
                    ++nrrxcon;
                    break;

                case rxdis_type:
                    ++nrrxdis;
                    break;

                case slotcon_type:
                    ++nrslotcon;
                    break;

                case slotdis_type:
                    ++nrslotdis;
                    break;
                }

                if (isArray(ad))
                    ++nrarray;

                if (isArraySize(ad))
                    ++nrarraysize;
            }

            if (nrrxcon != nrslotcon || nrrxcon > 1)
                yyerror("SIP_RXOBJ_CON and SIP_SLOT_CON must both be given and at most once");

            if (nrrxdis != nrslotdis || nrrxdis > 1)
                yyerror("SIP_RXOBJ_DIS and SIP_SLOT_DIS must both be given and at most once");

            if (nrarray != nrarraysize || nrarray > 1)
                yyerror("/Array/ and /ArraySize/ must both be given and at most once");

            (yyval.signature) = (yyvsp[(1) - (1)].signature);
        }
    break;

  case 439:

/* Line 1455 of yacc.c  */
#line 3345 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            /* No arguments. */

            (yyval.signature).nrArgs = 0;
        }
    break;

  case 440:

/* Line 1455 of yacc.c  */
#line 3350 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            /* The single or first argument. */

            (yyval.signature).args[0] = (yyvsp[(1) - (1)].memArg);
            (yyval.signature).nrArgs = 1;
        }
    break;

  case 441:

/* Line 1455 of yacc.c  */
#line 3356 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            /* Check that it wasn't ...(,arg...). */
            if ((yyvsp[(1) - (3)].signature).nrArgs == 0)
                yyerror("First argument of the list is missing");

            /* Check there is nothing after an ellipsis. */
            if ((yyvsp[(1) - (3)].signature).args[(yyvsp[(1) - (3)].signature).nrArgs - 1].atype == ellipsis_type)
                yyerror("An ellipsis must be at the end of the argument list");

            /*
             * If this argument has no default value, then the
             * previous one mustn't either.
             */
            if ((yyvsp[(3) - (3)].memArg).defval == NULL && (yyvsp[(1) - (3)].signature).args[(yyvsp[(1) - (3)].signature).nrArgs - 1].defval != NULL)
                yyerror("Compulsory argument given after optional argument");

            /* Check there is room. */
            if ((yyvsp[(1) - (3)].signature).nrArgs == MAX_NR_ARGS)
                yyerror("Internal error - increase the value of MAX_NR_ARGS");

            (yyval.signature) = (yyvsp[(1) - (3)].signature);

            (yyval.signature).args[(yyval.signature).nrArgs] = (yyvsp[(3) - (3)].memArg);
            (yyval.signature).nrArgs++;
        }
    break;

  case 442:

/* Line 1455 of yacc.c  */
#line 3383 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            checkNoAnnos(&(yyvsp[(3) - (4)].optflags), "SIP_SIGNAL has no annotations");

            (yyval.memArg).atype = signal_type;
            (yyval.memArg).argflags = ARG_IS_CONST;
            (yyval.memArg).nrderefs = 0;
            (yyval.memArg).name = cacheName(currentSpec, (yyvsp[(2) - (4)].text));
            (yyval.memArg).defval = (yyvsp[(4) - (4)].valp);

            currentSpec -> sigslots = TRUE;
        }
    break;

  case 443:

/* Line 1455 of yacc.c  */
#line 3394 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            checkNoAnnos(&(yyvsp[(3) - (4)].optflags), "SIP_SLOT has no annotations");

            (yyval.memArg).atype = slot_type;
            (yyval.memArg).argflags = ARG_IS_CONST;
            (yyval.memArg).nrderefs = 0;
            (yyval.memArg).name = cacheName(currentSpec, (yyvsp[(2) - (4)].text));
            (yyval.memArg).defval = (yyvsp[(4) - (4)].valp);

            currentSpec -> sigslots = TRUE;
        }
    break;

  case 444:

/* Line 1455 of yacc.c  */
#line 3405 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            checkNoAnnos(&(yyvsp[(3) - (4)].optflags), "SIP_ANYSLOT has no annotations");

            (yyval.memArg).atype = anyslot_type;
            (yyval.memArg).argflags = ARG_IS_CONST;
            (yyval.memArg).nrderefs = 0;
            (yyval.memArg).name = cacheName(currentSpec, (yyvsp[(2) - (4)].text));
            (yyval.memArg).defval = (yyvsp[(4) - (4)].valp);

            currentSpec -> sigslots = TRUE;
        }
    break;

  case 445:

/* Line 1455 of yacc.c  */
#line 3416 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            const char *annos[] = {
                "SingleShot",
                NULL
            };

            checkAnnos(&(yyvsp[(3) - (3)].optflags), annos);

            (yyval.memArg).atype = rxcon_type;
            (yyval.memArg).argflags = 0;
            (yyval.memArg).nrderefs = 0;
            (yyval.memArg).name = cacheName(currentSpec, (yyvsp[(2) - (3)].text));

            if (getOptFlag(&(yyvsp[(3) - (3)].optflags), "SingleShot", bool_flag) != NULL)
                (yyval.memArg).argflags |= ARG_SINGLE_SHOT;

            currentSpec -> sigslots = TRUE;
        }
    break;

  case 446:

/* Line 1455 of yacc.c  */
#line 3434 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            checkNoAnnos(&(yyvsp[(3) - (3)].optflags), "SIP_RXOBJ_DIS has no annotations");

            (yyval.memArg).atype = rxdis_type;
            (yyval.memArg).argflags = 0;
            (yyval.memArg).nrderefs = 0;
            (yyval.memArg).name = cacheName(currentSpec, (yyvsp[(2) - (3)].text));

            currentSpec -> sigslots = TRUE;
        }
    break;

  case 447:

/* Line 1455 of yacc.c  */
#line 3444 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            checkNoAnnos(&(yyvsp[(6) - (6)].optflags), "SIP_SLOT_CON has no annotations");

            (yyval.memArg).atype = slotcon_type;
            (yyval.memArg).argflags = ARG_IS_CONST;
            (yyval.memArg).nrderefs = 0;
            (yyval.memArg).name = cacheName(currentSpec, (yyvsp[(5) - (6)].text));

            memset(&(yyvsp[(3) - (6)].signature).result, 0, sizeof (argDef));
            (yyvsp[(3) - (6)].signature).result.atype = void_type;

            (yyval.memArg).u.sa = sipMalloc(sizeof (signatureDef));
            *(yyval.memArg).u.sa = (yyvsp[(3) - (6)].signature);

            currentSpec -> sigslots = TRUE;
        }
    break;

  case 448:

/* Line 1455 of yacc.c  */
#line 3460 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            checkNoAnnos(&(yyvsp[(6) - (6)].optflags), "SIP_SLOT_DIS has no annotations");

            (yyval.memArg).atype = slotdis_type;
            (yyval.memArg).argflags = ARG_IS_CONST;
            (yyval.memArg).nrderefs = 0;
            (yyval.memArg).name = cacheName(currentSpec, (yyvsp[(5) - (6)].text));

            memset(&(yyvsp[(3) - (6)].signature).result, 0, sizeof (argDef));
            (yyvsp[(3) - (6)].signature).result.atype = void_type;

            (yyval.memArg).u.sa = sipMalloc(sizeof (signatureDef));
            *(yyval.memArg).u.sa = (yyvsp[(3) - (6)].signature);

            currentSpec -> sigslots = TRUE;
        }
    break;

  case 449:

/* Line 1455 of yacc.c  */
#line 3476 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            checkNoAnnos(&(yyvsp[(3) - (3)].optflags), "SIP_QOBJECT has no annotations");

            (yyval.memArg).atype = qobject_type;
            (yyval.memArg).argflags = 0;
            (yyval.memArg).nrderefs = 0;
            (yyval.memArg).name = cacheName(currentSpec, (yyvsp[(2) - (3)].text));
        }
    break;

  case 450:

/* Line 1455 of yacc.c  */
#line 3484 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.memArg) = (yyvsp[(1) - (2)].memArg);
            (yyval.memArg).defval = (yyvsp[(2) - (2)].valp);
        }
    break;

  case 451:

/* Line 1455 of yacc.c  */
#line 3491 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {currentIsSignal = TRUE;}
    break;

  case 453:

/* Line 1455 of yacc.c  */
#line 3492 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {currentIsSlot = TRUE;}
    break;

  case 456:

/* Line 1455 of yacc.c  */
#line 3497 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {currentIsStatic = TRUE;}
    break;

  case 461:

/* Line 1455 of yacc.c  */
#line 3507 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {currentOverIsVirt = TRUE;}
    break;

  case 464:

/* Line 1455 of yacc.c  */
#line 3511 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
            {
                const char *annos[] = {
                    "DocType",
                    "Encoding",
                    "PyInt",
                    "PyName",
                    NULL
                };

                checkAnnos(&(yyvsp[(3) - (8)].optflags), annos);

                if ((yyvsp[(6) - (8)].codeb) != NULL)
                {
                    if ((yyvsp[(4) - (8)].variable).access_code != NULL)
                        yyerror("%AccessCode already defined");

                    (yyvsp[(4) - (8)].variable).access_code = (yyvsp[(6) - (8)].codeb);

                    deprecated("%AccessCode should be used a sub-directive");
                }

                if ((yyvsp[(7) - (8)].codeb) != NULL)
                {
                    if ((yyvsp[(4) - (8)].variable).get_code != NULL)
                        yyerror("%GetCode already defined");

                    (yyvsp[(4) - (8)].variable).get_code = (yyvsp[(7) - (8)].codeb);

                    deprecated("%GetCode should be used a sub-directive");
                }

                if ((yyvsp[(8) - (8)].codeb) != NULL)
                {
                    if ((yyvsp[(4) - (8)].variable).set_code != NULL)
                        yyerror("%SetCode already defined");

                    (yyvsp[(4) - (8)].variable).set_code = (yyvsp[(8) - (8)].codeb);

                    deprecated("%SetCode should be used a sub-directive");
                }

                newVar(currentSpec, currentModule, (yyvsp[(2) - (8)].text), currentIsStatic, &(yyvsp[(1) - (8)].memArg),
                        &(yyvsp[(3) - (8)].optflags), (yyvsp[(4) - (8)].variable).access_code, (yyvsp[(4) - (8)].variable).get_code, (yyvsp[(4) - (8)].variable).set_code,
                        sectionFlags);
            }

            currentIsStatic = FALSE;
        }
    break;

  case 465:

/* Line 1455 of yacc.c  */
#line 3563 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.variable).token = 0;
            (yyval.variable).access_code = NULL;
            (yyval.variable).get_code = NULL;
            (yyval.variable).set_code = NULL;
        }
    break;

  case 466:

/* Line 1455 of yacc.c  */
#line 3569 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.variable) = (yyvsp[(2) - (3)].variable);
        }
    break;

  case 468:

/* Line 1455 of yacc.c  */
#line 3575 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.variable) = (yyvsp[(1) - (2)].variable);

            switch ((yyvsp[(2) - (2)].variable).token)
            {
            case TK_ACCESSCODE: (yyval.variable).access_code = (yyvsp[(2) - (2)].variable).access_code; break;
            case TK_GETCODE: (yyval.variable).get_code = (yyvsp[(2) - (2)].variable).get_code; break;
            case TK_SETCODE: (yyval.variable).set_code = (yyvsp[(2) - (2)].variable).set_code; break;
            }
        }
    break;

  case 469:

/* Line 1455 of yacc.c  */
#line 3587 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.variable).token = TK_IF;
        }
    break;

  case 470:

/* Line 1455 of yacc.c  */
#line 3590 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.variable).token = TK_END;
        }
    break;

  case 471:

/* Line 1455 of yacc.c  */
#line 3593 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
            {
                (yyval.variable).token = TK_ACCESSCODE;
                (yyval.variable).access_code = (yyvsp[(2) - (2)].codeb);
            }
            else
            {
                (yyval.variable).token = 0;
                (yyval.variable).access_code = NULL;
                (yyval.variable).get_code = NULL;
                (yyval.variable).set_code = NULL;
            }
        }
    break;

  case 472:

/* Line 1455 of yacc.c  */
#line 3607 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
            {
                (yyval.variable).token = TK_GETCODE;
                (yyval.variable).get_code = (yyvsp[(2) - (2)].codeb);
            }
            else
            {
                (yyval.variable).token = 0;
                (yyval.variable).access_code = NULL;
                (yyval.variable).get_code = NULL;
                (yyval.variable).set_code = NULL;
            }
        }
    break;

  case 473:

/* Line 1455 of yacc.c  */
#line 3621 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (notSkipping())
            {
                (yyval.variable).token = TK_SETCODE;
                (yyval.variable).set_code = (yyvsp[(2) - (2)].codeb);
            }
            else
            {
                (yyval.variable).token = 0;
                (yyval.variable).access_code = NULL;
                (yyval.variable).get_code = NULL;
                (yyval.variable).set_code = NULL;
            }
        }
    break;

  case 474:

/* Line 1455 of yacc.c  */
#line 3637 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.memArg) = (yyvsp[(2) - (4)].memArg);
            (yyval.memArg).nrderefs += (yyvsp[(3) - (4)].number);
            (yyval.memArg).argflags |= ARG_IS_CONST | (yyvsp[(4) - (4)].number);
        }
    break;

  case 475:

/* Line 1455 of yacc.c  */
#line 3642 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.memArg) = (yyvsp[(1) - (3)].memArg);
            (yyval.memArg).nrderefs += (yyvsp[(2) - (3)].number);
            (yyval.memArg).argflags |= (yyvsp[(3) - (3)].number);

            /* PyObject * is a synonym for SIP_PYOBJECT. */
            if ((yyvsp[(1) - (3)].memArg).atype == defined_type && strcmp((yyvsp[(1) - (3)].memArg).u.snd->name, "PyObject") == 0 && (yyvsp[(1) - (3)].memArg).u.snd->next == NULL && (yyvsp[(2) - (3)].number) == 1 && (yyvsp[(3) - (3)].number) == 0)
            {
                (yyval.memArg).atype = pyobject_type;
                (yyval.memArg).nrderefs = 0;
            }
        }
    break;

  case 476:

/* Line 1455 of yacc.c  */
#line 3656 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            const char *annos[] = {
                "AllowNone",
                "Array",
                "ArraySize",
                "Constrained",
                "DocType",
                "DocValue",
                "Encoding",
                "GetWrapper",
                "In",
                "KeepReference",
                "NoCopy",
                "Out",
                "PyInt",
                "ResultSize",
                "Transfer",
                "TransferBack",
                "TransferThis",
                NULL
            };

            optFlag *of;

            checkAnnos(&(yyvsp[(3) - (3)].optflags), annos);

            (yyval.memArg) = (yyvsp[(1) - (3)].memArg);
            (yyval.memArg).name = cacheName(currentSpec, (yyvsp[(2) - (3)].text));

            if (getAllowNone(&(yyvsp[(3) - (3)].optflags)))
                (yyval.memArg).argflags |= ARG_ALLOW_NONE;

            if (getOptFlag(&(yyvsp[(3) - (3)].optflags),"GetWrapper",bool_flag) != NULL)
                (yyval.memArg).argflags |= ARG_GET_WRAPPER;

            if (getOptFlag(&(yyvsp[(3) - (3)].optflags),"Array",bool_flag) != NULL)
                (yyval.memArg).argflags |= ARG_ARRAY;

            if (getOptFlag(&(yyvsp[(3) - (3)].optflags),"ArraySize",bool_flag) != NULL)
                (yyval.memArg).argflags |= ARG_ARRAY_SIZE;

            if (getTransfer(&(yyvsp[(3) - (3)].optflags)))
                (yyval.memArg).argflags |= ARG_XFERRED;

            if (getOptFlag(&(yyvsp[(3) - (3)].optflags),"TransferThis",bool_flag) != NULL)
                (yyval.memArg).argflags |= ARG_THIS_XFERRED;

            if (getOptFlag(&(yyvsp[(3) - (3)].optflags),"TransferBack",bool_flag) != NULL)
                (yyval.memArg).argflags |= ARG_XFERRED_BACK;

            if ((of = getOptFlag(&(yyvsp[(3) - (3)].optflags), "KeepReference", opt_integer_flag)) != NULL)
            {
                (yyval.memArg).argflags |= ARG_KEEP_REF;

                if (((yyval.memArg).key = of->fvalue.ival) < -1)
                    yyerror("/KeepReference/ key cannot be negative");

                /* If there was no explicit key then auto-allocate one. */
                if ((yyval.memArg).key == -1)
                    (yyval.memArg).key = currentModule->next_key--;
            }

            if (getOptFlag(&(yyvsp[(3) - (3)].optflags),"In",bool_flag) != NULL)
                (yyval.memArg).argflags |= ARG_IN;

            if (getOptFlag(&(yyvsp[(3) - (3)].optflags),"Out",bool_flag) != NULL)
                (yyval.memArg).argflags |= ARG_OUT;

            if (getOptFlag(&(yyvsp[(3) - (3)].optflags), "ResultSize", bool_flag) != NULL)
                (yyval.memArg).argflags |= ARG_RESULT_SIZE;

            if (getOptFlag(&(yyvsp[(3) - (3)].optflags), "NoCopy", bool_flag) != NULL)
                (yyval.memArg).argflags |= ARG_NO_COPY;

            if (getOptFlag(&(yyvsp[(3) - (3)].optflags),"Constrained",bool_flag) != NULL)
            {
                (yyval.memArg).argflags |= ARG_CONSTRAINED;

                switch ((yyval.memArg).atype)
                {
                case bool_type:
                    (yyval.memArg).atype = cbool_type;
                    break;

                case int_type:
                    (yyval.memArg).atype = cint_type;
                    break;

                case float_type:
                    (yyval.memArg).atype = cfloat_type;
                    break;

                case double_type:
                    (yyval.memArg).atype = cdouble_type;
                    break;
                }
            }

            applyTypeFlags(currentModule, &(yyval.memArg), &(yyvsp[(3) - (3)].optflags));
            (yyval.memArg).docval = getDocValue(&(yyvsp[(3) - (3)].optflags));
        }
    break;

  case 477:

/* Line 1455 of yacc.c  */
#line 3759 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.number) = 0;
        }
    break;

  case 478:

/* Line 1455 of yacc.c  */
#line 3762 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (currentSpec -> genc)
                yyerror("References not allowed in a C module");

            (yyval.number) = ARG_IS_REF;
        }
    break;

  case 479:

/* Line 1455 of yacc.c  */
#line 3770 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.number) = 0;
        }
    break;

  case 480:

/* Line 1455 of yacc.c  */
#line 3773 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.number) = (yyvsp[(1) - (2)].number) + 1;
        }
    break;

  case 481:

/* Line 1455 of yacc.c  */
#line 3778 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            memset(&(yyval.memArg), 0, sizeof (argDef));
            (yyval.memArg).atype = defined_type;
            (yyval.memArg).u.snd = (yyvsp[(1) - (1)].scpvalp);

            /* Try and resolve typedefs as early as possible. */
            resolveAnyTypedef(currentSpec, &(yyval.memArg));
        }
    break;

  case 482:

/* Line 1455 of yacc.c  */
#line 3786 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            templateDef *td;

            td = sipMalloc(sizeof(templateDef));
            td->fqname = (yyvsp[(1) - (4)].scpvalp);
            td->types = (yyvsp[(3) - (4)].signature);

            memset(&(yyval.memArg), 0, sizeof (argDef));
            (yyval.memArg).atype = template_type;
            (yyval.memArg).u.td = td;
        }
    break;

  case 483:

/* Line 1455 of yacc.c  */
#line 3797 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            memset(&(yyval.memArg), 0, sizeof (argDef));

            /* In a C module all structures must be defined. */
            if (currentSpec -> genc)
            {
                (yyval.memArg).atype = defined_type;
                (yyval.memArg).u.snd = (yyvsp[(2) - (2)].scpvalp);
            }
            else
            {
                (yyval.memArg).atype = struct_type;
                (yyval.memArg).u.sname = (yyvsp[(2) - (2)].scpvalp);
            }
        }
    break;

  case 484:

/* Line 1455 of yacc.c  */
#line 3812 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            memset(&(yyval.memArg), 0, sizeof (argDef));
            (yyval.memArg).atype = ushort_type;
        }
    break;

  case 485:

/* Line 1455 of yacc.c  */
#line 3816 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            memset(&(yyval.memArg), 0, sizeof (argDef));
            (yyval.memArg).atype = short_type;
        }
    break;

  case 486:

/* Line 1455 of yacc.c  */
#line 3820 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            memset(&(yyval.memArg), 0, sizeof (argDef));
            (yyval.memArg).atype = uint_type;
        }
    break;

  case 487:

/* Line 1455 of yacc.c  */
#line 3824 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            memset(&(yyval.memArg), 0, sizeof (argDef));
            (yyval.memArg).atype = uint_type;
        }
    break;

  case 488:

/* Line 1455 of yacc.c  */
#line 3828 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            memset(&(yyval.memArg), 0, sizeof (argDef));
            (yyval.memArg).atype = int_type;
        }
    break;

  case 489:

/* Line 1455 of yacc.c  */
#line 3832 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            memset(&(yyval.memArg), 0, sizeof (argDef));
            (yyval.memArg).atype = long_type;
        }
    break;

  case 490:

/* Line 1455 of yacc.c  */
#line 3836 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            memset(&(yyval.memArg), 0, sizeof (argDef));
            (yyval.memArg).atype = ulong_type;
        }
    break;

  case 491:

/* Line 1455 of yacc.c  */
#line 3840 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            memset(&(yyval.memArg), 0, sizeof (argDef));
            (yyval.memArg).atype = longlong_type;
        }
    break;

  case 492:

/* Line 1455 of yacc.c  */
#line 3844 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            memset(&(yyval.memArg), 0, sizeof (argDef));
            (yyval.memArg).atype = ulonglong_type;
        }
    break;

  case 493:

/* Line 1455 of yacc.c  */
#line 3848 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            memset(&(yyval.memArg), 0, sizeof (argDef));
            (yyval.memArg).atype = float_type;
        }
    break;

  case 494:

/* Line 1455 of yacc.c  */
#line 3852 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            memset(&(yyval.memArg), 0, sizeof (argDef));
            (yyval.memArg).atype = double_type;
        }
    break;

  case 495:

/* Line 1455 of yacc.c  */
#line 3856 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            memset(&(yyval.memArg), 0, sizeof (argDef));
            (yyval.memArg).atype = bool_type;
        }
    break;

  case 496:

/* Line 1455 of yacc.c  */
#line 3860 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            memset(&(yyval.memArg), 0, sizeof (argDef));
            (yyval.memArg).atype = sstring_type;
        }
    break;

  case 497:

/* Line 1455 of yacc.c  */
#line 3864 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            memset(&(yyval.memArg), 0, sizeof (argDef));
            (yyval.memArg).atype = ustring_type;
        }
    break;

  case 498:

/* Line 1455 of yacc.c  */
#line 3868 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            memset(&(yyval.memArg), 0, sizeof (argDef));
            (yyval.memArg).atype = string_type;
        }
    break;

  case 499:

/* Line 1455 of yacc.c  */
#line 3872 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            memset(&(yyval.memArg), 0, sizeof (argDef));
            (yyval.memArg).atype = wstring_type;
        }
    break;

  case 500:

/* Line 1455 of yacc.c  */
#line 3876 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            memset(&(yyval.memArg), 0, sizeof (argDef));
            (yyval.memArg).atype = void_type;
        }
    break;

  case 501:

/* Line 1455 of yacc.c  */
#line 3880 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            memset(&(yyval.memArg), 0, sizeof (argDef));
            (yyval.memArg).atype = pyobject_type;
        }
    break;

  case 502:

/* Line 1455 of yacc.c  */
#line 3884 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            memset(&(yyval.memArg), 0, sizeof (argDef));
            (yyval.memArg).atype = pytuple_type;
        }
    break;

  case 503:

/* Line 1455 of yacc.c  */
#line 3888 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            memset(&(yyval.memArg), 0, sizeof (argDef));
            (yyval.memArg).atype = pylist_type;
        }
    break;

  case 504:

/* Line 1455 of yacc.c  */
#line 3892 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            memset(&(yyval.memArg), 0, sizeof (argDef));
            (yyval.memArg).atype = pydict_type;
        }
    break;

  case 505:

/* Line 1455 of yacc.c  */
#line 3896 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            memset(&(yyval.memArg), 0, sizeof (argDef));
            (yyval.memArg).atype = pycallable_type;
        }
    break;

  case 506:

/* Line 1455 of yacc.c  */
#line 3900 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            memset(&(yyval.memArg), 0, sizeof (argDef));
            (yyval.memArg).atype = pyslice_type;
        }
    break;

  case 507:

/* Line 1455 of yacc.c  */
#line 3904 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            memset(&(yyval.memArg), 0, sizeof (argDef));
            (yyval.memArg).atype = pytype_type;
        }
    break;

  case 508:

/* Line 1455 of yacc.c  */
#line 3908 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            memset(&(yyval.memArg), 0, sizeof (argDef));
            (yyval.memArg).atype = ssize_type;
        }
    break;

  case 509:

/* Line 1455 of yacc.c  */
#line 3912 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            memset(&(yyval.memArg), 0, sizeof (argDef));
            (yyval.memArg).atype = ellipsis_type;
        }
    break;

  case 510:

/* Line 1455 of yacc.c  */
#line 3918 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            /* The single or first type. */

            (yyval.signature).args[0] = (yyvsp[(1) - (1)].memArg);
            (yyval.signature).nrArgs = 1;
        }
    break;

  case 511:

/* Line 1455 of yacc.c  */
#line 3924 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            /* Check there is nothing after an ellipsis. */
            if ((yyvsp[(1) - (3)].signature).args[(yyvsp[(1) - (3)].signature).nrArgs - 1].atype == ellipsis_type)
                yyerror("An ellipsis must be at the end of the argument list");

            /* Check there is room. */
            if ((yyvsp[(1) - (3)].signature).nrArgs == MAX_NR_ARGS)
                yyerror("Internal error - increase the value of MAX_NR_ARGS");

            (yyval.signature) = (yyvsp[(1) - (3)].signature);

            (yyval.signature).args[(yyval.signature).nrArgs] = (yyvsp[(3) - (3)].memArg);
            (yyval.signature).nrArgs++;
        }
    break;

  case 512:

/* Line 1455 of yacc.c  */
#line 3940 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            (yyval.throwlist) = NULL;
        }
    break;

  case 513:

/* Line 1455 of yacc.c  */
#line 3943 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            if (currentSpec->genc)
                yyerror("Exceptions not allowed in a C module");

            (yyval.throwlist) = (yyvsp[(3) - (4)].throwlist);
        }
    break;

  case 514:

/* Line 1455 of yacc.c  */
#line 3951 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            /* Empty list so use a blank. */

            (yyval.throwlist) = sipMalloc(sizeof (throwArgs));
            (yyval.throwlist) -> nrArgs = 0;
        }
    break;

  case 515:

/* Line 1455 of yacc.c  */
#line 3957 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            /* The only or first exception. */

            (yyval.throwlist) = sipMalloc(sizeof (throwArgs));
            (yyval.throwlist) -> nrArgs = 1;
            (yyval.throwlist) -> args[0] = findException(currentSpec, (yyvsp[(1) - (1)].scpvalp), FALSE);
        }
    break;

  case 516:

/* Line 1455 of yacc.c  */
#line 3964 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"
    {
            /* Check that it wasn't ...(,arg...). */

            if ((yyvsp[(1) - (3)].throwlist) -> nrArgs == 0)
                yyerror("First exception of throw specifier is missing");

            /* Check there is room. */

            if ((yyvsp[(1) - (3)].throwlist) -> nrArgs == MAX_NR_ARGS)
                yyerror("Internal error - increase the value of MAX_NR_ARGS");

            (yyval.throwlist) = (yyvsp[(1) - (3)].throwlist);
            (yyval.throwlist) -> args[(yyval.throwlist) -> nrArgs++] = findException(currentSpec, (yyvsp[(3) - (3)].scpvalp), FALSE);
        }
    break;



/* Line 1455 of yacc.c  */
#line 8112 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1675 of yacc.c  */
#line 3980 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.y"



/*
 * Parse the specification.
 */
void parse(sipSpec *spec, FILE *fp, char *filename, stringList *tsl,
        stringList *xfl, KwArgs kwArgs, int protHack)
{
    classTmplDef *tcd;

    /* Initialise the spec. */
 
    spec->modules = NULL;
    spec->namecache = NULL;
    spec->ifacefiles = NULL;
    spec->classes = NULL;
    spec->classtemplates = NULL;
    spec->exceptions = NULL;
    spec->mappedtypes = NULL;
    spec->mappedtypetemplates = NULL;
    spec->enums = NULL;
    spec->vars = NULL;
    spec->typedefs = NULL;
    spec->exphdrcode = NULL;
    spec->docs = NULL;
    spec->sigslots = FALSE;
    spec->genc = -1;
    spec->plugins = NULL;
    spec->extracts = NULL;

    currentSpec = spec;
    neededQualifiers = tsl;
    excludedQualifiers = xfl;
    currentModule = NULL;
    currentMappedType = NULL;
    currentOverIsVirt = FALSE;
    currentCtorIsExplicit = FALSE;
    currentIsStatic = FALSE;
    currentIsSignal = FALSE;
    currentIsSlot = FALSE;
    currentIsTemplate = FALSE;
    previousFile = NULL;
    skipStackPtr = 0;
    currentScopeIdx = 0;
    sectionFlags = 0;
    defaultKwArgs = kwArgs;
    makeProtPublic = protHack;

    newModule(fp, filename);
    spec->module = currentModule;

    yyparse();

    handleEOF();
    handleEOM();

    /*
     * Go through each template class and remove it from the list of classes.
     */
    for (tcd = spec->classtemplates; tcd != NULL; tcd = tcd->next)
    {
        classDef **cdp;

        for (cdp = &spec->classes; *cdp != NULL; cdp = &(*cdp)->next)
            if (*cdp == tcd->cd)
            {
                ifaceFileDef **ifdp;

                /* Remove the interface file as well. */
                for (ifdp = &spec->ifacefiles; *ifdp != NULL; ifdp = &(*ifdp)->next)
                    if (*ifdp == tcd->cd->iff)
                    {
                        *ifdp = (*ifdp)->next;
                        break;
                    }

                *cdp = (*cdp)->next;
                break;
            }
    }
}


/*
 * Tell the parser that a complete file has now been read.
 */
void parserEOF(char *name, parserContext *pc)
{
    previousFile = sipStrdup(name);
    currentContext = *pc;
}


/*
 * Append a class definition to a class list if it doesn't already appear.
 * Append is needed specifically for the list of super-classes because the
 * order is important to Python.
 */
void appendToClassList(classList **clp,classDef *cd)
{
    classList *new;

    /* Find the end of the list. */

    while (*clp != NULL)
    {
        if ((*clp) -> cd == cd)
            return;

        clp = &(*clp) -> next;
    }

    new = sipMalloc(sizeof (classList));

    new -> cd = cd;
    new -> next = NULL;

    *clp = new;
}


/*
 * Create a new module for the current specification and make it current.
 */
static void newModule(FILE *fp, char *filename)
{
    moduleDef *mod;

    parseFile(fp, filename, currentModule, FALSE);

    mod = allocModule();
    mod->file = filename;

    if (currentModule != NULL)
        mod->defexception = currentModule->defexception;

    currentModule = mod;
}


/*
 * Allocate and initialise the memory for a new module.
 */
static moduleDef *allocModule()
{
    moduleDef *newmod, **tailp;

    newmod = sipMalloc(sizeof (moduleDef));

    newmod->version = -1;
    newmod->encoding = no_type;
    newmod->qobjclass = -1;
    newmod->nrvirthandlers = -1;
    newmod->next_key = -2;

    /*
     * The consolidated module support needs these to be in order that they
     * appeared.
     */
    for (tailp = &currentSpec->modules; *tailp != NULL; tailp = &(*tailp)->next)
        ;

    *tailp = newmod;

    return newmod;
}


/*
 * Switch to parsing a new file.
 */
static void parseFile(FILE *fp, char *name, moduleDef *prevmod, int optional)
{
    parserContext pc;

    pc.filename = name;
    pc.ifdepth = skipStackPtr;
    pc.prevmod = prevmod;

    if (setInputFile(fp, &pc, optional))
        currentContext = pc;
}


/*
 * Find an interface file, or create a new one.
 */
ifaceFileDef *findIfaceFile(sipSpec *pt, moduleDef *mod, scopedNameDef *fqname,
        ifaceFileType iftype, apiVersionRangeDef *api_range, argDef *ad)
{
    ifaceFileDef *iff, *first_alt = NULL;

    /* See if the name is already used. */

    for (iff = pt->ifacefiles; iff != NULL; iff = iff->next)
    {
        if (compareScopedNames(iff->fqcname, fqname) != 0)
            continue;

        /*
         * If they are both versioned then assume the user knows what they are
         * doing.
         */
        if (iff->api_range != NULL && api_range != NULL && iff->module == mod)
        {
            /* Remember the first of the alternate APIs. */
            if ((first_alt = iff->first_alt) == NULL)
                first_alt = iff;

            break;
        }

        /*
         * They must be the same type except that we allow a class if we want
         * an exception.  This is because we allow classes to be used before
         * they are defined.
         */
        if (iff->type != iftype)
            if (iftype != exception_iface || iff->type != class_iface)
                yyerror("A class, exception, namespace or mapped type has already been defined with the same name");

        /* Ignore an external class declared in another module. */
        if (iftype == class_iface && iff->module != mod)
        {
            classDef *cd;

            for (cd = pt->classes; cd != NULL; cd = cd->next)
                if (cd->iff == iff)
                    break;

            if (cd != NULL && iff->module != NULL && isExternal(cd))
                continue;
        }

        /*
         * If this is a mapped type with the same name defined in a different
         * module, then check that this type isn't the same as any of the
         * mapped types defined in that module.
         */
        if (iftype == mappedtype_iface && iff->module != mod)
        {
            mappedTypeDef *mtd;

            /*
             * This is a bit of a cheat.  With consolidated modules it's
             * possible to have two implementations of a mapped type in
             * different branches of the module hierarchy.  We assume that, if
             * there really are multiple implementations in the same branch,
             * then it will be picked up in a non-consolidated build.
             */
            if (isConsolidated(pt->module))
                continue;

            for (mtd = pt->mappedtypes; mtd != NULL; mtd = mtd->next)
            {
                if (mtd->iff != iff)
                    continue;

                if (ad->atype != template_type ||
                    mtd->type.atype != template_type ||
                    sameBaseType(ad, &mtd->type))
                    yyerror("Mapped type has already been defined in another module");
            }

            /*
             * If we got here then we have a mapped type based on an existing
             * template, but with unique parameters.  We don't want to use
             * interface files from other modules, so skip this one.
             */

            continue;
        }

        /* Ignore a namespace defined in another module. */
        if (iftype == namespace_iface && iff->module != mod)
            continue;

        return iff;
    }

    iff = sipMalloc(sizeof (ifaceFileDef));

    iff->name = cacheName(pt, scopedNameToString(fqname));
    iff->api_range = api_range;

    if (first_alt != NULL)
    {
        iff->first_alt = first_alt;
        iff->next_alt = first_alt->next_alt;

        first_alt->next_alt = iff;
    }
    else
    {
        /* This is the first alternate so point to itself. */
        iff->first_alt = iff;
    }

    iff->type = iftype;
    iff->ifacenr = -1;
    iff->fqcname = fqname;
    iff->module = NULL;
    iff->hdrcode = NULL;
    iff->used = NULL;
    iff->next = pt->ifacefiles;

    pt->ifacefiles = iff;

    return iff;
}


/*
 * Find a class definition in a parse tree.
 */
static classDef *findClass(sipSpec *pt, ifaceFileType iftype,
        apiVersionRangeDef *api_range, scopedNameDef *fqname)
{
    return findClassWithInterface(pt, findIfaceFile(pt, currentModule, fqname, iftype, api_range, NULL));
}


/*
 * Find a class definition given an existing interface file.
 */
static classDef *findClassWithInterface(sipSpec *pt, ifaceFileDef *iff)
{
    classDef *cd;

    for (cd = pt -> classes; cd != NULL; cd = cd -> next)
        if (cd -> iff == iff)
            return cd;

    /* Create a new one. */
    cd = sipMalloc(sizeof (classDef));

    cd->iff = iff;
    cd->pyname = cacheName(pt, classBaseName(cd));
    cd->next = pt->classes;

    pt->classes = cd;

    return cd;
}


/*
 * Add an interface file to an interface file list if it isn't already there.
 */
void addToUsedList(ifaceFileList **ifflp, ifaceFileDef *iff)
{
    /* Make sure we don't try to add an interface file to its own list. */
    if (&iff->used != ifflp)
    {
        ifaceFileList *iffl;

        while ((iffl = *ifflp) != NULL)
        {
            /* Don't bother if it is already there. */
            if (iffl->iff == iff)
                return;

            ifflp = &iffl -> next;
        }

        iffl = sipMalloc(sizeof (ifaceFileList));

        iffl->iff = iff;
        iffl->next = NULL;

        *ifflp = iffl;
    }
}


/*
 * Find an undefined (or create a new) exception definition in a parse tree.
 */
static exceptionDef *findException(sipSpec *pt, scopedNameDef *fqname, int new)
{
    exceptionDef *xd, **tail;
    ifaceFileDef *iff;
    classDef *cd;

    iff = findIfaceFile(pt, currentModule, fqname, exception_iface, NULL, NULL);

    /* See if it is an existing one. */
    for (xd = pt->exceptions; xd != NULL; xd = xd->next)
        if (xd->iff == iff)
            return xd;

    /*
     * If it is an exception interface file then we have never seen this
     * name before.  We require that exceptions are defined before being
     * used, but don't make the same requirement of classes (for reasons of
     * backwards compatibility).  Therefore the name must be reinterpreted
     * as a (as yet undefined) class.
     */
    if (new)
    {
        if (iff->type == exception_iface)
            cd = NULL;
        else
            yyerror("There is already a class with the same name or the exception has been used before being defined");
    }
    else
    {
        if (iff->type == exception_iface)
            iff->type = class_iface;

        cd = findClassWithInterface(pt, iff);
    }

    /* Create a new one. */
    xd = sipMalloc(sizeof (exceptionDef));

    xd->exceptionnr = -1;
    xd->iff = iff;
    xd->pyname = NULL;
    xd->cd = cd;
    xd->bibase = NULL;
    xd->base = NULL;
    xd->raisecode = NULL;
    xd->next = NULL;

    /* Append it to the list. */
    for (tail = &pt->exceptions; *tail != NULL; tail = &(*tail)->next)
        ;

    *tail = xd;

    return xd;
}


/*
 * Find an undefined (or create a new) class definition in a parse tree.
 */
static classDef *newClass(sipSpec *pt, ifaceFileType iftype,
        apiVersionRangeDef *api_range, scopedNameDef *fqname)
{
    int flags;
    classDef *cd, *scope;
    codeBlock *hdrcode;

    if (sectionFlags & SECT_IS_PRIVATE)
        yyerror("Classes, structs and namespaces must be in the public or protected sections");

    flags = 0;

    if ((scope = currentScope()) != NULL)
    {
        if (sectionFlags & SECT_IS_PROT && !makeProtPublic)
        {
            flags = CLASS_IS_PROTECTED;

            if (scope->iff->type == class_iface)
                setHasShadow(scope);
        }

        /* Header code from outer scopes is also included. */
        hdrcode = scope->iff->hdrcode;
    }
    else
        hdrcode = NULL;

    if (pt -> genc)
    {
        /* C structs are always global types. */
        while (fqname -> next != NULL)
            fqname = fqname -> next;

        scope = NULL;
    }

    cd = findClass(pt, iftype, api_range, fqname);

    /* Check it hasn't already been defined. */
    if (iftype != namespace_iface && cd->iff->module != NULL)
        yyerror("The struct/class has already been defined");

    /* Complete the initialisation. */
    cd->classflags |= flags;
    cd->ecd = scope;
    cd->iff->module = currentModule;

    if (currentIsTemplate)
        setIsTemplateClass(cd);

    appendCodeBlock(&cd->iff->hdrcode, hdrcode);

    /* See if it is a namespace extender. */
    if (iftype == namespace_iface)
    {
        classDef *ns;

        for (ns = pt->classes; ns != NULL; ns = ns->next)
        {
            if (ns == cd)
                continue;

            if (ns->iff->type != namespace_iface)
                continue;

            if (compareScopedNames(ns->iff->fqcname, fqname) != 0)
                continue;

            cd->real = ns;
            break;
        }
    }

    return cd;
}


/*
 * Tidy up after finishing a class definition.
 */
static void finishClass(sipSpec *pt, moduleDef *mod, classDef *cd,
        optFlags *of)
{
    const char *pyname;
    optFlag *flg;

    /* Get the Python name and see if it is different to the C++ name. */
    pyname = getPythonName(mod, of, classBaseName(cd));

    cd->pyname = NULL;
    checkAttributes(pt, mod, cd->ecd, NULL, pyname, FALSE);
    cd->pyname = cacheName(pt, pyname);

    if ((flg = getOptFlag(of, "Metatype", dotted_name_flag)) != NULL)
        cd->metatype = cacheName(pt, flg->fvalue.sval);

    if ((flg = getOptFlag(of, "Supertype", dotted_name_flag)) != NULL)
        cd->supertype = cacheName(pt, flg->fvalue.sval);

    if ((flg = getOptFlag(of, "PyQt4Flags", integer_flag)) != NULL)
        cd->pyqt4_flags = flg->fvalue.ival;

    if (getOptFlag(of, "PyQt4NoQMetaObject", bool_flag) != NULL)
        setPyQt4NoQMetaObject(cd);

    if (isOpaque(cd))
    {
        if (getOptFlag(of, "External", bool_flag) != NULL)
            setIsExternal(cd);
    }
    else
    {
        int seq_might, seq_not;
        memberDef *md;

        if (getOptFlag(of, "NoDefaultCtors", bool_flag) != NULL)
            setNoDefaultCtors(cd);

        if (cd -> ctors == NULL)
        {
            if (!noDefaultCtors(cd))
            {
                /* Provide a default ctor. */

                cd->ctors = sipMalloc(sizeof (ctorDef));
 
                cd->ctors->ctorflags = SECT_IS_PUBLIC;
                cd->ctors->pysig.result.atype = void_type;
                cd->ctors->cppsig = &cd->ctors->pysig;

                cd->defctor = cd->ctors;

                setCanCreate(cd);
            }
        }
        else if (cd -> defctor == NULL)
        {
            ctorDef *ct, *last = NULL;

            for (ct = cd -> ctors; ct != NULL; ct = ct -> next)
            {
                if (!isPublicCtor(ct))
                    continue;

                if (ct -> pysig.nrArgs == 0 || ct -> pysig.args[0].defval != NULL)
                {
                    cd -> defctor = ct;
                    break;
                }

                if (last == NULL)
                    last = ct;
            }

            /* The last resort is the first public ctor. */
            if (cd->defctor == NULL)
                cd->defctor = last;
        }

        if (getDeprecated(of))
            setIsDeprecatedClass(cd);

        if (cd->convtocode != NULL && getAllowNone(of))
            setClassHandlesNone(cd);

        if (getOptFlag(of,"Abstract",bool_flag) != NULL)
        {
            setIsAbstractClass(cd);
            setIsIncomplete(cd);
            resetCanCreate(cd);
        }

        /* We assume a public dtor if nothing specific was provided. */
        if (!isDtor(cd))
            setIsPublicDtor(cd);

        if (getOptFlag(of, "DelayDtor", bool_flag) != NULL)
        {
            setIsDelayedDtor(cd);
            setHasDelayedDtors(mod);
        }

        /*
         * There are subtle differences between the add and concat methods and
         * the multiply and repeat methods.  The number versions can have their
         * operands swapped and may return NotImplemented.  If the user has
         * used the /Numeric/ annotation or there are other numeric operators
         * then we use add/multiply.  Otherwise, if there are indexing
         * operators then we use concat/repeat.
         */
        seq_might = seq_not = FALSE;

        for (md = cd -> members; md != NULL; md = md -> next)
            switch (md -> slot)
            {
            case getitem_slot:
            case setitem_slot:
            case delitem_slot:
                /* This might be a sequence. */
                seq_might = TRUE;
                break;

            case sub_slot:
            case isub_slot:
            case div_slot:
            case idiv_slot:
            case mod_slot:
            case imod_slot:
            case floordiv_slot:
            case ifloordiv_slot:
            case truediv_slot:
            case itruediv_slot:
            case pos_slot:
            case neg_slot:
                /* This is definately not a sequence. */
                seq_not = TRUE;
                break;
            }

        if (!seq_not && seq_might)
            for (md = cd -> members; md != NULL; md = md -> next)
            {
                /* Ignore if the user has been explicit. */
                if (isNumeric(md))
                    continue;

                switch (md -> slot)
                {
                case add_slot:
                    md -> slot = concat_slot;
                    break;

                case iadd_slot:
                    md -> slot = iconcat_slot;
                    break;

                case mul_slot:
                    md -> slot = repeat_slot;
                    break;

                case imul_slot:
                    md -> slot = irepeat_slot;
                    break;
                }
            }
    }

    if (inMainModule())
    {
        setIsUsedName(cd->iff->name);
        setIsUsedName(cd->pyname);
    }
}


/*
 * Return the encoded name of a template (ie. including its argument types) as
 * a scoped name.
 */
scopedNameDef *encodedTemplateName(templateDef *td)
{
    int a;
    scopedNameDef *snd;

    snd = copyScopedName(td->fqname);

    for (a = 0; a < td->types.nrArgs; ++a)
    {
        char buf[50];
        int flgs;
        scopedNameDef *arg_snd;
        argDef *ad = &td->types.args[a];

        flgs = 0;

        if (isConstArg(ad))
            flgs += 1;

        if (isReference(ad))
            flgs += 2;

        /* We use numbers so they don't conflict with names. */
        sprintf(buf, "%02d%d%d", ad->atype, flgs, ad->nrderefs);

        switch (ad->atype)
        {
        case defined_type:
            arg_snd = copyScopedName(ad->u.snd);
            break;

        case template_type:
            arg_snd = encodedTemplateName(ad->u.td);
            break;

        case struct_type:
            arg_snd = copyScopedName(ad->u.sname);
            break;

        default:
            arg_snd = NULL;
        }

        /*
         * Replace the first element of the argument name with a copy with the
         * encoding prepended.
         */
        if (arg_snd != NULL)
            arg_snd->name = concat(buf, arg_snd->name, NULL);
        else
            arg_snd = text2scopePart(sipStrdup(buf));

        appendScopedName(&snd, arg_snd);
    }

    return snd;
}


/*
 * Create a new mapped type.
 */
static mappedTypeDef *newMappedType(sipSpec *pt, argDef *ad, optFlags *of)
{
    mappedTypeDef *mtd;
    scopedNameDef *snd;
    ifaceFileDef *iff;
    const char *cname;

    /* Check that the type is one we want to map. */
    switch (ad->atype)
    {
    case defined_type:
        snd = ad->u.snd;
        cname = scopedNameTail(snd);
        break;

    case template_type:
        snd = encodedTemplateName(ad->u.td);
        cname = NULL;
        break;

    case struct_type:
        snd = ad->u.sname;
        cname = scopedNameTail(snd);
        break;

    default:
        yyerror("Invalid type for %MappedType");
    }

    iff = findIfaceFile(pt, currentModule, snd, mappedtype_iface,
            getAPIRange(of), ad);

    /* Check it hasn't already been defined. */
    for (mtd = pt->mappedtypes; mtd != NULL; mtd = mtd->next)
        if (mtd->iff == iff)
        {
            /*
             * We allow types based on the same template but with different
             * arguments.
             */
            if (ad->atype != template_type || sameBaseType(ad, &mtd->type))
                yyerror("Mapped type has already been defined in this module");
        }

    /* The module may not have been set yet. */
    iff->module = currentModule;

    /* Create a new mapped type. */
    mtd = allocMappedType(pt, ad);

    if (cname != NULL)
        mtd->pyname = cacheName(pt, getPythonName(currentModule, of, cname));

    if (getOptFlag(of, "NoRelease", bool_flag) != NULL)
        setNoRelease(mtd);

    if (getAllowNone(of))
        setHandlesNone(mtd);

    mtd->doctype = getDocType(of);

    mtd->iff = iff;
    mtd->next = pt->mappedtypes;

    pt->mappedtypes = mtd;

    if (inMainModule())
    {
        setIsUsedName(mtd->cname);

        if (mtd->pyname)
            setIsUsedName(mtd->pyname);
    }

    return mtd;
}


/*
 * Allocate, initialise and return a mapped type structure.
 */
mappedTypeDef *allocMappedType(sipSpec *pt, argDef *type)
{
    mappedTypeDef *mtd;

    mtd = sipMalloc(sizeof (mappedTypeDef));

    mtd->type = *type;
    mtd->type.argflags = 0;
    mtd->type.nrderefs = 0;

    mtd->cname = cacheName(pt, type2string(&mtd->type));

    return mtd;
}


/*
 * Create a new enum.
 */
static enumDef *newEnum(sipSpec *pt, moduleDef *mod, mappedTypeDef *mt_scope,
        char *name, optFlags *of, int flags)
{
    enumDef *ed, *first_alt, *next_alt;
    classDef *c_scope;
    ifaceFileDef *scope;

    if (mt_scope != NULL)
    {
        scope = mt_scope->iff;
        c_scope = NULL;
    }
    else
    {
        if ((c_scope = currentScope()) != NULL)
            scope = c_scope->iff;
        else
            scope = NULL;
    }

    ed = sipMalloc(sizeof (enumDef));

    /* Assume the enum isn't versioned. */
    first_alt = ed;
    next_alt = NULL;

    if (name != NULL)
    {
        ed->pyname = cacheName(pt, getPythonName(mod, of, name));
        checkAttributes(pt, mod, c_scope, mt_scope, ed->pyname->text, FALSE);

        ed->fqcname = text2scopedName(scope, name);
        ed->cname = cacheName(pt, scopedNameToString(ed->fqcname));

        if (inMainModule())
        {
            setIsUsedName(ed->pyname);
            setIsUsedName(ed->cname);
        }

        /* If the scope is versioned then look for any alternate. */
        if (scope != NULL && scope->api_range != NULL)
        {
            enumDef *alt;

            for (alt = pt->enums; alt != NULL; alt = alt->next)
            {
                if (alt->module != mod || alt->fqcname == NULL)
                    continue;

                if (compareScopedNames(alt->fqcname, ed->fqcname) == 0)
                {
                    first_alt = alt->first_alt;
                    next_alt = first_alt->next_alt;
                    first_alt->next_alt = ed;

                    break;
                }
            }
        }
    }
    else
    {
        ed->pyname = NULL;
        ed->fqcname = NULL;
        ed->cname = NULL;
    }

    if (flags & SECT_IS_PROT)
    {
        if (makeProtPublic)
        {
            flags &= ~SECT_IS_PROT;
            flags |= SECT_IS_PUBLIC;
        }
        else if (c_scope != NULL)
        {
            setHasShadow(c_scope);
        }
    }

    ed->enumflags = flags;
    ed->enumnr = -1;
    ed->ecd = c_scope;
    ed->emtd = mt_scope;
    ed->first_alt = first_alt;
    ed->next_alt = next_alt;
    ed->module = mod;
    ed->members = NULL;
    ed->slots = NULL;
    ed->overs = NULL;
    ed->next = pt -> enums;

    pt->enums = ed;

    return ed;
}


/*
 * Get the type values and (optionally) the type names for substitution in
 * handwritten code.
 */
void appendTypeStrings(scopedNameDef *ename, signatureDef *patt, signatureDef *src, signatureDef *known, scopedNameDef **names, scopedNameDef **values)
{
    int a;

    for (a = 0; a < patt->nrArgs; ++a)
    {
        argDef *pad = &patt->args[a];

        if (pad->atype == defined_type)
        {
            char *nam = NULL, *val;
            argDef *sad;

            /*
             * If the type names are already known then check that this is one
             * of them.
             */
            if (known == NULL)
                nam = scopedNameTail(pad->u.snd);
            else if (pad->u.snd->next == NULL)
            {
                int k;

                for (k = 0; k < known->nrArgs; ++k)
                {
                    /* Skip base types. */
                    if (known->args[k].atype != defined_type)
                        continue;

                    if (strcmp(pad->u.snd->name, known->args[k].u.snd->name) == 0)
                    {
                        nam = pad->u.snd->name;
                        break;
                    }
                }
            }

            if (nam == NULL)
                continue;

            /* Add the name. */
            appendScopedName(names, text2scopePart(nam));

            /*
             * Add the corresponding value.  For defined types we don't want 
             * any indirection or references.
             */
            sad = &src->args[a];

            if (sad->atype == defined_type)
                val = scopedNameToString(sad->u.snd);
            else
                val = type2string(sad);

            appendScopedName(values, text2scopePart(val));
        }
        else if (pad->atype == template_type)
        {
            argDef *sad = &src->args[a];

            /* These checks shouldn't be necessary, but... */
            if (sad->atype == template_type && pad->u.td->types.nrArgs == sad->u.td->types.nrArgs)
                appendTypeStrings(ename, &pad->u.td->types, &sad->u.td->types, known, names, values);
        }
    }
}


/*
 * Convert a type to a string on the heap.  The string will use the minimum
 * whitespace while still remaining valid C++.
 */
static char *type2string(argDef *ad)
{
    int i, on_heap = FALSE;
    int nr_derefs = ad->nrderefs;
    int is_reference = isReference(ad);
    char *s;

    /* Use the original type if possible. */
    if (ad->original_type != NULL && !noTypeName(ad->original_type))
    {
        s = scopedNameToString(ad->original_type->fqname);
        on_heap = TRUE;

        nr_derefs -= ad->original_type->type.nrderefs;

        if (isReference(&ad->original_type->type))
            is_reference = FALSE;
    }
    else
        switch (ad->atype)
        {
        case template_type:
            {
                templateDef *td = ad->u.td;

                s = scopedNameToString(td->fqname);
                append(&s, "<");

                for (i = 0; i < td->types.nrArgs; ++i)
                {
                    char *sub_type = type2string(&td->types.args[i]);

                    if (i > 0)
                        append(&s, ",");

                    append(&s, sub_type);
                    free(sub_type);
                }

                if (s[strlen(s) - 1] == '>')
                    append(&s, " >");
                else
                    append(&s, ">");

                on_heap = TRUE;
                break;
            }

        case struct_type:
            s = scopedNameToString(ad->u.sname);
            on_heap = TRUE;
            break;

        case defined_type:
            s = scopedNameToString(ad->u.snd);
            on_heap = TRUE;
            break;

        case ubyte_type:
        case ustring_type:
            s = "unsigned char";
            break;

        case byte_type:
        case ascii_string_type:
        case latin1_string_type:
        case utf8_string_type:
        case string_type:
            s = "char";
            break;

        case sbyte_type:
        case sstring_type:
            s = "signed char";
            break;

        case wstring_type:
            s = "wchar_t";
            break;

        case ushort_type:
            s = "unsigned short";
            break;

        case short_type:
            s = "short";
            break;

        case uint_type:
            s = "uint";
            break;

        case int_type:
        case cint_type:
            s = "int";
            break;

        case ulong_type:
            s = "unsigned long";
            break;

        case long_type:
            s = "long";
            break;

        case ulonglong_type:
            s = "unsigned long long";
            break;

        case longlong_type:
            s = "long long";
            break;

        case float_type:
        case cfloat_type:
            s = "float";
            break;

        case double_type:
        case cdouble_type:
            s = "double";
            break;

        case bool_type:
        case cbool_type:
            s = "bool";
            break;

        default:
            fatal("Unsupported type argument to type2string(): %d\n", ad->atype);
        }

    /* Make sure the string is on the heap. */
    if (!on_heap)
        s = sipStrdup(s);

    while (nr_derefs-- > 0)
        append(&s, "*");

    if (is_reference)
        append(&s, "&");

    return s;
}


/*
 * Convert a scoped name to a string on the heap.
 */
static char *scopedNameToString(scopedNameDef *name)
{
    static const char scope_string[] = "::";
    size_t len;
    scopedNameDef *snd;
    char *s, *dp;

    /* Work out the length of buffer needed. */
    len = 0;

    for (snd = name; snd != NULL; snd = snd->next)
    {
        len += strlen(snd->name);

        if (snd->next != NULL)
        {
            /* Ignore the encoded part of template names. */
            if (isdigit(snd->next->name[0]))
                break;

            len += strlen(scope_string);
        }
    }

    /* Allocate and populate the buffer. */
    dp = s = sipMalloc(len + 1);

    for (snd = name; snd != NULL; snd = snd->next)
    {
        strcpy(dp, snd->name);
        dp += strlen(snd->name);

        if (snd->next != NULL)
        {
            /* Ignore the encoded part of template names. */
            if (isdigit(snd->next->name[0]))
                break;

            strcpy(dp, scope_string);
            dp += strlen(scope_string);
        }
    }

    return s;
}


/*
 * Instantiate a class template.
 */
static void instantiateClassTemplate(sipSpec *pt, moduleDef *mod,
        classDef *scope, scopedNameDef *fqname, classTmplDef *tcd,
        templateDef *td)
{
    scopedNameDef *type_names, *type_values;
    classDef *cd;
    ctorDef *oct, **cttail;
    argDef *ad;
    ifaceFileList *iffl, **used;

    type_names = type_values = NULL;
    appendTypeStrings(classFQCName(tcd->cd), &tcd->sig, &td->types, NULL, &type_names, &type_values);

    /*
     * Add a mapping from the template name to the instantiated name.  If we
     * have got this far we know there is room for it.
     */
    ad = &tcd->sig.args[tcd->sig.nrArgs++];
    memset(ad, 0, sizeof (argDef));
    ad->atype = defined_type;
    ad->u.snd = classFQCName(tcd->cd);

    appendScopedName(&type_names, text2scopePart(scopedNameTail(classFQCName(tcd->cd))));
    appendScopedName(&type_values, text2scopePart(scopedNameToString(fqname)));

    /* Create the new class. */
    cd = sipMalloc(sizeof (classDef));

    /* Start with a shallow copy. */
    *cd = *tcd->cd;

    resetIsTemplateClass(cd);
    cd->pyname = cacheName(pt, scopedNameTail(fqname));
    cd->td = td;

    /* Handle the interface file. */
    cd->iff = findIfaceFile(pt, mod, fqname, class_iface,
            (scope != NULL ? scope->iff->api_range : NULL), NULL);
    cd->iff->module = mod;

    /* Make a copy of the used list and add the enclosing scope. */
    used = &cd->iff->used;

    for (iffl = tcd->cd->iff->used; iffl != NULL; iffl = iffl->next)
        addToUsedList(used, iffl->iff);

    /* Include any scope header code. */
    if (scope != NULL)
        appendCodeBlock(&cd->iff->hdrcode, scope->iff->hdrcode);

    if (inMainModule())
    {
        setIsUsedName(cd->iff->name);
        setIsUsedName(cd->pyname);
    }

    cd->ecd = currentScope();

    /* Handle the enums. */
    instantiateTemplateEnums(pt, tcd, td, cd, used, type_names, type_values);

    /* Handle the variables. */
    instantiateTemplateVars(pt, tcd, td, cd, used, type_names, type_values);

    /* Handle the ctors. */
    cd->ctors = NULL;
    cttail = &cd->ctors;

    for (oct = tcd->cd->ctors; oct != NULL; oct = oct->next)
    {
        ctorDef *nct = sipMalloc(sizeof (ctorDef));

        /* Start with a shallow copy. */
        *nct = *oct;

        templateSignature(&nct->pysig, FALSE, tcd, td, cd);

        if (oct->cppsig == NULL)
            nct->cppsig = NULL;
        else if (oct->cppsig == &oct->pysig)
            nct->cppsig = &nct->pysig;
        else
        {
            nct->cppsig = sipMalloc(sizeof (signatureDef));

            *nct->cppsig = *oct->cppsig;

            templateSignature(nct->cppsig, FALSE, tcd, td, cd);
        }

        nct->methodcode = templateCode(pt, used, nct->methodcode, type_names, type_values);

        nct->next = NULL;
        *cttail = nct;
        cttail = &nct->next;

        /* Handle the default ctor. */
        if (tcd->cd->defctor == oct)
            cd->defctor = nct;
    }

    cd->dealloccode = templateCode(pt, used, cd->dealloccode, type_names, type_values);
    cd->dtorcode = templateCode(pt, used, cd->dtorcode, type_names, type_values);

    /* Handle the methods. */
    cd->members = instantiateTemplateMethods(tcd->cd->members, mod);
    cd->overs = instantiateTemplateOverloads(pt, tcd->cd->overs,
            tcd->cd->members, cd->members, tcd, td, cd, used, type_names,
            type_values);

    cd->cppcode = templateCode(pt, used, cd->cppcode, type_names, type_values);
    cd->iff->hdrcode = templateCode(pt, used, cd->iff->hdrcode, type_names, type_values);
    cd->convtosubcode = templateCode(pt, used, cd->convtosubcode, type_names, type_values);
    cd->convtocode = templateCode(pt, used, cd->convtocode, type_names, type_values);
    cd->travcode = templateCode(pt, used, cd->travcode, type_names, type_values);
    cd->clearcode = templateCode(pt, used, cd->clearcode, type_names, type_values);
    cd->getbufcode = templateCode(pt, used, cd->getbufcode, type_names, type_values);
    cd->releasebufcode = templateCode(pt, used, cd->releasebufcode, type_names, type_values);
    cd->readbufcode = templateCode(pt, used, cd->readbufcode, type_names, type_values);
    cd->writebufcode = templateCode(pt, used, cd->writebufcode, type_names, type_values);
    cd->segcountcode = templateCode(pt, used, cd->segcountcode, type_names, type_values);
    cd->charbufcode = templateCode(pt, used, cd->charbufcode, type_names, type_values);
    cd->picklecode = templateCode(pt, used, cd->picklecode, type_names, type_values);
    cd->next = pt->classes;

    pt->classes = cd;

    tcd->sig.nrArgs--;

    freeScopedName(type_names);
    freeScopedName(type_values);
}


/*
 * Instantiate the methods of a template class.
 */
static memberDef *instantiateTemplateMethods(memberDef *tmd, moduleDef *mod)
{
    memberDef *md, *methods, **mdtail;

    methods = NULL;
    mdtail = &methods;

    for (md = tmd; md != NULL; md = md->next)
    {
        memberDef *nmd = sipMalloc(sizeof (memberDef));

        /* Start with a shallow copy. */
        *nmd = *md;

        nmd->module = mod;

        if (inMainModule())
            setIsUsedName(nmd->pyname);

        nmd->next = NULL;
        *mdtail = nmd;
        mdtail = &nmd->next;
    }

    return methods;
}


/*
 * Instantiate the overloads of a template class.
 */
static overDef *instantiateTemplateOverloads(sipSpec *pt, overDef *tod,
        memberDef *tmethods, memberDef *methods, classTmplDef *tcd,
        templateDef *td, classDef *cd, ifaceFileList **used,
        scopedNameDef *type_names, scopedNameDef *type_values)
{
    overDef *od, *overloads, **odtail;

    overloads = NULL;
    odtail = &overloads;

    for (od = tod; od != NULL; od = od->next)
    {
        overDef *nod = sipMalloc(sizeof (overDef));
        memberDef *nmd, *omd;

        /* Start with a shallow copy. */
        *nod = *od;

        for (nmd = methods, omd = tmethods; omd != NULL; omd = omd->next, nmd = nmd->next)
            if (omd == od->common)
            {
                nod->common = nmd;
                break;
            }

        templateSignature(&nod->pysig, TRUE, tcd, td, cd);

        if (od->cppsig == &od->pysig)
            nod->cppsig = &nod->pysig;
        else
        {
            nod->cppsig = sipMalloc(sizeof (signatureDef));

            *nod->cppsig = *od->cppsig;

            templateSignature(nod->cppsig, TRUE, tcd, td, cd);
        }

        nod->methodcode = templateCode(pt, used, nod->methodcode, type_names, type_values);

        /* Handle any virtual handler. */
        if (od->virthandler != NULL)
        {
            moduleDef *mod = cd->iff->module;

            nod->virthandler = sipMalloc(sizeof (virtHandlerDef));

            /* Start with a shallow copy. */
            *nod->virthandler = *od->virthandler;

            if (od->virthandler->cppsig == &od->pysig)
                nod->virthandler->cppsig = &nod->pysig;
            else
            {
                nod->virthandler->cppsig = sipMalloc(sizeof (signatureDef));

                *nod->virthandler->cppsig = *od->virthandler->cppsig;

                templateSignature(nod->virthandler->cppsig, TRUE, tcd, td, cd);
            }

            nod->virthandler->module = mod;
            nod->virthandler->virtcode = templateCode(pt, used, nod->virthandler->virtcode, type_names, type_values);
            nod->virthandler->next = mod->virthandlers;

            mod->virthandlers = nod->virthandler;
        }

        nod->next = NULL;
        *odtail = nod;
        odtail = &nod->next;
    }

    return overloads;
}


/*
 * Instantiate the enums of a template class.
 */
static void instantiateTemplateEnums(sipSpec *pt, classTmplDef *tcd,
        templateDef *td, classDef *cd, ifaceFileList **used,
        scopedNameDef *type_names, scopedNameDef *type_values)
{
    enumDef *ted;
    moduleDef *mod = cd->iff->module;

    for (ted = pt->enums; ted != NULL; ted = ted->next)
        if (ted->ecd == tcd->cd)
        {
            enumDef *ed;
            enumMemberDef *temd;

            ed = sipMalloc(sizeof (enumDef));

            /* Start with a shallow copy. */
            *ed = *ted;

            if (ed->fqcname != NULL)
            {
                ed->fqcname = text2scopedName(cd->iff,
                        scopedNameTail(ed->fqcname));
                ed->cname = cacheName(pt, scopedNameToString(ed->fqcname));
            }

            if (inMainModule())
            {
                if (ed->pyname != NULL)
                    setIsUsedName(ed->pyname);

                if (ed->cname != NULL)
                    setIsUsedName(ed->cname);
            }

            ed->ecd = cd;
            ed->first_alt = ed;
            ed->module = mod;
            ed->members = NULL;

            for (temd = ted->members; temd != NULL; temd = temd->next)
            {
                enumMemberDef *emd;

                emd = sipMalloc(sizeof (enumMemberDef));

                /* Start with a shallow copy. */
                *emd = *temd;
                emd->ed = ed;

                emd->next = ed->members;
                ed->members = emd;
            }

            ed->slots = instantiateTemplateMethods(ted->slots, mod);
            ed->overs = instantiateTemplateOverloads(pt, ted->overs,
                    ted->slots, ed->slots, tcd, td, cd, used, type_names,
                    type_values);

            ed->next = pt->enums;
            pt->enums = ed;
        }
}


/*
 * Instantiate the variables of a template class.
 */
static void instantiateTemplateVars(sipSpec *pt, classTmplDef *tcd,
        templateDef *td, classDef *cd, ifaceFileList **used,
        scopedNameDef *type_names, scopedNameDef *type_values)
{
    varDef *tvd;

    for (tvd = pt->vars; tvd != NULL; tvd = tvd->next)
        if (tvd->ecd == tcd->cd)
        {
            varDef *vd;

            vd = sipMalloc(sizeof (varDef));

            /* Start with a shallow copy. */
            *vd = *tvd;

            if (inMainModule())
                setIsUsedName(vd->pyname);

            vd->fqcname = text2scopedName(cd->iff,
                    scopedNameTail(vd->fqcname));
            vd->ecd = cd;
            vd->module = cd->iff->module;

            templateType(&vd->type, tcd, td, cd);

            vd->accessfunc = templateCode(pt, used, vd->accessfunc, type_names, type_values);
            vd->getcode = templateCode(pt, used, vd->getcode, type_names, type_values);
            vd->setcode = templateCode(pt, used, vd->setcode, type_names, type_values);

            addVariable(pt, vd);
        }
}


/*
 * Replace any template arguments in a signature.
 */
static void templateSignature(signatureDef *sd, int result, classTmplDef *tcd, templateDef *td, classDef *ncd)
{
    int a;

    if (result)
        templateType(&sd->result, tcd, td, ncd);

    for (a = 0; a < sd->nrArgs; ++a)
        templateType(&sd->args[a], tcd, td, ncd);
}


/*
 * Replace any template arguments in a type.
 */
static void templateType(argDef *ad, classTmplDef *tcd, templateDef *td, classDef *ncd)
{
    int a;
    char *name;

    /* Descend into any sub-templates. */
    if (ad->atype == template_type)
    {
        templateDef *new_td = sipMalloc(sizeof (templateDef));

        /* Make a deep copy of the template definition. */
        *new_td = *ad->u.td;
        ad->u.td = new_td;

        templateSignature(&ad->u.td->types, FALSE, tcd, td, ncd);

        return;
    }

    /* Ignore if it isn't an unscoped name. */
    if (ad->atype != defined_type || ad->u.snd->next != NULL)
        return;

    name = ad->u.snd->name;

    for (a = 0; a < tcd->sig.nrArgs - 1; ++a)
        if (strcmp(name, scopedNameTail(tcd->sig.args[a].u.snd)) == 0)
        {
            argDef *tad = &td->types.args[a];

            ad->atype = tad->atype;

            /* We take the constrained flag from the real type. */
            resetIsConstrained(ad);

            if (isConstrained(tad))
                setIsConstrained(ad);

            ad->u = tad->u;

            return;
        }

    /* Handle the class name itself. */
    if (strcmp(name, scopedNameTail(classFQCName(tcd->cd))) == 0)
    {
        ad->atype = class_type;
        ad->u.cd = ncd;
        ad->original_type = NULL;
    }
}


/*
 * Replace any template arguments in a literal code block.
 */
codeBlock *templateCode(sipSpec *pt, ifaceFileList **used, codeBlock *ocb,
        scopedNameDef *names, scopedNameDef *values)
{
    codeBlock *ncb = NULL, **tail = &ncb;

    while (ocb != NULL)
    {
        char *at = ocb->frag;

        do
        {
            char *first = NULL;
            codeBlock *cb;
            scopedNameDef *nam, *val, *nam_first, *val_first;

            /*
             * Go through the rest of this fragment looking for each of the
             * types and the name of the class itself.
             */
            nam = names;
            val = values;

            while (nam != NULL && val != NULL)
            {
                char *cp;

                if ((cp = strstr(at, nam->name)) != NULL)
                    if (first == NULL || first > cp)
                    {
                        nam_first = nam;
                        val_first = val;
                        first = cp;
                    }

                nam = nam->next;
                val = val->next;
            }

            /* Create the new fragment. */
            cb = sipMalloc(sizeof (codeBlock));

            if (at == ocb->frag)
            {
                cb->filename = ocb->filename;
                cb->linenr = ocb->linenr;
            }
            else
                cb->filename = NULL;

            cb->next = NULL;
            *tail = cb;
            tail = &cb->next;

            /* See if anything was found. */
            if (first == NULL)
            {
                /* We can just point to this. */
                cb->frag = at;

                /* All done with this one. */
                at = NULL;
            }
            else
            {
                static char *gen_names[] = {
                    "sipType_",
                    "sipClass_",
                    "sipEnum_",
                    "sipException_",
                    NULL
                };

                char *dp, *sp, **gn;
                int genname = FALSE;

                /*
                 * If the context in which the text is used is in the name of a
                 * SIP generated object then translate any "::" scoping to "_".
                 */
                for (gn = gen_names; *gn != NULL; ++gn)
                    if (search_back(first, at, *gn))
                    {
                        addUsedFromCode(pt, used, val_first->name);
                        genname = TRUE;
                        break;
                    }

                /* Fragment the fragment. */
                cb->frag = sipMalloc(first - at + strlen(val_first->name) + 1);

                strncpy(cb->frag, at, first - at);

                dp = &cb->frag[first - at];
                sp = val_first->name;

                if (genname)
                {
                    char gch;

                    while ((gch = *sp++) != '\0')
                        if (gch == ':' && *sp == ':')
                        {
                            *dp++ = '_';
                            ++sp;
                        }
                        else
                            *dp++ = gch;

                    *dp = '\0';
                }
                else
                    strcpy(dp, sp);

                /* Move past the replaced text. */
                at = first + strlen(nam_first->name);
            }
        }
        while (at != NULL && *at != '\0');

        ocb = ocb->next;
    }

    return ncb;
}


/*
 * Return TRUE if the text at the end of a string matches the target string.
 */
static int search_back(const char *end, const char *start, const char *target)
{
    size_t tlen = strlen(target);

    if (start + tlen >= end)
        return FALSE;

    return (strncmp(end - tlen, target, tlen) == 0);
}


/*
 * Add any needed interface files based on handwritten code.
 */
static void addUsedFromCode(sipSpec *pt, ifaceFileList **used, const char *sname)
{
    ifaceFileDef *iff;
    enumDef *ed;

    for (iff = pt->ifacefiles; iff != NULL; iff = iff->next)
    {
        if (iff->type != class_iface && iff->type != exception_iface)
            continue;

        if (sameName(iff->fqcname, sname))
        {
            addToUsedList(used, iff);
            return;
        }
    }

    for (ed = pt->enums; ed != NULL; ed = ed->next)
    {
        if (ed->ecd == NULL)
            continue;

        if (sameName(ed->fqcname, sname))
        {
            addToUsedList(used, ed->ecd->iff);
            return;
        }
    }
}


/*
 * Compare a scoped name with its string equivalent.
 */
static int sameName(scopedNameDef *snd, const char *sname)
{
    while (snd != NULL && *sname != '\0')
    {
        const char *sp = snd->name;

        while (*sp != '\0' && *sname != ':' && *sname != '\0')
            if (*sp++ != *sname++)
                return FALSE;

        if (*sp != '\0' || (*sname != ':' && *sname != '\0'))
            return FALSE;

        snd = snd->next;

        if (*sname == ':')
            sname += 2;
    }

    return (snd == NULL && *sname == '\0');
}


/*
 * Compare a (possibly) relative scoped name with a fully qualified scoped name
 * while taking the current scope into account.
 */
static int foundInScope(scopedNameDef *fq_name, scopedNameDef *rel_name)
{
    classDef *scope;

    for (scope = currentScope(); scope != NULL; scope = scope->ecd)
    {
        scopedNameDef *snd;
        int found;

        snd = copyScopedName(classFQCName(scope));
        appendScopedName(&snd, copyScopedName(rel_name));

        found = (compareScopedNames(fq_name, snd) == 0);

        freeScopedName(snd);

        if (found)
            return TRUE;
    }

    return compareScopedNames(fq_name, rel_name) == 0;
}


/*
 * Create a new typedef.
 */
static void newTypedef(sipSpec *pt, moduleDef *mod, char *name, argDef *type,
        optFlags *optflgs)
{
    typedefDef *td, **tdp;
    scopedNameDef *fqname;
    classDef *scope;

    scope = currentScope();
    fqname = text2scopedName((scope != NULL ? scope->iff : NULL), name);

    /* See if we are instantiating a template class. */
    if (type->atype == template_type)
    {
        classTmplDef *tcd;
        templateDef *td = type->u.td;

        for (tcd = pt->classtemplates; tcd != NULL; tcd = tcd->next)
            if (foundInScope(tcd->cd->iff->fqcname, td->fqname) &&
                sameTemplateSignature(&tcd->sig, &td->types, FALSE))
            {
                instantiateClassTemplate(pt, mod, scope, fqname, tcd, td);

                /* All done. */
                return;
            }
    }

    /*
     * Check it doesn't already exist and find the position in the sorted list
     * where it should be put.
     */
    for (tdp = &pt->typedefs; *tdp != NULL; tdp = &(*tdp)->next)
    {
        int res = compareScopedNames((*tdp)->fqname, fqname);

        if (res == 0)
        {
            fatalScopedName(fqname);
            fatal(" already defined\n");
        }

        if (res > 0)
            break;
    }

    td = sipMalloc(sizeof (typedefDef));

    td->tdflags = 0;
    td->fqname = fqname;
    td->ecd = scope;
    td->module = mod;
    td->type = *type;

    td->next = *tdp;
    *tdp = td;

    if (getOptFlag(optflgs, "NoTypeName", bool_flag) != NULL)
        setNoTypeName(td);

    mod->nrtypedefs++;
}


/*
 * Speculatively try and resolve any typedefs.  In some cases (eg. when
 * comparing template signatures) it helps to use the real type if it is known.
 * Note that this wouldn't be necessary if we required that all types be known
 * before they are used.
 */
static void resolveAnyTypedef(sipSpec *pt, argDef *ad)
{
    argDef orig = *ad;

    while (ad->atype == defined_type)
    {
        ad->atype = no_type;
        searchTypedefs(pt, ad->u.snd, ad);

        /*
         * Don't resolve to a template type as it may be superceded later on
         * by a more specific mapped type.
         */
        if (ad->atype == no_type || ad->atype == template_type)
        {
            *ad = orig;
            break;
        }
    }
}


/*
 * Return TRUE if the template signatures are the same.  A deep comparison is
 * used for mapped type templates where we want to recurse into any nested
 * templates.
 */
int sameTemplateSignature(signatureDef *tmpl_sd, signatureDef *args_sd,
        int deep)
{
    int a;

    if (tmpl_sd->nrArgs != args_sd->nrArgs)
        return FALSE;

    for (a = 0; a < tmpl_sd->nrArgs; ++a)
    {
        argDef *tmpl_ad = &tmpl_sd->args[a];
        argDef *args_ad = &args_sd->args[a];

        /*
         * If we are doing a shallow comparision (ie. for class templates) then
         * a type name in the template signature matches anything in the
         * argument signature.
         */
        if (tmpl_ad->atype == defined_type && !deep)
            continue;

        /*
         * For type names only compare the references and pointers, and do the
         * same for any nested templates.
         */
        if (tmpl_ad->atype == defined_type && args_ad->atype == defined_type)
        {
            if (isReference(tmpl_ad) != isReference(args_ad) || tmpl_ad->nrderefs != args_ad->nrderefs)
                return FALSE;
        }
        else if (tmpl_ad->atype == template_type && args_ad->atype == template_type)
        {
            if (!sameTemplateSignature(&tmpl_ad->u.td->types, &args_ad->u.td->types, deep))
                return FALSE;
        }
        else if (!sameBaseType(tmpl_ad, args_ad))
            return FALSE;
    }

    return TRUE;
}


/*
 * Create a new variable.
 */
static void newVar(sipSpec *pt, moduleDef *mod, char *name, int isstatic,
        argDef *type, optFlags *of, codeBlock *acode, codeBlock *gcode,
        codeBlock *scode, int section)
{
    varDef *var;
    classDef *escope = currentScope();
    nameDef *nd;

    /* Check the section. */
    if (section != 0)
    {
        if ((section & SECT_IS_PUBLIC) == 0)
            yyerror("Class variables must be in the public section");

        if (!isstatic && acode != NULL)
            yyerror("%AccessCode cannot be specified for non-static class variables");
    }

    if (isstatic && pt->genc)
        yyerror("Cannot have static members in a C structure");

    if (gcode != NULL || scode != NULL)
    {
        if (acode != NULL)
            yyerror("Cannot mix %AccessCode and %GetCode or %SetCode");

        if (escope == NULL)
            yyerror("Cannot specify %GetCode or %SetCode for global variables");
    }

    applyTypeFlags(mod, type, of);

    nd = cacheName(pt, getPythonName(mod, of, name));

    if (inMainModule())
        setIsUsedName(nd);

    checkAttributes(pt, mod, escope, NULL, nd->text, FALSE);

    var = sipMalloc(sizeof (varDef));

    var->pyname = nd;
    var->fqcname = text2scopedName((escope != NULL ? escope->iff : NULL),
            name);
    var->ecd = escope;
    var->module = mod;
    var->varflags = 0;
    var->type = *type;
    var->accessfunc = acode;
    var->getcode = gcode;
    var->setcode = scode;

    if (isstatic || (escope != NULL && escope->iff->type == namespace_iface))
        setIsStaticVar(var);

    addVariable(pt, var);
}


/*
 * Create a new ctor.
 */
static void newCtor(moduleDef *mod, char *name, int sectFlags,
        signatureDef *args, optFlags *optflgs, codeBlock *methodcode,
        throwArgs *exceptions, signatureDef *cppsig, int explicit,
        codeBlock *docstring)
{
    ctorDef *ct, **ctp;
    classDef *cd = currentScope();

    /* Check the name of the constructor. */
    if (strcmp(classBaseName(cd), name) != 0)
        yyerror("Constructor doesn't have the same name as its class");

    if (docstring != NULL)
        appendCodeBlock(&cd->docstring, docstring);

    /* Add to the list of constructors. */
    ct = sipMalloc(sizeof (ctorDef));

    if (sectFlags & SECT_IS_PROT && makeProtPublic)
    {
        sectFlags &= ~SECT_IS_PROT;
        sectFlags |= SECT_IS_PUBLIC;
    }

    /* Allow the signature to be used like an function signature. */
    memset(&args->result, 0, sizeof (argDef));
    args->result.atype = void_type;

    ct->ctorflags = sectFlags;
    ct->api_range = getAPIRange(optflgs);
    ct->pysig = *args;
    ct->cppsig = (cppsig != NULL ? cppsig : &ct->pysig);
    ct->exceptions = exceptions;
    ct->methodcode = methodcode;

    if (!isPrivateCtor(ct))
        setCanCreate(cd);

    if (isProtectedCtor(ct))
        setHasShadow(cd);

    if (explicit)
        setIsExplicitCtor(ct);

    getHooks(optflgs, &ct->prehook, &ct->posthook);

    if (getReleaseGIL(optflgs))
        setIsReleaseGILCtor(ct);
    else if (getHoldGIL(optflgs))
        setIsHoldGILCtor(ct);

    if (getTransfer(optflgs))
        setIsResultTransferredCtor(ct);

    if (getDeprecated(optflgs))
        setIsDeprecatedCtor(ct);

    if (!isPrivateCtor(ct))
        ct->kwargs = keywordArgs(mod, optflgs, &ct->pysig);

    if (getOptFlag(optflgs, "NoDerived", bool_flag) != NULL)
    {
        if (cppsig != NULL)
            yyerror("The /NoDerived/ annotation cannot be used with a C++ signature");

        if (methodcode == NULL)
            yyerror("The /NoDerived/ annotation must be used with %MethodCode");

        ct->cppsig = NULL;
    }

    if (getOptFlag(optflgs, "Default", bool_flag) != NULL)
    {
        if (cd->defctor != NULL)
            yyerror("A constructor with the /Default/ annotation has already been defined");

        cd->defctor = ct;
    }

    /* Append to the list. */
    for (ctp = &cd->ctors; *ctp != NULL; ctp = &(*ctp)->next)
        ;

    *ctp = ct;
}


/*
 * Create a new function.
 */
static void newFunction(sipSpec *pt, moduleDef *mod, classDef *c_scope,
        mappedTypeDef *mt_scope, int sflags, int isstatic, int issignal,
        int isslot, int isvirt, char *name, signatureDef *sig, int isconst,
        int isabstract, optFlags *optflgs, codeBlock *methodcode,
        codeBlock *vcode, throwArgs *exceptions, signatureDef *cppsig,
        codeBlock *docstring)
{
    static const char *annos[] = {
        "__len__",
        "API",
        "AutoGen",
        "Deprecated",
        "DocType",
        "Encoding",
        "Factory",
        "HoldGIL",
        "KeywordArgs",
        "NewThread",
        "NoArgParser",
        "NoCopy",
        "Numeric",
        "PostHook",
        "PreHook",
        "PyInt",
        "PyName",
        "RaisesPyException",
        "ReleaseGIL",
        "Transfer",
        "TransferBack",
        "TransferThis",
        NULL
    };

    const char *pyname;
    int factory, xferback, no_arg_parser;
    overDef *od, **odp, **headp;
    optFlag *of;
    virtHandlerDef *vhd;

    checkAnnos(optflgs, annos);

    /* Extra checks for a C module. */
    if (pt->genc)
    {
        if (c_scope != NULL)
            yyerror("Function declaration not allowed in a struct in a C module");

        if (isstatic)
            yyerror("Static functions not allowed in a C module");

        if (exceptions != NULL)
            yyerror("Exceptions not allowed in a C module");
    }

    if (mt_scope != NULL)
        headp = &mt_scope->overs;
    else if (c_scope != NULL)
        headp = &c_scope->overs;
    else
        headp = &mod->overs;

    /*
     * See if the function has a non-lazy method.  These are methods that
     * Python expects to see defined in the type before any instance of the
     * type is created.
     */
    if (c_scope != NULL)
    {
        static const char *lazy[] = {
            "__getattribute__",
            "__getattr__",
            "__enter__",
            "__exit__",
            NULL
        };

        const char **l;

        for (l = lazy; *l != NULL; ++l)
            if (strcmp(name, *l) == 0)
            {
                setHasNonlazyMethod(c_scope);
                break;
            }
    }

    /* See if it is a factory method. */
    if (getOptFlag(optflgs, "Factory", bool_flag) != NULL)
        factory = TRUE;
    else
    {
        int a;

        factory = FALSE;

        /* Check /TransferThis/ wasn't specified. */
        if (c_scope == NULL || isstatic)
            for (a = 0; a < sig->nrArgs; ++a)
                if (isThisTransferred(&sig->args[a]))
                    yyerror("/TransferThis/ may only be specified in constructors and class methods");
    }

    /* See if the result is to be returned to Python ownership. */
    xferback = (getOptFlag(optflgs, "TransferBack", bool_flag) != NULL);

    if (factory && xferback)
        yyerror("/TransferBack/ and /Factory/ cannot both be specified");

    /* Create a new overload definition. */

    od = sipMalloc(sizeof (overDef));

    /* Set the overload flags. */

    if ((sflags & SECT_IS_PROT) && makeProtPublic)
    {
        sflags &= ~SECT_IS_PROT;
        sflags |= SECT_IS_PUBLIC | OVER_REALLY_PROT;
    }

    od->overflags = sflags;

    if (issignal)
    {
        resetIsSlot(od);
        setIsSignal(od);
    }
    else if (isslot)
    {
        resetIsSignal(od);
        setIsSlot(od);
    }

    if (factory)
        setIsFactory(od);

    if (xferback)
        setIsResultTransferredBack(od);

    if (getTransfer(optflgs))
        setIsResultTransferred(od);

    if (getOptFlag(optflgs, "TransferThis", bool_flag) != NULL)
        setIsThisTransferredMeth(od);

    if (methodcode == NULL && getOptFlag(optflgs, "RaisesPyException", bool_flag) != NULL)
        setRaisesPyException(od);

    if (isProtected(od))
        setHasShadow(c_scope);

    if ((isSlot(od) || isSignal(od)) && !isPrivate(od))
    {
        if (isSignal(od))
            setHasShadow(c_scope);

        pt->sigslots = TRUE;
    }

    if (isSignal(od) && (methodcode != NULL || vcode != NULL))
        yyerror("Cannot provide code for signals");

    if (isstatic)
    {
        if (isSignal(od))
            yyerror("Static functions cannot be signals");

        if (isvirt)
            yyerror("Static functions cannot be virtual");

        setIsStatic(od);
    }

    if (isconst)
        setIsConst(od);

    if (isabstract)
    {
        if (sflags == 0)
            yyerror("Non-class function specified as abstract");

        setIsAbstract(od);
    }

    if ((of = getOptFlag(optflgs, "AutoGen", opt_name_flag)) != NULL)
    {
        if (of->fvalue.sval == NULL || isEnabledFeature(of->fvalue.sval))
            setIsAutoGen(od);
    }

    if (isvirt)
    {
        if (isSignal(od) && pluginPyQt3(pt))
            yyerror("Virtual signals aren't supported");

        setIsVirtual(od);
        setHasShadow(c_scope);

        vhd = sipMalloc(sizeof (virtHandlerDef));

        vhd->virthandlernr = -1;
        vhd->vhflags = 0;
        vhd->pysig = &od->pysig;
        vhd->cppsig = (cppsig != NULL ? cppsig : &od->pysig);
        vhd->virtcode = vcode;

        if (factory || xferback)
            setIsTransferVH(vhd);

        /*
         * Only add it to the module's virtual handlers if we are not in a
         * class template.
         */
        if (!currentIsTemplate)
        {
            vhd->module = mod;

            vhd->next = mod->virthandlers;
            mod->virthandlers = vhd;
        }
    }
    else
    {
        if (vcode != NULL)
            yyerror("%VirtualCatcherCode provided for non-virtual function");

        vhd = NULL;
    }

    od->cppname = name;
    od->pysig = *sig;
    od->cppsig = (cppsig != NULL ? cppsig : &od->pysig);
    od->exceptions = exceptions;
    od->methodcode = methodcode;
    od->virthandler = vhd;

    no_arg_parser = (getOptFlag(optflgs, "NoArgParser", bool_flag) != NULL);

    if (no_arg_parser)
    {
        if (methodcode == NULL)
            yyerror("%MethodCode must be supplied if /NoArgParser/ is specified");
    }

    if (getOptFlag(optflgs, "NoCopy", bool_flag) != NULL)
        setNoCopy(&od->pysig.result);

    pyname = getPythonName(mod, optflgs, name);

    od->common = findFunction(pt, mod, c_scope, mt_scope, pyname,
            (methodcode != NULL), sig->nrArgs, no_arg_parser);
    
    if (strcmp(pyname, "__delattr__") == 0)
        setIsDelattr(od);

    if (docstring != NULL)
        appendCodeBlock(&od->common->docstring, docstring);

    od->api_range = getAPIRange(optflgs);

    if (od->api_range == NULL)
        setNotVersioned(od->common);

    if (getOptFlag(optflgs, "Numeric", bool_flag) != NULL)
        setIsNumeric(od->common);

    /* Methods that run in new threads must be virtual. */
    if (getOptFlag(optflgs, "NewThread", bool_flag) != NULL)
    {
        argDef *res;

        if (!isvirt)
            yyerror("/NewThread/ may only be specified for virtual functions");

        /*
         * This is an arbitary limitation to make the code generator slightly
         * easier - laziness on my part.
         */
        res = &od->cppsig->result;

        if (res->atype != void_type || res->nrderefs != 0)
            yyerror("/NewThread/ may only be specified for void functions");

        setIsNewThread(od);
    }

    getHooks(optflgs, &od->prehook, &od->posthook);

    if (getReleaseGIL(optflgs))
        setIsReleaseGIL(od);
    else if (getHoldGIL(optflgs))
        setIsHoldGIL(od);

    if (getDeprecated(optflgs))
        setIsDeprecated(od);

    if (!isPrivate(od) && !isSignal(od) && (od->common->slot == no_slot || od->common->slot == call_slot))
    {
        od->kwargs = keywordArgs(mod, optflgs, &od->pysig);

        if (od->kwargs != NoKwArgs)
            setUseKeywordArgs(od->common);
    }

    /* See if we want to auto-generate a __len__() method. */
    if (getOptFlag(optflgs, "__len__", bool_flag) != NULL)
    {
        overDef *len;

        len = sipMalloc(sizeof (overDef));

        len->cppname = "__len__";
        len->overflags = SECT_IS_PUBLIC;
        len->pysig.result.atype = ssize_type;
        len->pysig.nrArgs = 0;
        len->cppsig = &len->pysig;

        len->common = findFunction(pt, mod, c_scope, mt_scope, len->cppname,
                TRUE, 0, FALSE);

        if ((len->methodcode = od->methodcode) == NULL)
        {
            char *buf = sipStrdup("            sipRes = (SIP_SSIZE_T)sipCpp->");
            codeBlock *code;

            append(&buf, od->cppname);
            append(&buf, "();\n");

            code = sipMalloc(sizeof (codeBlock));

            code->frag = buf;
            code->filename = "Auto-generated";
            code->linenr = 1;
            code->next = NULL;

            len->methodcode = code;
        }

        len->next = NULL;

        od->next = len;
    }
    else
    {
        od->next = NULL;
    }

    /* Append to the list. */
    for (odp = headp; *odp != NULL; odp = &(*odp)->next)
        ;

    *odp = od;
}


/*
 * Return the Python name based on the C/C++ name and any /PyName/ annotation.
 */
static const char *getPythonName(moduleDef *mod, optFlags *optflgs,
        const char *cname)
{
    const char *pname;
    optFlag *of;
    autoPyNameDef *apnd;

    /* Use the explicit name if given. */
    if ((of = getOptFlag(optflgs, "PyName", name_flag)) != NULL)
        return of->fvalue.sval;

    /* Apply any automatic naming rules. */
    pname = cname;

    for (apnd = mod->autopyname; apnd != NULL; apnd = apnd->next)
    {
        size_t len = strlen(apnd->remove_leading);

        if (strncmp(pname, apnd->remove_leading, len) == 0)
            pname += len;
    }

    return pname;
}


/*
 * Cache a name in a module.  Entries in the cache are stored in order of
 * decreasing length.
 */
nameDef *cacheName(sipSpec *pt, const char *name)
{
    nameDef *nd, **ndp;
    size_t len;

    /* Allow callers to be lazy about checking if there is really a name. */
    if (name == NULL)
        return NULL;

    /* Skip entries that are too large. */
    ndp = &pt->namecache;
    len = strlen(name);

    while (*ndp != NULL && (*ndp)->len > len)
        ndp = &(*ndp)->next;

    /* Check entries that are the right length. */
    for (nd = *ndp; nd != NULL && nd->len == len; nd = nd->next)
        if (memcmp(nd->text, name, len) == 0)
            return nd;

    /* Create a new one. */
    nd = sipMalloc(sizeof (nameDef));

    nd->nameflags = 0;
    nd->text = name;
    nd->len = len;
    nd->next = *ndp;

    *ndp = nd;

    return nd;
}


/*
 * Find (or create) an overloaded function name.
 */
static memberDef *findFunction(sipSpec *pt, moduleDef *mod, classDef *c_scope,
        mappedTypeDef *mt_scope, const char *pname, int hwcode, int nrargs,
        int no_arg_parser)
{
    static struct slot_map {
        const char *name;   /* The slot name. */
        slotType type;      /* The corresponding type. */
        int needs_hwcode;   /* Set if handwritten code is required. */
        int nrargs;         /* Nr. of arguments. */
    } slot_table[] = {
        {"__str__", str_slot, TRUE, 0},
        {"__unicode__", unicode_slot, TRUE, 0},
        {"__int__", int_slot, FALSE, 0},
        {"__long__", long_slot, FALSE, 0},
        {"__float__", float_slot, FALSE, 0},
        {"__len__", len_slot, TRUE, 0},
        {"__contains__", contains_slot, TRUE, 1},
        {"__add__", add_slot, FALSE, 1},
        {"__sub__", sub_slot, FALSE, 1},
        {"__mul__", mul_slot, FALSE, 1},
        {"__div__", div_slot, FALSE, 1},
        {"__mod__", mod_slot, FALSE, 1},
        {"__floordiv__", floordiv_slot, TRUE, 1},
        {"__truediv__", truediv_slot, FALSE, 1},
        {"__and__", and_slot, FALSE, 1},
        {"__or__", or_slot, FALSE, 1},
        {"__xor__", xor_slot, FALSE, 1},
        {"__lshift__", lshift_slot, FALSE, 1},
        {"__rshift__", rshift_slot, FALSE, 1},
        {"__iadd__", iadd_slot, FALSE, 1},
        {"__isub__", isub_slot, FALSE, 1},
        {"__imul__", imul_slot, FALSE, 1},
        {"__idiv__", idiv_slot, FALSE, 1},
        {"__imod__", imod_slot, FALSE, 1},
        {"__ifloordiv__", ifloordiv_slot, TRUE, 1},
        {"__itruediv__", itruediv_slot, FALSE, 1},
        {"__iand__", iand_slot, FALSE, 1},
        {"__ior__", ior_slot, FALSE, 1},
        {"__ixor__", ixor_slot, FALSE, 1},
        {"__ilshift__", ilshift_slot, FALSE, 1},
        {"__irshift__", irshift_slot, FALSE, 1},
        {"__invert__", invert_slot, FALSE, 0},
        {"__call__", call_slot, FALSE, -1},
        {"__getitem__", getitem_slot, FALSE, 1},
        {"__setitem__", setitem_slot, TRUE, 2},
        {"__delitem__", delitem_slot, TRUE, 1},
        {"__lt__", lt_slot, FALSE, 1},
        {"__le__", le_slot, FALSE, 1},
        {"__eq__", eq_slot, FALSE, 1},
        {"__ne__", ne_slot, FALSE, 1},
        {"__gt__", gt_slot, FALSE, 1},
        {"__ge__", ge_slot, FALSE, 1},
        {"__cmp__", cmp_slot, FALSE, 1},
        {"__bool__", bool_slot, TRUE, 0},
        {"__nonzero__", bool_slot, TRUE, 0},
        {"__neg__", neg_slot, FALSE, 0},
        {"__pos__", pos_slot, FALSE, 0},
        {"__abs__", abs_slot, TRUE, 0},
        {"__repr__", repr_slot, TRUE, 0},
        {"__hash__", hash_slot, TRUE, 0},
        {"__index__", index_slot, TRUE, 0},
        {"__iter__", iter_slot, TRUE, 0},
        {"__next__", next_slot, TRUE, 0},
        {"__setattr__", setattr_slot, TRUE, 2},
        {"__delattr__", delattr_slot, TRUE, 1},
        {NULL}
    };

    memberDef *md, **flist;
    struct slot_map *sm;
    slotType st;

    /* Get the slot type. */
    st = no_slot;

    for (sm = slot_table; sm->name != NULL; ++sm)
        if (strcmp(sm->name, pname) == 0)
        {
            if (sm->needs_hwcode && !hwcode)
                yyerror("This Python slot requires %MethodCode");

            if (sm->nrargs >= 0)
            {
                if (mt_scope == NULL && c_scope == NULL)
                {
                    /* Global operators need one extra argument. */
                    if (sm -> nrargs + 1 != nrargs)
                        yyerror("Incorrect number of arguments to global operator");
                }
                else if (sm->nrargs != nrargs)
                    yyerror("Incorrect number of arguments to Python slot");
            }

            st = sm->type;

            break;
        }

    /* Check there is no name clash. */
    checkAttributes(pt, mod, c_scope, mt_scope, pname, TRUE);

    /* See if it already exists. */
    if (mt_scope != NULL)
        flist = &mt_scope->members;
    else if (c_scope != NULL)
        flist = &c_scope->members;
    else
        flist = &mod->othfuncs;

    /* __delattr__ is implemented as __setattr__. */
    if (st == delattr_slot)
    {
        if (inMainModule())
            setIsUsedName(cacheName(pt, pname));

        st = setattr_slot;
        pname = "__setattr__";
    }

    for (md = *flist; md != NULL; md = md->next)
        if (strcmp(md->pyname->text, pname) == 0 && md->module == mod)
            break;

    if (md == NULL)
    {
        /* Create a new one. */
        md = sipMalloc(sizeof (memberDef));

        md->pyname = cacheName(pt, pname);
        md->memberflags = 0;
        md->slot = st;
        md->module = mod;
        md->next = *flist;

        *flist = md;

        if (inMainModule())
            setIsUsedName(md->pyname);

        if (no_arg_parser)
            setNoArgParser(md);
    }
    else if (noArgParser(md))
        yyerror("Another overload has already been defined that is annotated as /NoArgParser/");

    /* Global operators are a subset. */
    if (mt_scope == NULL && c_scope == NULL && st != no_slot && st != neg_slot && st != pos_slot && !isNumberSlot(md) && !isRichCompareSlot(md))
        yyerror("Global operators must be either numeric or comparison operators");

    return md;
}


/*
 * Search a set of flags for a particular one.
 */
static optFlag *findOptFlag(optFlags *flgs, const char *name)
{
    int f;

    for (f = 0; f < flgs->nrFlags; ++f)
    {
        optFlag *of = &flgs->flags[f];

        if (strcmp(of->fname, name) == 0)
            return of;
    }

    return NULL;
}


/*
 * Search a set of flags for a particular one and check its type.
 */
static optFlag *getOptFlag(optFlags *flgs, const char *name, flagType ft)
{
    optFlag *of = findOptFlag(flgs, name);

    if (of != NULL)
    {
        /* An optional name can look like a boolean or a name. */
        if (ft == opt_name_flag)
        {
            if (of->ftype == bool_flag)
            {
                of->ftype = opt_name_flag;
                of->fvalue.sval = NULL;
            }
            else if (of->ftype == name_flag)
            {
                of->ftype = opt_name_flag;
            }
        }

        /* An optional integer can look like a boolean or an integer. */
        if (ft == opt_integer_flag)
        {
            if (of->ftype == bool_flag)
            {
                of->ftype = opt_integer_flag;
                of->fvalue.ival = -1;
            }
            else if (of->ftype == integer_flag)
            {
                of->ftype = opt_integer_flag;
            }
        }

        if (ft != of->ftype)
            yyerror("Annotation has a value of the wrong type");
    }

    return of;
}


/*
 * A name is going to be used as a Python attribute name within a Python scope
 * (ie. a Python dictionary), so check against what we already know is going in
 * the same scope in case there is a clash.
 */
static void checkAttributes(sipSpec *pt, moduleDef *mod, classDef *py_c_scope,
        mappedTypeDef *py_mt_scope, const char *attr, int isfunc)
{
    enumDef *ed;
    varDef *vd;
    classDef *cd;

    /* Check the enums. */

    for (ed = pt->enums; ed != NULL; ed = ed->next)
    {
        enumMemberDef *emd;

        if (ed->pyname == NULL)
            continue;

        if (py_c_scope != NULL)
        {
            if (ed->ecd != py_c_scope)
                continue;
        }
        else if (py_mt_scope != NULL)
        {
            if (ed->emtd != py_mt_scope)
                continue;
        }
        else if (ed->ecd != NULL || ed->emtd != NULL)
        {
            continue;
        }

        if (strcmp(ed->pyname->text, attr) == 0)
            yyerror("There is already an enum in scope with the same Python name");

        for (emd = ed->members; emd != NULL; emd = emd->next)
            if (strcmp(emd->pyname->text, attr) == 0)
                yyerror("There is already an enum member in scope with the same Python name");
    }

    /*
     * Only check the members if this attribute isn't a member because we
     * can handle members with the same name in the same scope.
     */
    if (!isfunc)
    {
        memberDef *md, *membs;
        overDef *overs;

        if (py_mt_scope != NULL)
        {
            membs = py_mt_scope->members;
            overs = py_mt_scope->overs;
        }
        else if (py_c_scope != NULL)
        {
            membs = py_c_scope->members;
            overs = py_c_scope->overs;
        }
        else
        {
            membs = mod->othfuncs;
            overs = mod->overs;
        }

        for (md = membs; md != NULL; md = md->next)
        {
            overDef *od;

            if (strcmp(md->pyname->text, attr) != 0)
                continue;

            /* Check for a conflict with all overloads. */
            for (od = overs; od != NULL; od = od->next)
            {
                if (od->common != md)
                    continue;

                yyerror("There is already a function in scope with the same Python name");
            }
        }
    }

    /* If the scope was a mapped type then that's all we have to check. */
    if (py_mt_scope != NULL)
        return;

    /* Check the variables. */
    for (vd = pt->vars; vd != NULL; vd = vd->next)
    {
        if (vd->ecd != py_c_scope)
            continue;

        if (strcmp(vd->pyname->text,attr) == 0)
            yyerror("There is already a variable in scope with the same Python name");
    }

    /* Check the classes. */
    for (cd = pt->classes; cd != NULL; cd = cd->next)
    {
        if (cd->ecd != py_c_scope || cd->pyname == NULL)
            continue;

        if (strcmp(cd->pyname->text, attr) == 0 && !isExternal(cd))
            yyerror("There is already a class or namespace in scope with the same Python name");
    }

    /* Check the exceptions. */
    if (py_c_scope == NULL)
    {
        exceptionDef *xd;

        for (xd = pt->exceptions; xd != NULL; xd = xd->next)
            if (xd->pyname != NULL && strcmp(xd->pyname, attr) == 0)
                yyerror("There is already an exception with the same Python name");
    }

    /* Check the properties. */
    if (py_c_scope != NULL)
    {
        propertyDef *pd;

        for (pd = py_c_scope->properties; pd != NULL; pd = pd->next)
            if (strcmp(pd->name->text, attr) == 0)
                yyerror("There is already a property with the same name");
    }
}


/*
 * Append a code block to a list of them.  Append is needed to give the
 * specifier easy control over the order of the documentation.
 */
void appendCodeBlock(codeBlock **headp, codeBlock *new)
{
    while (*headp != NULL)
        headp = &(*headp)->next;

    *headp = new;
}


/*
 * Handle the end of a fully parsed a file.
 */
static void handleEOF()
{
    /*
     * Check that the number of nested if's is the same as when we started
     * the file.
     */

    if (skipStackPtr > currentContext.ifdepth)
        fatal("Too many %%If statements in %s\n", previousFile);

    if (skipStackPtr < currentContext.ifdepth)
        fatal("Too many %%End statements in %s\n", previousFile);
}


/*
 * Handle the end of a fully parsed a module.
 */
static void handleEOM()
{
    moduleDef *from;

    /* Check it has been named. */
    if (currentModule->name == NULL)
        fatal("No %%Module has been specified for module defined in %s\n",
                previousFile);

    from = currentContext.prevmod;

    if (from != NULL && from->encoding == no_type)
        from->encoding = currentModule->encoding;

    /* The previous module is now current. */
    currentModule = from;
}


/*
 * Find an existing qualifier.
 */
static qualDef *findQualifier(const char *name)
{
    moduleDef *mod;

    for (mod = currentSpec->modules; mod != NULL; mod = mod->next)
    {
        qualDef *qd;

        for (qd = mod->qualifiers; qd != NULL; qd = qd->next)
            if (strcmp(qd->name, name) == 0)
                return qd;
    }

    /* Qualifiers corresponding to the SIP version are created on the fly. */
    if (name[0] == 'S' && name[1] == 'I' && name[2] == 'P' && name[3] == '_')
    {
        const char *cp = &name[3];
        int major, minor, patch;

        cp = getInt(cp, &major);
        cp = getInt(cp, &minor);
        cp = getInt(cp, &patch);

        if (*cp != '\0')
            yyerror("Unexpected character after SIP version number");

        return allocQualifier(currentModule, -1,
                (major << 16) | (minor << 8) | patch, name, time_qualifier);
    }

    return NULL;
}


/*
 * Get an integer from string.
 */
static const char *getInt(const char *cp, int *ip)
{
    /* Handle the default value. */
    *ip = 0;

    if (*cp == '\0')
        return cp;

    /* There must be a leading underscore. */
    if (*cp++ != '_')
        yyerror("An underscore must separate the parts of a SIP version number");

    while (isdigit(*cp))
    {
        *ip *= 10;
        *ip += *cp - '0';
        ++cp;
    }

    return cp;
}


/*
 * Find an existing API.
 */
apiVersionRangeDef *findAPI(sipSpec *pt, const char *name)
{
    moduleDef *mod;

    for (mod = pt->modules; mod != NULL; mod = mod->next)
    {
        apiVersionRangeDef *avd;

        for (avd = mod->api_versions; avd != NULL; avd = avd->next)
            if (strcmp(avd->api_name->text, name) == 0)
                return avd;
    }

    return NULL;
}


/*
 * Return a copy of a scoped name.
 */
scopedNameDef *copyScopedName(scopedNameDef *snd)
{
    scopedNameDef *head;

    head = NULL;

    while (snd != NULL)
    {
        appendScopedName(&head,text2scopePart(snd -> name));
        snd = snd -> next;
    }

    return head;
}


/*
 * Append a name to a list of scopes.
 */
void appendScopedName(scopedNameDef **headp,scopedNameDef *newsnd)
{
    while (*headp != NULL)
        headp = &(*headp) -> next;

    *headp = newsnd;
}


/*
 * Free a scoped name - but not the text itself.
 */
void freeScopedName(scopedNameDef *snd)
{
    while (snd != NULL)
    {
        scopedNameDef *next = snd -> next;

        free(snd);

        snd = next;
    }
}


/*
 * Convert a text string to a scope part structure.
 */
static scopedNameDef *text2scopePart(char *text)
{
    scopedNameDef *snd;

    snd = sipMalloc(sizeof (scopedNameDef));

    snd->name = text;
    snd->next = NULL;

    return snd;
}


/*
 * Convert a text string to a fully scoped name.
 */
static scopedNameDef *text2scopedName(ifaceFileDef *scope, char *text)
{
    return scopeScopedName(scope, text2scopePart(text));
}


/*
 * Prepend any current scope to a scoped name.
 */
static scopedNameDef *scopeScopedName(ifaceFileDef *scope, scopedNameDef *name)
{
    scopedNameDef *snd;

    snd = (scope != NULL ? copyScopedName(scope->fqcname) : NULL);

    appendScopedName(&snd, name);

    return snd;
}


/*
 * Return a pointer to the tail part of a scoped name.
 */
char *scopedNameTail(scopedNameDef *snd)
{
    if (snd == NULL)
        return NULL;

    while (snd -> next != NULL)
        snd = snd -> next;

    return snd -> name;
}


/*
 * Push the given scope onto the scope stack.
 */
static void pushScope(classDef *scope)
{
    if (currentScopeIdx >= MAX_NESTED_SCOPE)
        fatal("Internal error: increase the value of MAX_NESTED_SCOPE\n");

    scopeStack[currentScopeIdx] = scope;
    sectFlagsStack[currentScopeIdx] = sectionFlags;

    ++currentScopeIdx;
}


/*
 * Pop the scope stack.
 */
static void popScope(void)
{
    if (currentScopeIdx > 0)
        sectionFlags = sectFlagsStack[--currentScopeIdx];
}


/*
 * Return non-zero if the current input should be parsed rather than be
 * skipped.
 */
static int notSkipping()
{
    return (skipStackPtr == 0 ? TRUE : skipStack[skipStackPtr - 1]);
}


/*
 * Return the value of an expression involving a time period.
 */
static int timePeriod(const char *lname, const char *uname)
{
    int this, line;
    qualDef *qd, *lower, *upper;
    moduleDef *mod;

    if (lname == NULL)
        lower = NULL;
    else if ((lower = findQualifier(lname)) == NULL || lower->qtype != time_qualifier)
        yyerror("Lower bound is not a time version");

    if (uname == NULL)
        upper = NULL;
    else if ((upper = findQualifier(uname)) == NULL || upper->qtype != time_qualifier)
        yyerror("Upper bound is not a time version");

    /* Sanity checks on the bounds. */
    if (lower == NULL && upper == NULL)
        yyerror("Lower and upper bounds cannot both be omitted");

    if (lower != NULL && upper != NULL)
    {
        if (lower->module != upper->module || lower->line != upper->line)
            yyerror("Lower and upper bounds are from different timelines");

        if (lower == upper)
            yyerror("Lower and upper bounds must be different");

        if (lower->order > upper->order)
            yyerror("Later version specified as lower bound");
    }

    /* Go through each slot in the relevant timeline. */
    if (lower != NULL)
    {
        mod = lower->module;
        line = lower->line;
    }
    else
    {
        mod = upper->module;
        line = upper->line;
    }

    /* Handle the SIP version number pseudo-timeline. */
    if (line < 0)
    {
        if (lower != NULL && lower->order > SIP_VERSION)
            return FALSE;

        if (upper != NULL && upper->order <= SIP_VERSION)
            return FALSE;

        return TRUE;
    }

    this = FALSE;

    for (qd = mod->qualifiers; qd != NULL; qd = qd->next)
    {
        if (qd->qtype != time_qualifier || qd->line != line)
            continue;

        if (lower != NULL && qd->order < lower->order)
            continue;

        if (upper != NULL && qd->order >= upper->order)
            continue;

        /*
         * This is within the required range so if it is also needed then the
         * expression is true.
         */
        if (isNeeded(qd))
        {
            this = TRUE;
            break;
        }
    }

    return this;
}


/*
 * Return the value of an expression involving a single platform or feature.
 */
static int platOrFeature(char *name,int optnot)
{
    int this;
    qualDef *qd;

    if ((qd = findQualifier(name)) == NULL || qd -> qtype == time_qualifier)
        yyerror("No such platform or feature");

    /* Assume this sub-expression is false. */

    this = FALSE;

    if (qd -> qtype == feature_qualifier)
    {
        if (!excludedFeature(excludedQualifiers,qd))
            this = TRUE;
    }
    else if (isNeeded(qd))
        this = TRUE;

    if (optnot)
        this = !this;

    return this;
}


/*
 * Return TRUE if the given qualifier is excluded.
 */
int excludedFeature(stringList *xsl,qualDef *qd)
{
    while (xsl != NULL)
    {
        if (strcmp(qd -> name,xsl -> s) == 0)
            return TRUE;

        xsl = xsl -> next;
    }

    return FALSE;
}


/*
 * Return TRUE if the given qualifier is needed.
 */
static int isNeeded(qualDef *qd)
{
    stringList *sl;

    for (sl = neededQualifiers; sl != NULL; sl = sl -> next)
        if (strcmp(qd -> name,sl -> s) == 0)
            return TRUE;

    return FALSE;
}


/*
 * Return the current scope.  currentScope() is only valid if notSkipping()
 * returns non-zero.
 */
static classDef *currentScope(void)
{
    return (currentScopeIdx > 0 ? scopeStack[currentScopeIdx - 1] : NULL);
}


/*
 * Create a new qualifier.
 */
static void newQualifier(moduleDef *mod, int line, int order, const char *name,
        qualType qt)
{
    /* Check it doesn't already exist. */
    if (findQualifier(name) != NULL)
        yyerror("Version is already defined");

    allocQualifier(mod, line, order, name, qt);
}


/*
 * Allocate a new qualifier.
 */
static qualDef *allocQualifier(moduleDef *mod, int line, int order,
        const char *name, qualType qt)
{
    qualDef *qd;

    qd = sipMalloc(sizeof (qualDef));

    qd->name = name;
    qd->qtype = qt;
    qd->module = mod;
    qd->line = line;
    qd->order = order;
    qd->next = mod->qualifiers;

    mod->qualifiers = qd;

    return qd;
}


/*
 * Create a new imported module.
 */
static void newImport(char *filename)
{
    moduleDef *from, *mod;
    moduleListDef *mld;

    /* Create a new module if it has not already been defined. */
    for (mod = currentSpec->modules; mod != NULL; mod = mod->next)
        if (strcmp(mod->file, filename) == 0)
            break;

    from = currentModule;

    if (mod == NULL)
    {
        newModule(NULL, filename);
        mod = currentModule;
    }
    else if (from->encoding == no_type)
    {
        /* Import any defaults from the already parsed module. */
        from->encoding = mod->encoding;
    }

    /* Add the new import unless it has already been imported. */
    for (mld = from->imports; mld != NULL; mld = mld->next)
        if (mld->module == mod)
            return;

    mld = sipMalloc(sizeof (moduleListDef));
    mld->module = mod;
    mld->next = from->imports;

    from->imports = mld;
}


/*
 * Set up pointers to hook names.
 */
static void getHooks(optFlags *optflgs,char **pre,char **post)
{
    optFlag *of;

    if ((of = getOptFlag(optflgs,"PreHook",name_flag)) != NULL)
        *pre = of -> fvalue.sval;
    else
        *pre = NULL;

    if ((of = getOptFlag(optflgs,"PostHook",name_flag)) != NULL)
        *post = of -> fvalue.sval;
    else
        *post = NULL;
}


/*
 * Get the /Transfer/ option flag.
 */
static int getTransfer(optFlags *optflgs)
{
    return (getOptFlag(optflgs, "Transfer", bool_flag) != NULL);
}


/*
 * Get the /ReleaseGIL/ option flag.
 */
static int getReleaseGIL(optFlags *optflgs)
{
    return (getOptFlag(optflgs, "ReleaseGIL", bool_flag) != NULL);
}


/*
 * Get the /HoldGIL/ option flag.
 */
static int getHoldGIL(optFlags *optflgs)
{
    return (getOptFlag(optflgs, "HoldGIL", bool_flag) != NULL);
}


/*
 * Get the /Deprecated/ option flag.
 */
static int getDeprecated(optFlags *optflgs)
{
    return (getOptFlag(optflgs, "Deprecated", bool_flag) != NULL);
}


/*
 * Get the /AllowNone/ option flag.
 */
static int getAllowNone(optFlags *optflgs)
{
    return (getOptFlag(optflgs, "AllowNone", bool_flag) != NULL);
}


/*
 * Get the /DocType/ option flag.
 */
static const char *getDocType(optFlags *optflgs)
{
    optFlag *of = getOptFlag(optflgs, "DocType", string_flag);

    if (of == NULL)
        return NULL;

    return of->fvalue.sval;
}


/*
 * Get the /DocValue/ option flag.
 */
static const char *getDocValue(optFlags *optflgs)
{
    optFlag *of = getOptFlag(optflgs, "DocValue", string_flag);

    if (of == NULL)
        return NULL;

    return of->fvalue.sval;
}


/*
 * Return TRUE if the PyQt3 plugin was specified.
 */
int pluginPyQt3(sipSpec *pt)
{
    return stringFind(pt->plugins, "PyQt3");
}


/*
 * Return TRUE if the PyQt4 plugin was specified.
 */
int pluginPyQt4(sipSpec *pt)
{
    return stringFind(pt->plugins, "PyQt4");
}


/*
 * Return TRUE if a list of strings contains a given entry.
 */
static int stringFind(stringList *sl, const char *s)
{
    while (sl != NULL)
    {
        if (strcmp(sl->s, s) == 0)
            return TRUE;

        sl = sl->next;
    }

    return FALSE;
}


/*
 * Set the name of a module.
 */
static void setModuleName(sipSpec *pt, moduleDef *mod, const char *fullname)
{
    mod->fullname = cacheName(pt, fullname);

    if (inMainModule())
        setIsUsedName(mod->fullname);

    if ((mod->name = strrchr(fullname, '.')) != NULL)
        mod->name++;
    else
        mod->name = fullname;
}


/*
 * Define a new class and set its name.
 */
static void defineClass(scopedNameDef *snd, classList *supers, optFlags *of)
{
    classDef *cd, *c_scope = currentScope();

    cd = newClass(currentSpec, class_iface, getAPIRange(of),
            scopeScopedName((c_scope != NULL ? c_scope->iff : NULL), snd));
    cd->supers = supers;

    pushScope(cd);
}


/*
 * Complete the definition of a class.
 */
static classDef *completeClass(scopedNameDef *snd, optFlags *of, int has_def)
{
    classDef *cd = currentScope();

    /* See if the class was defined or just declared. */
    if (has_def)
    {
        if (snd->next != NULL)
            yyerror("A scoped name cannot be given in a class/struct definition");

    }
    else if (cd->supers != NULL)
        yyerror("Class/struct has super-classes but no definition");
    else
        setIsOpaque(cd);

    finishClass(currentSpec, currentModule, cd, of);
    popScope();

    /*
     * Check that external classes have only been declared at the global scope.
     */
    if (isExternal(cd) && currentScope() != NULL)
        yyerror("External classes/structs can only be declared in the global scope");

    return cd;
}


/*
 * Add a variable to the list so that the list remains sorted.
 */
static void addVariable(sipSpec *pt, varDef *vd)
{
    varDef **at = &pt->vars;

    while (*at != NULL)
    {
        if (strcmp(vd->pyname->text, (*at)->pyname->text) < 0)
            break;

        at = &(*at)->next;
    }

    vd->next = *at;
    *at = vd;
}


/*
 * Update a type according to optional flags.
 */
static void applyTypeFlags(moduleDef *mod, argDef *ad, optFlags *flags)
{
    ad->doctype = getDocType(flags);

    if (getOptFlag(flags, "PyInt", bool_flag) != NULL)
    {
        if (ad->atype == string_type)
            ad->atype = byte_type;
        else if (ad->atype == sstring_type)
            ad->atype = sbyte_type;
        else if (ad->atype == ustring_type)
            ad->atype = ubyte_type;
    }

    if (ad->atype == string_type && !isArray(ad) && !isReference(ad))
    {
        optFlag *of;

        if ((of = getOptFlag(flags, "Encoding", string_flag)) == NULL)
        {
            if (mod->encoding != no_type)
                ad->atype = mod->encoding;
            else
                ad->atype = string_type;
        }
        else if ((ad->atype = convertEncoding(of->fvalue.sval)) == no_type)
            yyerror("The value of the /Encoding/ annotation must be one of \"ASCII\", \"Latin-1\", \"UTF-8\" or \"None\"");
    }
}


/*
 * Return the keyword argument support converted from a string.
 */
static KwArgs convertKwArgs(const char *kwargs)
{
    if (strcmp(kwargs, "None") == 0)
        return NoKwArgs;

    if (strcmp(kwargs, "All") == 0)
        return AllKwArgs;

    if (strcmp(kwargs, "Optional") == 0)
        return OptionalKwArgs;

    yyerror("The style of keyword argument support must be one of \"All\", \"Optional\" or \"None\"");
}


/*
 * Return the argument type for a string with the given encoding or no_type if
 * the encoding was invalid.
 */
static argType convertEncoding(const char *encoding)
{
    if (strcmp(encoding, "ASCII") == 0)
        return ascii_string_type;

    if (strcmp(encoding, "Latin-1") == 0)
        return latin1_string_type;

    if (strcmp(encoding, "UTF-8") == 0)
        return utf8_string_type;

    if (strcmp(encoding, "None") == 0)
        return string_type;

    return no_type;
}


/*
 * Get the /API/ option flag.
 */
static apiVersionRangeDef *getAPIRange(optFlags *optflgs)
{
    optFlag *of;

    if ((of = getOptFlag(optflgs, "API", api_range_flag)) == NULL)
        return NULL;

    return of->fvalue.aval;
}


/*
 * Return the API range structure and version number corresponding to the
 * given API range.
 */
static apiVersionRangeDef *convertAPIRange(moduleDef *mod, nameDef *name,
        int from, int to)
{
    int index;
    apiVersionRangeDef *avd, **avdp;

    /* Handle the trivial case. */
    if (from == 0 && to == 0)
        return NULL;

    for (index = 0, avdp = &mod->api_ranges; (*avdp) != NULL; avdp = &(*avdp)->next, ++index)
    {
        avd = *avdp;

        if (avd->api_name == name && avd->from == from && avd->to == to)
            return avd;
    }

    /* The new one must be appended so that version numbers remain valid. */
    avd = sipMalloc(sizeof (apiVersionRangeDef));

    avd->api_name = name;
    avd->from = from;
    avd->to = to;
    avd->index = index;

    avd->next = NULL;
    *avdp = avd;

    return avd;
}


/*
 * Return the style of keyword argument support for a signature.
 */
static KwArgs keywordArgs(moduleDef *mod, optFlags *optflgs, signatureDef *sd)
{
    KwArgs kwargs;
    optFlag *ka_anno, *no_ka_anno;

    /* Get the default. */
    kwargs = mod->kwargs;

    /*
     * Get the possible annotations allowing /KeywordArgs/ to have different
     * types of values.
     */
    ka_anno = findOptFlag(optflgs, "KeywordArgs");
    no_ka_anno = getOptFlag(optflgs, "NoKeywordArgs", bool_flag);

    if (no_ka_anno != NULL)
    {
        if (ka_anno != NULL)
            yyerror("/KeywordArgs/ and /NoKeywordArgs/ cannot both be specified");

        deprecated("/NoKeywordArgs/ is deprecated, use /KeywordArgs=\"None\" instead");

        kwargs = NoKwArgs;
    }
    else if (ka_anno != NULL)
    {
        /* A string value is the non-deprecated type. */
        if (ka_anno->ftype == string_flag)
        {
            kwargs = convertKwArgs(ka_anno->fvalue.sval);
        }
        else
        {
            deprecated("/KeywordArgs/ is deprecated, use /KeywordArgs=\"All\" instead");

            /* Get it again to check the type. */
            ka_anno = getOptFlag(optflgs, "KeywordArgs", bool_flag);
        }
    }

    /* An ellipsis cannot be used with keyword arguments. */
    if (sd->nrArgs > 0 && sd->args[sd->nrArgs - 1].atype == ellipsis_type)
        kwargs = NoKwArgs;

    if (kwargs != NoKwArgs)
    {
        int a, is_name = FALSE;

        /*
         * Mark argument names as being used and check there is at least one.
         */
        for (a = 0; a < sd->nrArgs; ++a)
        {
            argDef *ad = &sd->args[a];

            if (kwargs == OptionalKwArgs && ad->defval == NULL)
                continue;

            if (ad->name != NULL)
            {
                setIsUsedName(ad->name);
                is_name = TRUE;
            }
        }

        if (!is_name)
            kwargs = NoKwArgs;
    }

    return kwargs;
}


/*
 * Extract the version of a string value optionally associated with a
 * particular feature.
 */
static char *convertFeaturedString(char *fs)
{
    while (fs != NULL)
    {
        char *next, *value;

        /* Individual values are ';' separated. */
        if ((next = strchr(fs, ';')) != NULL)
            *next++ = '\0';

        /* Features and values are ':' separated. */
        if ((value = strchr(fs, ':')) == NULL)
        {
            /* This is an unconditional value so just return it. */
            return strip(fs);
        }

        *value++ = '\0';

        if (isEnabledFeature(strip(fs)))
            return strip(value);

        fs = next;
    }

    /* No value was enabled. */
    return NULL;
}


/*
 * Return the stripped version of a string.
 */
static char *strip(char *s)
{
    while (*s == ' ')
        ++s;

    if (*s != '\0')
    {
        char *cp = &s[strlen(s) - 1];

        while (*cp == ' ')
            *cp-- = '\0';
    }

    return s;
}


/*
 * Return TRUE if the given feature is enabled.
 */
static int isEnabledFeature(const char *name)
{
    qualDef *qd;

    if ((qd = findQualifier(name)) == NULL || qd->qtype != feature_qualifier)
        yyerror("No such feature");

    return !excludedFeature(excludedQualifiers, qd);
}


/*
 * Add a property definition to a class.
 */
static void addProperty(sipSpec *pt, moduleDef *mod, classDef *cd,
        const char *name, const char *get, const char *set,
        codeBlock *docstring)
{
    propertyDef *pd;

    checkAttributes(pt, mod, cd, NULL, name, FALSE);

    pd = sipMalloc(sizeof (propertyDef));

    pd->name = cacheName(pt, name);
    pd->get = get;
    pd->set = set;
    pd->docstring = docstring;
    pd->next = cd->properties;

    cd->properties = pd;

    if (inMainModule())
        setIsUsedName(pd->name);
}


/*
 * Configure a module and return the (possibly new) current module.
 */
static moduleDef *configureModule(sipSpec *pt, moduleDef *module,
        const char *filename, const char *name, int version, int c_module,
        KwArgs kwargs, int use_arg_names, codeBlock *docstring)
{
    moduleDef *mod;

    /* Check the module hasn't already been defined. */
    for (mod = pt->modules; mod != NULL; mod = mod->next)
        if (mod->fullname != NULL && strcmp(mod->fullname->text, name) == 0)
            yyerror("Module is already defined");

    /*
     * If we are in a container module then create a component module and make
     * it current.
     */
    if (isContainer(module) || module->container != NULL)
    {
        mod = allocModule();

        mod->file = filename;
        mod->container = (isContainer(module) ? module : module->container);

        module = mod;
    }

    setModuleName(pt, module, name);
    module->kwargs = kwargs;
    module->version = version;
    module->docstring = docstring;

    if (use_arg_names)
        setUseArgNames(module);

    if (pt->genc < 0)
        pt->genc = c_module;
    else if (pt->genc != c_module)
        yyerror("Cannot mix C and C++ modules");

    return module;
}


/*
 * Add a Python naming rule to a module.
 */
static void addAutoPyName(moduleDef *mod, const char *remove_leading)
{
    autoPyNameDef *apnd, **apndp;

    for (apndp = &mod->autopyname; *apndp != NULL; apndp = &(*apndp)->next)
        ;

    apnd = sipMalloc(sizeof (autoPyNameDef));
    apnd->remove_leading = remove_leading;
    apnd->next = *apndp;

    *apndp = apnd;
}


/*
 * Check that no invalid or unknown annotations are given.
 */
static void checkAnnos(optFlags *annos, const char *valid[])
{
    if (parsingCSignature && annos->nrFlags != 0)
    {
        deprecated("Annotations should not be used in explicit C/C++ signatures");
    }
    else
    {
        int i;

        for (i = 0; i < annos->nrFlags; i++)
        {
            const char **name;

            for (name = valid; *name != NULL; ++name)
                if (strcmp(*name, annos->flags[i].fname) == 0)
                    break;

            if (*name == NULL)
                deprecated("Annotation is invalid");
        }
    }
}


/*
 * Check that no annotations were given.
 */
static void checkNoAnnos(optFlags *annos, const char *msg)
{
    if (annos->nrFlags != 0)
        deprecated(msg);
}

