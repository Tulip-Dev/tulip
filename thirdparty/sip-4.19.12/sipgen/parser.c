/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     TK_API = 258,
     TK_AUTOPYNAME = 259,
     TK_DEFDOCSTRFMT = 260,
     TK_DEFDOCSTRSIG = 261,
     TK_DEFENCODING = 262,
     TK_PLUGIN = 263,
     TK_VIRTERRORHANDLER = 264,
     TK_EXPTYPEHINTCODE = 265,
     TK_TYPEHINTCODE = 266,
     TK_DOCSTRING = 267,
     TK_DOC = 268,
     TK_EXPORTEDDOC = 269,
     TK_EXTRACT = 270,
     TK_MAKEFILE = 271,
     TK_ACCESSCODE = 272,
     TK_GETCODE = 273,
     TK_SETCODE = 274,
     TK_PREINITCODE = 275,
     TK_INITCODE = 276,
     TK_POSTINITCODE = 277,
     TK_FINALCODE = 278,
     TK_UNITCODE = 279,
     TK_UNITPOSTINCLUDECODE = 280,
     TK_MODCODE = 281,
     TK_TYPECODE = 282,
     TK_PREPYCODE = 283,
     TK_COPYING = 284,
     TK_MAPPEDTYPE = 285,
     TK_CODELINE = 286,
     TK_IF = 287,
     TK_END = 288,
     TK_NAME_VALUE = 289,
     TK_PATH_VALUE = 290,
     TK_STRING_VALUE = 291,
     TK_VIRTUALCATCHERCODE = 292,
     TK_TRAVERSECODE = 293,
     TK_CLEARCODE = 294,
     TK_GETBUFFERCODE = 295,
     TK_RELEASEBUFFERCODE = 296,
     TK_READBUFFERCODE = 297,
     TK_WRITEBUFFERCODE = 298,
     TK_SEGCOUNTCODE = 299,
     TK_CHARBUFFERCODE = 300,
     TK_PICKLECODE = 301,
     TK_VIRTUALCALLCODE = 302,
     TK_METHODCODE = 303,
     TK_PREMETHODCODE = 304,
     TK_INSTANCECODE = 305,
     TK_FROMTYPE = 306,
     TK_TOTYPE = 307,
     TK_TOSUBCLASS = 308,
     TK_INCLUDE = 309,
     TK_OPTINCLUDE = 310,
     TK_IMPORT = 311,
     TK_EXPHEADERCODE = 312,
     TK_MODHEADERCODE = 313,
     TK_TYPEHEADERCODE = 314,
     TK_MODULE = 315,
     TK_CMODULE = 316,
     TK_CONSMODULE = 317,
     TK_COMPOMODULE = 318,
     TK_CLASS = 319,
     TK_STRUCT = 320,
     TK_PUBLIC = 321,
     TK_PROTECTED = 322,
     TK_PRIVATE = 323,
     TK_SIGNALS = 324,
     TK_SIGNAL_METHOD = 325,
     TK_SLOTS = 326,
     TK_SLOT_METHOD = 327,
     TK_BOOL = 328,
     TK_SHORT = 329,
     TK_INT = 330,
     TK_LONG = 331,
     TK_FLOAT = 332,
     TK_DOUBLE = 333,
     TK_CHAR = 334,
     TK_WCHAR_T = 335,
     TK_VOID = 336,
     TK_PYOBJECT = 337,
     TK_PYTUPLE = 338,
     TK_PYLIST = 339,
     TK_PYDICT = 340,
     TK_PYCALLABLE = 341,
     TK_PYSLICE = 342,
     TK_PYTYPE = 343,
     TK_PYBUFFER = 344,
     TK_VIRTUAL = 345,
     TK_ENUM = 346,
     TK_SIGNED = 347,
     TK_UNSIGNED = 348,
     TK_SCOPE = 349,
     TK_LOGICAL_OR = 350,
     TK_CONST = 351,
     TK_STATIC = 352,
     TK_SIPSIGNAL = 353,
     TK_SIPSLOT = 354,
     TK_SIPANYSLOT = 355,
     TK_SIPRXCON = 356,
     TK_SIPRXDIS = 357,
     TK_SIPSLOTCON = 358,
     TK_SIPSLOTDIS = 359,
     TK_SIPSSIZET = 360,
     TK_NUMBER_VALUE = 361,
     TK_REAL_VALUE = 362,
     TK_TYPEDEF = 363,
     TK_NAMESPACE = 364,
     TK_TIMELINE = 365,
     TK_PLATFORMS = 366,
     TK_FEATURE = 367,
     TK_LICENSE = 368,
     TK_QCHAR_VALUE = 369,
     TK_TRUE_VALUE = 370,
     TK_FALSE_VALUE = 371,
     TK_NULL_VALUE = 372,
     TK_OPERATOR = 373,
     TK_THROW = 374,
     TK_QOBJECT = 375,
     TK_EXCEPTION = 376,
     TK_RAISECODE = 377,
     TK_VIRTERRORCODE = 378,
     TK_EXPLICIT = 379,
     TK_TEMPLATE = 380,
     TK_FINAL = 381,
     TK_ELLIPSIS = 382,
     TK_DEFMETATYPE = 383,
     TK_DEFSUPERTYPE = 384,
     TK_PROPERTY = 385,
     TK_HIDE_NS = 386,
     TK_FORMAT = 387,
     TK_GET = 388,
     TK_ID = 389,
     TK_KWARGS = 390,
     TK_LANGUAGE = 391,
     TK_LICENSEE = 392,
     TK_NAME = 393,
     TK_OPTIONAL = 394,
     TK_ORDER = 395,
     TK_REMOVELEADING = 396,
     TK_SET = 397,
     TK_SIGNATURE = 398,
     TK_TIMESTAMP = 399,
     TK_TYPE = 400,
     TK_USEARGNAMES = 401,
     TK_USELIMITEDAPI = 402,
     TK_ALLRAISEPYEXC = 403,
     TK_CALLSUPERINIT = 404,
     TK_DEFERRORHANDLER = 405,
     TK_VERSION = 406
   };
#endif
/* Tokens.  */
#define TK_API 258
#define TK_AUTOPYNAME 259
#define TK_DEFDOCSTRFMT 260
#define TK_DEFDOCSTRSIG 261
#define TK_DEFENCODING 262
#define TK_PLUGIN 263
#define TK_VIRTERRORHANDLER 264
#define TK_EXPTYPEHINTCODE 265
#define TK_TYPEHINTCODE 266
#define TK_DOCSTRING 267
#define TK_DOC 268
#define TK_EXPORTEDDOC 269
#define TK_EXTRACT 270
#define TK_MAKEFILE 271
#define TK_ACCESSCODE 272
#define TK_GETCODE 273
#define TK_SETCODE 274
#define TK_PREINITCODE 275
#define TK_INITCODE 276
#define TK_POSTINITCODE 277
#define TK_FINALCODE 278
#define TK_UNITCODE 279
#define TK_UNITPOSTINCLUDECODE 280
#define TK_MODCODE 281
#define TK_TYPECODE 282
#define TK_PREPYCODE 283
#define TK_COPYING 284
#define TK_MAPPEDTYPE 285
#define TK_CODELINE 286
#define TK_IF 287
#define TK_END 288
#define TK_NAME_VALUE 289
#define TK_PATH_VALUE 290
#define TK_STRING_VALUE 291
#define TK_VIRTUALCATCHERCODE 292
#define TK_TRAVERSECODE 293
#define TK_CLEARCODE 294
#define TK_GETBUFFERCODE 295
#define TK_RELEASEBUFFERCODE 296
#define TK_READBUFFERCODE 297
#define TK_WRITEBUFFERCODE 298
#define TK_SEGCOUNTCODE 299
#define TK_CHARBUFFERCODE 300
#define TK_PICKLECODE 301
#define TK_VIRTUALCALLCODE 302
#define TK_METHODCODE 303
#define TK_PREMETHODCODE 304
#define TK_INSTANCECODE 305
#define TK_FROMTYPE 306
#define TK_TOTYPE 307
#define TK_TOSUBCLASS 308
#define TK_INCLUDE 309
#define TK_OPTINCLUDE 310
#define TK_IMPORT 311
#define TK_EXPHEADERCODE 312
#define TK_MODHEADERCODE 313
#define TK_TYPEHEADERCODE 314
#define TK_MODULE 315
#define TK_CMODULE 316
#define TK_CONSMODULE 317
#define TK_COMPOMODULE 318
#define TK_CLASS 319
#define TK_STRUCT 320
#define TK_PUBLIC 321
#define TK_PROTECTED 322
#define TK_PRIVATE 323
#define TK_SIGNALS 324
#define TK_SIGNAL_METHOD 325
#define TK_SLOTS 326
#define TK_SLOT_METHOD 327
#define TK_BOOL 328
#define TK_SHORT 329
#define TK_INT 330
#define TK_LONG 331
#define TK_FLOAT 332
#define TK_DOUBLE 333
#define TK_CHAR 334
#define TK_WCHAR_T 335
#define TK_VOID 336
#define TK_PYOBJECT 337
#define TK_PYTUPLE 338
#define TK_PYLIST 339
#define TK_PYDICT 340
#define TK_PYCALLABLE 341
#define TK_PYSLICE 342
#define TK_PYTYPE 343
#define TK_PYBUFFER 344
#define TK_VIRTUAL 345
#define TK_ENUM 346
#define TK_SIGNED 347
#define TK_UNSIGNED 348
#define TK_SCOPE 349
#define TK_LOGICAL_OR 350
#define TK_CONST 351
#define TK_STATIC 352
#define TK_SIPSIGNAL 353
#define TK_SIPSLOT 354
#define TK_SIPANYSLOT 355
#define TK_SIPRXCON 356
#define TK_SIPRXDIS 357
#define TK_SIPSLOTCON 358
#define TK_SIPSLOTDIS 359
#define TK_SIPSSIZET 360
#define TK_NUMBER_VALUE 361
#define TK_REAL_VALUE 362
#define TK_TYPEDEF 363
#define TK_NAMESPACE 364
#define TK_TIMELINE 365
#define TK_PLATFORMS 366
#define TK_FEATURE 367
#define TK_LICENSE 368
#define TK_QCHAR_VALUE 369
#define TK_TRUE_VALUE 370
#define TK_FALSE_VALUE 371
#define TK_NULL_VALUE 372
#define TK_OPERATOR 373
#define TK_THROW 374
#define TK_QOBJECT 375
#define TK_EXCEPTION 376
#define TK_RAISECODE 377
#define TK_VIRTERRORCODE 378
#define TK_EXPLICIT 379
#define TK_TEMPLATE 380
#define TK_FINAL 381
#define TK_ELLIPSIS 382
#define TK_DEFMETATYPE 383
#define TK_DEFSUPERTYPE 384
#define TK_PROPERTY 385
#define TK_HIDE_NS 386
#define TK_FORMAT 387
#define TK_GET 388
#define TK_ID 389
#define TK_KWARGS 390
#define TK_LANGUAGE 391
#define TK_LICENSEE 392
#define TK_NAME 393
#define TK_OPTIONAL 394
#define TK_ORDER 395
#define TK_REMOVELEADING 396
#define TK_SET 397
#define TK_SIGNATURE 398
#define TK_TIMESTAMP 399
#define TK_TYPE 400
#define TK_USEARGNAMES 401
#define TK_USELIMITEDAPI 402
#define TK_ALLRAISEPYEXC 403
#define TK_CALLSUPERINIT 404
#define TK_DEFERRORHANDLER 405
#define TK_VERSION 406




/* Copy the first part of user declarations.  */
#line 19 "sip-4.19.12/sipgen/metasrc/parser.y"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "sip.h"


#define MAX_NESTED_IF       10
#define MAX_NESTED_SCOPE    10

#define inMainModule()      (currentSpec->module == currentModule || currentModule->container != NULL)


static sipSpec *currentSpec;            /* The current spec being parsed. */
static int strictParse;                 /* Set if the platform is enforced. */
static stringList *backstops;           /* The list of backstops. */
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
static int stackPtr;                    /* The stack pointer. */
static int skipStack[MAX_NESTED_IF];    /* Stack of skip flags. */
static classDef *scopeStack[MAX_NESTED_SCOPE];  /* The scope stack. */
static int sectFlagsStack[MAX_NESTED_SCOPE];    /* The section flags stack. */
static int currentScopeIdx;             /* The scope stack index. */
static int currentTimelineOrder;        /* The current timeline order. */
static classList *currentSupers;        /* The current super-class list. */
static platformDef *currentPlatforms;   /* The current platforms list. */
static platformDef *platformStack[MAX_NESTED_IF];   /* Stack of platforms. */
static KwArgs defaultKwArgs;            /* The default keyword arguments support. */
static int makeProtPublic;              /* Treat protected items as public. */
static int parsingCSignature;           /* An explicit C/C++ signature is being parsed. */


static const char *getPythonName(moduleDef *mod, optFlags *optflgs,
        const char *cname);
static classDef *findClass(sipSpec *pt, ifaceFileType iftype,
        apiVersionRangeDef *api_range, scopedNameDef *fqname, int tmpl_arg);
static classDef *findClassWithInterface(sipSpec *pt, ifaceFileDef *iff,
        int tmpl_arg);
static classDef *newClass(sipSpec *pt, ifaceFileType iftype,
        apiVersionRangeDef *api_range, scopedNameDef *snd,
        const char *virt_error_handler, typeHintDef *typehint_in,
        typeHintDef *typehint_out, const char *typehint_value);
static void finishClass(sipSpec *, moduleDef *, classDef *, optFlags *);
static exceptionDef *findException(sipSpec *pt, scopedNameDef *fqname, int new);
static mappedTypeDef *newMappedType(sipSpec *,argDef *, optFlags *);
static enumDef *newEnum(sipSpec *pt, moduleDef *mod, mappedTypeDef *mt_scope,
        char *name, optFlags *of, int flags, int isscoped);
static void instantiateClassTemplate(sipSpec *pt, moduleDef *mod,
        classDef *scope, scopedNameDef *fqname, classTmplDef *tcd,
        templateDef *td, const char *pyname, int use_template_name,
        docstringDef *docstring);
static void newTypedef(sipSpec *, moduleDef *, char *, argDef *, optFlags *,
        docstringDef *);
static void newVar(sipSpec *pt, moduleDef *mod, char *name, int isstatic,
        argDef *type, optFlags *of, codeBlock *acode, codeBlock *gcode,
        codeBlock *scode, int section);
static void newCtor(moduleDef *, char *, int, signatureDef *, optFlags *,
                    codeBlock *, throwArgs *, signatureDef *, int,
                    docstringDef *, codeBlock *);
static void newFunction(sipSpec *, moduleDef *, classDef *, ifaceFileDef *,
        mappedTypeDef *, int, int, int, int, int, char *, signatureDef *, int,
        int, optFlags *, codeBlock *, codeBlock *, codeBlock *, throwArgs *,
        signatureDef *, docstringDef *, int, codeBlock *);
static optFlag *findOptFlag(optFlags *flgs, const char *name);
static optFlag *getOptFlag(optFlags *flgs, const char *name, flagType ft);
static memberDef *findFunction(sipSpec *, moduleDef *, classDef *,
        ifaceFileDef *, mappedTypeDef *, const char *, int, int, int);
static void checkAttributes(sipSpec *, moduleDef *, classDef *,
        mappedTypeDef *, const char *, int);
static void newModule(FILE *fp, const char *filename);
static moduleDef *allocModule();
static void parseFile(FILE *fp, const char *name, moduleDef *prevmod,
        int optional);
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
static void newQualifier(moduleDef *, int, int, int, const char *, qualType);
static qualDef *allocQualifier(moduleDef *, int, int, int, const char *,
        qualType);
static void newImport(const char *filename);
static int timePeriod(const char *lname, const char *uname);
static int platOrFeature(char *name, int optnot);
static void addPlatform(qualDef *qd);
static int notSkipping(void);
static void getHooks(optFlags *,char **,char **);
static int getTransfer(optFlags *optflgs);
static int getReleaseGIL(optFlags *optflgs);
static int getHoldGIL(optFlags *optflgs);
static int getDeprecated(optFlags *optflgs);
static int getAllowNone(optFlags *optflgs);
static int getDisallowNone(optFlags *optflgs);
static const char *getVirtErrorHandler(optFlags *optflgs);
static const char *getDocType(optFlags *optflgs);
static const char *getTypeHintValue(optFlags *optflgs);
static void getTypeHints(optFlags *optflgs, typeHintDef **in,
        typeHintDef **out);
static int getNoTypeHint(optFlags *optflgs);
static void templateSignature(signatureDef *sd, int result, classTmplDef *tcd,
        templateDef *td, classDef *ncd, scopedNameDef *type_names,
        scopedNameDef *type_values);
static void templateType(argDef *ad, classTmplDef *tcd, templateDef *td,
        classDef *ncd, scopedNameDef *type_names, scopedNameDef *type_values);
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
static void instantiateTemplateTypedefs(sipSpec *pt, classTmplDef *tcd,
        templateDef *td, classDef *cd, scopedNameDef *type_names,
        scopedNameDef *type_values);
static overDef *instantiateTemplateOverloads(sipSpec *pt, overDef *tod,
        memberDef *tmethods, memberDef *methods, classTmplDef *tcd,
        templateDef *td, classDef *cd, ifaceFileList **used,
        scopedNameDef *type_names, scopedNameDef *type_values);
static void resolveAnyTypedef(sipSpec *pt, argDef *ad);
static void addTypedef(sipSpec *pt, typedefDef *tdd);
static void addVariable(sipSpec *pt, varDef *vd);
static void applyTypeFlags(moduleDef *mod, argDef *ad, optFlags *flags);
static Format convertFormat(const char *format);
static Signature convertSignature(const char *signature);
static argType convertEncoding(const char *encoding);
static apiVersionRangeDef *getAPIRange(optFlags *optflgs);
static apiVersionRangeDef *convertAPIRange(moduleDef *mod, nameDef *name,
        int from, int to);
static char *convertFeaturedString(char *fs);
static scopedNameDef *text2scopePart(char *text);
static KwArgs keywordArgs(moduleDef *mod, optFlags *optflgs, signatureDef *sd,
        int need_name);
static char *strip(char *s);
static int isEnabledFeature(const char *name);
static void addProperty(sipSpec *pt, moduleDef *mod, classDef *cd,
        const char *name, const char *get, const char *set,
        docstringDef *docstring);
static moduleDef *configureModule(sipSpec *pt, moduleDef *module,
        const char *filename, const char *name, int c_module, KwArgs kwargs,
        int use_arg_names, int use_limited_api, int call_super_init,
        int all_raise_py_exc, const char *def_error_handler,
        docstringDef *docstring);
static void addAutoPyName(moduleDef *mod, const char *remove_leading);
static KwArgs convertKwArgs(const char *kwargs);
static void checkAnnos(optFlags *annos, const char *valid[]);
static void checkNoAnnos(optFlags *annos, const char *msg);
static void appendCodeBlock(codeBlockList **headp, codeBlock *cb);
static void handleKeepReference(optFlags *optflgs, argDef *ad, moduleDef *mod);
static void mappedTypeAnnos(mappedTypeDef *mtd, optFlags *optflgs);
static void add_new_deref(argDef *new, argDef *orig, int isconst);
static void add_derefs(argDef *dst, argDef *src);
static int isBackstop(qualDef *qd);
static void checkEllipsis(signatureDef *sd);
static scopedNameDef *fullyQualifiedName(scopedNameDef *snd);


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

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 203 "sip-4.19.12/sipgen/metasrc/parser.y"
{
    char            qchar;
    char            *text;
    long            number;
    double          real;
    argDef          memArg;
    signatureDef    signature;
    signatureDef    *optsignature;
    throwArgs       *throwlist;
    codeBlock       *codeb;
    docstringDef    *docstr;
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
    defDocstringFmtCfg  defdocstringfmt;
    defDocstringSigCfg  defdocstringsig;
    defEncodingCfg  defencoding;
    defMetatypeCfg  defmetatype;
    defSupertypeCfg defsupertype;
    hiddenNsCfg     hiddenns;
    exceptionCfg    exception;
    docstringCfg    docstring;
    extractCfg      extract;
    featureCfg      feature;
    licenseCfg      license;
    importCfg       import;
    includeCfg      include;
    moduleCfg       module;
    pluginCfg       plugin;
    propertyCfg     property;
    variableCfg     variable;
    vehCfg          veh;
    int             token;
}
/* Line 193 of yacc.c.  */
#line 627 "sip-4.19.12/sipgen/parser.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 640 "sip-4.19.12/sipgen/parser.c"

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
# if defined YYENABLE_NLS && YYENABLE_NLS
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
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
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
  yytype_int16 yyss;
  YYSTYPE yyvs;
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
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1611

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  174
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  254
/* YYNRULES -- Number of rules.  */
#define YYNRULES  594
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1042

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   406

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   159,     2,     2,     2,   172,   164,     2,
     152,   153,   162,   161,   154,   160,     2,   163,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   169,   158,
     167,   155,   168,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   170,     2,   171,   173,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   156,   165,   157,   166,     2,     2,     2,
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
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151
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
     100,   102,   104,   106,   108,   110,   112,   115,   117,   121,
     123,   127,   131,   134,   136,   140,   142,   146,   150,   153,
     155,   159,   161,   165,   169,   172,   174,   178,   180,   184,
     188,   192,   194,   198,   200,   204,   208,   211,   214,   218,
     220,   224,   228,   232,   238,   239,   243,   248,   250,   253,
     255,   257,   259,   261,   264,   265,   271,   272,   279,   284,
     286,   289,   291,   293,   295,   297,   300,   303,   305,   307,
     309,   324,   325,   331,   332,   336,   338,   341,   342,   348,
     350,   353,   355,   358,   360,   364,   366,   370,   374,   375,
     381,   383,   386,   388,   389,   395,   397,   400,   404,   409,
     411,   415,   417,   421,   422,   424,   428,   430,   434,   438,
     442,   446,   450,   453,   455,   459,   461,   465,   469,   472,
     474,   478,   480,   484,   488,   491,   493,   497,   499,   503,
     507,   511,   513,   517,   519,   523,   527,   528,   533,   535,
     538,   540,   542,   544,   548,   550,   554,   556,   560,   564,
     565,   570,   572,   575,   577,   579,   581,   585,   589,   590,
     594,   598,   600,   604,   608,   612,   616,   620,   624,   628,
     632,   636,   640,   641,   646,   648,   651,   653,   655,   657,
     659,   661,   663,   664,   666,   669,   671,   675,   677,   681,
     685,   689,   692,   695,   697,   701,   703,   707,   711,   712,
     715,   716,   719,   720,   723,   726,   729,   732,   735,   738,
     741,   744,   747,   750,   753,   756,   759,   762,   765,   768,
     771,   774,   777,   780,   783,   786,   789,   792,   795,   798,
     801,   804,   807,   810,   814,   816,   820,   824,   828,   829,
     831,   835,   837,   841,   845,   849,   850,   852,   856,   858,
     862,   864,   868,   872,   876,   881,   884,   886,   889,   890,
     900,   901,   903,   905,   906,   908,   909,   911,   912,   914,
     916,   919,   921,   923,   928,   929,   931,   932,   935,   936,
     939,   941,   945,   947,   949,   951,   953,   955,   957,   958,
     960,   962,   964,   966,   968,   970,   974,   975,   979,   982,
     984,   986,   990,   992,   994,   996,   998,  1003,  1005,  1007,
    1009,  1011,  1013,  1015,  1016,  1018,  1022,  1029,  1042,  1043,
    1044,  1053,  1054,  1058,  1063,  1064,  1065,  1074,  1075,  1078,
    1080,  1084,  1087,  1088,  1090,  1092,  1094,  1095,  1099,  1100,
    1102,  1105,  1107,  1109,  1111,  1113,  1115,  1117,  1119,  1121,
    1123,  1125,  1127,  1129,  1131,  1133,  1135,  1137,  1139,  1141,
    1143,  1145,  1147,  1149,  1151,  1153,  1155,  1157,  1159,  1161,
    1164,  1167,  1170,  1174,  1178,  1182,  1185,  1189,  1193,  1195,
    1199,  1203,  1207,  1211,  1212,  1217,  1219,  1222,  1224,  1226,
    1228,  1230,  1232,  1233,  1235,  1248,  1249,  1253,  1255,  1267,
    1268,  1269,  1276,  1277,  1278,  1286,  1287,  1289,  1307,  1315,
    1333,  1350,  1352,  1354,  1356,  1358,  1360,  1362,  1364,  1366,
    1369,  1372,  1375,  1378,  1381,  1384,  1387,  1390,  1393,  1396,
    1400,  1404,  1406,  1409,  1412,  1414,  1417,  1420,  1423,  1425,
    1428,  1429,  1431,  1432,  1434,  1435,  1438,  1439,  1443,  1445,
    1449,  1451,  1455,  1457,  1463,  1465,  1467,  1468,  1471,  1472,
    1475,  1476,  1479,  1480,  1483,  1485,  1486,  1488,  1492,  1497,
    1502,  1507,  1511,  1515,  1522,  1529,  1533,  1536,  1537,  1541,
    1542,  1546,  1548,  1549,  1553,  1555,  1557,  1559,  1560,  1564,
    1566,  1575,  1576,  1580,  1582,  1585,  1587,  1589,  1592,  1595,
    1598,  1603,  1607,  1611,  1612,  1614,  1615,  1619,  1622,  1624,
    1629,  1632,  1635,  1637,  1639,  1642,  1644,  1646,  1649,  1652,
    1656,  1658,  1660,  1662,  1665,  1668,  1670,  1672,  1674,  1676,
    1678,  1680,  1682,  1684,  1686,  1688,  1690,  1692,  1694,  1696,
    1700,  1701,  1706,  1707,  1709
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     175,     0,    -1,   176,    -1,   175,   176,    -1,    -1,   177,
     178,    -1,   269,    -1,   255,    -1,   262,    -1,   192,    -1,
     291,    -1,   279,    -1,   283,    -1,   284,    -1,   200,    -1,
     230,    -1,   222,    -1,   226,    -1,   239,    -1,   180,    -1,
     184,    -1,   188,    -1,   243,    -1,   247,    -1,   251,    -1,
     292,    -1,   293,    -1,   306,    -1,   308,    -1,   309,    -1,
     310,    -1,   311,    -1,   312,    -1,   313,    -1,   314,    -1,
     315,    -1,   317,    -1,   318,    -1,   328,    -1,   332,    -1,
     210,    -1,   212,    -1,   196,    -1,   179,    -1,   234,    -1,
     238,    -1,   218,    -1,   358,    -1,   364,    -1,   361,    -1,
     204,    -1,   357,    -1,   335,    -1,   392,    -1,   416,    -1,
     294,    -1,     5,   181,    -1,    36,    -1,   152,   182,   153,
      -1,   183,    -1,   182,   154,   183,    -1,   138,   155,    36,
      -1,     6,   185,    -1,    36,    -1,   152,   186,   153,    -1,
     187,    -1,   186,   154,   187,    -1,   138,   155,    36,    -1,
       7,   189,    -1,    36,    -1,   152,   190,   153,    -1,   191,
      -1,   190,   154,   191,    -1,   138,   155,    36,    -1,     8,
     193,    -1,    34,    -1,   152,   194,   153,    -1,   195,    -1,
     194,   154,   195,    -1,   138,   155,    34,    -1,     9,   197,
     333,    -1,    34,    -1,   152,   198,   153,    -1,   199,    -1,
     198,   154,   199,    -1,   138,   155,    34,    -1,     3,   201,
      -1,    34,   106,    -1,   152,   202,   153,    -1,   203,    -1,
     202,   154,   203,    -1,   138,   155,   381,    -1,   151,   155,
     106,    -1,   121,   351,   205,   397,   206,    -1,    -1,   152,
     351,   153,    -1,   156,   207,   157,   158,    -1,   208,    -1,
     207,   208,    -1,   234,    -1,   238,    -1,   209,    -1,   294,
      -1,   122,   333,    -1,    -1,    30,   424,   397,   211,   214,
      -1,    -1,   363,    30,   424,   397,   213,   214,    -1,   156,
     215,   157,   158,    -1,   216,    -1,   215,   216,    -1,   234,
      -1,   238,    -1,   294,    -1,   307,    -1,    51,   333,    -1,
      52,   333,    -1,   303,    -1,   335,    -1,   217,    -1,    97,
     420,    34,   152,   405,   153,   394,   426,   397,   389,   158,
     327,   403,   402,    -1,    -1,   109,    34,   219,   220,   158,
      -1,    -1,   156,   221,   157,    -1,   179,    -1,   221,   179,
      -1,    -1,   111,   223,   156,   224,   157,    -1,   225,    -1,
     224,   225,    -1,    34,    -1,   112,   227,    -1,    34,    -1,
     152,   228,   153,    -1,   229,    -1,   228,   154,   229,    -1,
     138,   155,   381,    -1,    -1,   110,   231,   156,   232,   157,
      -1,   233,    -1,   232,   233,    -1,    34,    -1,    -1,    32,
     152,   235,   237,   153,    -1,    34,    -1,   159,    34,    -1,
     236,    95,    34,    -1,   236,    95,   159,    34,    -1,   236,
      -1,   339,   160,   339,    -1,    33,    -1,   113,   240,   397,
      -1,    -1,    36,    -1,   152,   241,   153,    -1,   242,    -1,
     241,   154,   242,    -1,   145,   155,    36,    -1,   137,   155,
      36,    -1,   143,   155,    36,    -1,   144,   155,    36,    -1,
     128,   244,    -1,   277,    -1,   152,   245,   153,    -1,   246,
      -1,   245,   154,   246,    -1,   138,   155,   277,    -1,   129,
     248,    -1,   277,    -1,   152,   249,   153,    -1,   250,    -1,
     249,   154,   250,    -1,   138,   155,   277,    -1,   131,   252,
      -1,   351,    -1,   152,   253,   153,    -1,   254,    -1,   253,
     154,   254,    -1,   138,   155,   351,    -1,    62,   256,   259,
      -1,   277,    -1,   152,   257,   153,    -1,   258,    -1,   257,
     154,   258,    -1,   138,   155,   277,    -1,    -1,   156,   260,
     157,   158,    -1,   261,    -1,   260,   261,    -1,   234,    -1,
     238,    -1,   323,    -1,    63,   263,   266,    -1,   277,    -1,
     152,   264,   153,    -1,   265,    -1,   264,   154,   265,    -1,
     138,   155,   277,    -1,    -1,   156,   267,   157,   158,    -1,
     268,    -1,   267,   268,    -1,   234,    -1,   238,    -1,   323,
      -1,    60,   270,   274,    -1,    61,   277,   278,    -1,    -1,
     277,   271,   278,    -1,   152,   272,   153,    -1,   273,    -1,
     272,   154,   273,    -1,   135,   155,    36,    -1,   136,   155,
      36,    -1,   138,   155,   277,    -1,   146,   155,   354,    -1,
     147,   155,   354,    -1,   148,   155,   354,    -1,   149,   155,
     354,    -1,   150,   155,    34,    -1,   151,   155,   106,    -1,
      -1,   156,   275,   157,   158,    -1,   276,    -1,   275,   276,
      -1,   234,    -1,   238,    -1,   319,    -1,   323,    -1,    34,
      -1,    35,    -1,    -1,   106,    -1,    54,   280,    -1,    35,
      -1,   152,   281,   153,    -1,   282,    -1,   281,   154,   282,
      -1,   138,   155,    35,    -1,   139,   155,   354,    -1,    55,
      35,    -1,    56,   285,    -1,    35,    -1,   152,   286,   153,
      -1,   287,    -1,   286,   154,   287,    -1,   138,   155,    35,
      -1,    -1,    17,   333,    -1,    -1,    18,   333,    -1,    -1,
      19,   333,    -1,    29,   333,    -1,    57,   333,    -1,    58,
     333,    -1,    59,   333,    -1,    38,   333,    -1,    39,   333,
      -1,    40,   333,    -1,    41,   333,    -1,    42,   333,    -1,
      43,   333,    -1,    44,   333,    -1,    45,   333,    -1,    50,
     333,    -1,    46,   333,    -1,    23,   333,    -1,    26,   333,
      -1,    27,   333,    -1,    20,   333,    -1,    21,   333,    -1,
      22,   333,    -1,    24,   333,    -1,    25,   333,    -1,    28,
     333,    -1,    10,   333,    -1,    11,   333,    -1,    11,   333,
      -1,    13,   333,    -1,    14,   333,    -1,     4,   320,    -1,
     152,   321,   153,    -1,   322,    -1,   321,   154,   322,    -1,
     141,   155,    36,    -1,    12,   324,   333,    -1,    -1,    36,
      -1,   152,   325,   153,    -1,   326,    -1,   325,   154,   326,
      -1,   132,   155,    36,    -1,   143,   155,    36,    -1,    -1,
     323,    -1,    15,   329,   333,    -1,    34,    -1,   152,   330,
     153,    -1,   331,    -1,   330,   154,   331,    -1,   134,   155,
      34,    -1,   140,   155,   106,    -1,    16,    35,   338,   333,
      -1,   334,    33,    -1,    31,    -1,   334,    31,    -1,    -1,
      91,   337,   339,   397,   336,   156,   340,   157,   158,    -1,
      -1,    64,    -1,    65,    -1,    -1,    35,    -1,    -1,    34,
      -1,    -1,   341,    -1,   342,    -1,   341,   342,    -1,   234,
      -1,   238,    -1,    34,   344,   397,   343,    -1,    -1,   154,
      -1,    -1,   155,   349,    -1,    -1,   155,   346,    -1,   349,
      -1,   346,   347,   349,    -1,   160,    -1,   161,    -1,   162,
      -1,   163,    -1,   164,    -1,   165,    -1,    -1,   159,    -1,
     166,    -1,   160,    -1,   161,    -1,   162,    -1,   164,    -1,
     350,   348,   355,    -1,    -1,   152,   351,   153,    -1,    94,
     352,    -1,   352,    -1,   353,    -1,   352,    94,   353,    -1,
      34,    -1,   115,    -1,   116,    -1,   351,    -1,   424,   152,
     356,   153,    -1,   107,    -1,   106,    -1,   354,    -1,   117,
      -1,    36,    -1,   114,    -1,    -1,   346,    -1,   356,   154,
     346,    -1,   108,   420,    34,   397,   158,   327,    -1,   108,
     420,   152,   162,    34,   153,   152,   425,   153,   397,   158,
     327,    -1,    -1,    -1,    65,   351,   359,   367,   397,   360,
     371,   158,    -1,    -1,   363,   362,   364,    -1,   125,   167,
     425,   168,    -1,    -1,    -1,    64,   351,   365,   367,   397,
     366,   371,   158,    -1,    -1,   169,   368,    -1,   369,    -1,
     368,   154,   369,    -1,   370,   351,    -1,    -1,    66,    -1,
      67,    -1,    68,    -1,    -1,   156,   372,   157,    -1,    -1,
     373,    -1,   372,   373,    -1,   234,    -1,   238,    -1,   218,
      -1,   358,    -1,   364,    -1,   361,    -1,   204,    -1,   357,
      -1,   335,    -1,   374,    -1,   323,    -1,   307,    -1,   294,
      -1,   295,    -1,   296,    -1,   297,    -1,   298,    -1,   299,
      -1,   300,    -1,   301,    -1,   302,    -1,   303,    -1,   304,
      -1,   305,    -1,   316,    -1,   384,    -1,   383,    -1,   408,
      -1,    53,   333,    -1,    52,   333,    -1,    51,   333,    -1,
      66,   382,   169,    -1,    67,   382,   169,    -1,    68,   382,
     169,    -1,    69,   169,    -1,   130,   375,   378,    -1,   152,
     376,   153,    -1,   377,    -1,   376,   154,   377,    -1,   133,
     155,    34,    -1,   138,   155,   381,    -1,   142,   155,    34,
      -1,    -1,   156,   379,   157,   158,    -1,   380,    -1,   379,
     380,    -1,   234,    -1,   238,    -1,   323,    -1,    34,    -1,
      36,    -1,    -1,    71,    -1,   391,   166,    34,   152,   153,
     426,   396,   397,   158,   403,   402,   404,    -1,    -1,   124,
     385,   386,    -1,   386,    -1,    34,   152,   405,   153,   426,
     397,   387,   158,   327,   403,   402,    -1,    -1,    -1,   170,
     388,   152,   405,   153,   171,    -1,    -1,    -1,   170,   390,
     420,   152,   405,   153,   171,    -1,    -1,    90,    -1,   420,
      34,   152,   405,   153,   394,   395,   426,   396,   397,   389,
     158,   327,   403,   402,   404,   401,    -1,   420,   118,   155,
     152,   420,   153,   158,    -1,   420,   118,   393,   152,   405,
     153,   394,   395,   426,   396,   397,   389,   158,   403,   402,
     404,   401,    -1,   118,   420,   152,   405,   153,   394,   395,
     426,   396,   397,   389,   158,   403,   402,   404,   401,    -1,
     161,    -1,   160,    -1,   162,    -1,   163,    -1,   172,    -1,
     164,    -1,   165,    -1,   173,    -1,   167,   167,    -1,   168,
     168,    -1,   161,   155,    -1,   160,   155,    -1,   162,   155,
      -1,   163,   155,    -1,   172,   155,    -1,   164,   155,    -1,
     165,   155,    -1,   173,   155,    -1,   167,   167,   155,    -1,
     168,   168,   155,    -1,   166,    -1,   152,   153,    -1,   170,
     171,    -1,   167,    -1,   167,   155,    -1,   155,   155,    -1,
     159,   155,    -1,   168,    -1,   168,   155,    -1,    -1,    96,
      -1,    -1,   126,    -1,    -1,   155,   106,    -1,    -1,   163,
     398,   163,    -1,   399,    -1,   398,   154,   399,    -1,    34,
      -1,    34,   155,   400,    -1,   277,    -1,    34,   169,   278,
     160,   278,    -1,    36,    -1,   106,    -1,    -1,    47,   333,
      -1,    -1,    48,   333,    -1,    -1,    49,   333,    -1,    -1,
      37,   333,    -1,   406,    -1,    -1,   407,    -1,   406,   154,
     407,    -1,    98,   339,   397,   345,    -1,    99,   339,   397,
     345,    -1,   100,   339,   397,   345,    -1,   101,   339,   397,
      -1,   102,   339,   397,    -1,   103,   152,   405,   153,   339,
     397,    -1,   104,   152,   405,   153,   339,   397,    -1,   120,
     339,   397,    -1,   421,   345,    -1,    -1,    70,   409,   411,
      -1,    -1,    72,   410,   411,    -1,   411,    -1,    -1,    97,
     412,   413,    -1,   413,    -1,   414,    -1,   416,    -1,    -1,
      90,   415,   392,    -1,   392,    -1,   420,    34,   397,   417,
     158,   288,   289,   290,    -1,    -1,   156,   418,   157,    -1,
     419,    -1,   418,   419,    -1,   234,    -1,   238,    -1,    17,
     333,    -1,    18,   333,    -1,    19,   333,    -1,    96,   424,
     423,   422,    -1,   424,   423,   422,    -1,   420,   339,   397,
      -1,    -1,   164,    -1,    -1,   423,   162,    96,    -1,   423,
     162,    -1,   351,    -1,   351,   167,   425,   168,    -1,    65,
     351,    -1,    93,    74,    -1,    74,    -1,    93,    -1,    93,
      75,    -1,    75,    -1,    76,    -1,    93,    76,    -1,    76,
      76,    -1,    93,    76,    76,    -1,    77,    -1,    78,    -1,
      73,    -1,    92,    79,    -1,    93,    79,    -1,    79,    -1,
      80,    -1,    81,    -1,    82,    -1,    83,    -1,    84,    -1,
      85,    -1,    86,    -1,    87,    -1,    88,    -1,    89,    -1,
     105,    -1,   127,    -1,   420,    -1,   425,   154,   420,    -1,
      -1,   119,   152,   427,   153,    -1,    -1,   351,    -1,   427,
     154,   351,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   576,   576,   577,   580,   580,   599,   600,   601,   602,
     603,   604,   605,   606,   607,   608,   609,   610,   611,   612,
     613,   614,   615,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   625,   626,   627,   628,   629,   630,   631,   632,
     633,   634,   635,   636,   639,   640,   641,   642,   643,   644,
     645,   646,   647,   648,   649,   650,   663,   669,   674,   679,
     680,   690,   697,   703,   708,   713,   714,   724,   731,   740,
     745,   750,   751,   761,   768,   779,   784,   789,   790,   800,
     807,   836,   841,   846,   847,   857,   864,   890,   898,   903,
     904,   915,   921,   929,   976,   980,  1087,  1092,  1093,  1104,
    1107,  1110,  1124,  1140,  1145,  1145,  1168,  1168,  1235,  1249,
    1250,  1253,  1254,  1255,  1259,  1263,  1272,  1281,  1290,  1291,
    1294,  1309,  1309,  1346,  1347,  1350,  1351,  1354,  1354,  1383,
    1384,  1387,  1393,  1399,  1404,  1409,  1410,  1420,  1427,  1427,
    1453,  1454,  1457,  1463,  1463,  1482,  1485,  1488,  1491,  1496,
    1497,  1502,  1510,  1547,  1555,  1561,  1566,  1567,  1580,  1588,
    1596,  1604,  1614,  1625,  1630,  1635,  1636,  1646,  1653,  1664,
    1669,  1674,  1675,  1685,  1692,  1704,  1709,  1714,  1715,  1725,
    1732,  1752,  1757,  1762,  1763,  1773,  1780,  1784,  1789,  1790,
    1800,  1803,  1806,  1820,  1838,  1843,  1848,  1849,  1859,  1866,
    1870,  1875,  1876,  1886,  1889,  1892,  1906,  1917,  1927,  1927,
    1940,  1945,  1946,  1963,  1975,  1993,  2005,  2017,  2029,  2041,
    2053,  2065,  2084,  2088,  2093,  2094,  2104,  2107,  2110,  2113,
    2127,  2128,  2144,  2147,  2150,  2159,  2165,  2170,  2171,  2182,
    2188,  2196,  2204,  2210,  2215,  2220,  2221,  2231,  2238,  2241,
    2246,  2249,  2254,  2257,  2262,  2268,  2274,  2280,  2285,  2290,
    2295,  2300,  2305,  2310,  2315,  2320,  2325,  2330,  2335,  2340,
    2346,  2351,  2357,  2363,  2369,  2375,  2381,  2386,  2392,  2398,
    2403,  2409,  2415,  2421,  2426,  2427,  2437,  2444,  2528,  2532,
    2538,  2543,  2544,  2555,  2561,  2569,  2572,  2575,  2584,  2590,
    2595,  2596,  2607,  2613,  2624,  2629,  2632,  2633,  2643,  2643,
    2667,  2670,  2673,  2678,  2681,  2686,  2689,  2694,  2695,  2698,
    2699,  2702,  2703,  2704,  2743,  2744,  2747,  2748,  2751,  2754,
    2759,  2760,  2778,  2781,  2784,  2787,  2790,  2793,  2798,  2801,
    2804,  2807,  2810,  2813,  2816,  2821,  2836,  2839,  2844,  2850,
    2853,  2854,  2862,  2867,  2870,  2875,  2884,  2894,  2898,  2902,
    2906,  2910,  2914,  2920,  2925,  2931,  2949,  2971,  3010,  3016,
    3010,  3059,  3059,  3085,  3090,  3096,  3090,  3135,  3136,  3139,
    3140,  3143,  3195,  3198,  3201,  3204,  3209,  3212,  3217,  3218,
    3219,  3222,  3223,  3224,  3225,  3226,  3227,  3228,  3229,  3230,
    3231,  3232,  3243,  3247,  3251,  3262,  3273,  3284,  3295,  3306,
    3317,  3328,  3339,  3350,  3361,  3372,  3383,  3384,  3385,  3386,
    3397,  3408,  3419,  3426,  3433,  3440,  3449,  3462,  3467,  3468,
    3480,  3487,  3494,  3503,  3507,  3512,  3513,  3523,  3526,  3529,
    3543,  3544,  3547,  3550,  3555,  3619,  3619,  3620,  3623,  3669,
    3672,  3672,  3683,  3686,  3686,  3698,  3701,  3706,  3724,  3744,
    3788,  3869,  3870,  3871,  3872,  3873,  3874,  3875,  3876,  3877,
    3878,  3879,  3880,  3881,  3882,  3883,  3884,  3885,  3886,  3887,
    3888,  3889,  3890,  3891,  3892,  3893,  3894,  3895,  3896,  3897,
    3900,  3903,  3908,  3911,  3916,  3919,  3927,  3930,  3936,  3940,
    3952,  3956,  3962,  3966,  3989,  3993,  3999,  4002,  4007,  4010,
    4015,  4018,  4023,  4026,  4031,  4083,  4088,  4094,  4117,  4129,
    4141,  4153,  4172,  4183,  4200,  4217,  4226,  4233,  4233,  4234,
    4234,  4235,  4239,  4239,  4240,  4244,  4245,  4249,  4249,  4250,
    4253,  4308,  4314,  4319,  4320,  4332,  4335,  4338,  4353,  4368,
    4385,  4390,  4404,  4514,  4517,  4525,  4528,  4531,  4536,  4544,
    4555,  4570,  4574,  4578,  4582,  4586,  4590,  4594,  4598,  4602,
    4606,  4610,  4614,  4618,  4622,  4626,  4630,  4634,  4638,  4642,
    4646,  4650,  4654,  4658,  4662,  4666,  4670,  4674,  4680,  4686,
    4702,  4705,  4713,  4719,  4726
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TK_API", "TK_AUTOPYNAME",
  "TK_DEFDOCSTRFMT", "TK_DEFDOCSTRSIG", "TK_DEFENCODING", "TK_PLUGIN",
  "TK_VIRTERRORHANDLER", "TK_EXPTYPEHINTCODE", "TK_TYPEHINTCODE",
  "TK_DOCSTRING", "TK_DOC", "TK_EXPORTEDDOC", "TK_EXTRACT", "TK_MAKEFILE",
  "TK_ACCESSCODE", "TK_GETCODE", "TK_SETCODE", "TK_PREINITCODE",
  "TK_INITCODE", "TK_POSTINITCODE", "TK_FINALCODE", "TK_UNITCODE",
  "TK_UNITPOSTINCLUDECODE", "TK_MODCODE", "TK_TYPECODE", "TK_PREPYCODE",
  "TK_COPYING", "TK_MAPPEDTYPE", "TK_CODELINE", "TK_IF", "TK_END",
  "TK_NAME_VALUE", "TK_PATH_VALUE", "TK_STRING_VALUE",
  "TK_VIRTUALCATCHERCODE", "TK_TRAVERSECODE", "TK_CLEARCODE",
  "TK_GETBUFFERCODE", "TK_RELEASEBUFFERCODE", "TK_READBUFFERCODE",
  "TK_WRITEBUFFERCODE", "TK_SEGCOUNTCODE", "TK_CHARBUFFERCODE",
  "TK_PICKLECODE", "TK_VIRTUALCALLCODE", "TK_METHODCODE",
  "TK_PREMETHODCODE", "TK_INSTANCECODE", "TK_FROMTYPE", "TK_TOTYPE",
  "TK_TOSUBCLASS", "TK_INCLUDE", "TK_OPTINCLUDE", "TK_IMPORT",
  "TK_EXPHEADERCODE", "TK_MODHEADERCODE", "TK_TYPEHEADERCODE", "TK_MODULE",
  "TK_CMODULE", "TK_CONSMODULE", "TK_COMPOMODULE", "TK_CLASS", "TK_STRUCT",
  "TK_PUBLIC", "TK_PROTECTED", "TK_PRIVATE", "TK_SIGNALS",
  "TK_SIGNAL_METHOD", "TK_SLOTS", "TK_SLOT_METHOD", "TK_BOOL", "TK_SHORT",
  "TK_INT", "TK_LONG", "TK_FLOAT", "TK_DOUBLE", "TK_CHAR", "TK_WCHAR_T",
  "TK_VOID", "TK_PYOBJECT", "TK_PYTUPLE", "TK_PYLIST", "TK_PYDICT",
  "TK_PYCALLABLE", "TK_PYSLICE", "TK_PYTYPE", "TK_PYBUFFER", "TK_VIRTUAL",
  "TK_ENUM", "TK_SIGNED", "TK_UNSIGNED", "TK_SCOPE", "TK_LOGICAL_OR",
  "TK_CONST", "TK_STATIC", "TK_SIPSIGNAL", "TK_SIPSLOT", "TK_SIPANYSLOT",
  "TK_SIPRXCON", "TK_SIPRXDIS", "TK_SIPSLOTCON", "TK_SIPSLOTDIS",
  "TK_SIPSSIZET", "TK_NUMBER_VALUE", "TK_REAL_VALUE", "TK_TYPEDEF",
  "TK_NAMESPACE", "TK_TIMELINE", "TK_PLATFORMS", "TK_FEATURE",
  "TK_LICENSE", "TK_QCHAR_VALUE", "TK_TRUE_VALUE", "TK_FALSE_VALUE",
  "TK_NULL_VALUE", "TK_OPERATOR", "TK_THROW", "TK_QOBJECT", "TK_EXCEPTION",
  "TK_RAISECODE", "TK_VIRTERRORCODE", "TK_EXPLICIT", "TK_TEMPLATE",
  "TK_FINAL", "TK_ELLIPSIS", "TK_DEFMETATYPE", "TK_DEFSUPERTYPE",
  "TK_PROPERTY", "TK_HIDE_NS", "TK_FORMAT", "TK_GET", "TK_ID", "TK_KWARGS",
  "TK_LANGUAGE", "TK_LICENSEE", "TK_NAME", "TK_OPTIONAL", "TK_ORDER",
  "TK_REMOVELEADING", "TK_SET", "TK_SIGNATURE", "TK_TIMESTAMP", "TK_TYPE",
  "TK_USEARGNAMES", "TK_USELIMITEDAPI", "TK_ALLRAISEPYEXC",
  "TK_CALLSUPERINIT", "TK_DEFERRORHANDLER", "TK_VERSION", "'('", "')'",
  "','", "'='", "'{'", "'}'", "';'", "'!'", "'-'", "'+'", "'*'", "'/'",
  "'&'", "'|'", "'~'", "'<'", "'>'", "':'", "'['", "']'", "'%'", "'^'",
  "$accept", "specification", "statement", "@1", "modstatement",
  "nsstatement", "defdocstringfmt", "defdocstringfmt_args",
  "defdocstringfmt_arg_list", "defdocstringfmt_arg", "defdocstringsig",
  "defdocstringsig_args", "defdocstringsig_arg_list",
  "defdocstringsig_arg", "defencoding", "defencoding_args",
  "defencoding_arg_list", "defencoding_arg", "plugin", "plugin_args",
  "plugin_arg_list", "plugin_arg", "virterrorhandler", "veh_args",
  "veh_arg_list", "veh_arg", "api", "api_args", "api_arg_list", "api_arg",
  "exception", "baseexception", "exception_body",
  "exception_body_directives", "exception_body_directive", "raisecode",
  "mappedtype", "@2", "mappedtypetmpl", "@3", "mtdefinition", "mtbody",
  "mtline", "mtfunction", "namespace", "@4", "optnsbody", "nsbody",
  "platforms", "@5", "platformlist", "platform", "feature", "feature_args",
  "feature_arg_list", "feature_arg", "timeline", "@6", "qualifierlist",
  "qualifiername", "ifstart", "@7", "oredqualifiers", "qualifiers",
  "ifend", "license", "license_args", "license_arg_list", "license_arg",
  "defmetatype", "defmetatype_args", "defmetatype_arg_list",
  "defmetatype_arg", "defsupertype", "defsupertype_args",
  "defsupertype_arg_list", "defsupertype_arg", "hiddenns", "hiddenns_args",
  "hiddenns_arg_list", "hiddenns_arg", "consmodule", "consmodule_args",
  "consmodule_arg_list", "consmodule_arg", "consmodule_body",
  "consmodule_body_directives", "consmodule_body_directive", "compmodule",
  "compmodule_args", "compmodule_arg_list", "compmodule_arg",
  "compmodule_body", "compmodule_body_directives",
  "compmodule_body_directive", "module", "module_args", "@8",
  "module_arg_list", "module_arg", "module_body", "module_body_directives",
  "module_body_directive", "dottedname", "optnumber", "include",
  "include_args", "include_arg_list", "include_arg", "optinclude",
  "import", "import_args", "import_arg_list", "import_arg",
  "optaccesscode", "optgetcode", "optsetcode", "copying", "exphdrcode",
  "modhdrcode", "typehdrcode", "travcode", "clearcode", "getbufcode",
  "releasebufcode", "readbufcode", "writebufcode", "segcountcode",
  "charbufcode", "instancecode", "picklecode", "finalcode", "modcode",
  "typecode", "preinitcode", "initcode", "postinitcode", "unitcode",
  "unitpostinccode", "prepycode", "exptypehintcode", "modtypehintcode",
  "classtypehintcode", "doc", "exporteddoc", "autopyname",
  "autopyname_args", "autopyname_arg_list", "autopyname_arg", "docstring",
  "docstring_args", "docstring_arg_list", "docstring_arg", "optdocstring",
  "extract", "extract_args", "extract_arg_list", "extract_arg", "makefile",
  "codeblock", "codelines", "enum", "@9", "optenumkey", "optfilename",
  "optname", "optenumbody", "enumbody", "enumline", "optcomma",
  "optenumassign", "optassign", "expr", "binop", "optunop", "value",
  "optcast", "scopedname", "scopednamehead", "scopepart", "bool_value",
  "simplevalue", "exprlist", "typedef", "struct", "@10", "@11",
  "classtmpl", "@12", "template", "class", "@13", "@14", "superclasses",
  "superlist", "superclass", "class_access", "optclassbody", "classbody",
  "classline", "property", "property_args", "property_arg_list",
  "property_arg", "property_body", "property_body_directives",
  "property_body_directive", "name_or_string", "optslot", "dtor", "ctor",
  "@15", "simplector", "optctorsig", "@16", "optsig", "@17", "optvirtual",
  "function", "operatorname", "optconst", "optfinal", "optabstract",
  "optflags", "flaglist", "flag", "flagvalue", "virtualcallcode",
  "methodcode", "premethodcode", "virtualcatchercode", "arglist",
  "rawarglist", "argvalue", "varmember", "@18", "@19", "simple_varmem",
  "@20", "varmem", "member", "@21", "variable", "variable_body",
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
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,    40,    41,    44,    61,   123,   125,    59,    33,
      45,    43,    42,    47,    38,   124,   126,    60,    62,    58,
      91,    93,    37,    94
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   174,   175,   175,   177,   176,   178,   178,   178,   178,
     178,   178,   178,   178,   178,   178,   178,   178,   178,   178,
     178,   178,   178,   178,   178,   178,   178,   178,   178,   178,
     178,   178,   178,   178,   178,   178,   178,   178,   178,   178,
     178,   178,   178,   178,   179,   179,   179,   179,   179,   179,
     179,   179,   179,   179,   179,   179,   180,   181,   181,   182,
     182,   183,   184,   185,   185,   186,   186,   187,   188,   189,
     189,   190,   190,   191,   192,   193,   193,   194,   194,   195,
     196,   197,   197,   198,   198,   199,   200,   201,   201,   202,
     202,   203,   203,   204,   205,   205,   206,   207,   207,   208,
     208,   208,   208,   209,   211,   210,   213,   212,   214,   215,
     215,   216,   216,   216,   216,   216,   216,   216,   216,   216,
     217,   219,   218,   220,   220,   221,   221,   223,   222,   224,
     224,   225,   226,   227,   227,   228,   228,   229,   231,   230,
     232,   232,   233,   235,   234,   236,   236,   236,   236,   237,
     237,   238,   239,   240,   240,   240,   241,   241,   242,   242,
     242,   242,   243,   244,   244,   245,   245,   246,   247,   248,
     248,   249,   249,   250,   251,   252,   252,   253,   253,   254,
     255,   256,   256,   257,   257,   258,   259,   259,   260,   260,
     261,   261,   261,   262,   263,   263,   264,   264,   265,   266,
     266,   267,   267,   268,   268,   268,   269,   269,   271,   270,
     270,   272,   272,   273,   273,   273,   273,   273,   273,   273,
     273,   273,   274,   274,   275,   275,   276,   276,   276,   276,
     277,   277,   278,   278,   279,   280,   280,   281,   281,   282,
     282,   283,   284,   285,   285,   286,   286,   287,   288,   288,
     289,   289,   290,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   321,   322,   323,   324,   324,
     324,   325,   325,   326,   326,   327,   327,   328,   329,   329,
     330,   330,   331,   331,   332,   333,   334,   334,   336,   335,
     337,   337,   337,   338,   338,   339,   339,   340,   340,   341,
     341,   342,   342,   342,   343,   343,   344,   344,   345,   345,
     346,   346,   347,   347,   347,   347,   347,   347,   348,   348,
     348,   348,   348,   348,   348,   349,   350,   350,   351,   351,
     352,   352,   353,   354,   354,   355,   355,   355,   355,   355,
     355,   355,   355,   356,   356,   356,   357,   357,   359,   360,
     358,   362,   361,   363,   365,   366,   364,   367,   367,   368,
     368,   369,   370,   370,   370,   370,   371,   371,   372,   372,
     372,   373,   373,   373,   373,   373,   373,   373,   373,   373,
     373,   373,   373,   373,   373,   373,   373,   373,   373,   373,
     373,   373,   373,   373,   373,   373,   373,   373,   373,   373,
     373,   373,   373,   373,   373,   373,   374,   375,   376,   376,
     377,   377,   377,   378,   378,   379,   379,   380,   380,   380,
     381,   381,   382,   382,   383,   385,   384,   384,   386,   387,
     388,   387,   389,   390,   389,   391,   391,   392,   392,   392,
     392,   393,   393,   393,   393,   393,   393,   393,   393,   393,
     393,   393,   393,   393,   393,   393,   393,   393,   393,   393,
     393,   393,   393,   393,   393,   393,   393,   393,   393,   393,
     394,   394,   395,   395,   396,   396,   397,   397,   398,   398,
     399,   399,   400,   400,   400,   400,   401,   401,   402,   402,
     403,   403,   404,   404,   405,   406,   406,   406,   407,   407,
     407,   407,   407,   407,   407,   407,   407,   409,   408,   410,
     408,   408,   412,   411,   411,   413,   413,   415,   414,   414,
     416,   417,   417,   418,   418,   419,   419,   419,   419,   419,
     420,   420,   421,   422,   422,   423,   423,   423,   424,   424,
     424,   424,   424,   424,   424,   424,   424,   424,   424,   424,
     424,   424,   424,   424,   424,   424,   424,   424,   424,   424,
     424,   424,   424,   424,   424,   424,   424,   424,   425,   425,
     426,   426,   427,   427,   427
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     0,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     1,     3,     1,
       3,     3,     2,     1,     3,     1,     3,     3,     2,     1,
       3,     1,     3,     3,     2,     1,     3,     1,     3,     3,
       3,     1,     3,     1,     3,     3,     2,     2,     3,     1,
       3,     3,     3,     5,     0,     3,     4,     1,     2,     1,
       1,     1,     1,     2,     0,     5,     0,     6,     4,     1,
       2,     1,     1,     1,     1,     2,     2,     1,     1,     1,
      14,     0,     5,     0,     3,     1,     2,     0,     5,     1,
       2,     1,     2,     1,     3,     1,     3,     3,     0,     5,
       1,     2,     1,     0,     5,     1,     2,     3,     4,     1,
       3,     1,     3,     0,     1,     3,     1,     3,     3,     3,
       3,     3,     2,     1,     3,     1,     3,     3,     2,     1,
       3,     1,     3,     3,     2,     1,     3,     1,     3,     3,
       3,     1,     3,     1,     3,     3,     0,     4,     1,     2,
       1,     1,     1,     3,     1,     3,     1,     3,     3,     0,
       4,     1,     2,     1,     1,     1,     3,     3,     0,     3,
       3,     1,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     0,     4,     1,     2,     1,     1,     1,     1,
       1,     1,     0,     1,     2,     1,     3,     1,     3,     3,
       3,     2,     2,     1,     3,     1,     3,     3,     0,     2,
       0,     2,     0,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     3,     1,     3,     3,     3,     0,     1,
       3,     1,     3,     3,     3,     0,     1,     3,     1,     3,
       1,     3,     3,     3,     4,     2,     1,     2,     0,     9,
       0,     1,     1,     0,     1,     0,     1,     0,     1,     1,
       2,     1,     1,     4,     0,     1,     0,     2,     0,     2,
       1,     3,     1,     1,     1,     1,     1,     1,     0,     1,
       1,     1,     1,     1,     1,     3,     0,     3,     2,     1,
       1,     3,     1,     1,     1,     1,     4,     1,     1,     1,
       1,     1,     1,     0,     1,     3,     6,    12,     0,     0,
       8,     0,     3,     4,     0,     0,     8,     0,     2,     1,
       3,     2,     0,     1,     1,     1,     0,     3,     0,     1,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       2,     2,     3,     3,     3,     2,     3,     3,     1,     3,
       3,     3,     3,     0,     4,     1,     2,     1,     1,     1,
       1,     1,     0,     1,    12,     0,     3,     1,    11,     0,
       0,     6,     0,     0,     7,     0,     1,    17,     7,    17,
      16,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     3,
       3,     1,     2,     2,     1,     2,     2,     2,     1,     2,
       0,     1,     0,     1,     0,     2,     0,     3,     1,     3,
       1,     3,     1,     5,     1,     1,     0,     2,     0,     2,
       0,     2,     0,     2,     1,     0,     1,     3,     4,     4,
       4,     3,     3,     6,     6,     3,     2,     0,     3,     0,
       3,     1,     0,     3,     1,     1,     1,     0,     3,     1,
       8,     0,     3,     1,     2,     1,     1,     2,     2,     2,
       4,     3,     3,     0,     1,     0,     3,     2,     1,     4,
       2,     2,     1,     1,     2,     1,     1,     2,     2,     3,
       1,     1,     1,     2,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       0,     4,     0,     1,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       4,     4,     2,     0,     1,     3,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   151,   352,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   572,   562,   565,   566,   570,   571,   575,   576,
     577,   578,   579,   580,   581,   582,   583,   584,   585,   310,
       0,   563,     0,     0,   586,     0,     0,   138,   127,     0,
     153,     0,     0,     0,   587,     0,     0,     0,     5,    43,
      19,    20,    21,     9,    42,    14,    50,    40,    41,    46,
      16,    17,    15,    44,    45,    18,    22,    23,    24,     7,
       8,     6,    11,    12,    13,    10,    25,    26,    55,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    52,   558,   349,   350,    51,    47,    49,   371,
      48,    53,    54,     0,   555,     0,     0,    86,    57,     0,
      56,    63,     0,    62,    69,     0,    68,    75,     0,    74,
      81,     0,     0,   306,   277,     0,   278,   280,   281,   298,
       0,     0,   313,   271,   272,   273,   274,   275,   269,   276,
     254,     0,   496,   143,   235,     0,   234,   241,   243,     0,
     242,   255,   256,   257,   230,   231,     0,   222,   208,   232,
       0,   186,   181,     0,   199,   194,   374,   368,   568,   311,
     312,   315,   573,   561,   564,   567,   574,   348,   555,     0,
     121,     0,     0,   133,     0,   132,   154,     0,   496,     0,
      94,     0,     0,   162,   163,     0,   168,   169,     0,   174,
     175,     0,     0,     0,     0,   496,     0,   553,    87,     0,
       0,     0,    89,     0,     0,    59,     0,     0,    65,     0,
       0,    71,     0,     0,    77,     0,     0,    83,    80,   307,
     305,     0,     0,     0,   300,   297,   314,     0,   560,     0,
     104,   315,     0,     0,     0,   237,     0,     0,   245,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   211,
       0,   206,   232,   233,   207,     0,     0,   183,     0,   180,
       0,     0,   196,     0,   193,   377,   377,   316,   496,   569,
     553,   496,     0,   123,     0,     0,     0,     0,   135,     0,
       0,     0,     0,     0,   156,   152,   515,     0,   496,   588,
       0,     0,     0,   165,     0,     0,   171,     0,     0,   177,
       0,   351,   496,   372,   515,   541,     0,     0,     0,   462,
     461,   463,   464,   466,   467,   481,   484,   488,     0,   465,
     468,     0,   557,   554,   551,     0,     0,    88,     0,     0,
      58,     0,     0,    64,     0,     0,    70,     0,     0,    76,
       0,     0,    82,     0,     0,     0,   299,     0,   304,   500,
       0,   498,     0,   145,     0,   149,     0,     0,     0,     0,
     236,     0,     0,   244,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   210,     0,     0,   288,   226,   227,
       0,   224,   228,   229,   209,     0,   182,     0,   190,   191,
       0,   188,   192,     0,   195,     0,   203,   204,     0,   201,
     205,   382,   496,   496,   308,   550,     0,     0,     0,     0,
     142,     0,   140,   131,     0,   129,     0,   134,     0,     0,
       0,     0,     0,   155,     0,   315,   315,   315,   315,   315,
       0,     0,   315,     0,   514,   516,   315,   328,     0,     0,
       0,   373,     0,   164,     0,     0,   170,     0,     0,   176,
       0,   559,   106,     0,     0,     0,   482,     0,   486,   487,
     472,   471,   473,   474,   476,   477,   485,   469,   489,   470,
     483,   475,   478,   515,   556,   440,   441,    91,    92,    90,
      61,    60,    67,    66,    73,    72,    79,    78,    85,    84,
     302,   303,   301,     0,     0,   497,     0,   105,   146,     0,
     144,   315,   239,   353,   354,   240,   238,   247,   246,   213,
     214,   215,   216,   217,   218,   219,   220,   221,   212,     0,
     282,   289,     0,     0,     0,   225,   185,   184,     0,   189,
     198,   197,     0,   202,   383,   384,   385,   378,   379,     0,
     375,   369,     0,   295,     0,   125,     0,   371,   122,   139,
     141,   128,   130,   137,   136,   159,   160,   161,   158,   157,
     496,   496,   496,   496,   496,   515,   515,   496,   490,     0,
     496,   346,   526,    95,     0,    93,   589,   167,   166,   173,
     172,   179,   178,     0,   490,     0,     0,     0,   545,   546,
       0,   543,   248,     0,   479,   480,     0,   230,   504,   505,
     502,   501,   499,     0,     0,     0,     0,     0,     0,   109,
     119,   111,   112,   113,   117,   114,   118,   147,     0,   150,
       0,     0,   284,     0,     0,     0,   291,   287,   223,   187,
     200,   382,   381,   386,   386,   317,   296,   366,     0,   124,
     126,   328,   328,   328,   521,   522,     0,     0,   525,   491,
     492,   517,   552,     0,   329,   330,   338,     0,     0,    97,
     101,    99,   100,   102,   107,   492,   547,   548,   549,   542,
     544,     0,   250,     0,   490,   232,   270,   266,   115,   116,
       0,     0,   110,   148,     0,   283,     0,     0,     0,   290,
       0,   380,   388,     0,     0,   326,   321,   322,     0,   318,
     319,     0,   518,   519,   520,   315,   315,   493,   590,     0,
     332,   333,   334,   335,   336,   337,   346,   339,   341,   342,
     343,   344,   340,     0,   103,     0,    98,   590,   249,     0,
     252,   458,   492,     0,     0,   108,   286,   285,   293,   294,
     292,     0,     0,   352,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   442,   442,   442,     0,
     527,   529,   537,   532,   445,     0,   397,   393,   391,   392,
     403,   404,   405,   406,   407,   408,   409,   410,   411,   412,
     413,   414,   402,   415,   401,   399,   398,   394,   396,   395,
     455,   389,   400,   417,   416,   447,     0,   539,   418,   531,
     534,   535,   536,   376,   370,   346,   496,     0,   320,     0,
     496,   496,     0,   494,   347,   331,   361,   358,   357,   362,
     360,   355,   359,   345,     0,    96,   494,   251,     0,   540,
     590,   232,   515,   279,   268,   515,   258,   259,   260,   261,
     262,   263,   264,   265,   267,   421,   420,   419,   443,     0,
       0,     0,   425,     0,     0,     0,     0,     0,     0,   433,
     387,   390,     0,   327,   324,   309,   496,   523,   524,   592,
       0,   496,   346,   496,   253,   494,   503,     0,     0,   422,
     423,   424,   537,   528,   530,   538,     0,   533,     0,   446,
       0,     0,     0,     0,   428,     0,   426,     0,   325,   323,
       0,   593,     0,   495,   452,   364,     0,   452,   496,   490,
     590,     0,     0,     0,     0,   427,     0,   437,   438,   439,
       0,   435,     0,   295,   591,     0,   453,     0,   356,   346,
       0,   452,   590,   496,   430,   431,   432,   429,     0,   436,
     590,   367,   594,     0,   510,   365,   295,     0,   496,   449,
     434,   494,     0,     0,   508,   510,   510,   452,   450,     0,
     496,   515,   511,     0,   512,   508,   508,     0,     0,   295,
       0,     0,   509,     0,   506,   512,   512,   295,   515,   510,
     510,     0,   513,     0,   460,   506,   506,   510,     0,   508,
     508,   454,   507,   457,   459,   508,     0,   448,   512,   120,
     451,   444
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,     3,    78,    79,    80,   140,   244,   245,
      81,   143,   247,   248,    82,   146,   250,   251,    83,   149,
     253,   254,    84,   152,   256,   257,    85,   137,   241,   242,
      86,   328,   615,   698,   699,   700,    87,   392,    88,   623,
     537,   648,   649,   650,    89,   313,   449,   586,    90,   212,
     454,   455,    91,   215,   317,   318,    92,   211,   451,   452,
      93,   271,   395,   396,    94,    95,   218,   323,   324,    96,
     223,   332,   333,    97,   226,   335,   336,    98,   229,   338,
     339,    99,   191,   296,   297,   299,   430,   431,   100,   194,
     301,   302,   304,   438,   439,   101,   187,   292,   288,   289,
     291,   420,   421,   188,   294,   102,   176,   274,   275,   103,
     104,   180,   277,   278,   712,   770,   869,   105,   106,   107,
     108,   811,   812,   813,   814,   815,   816,   817,   818,   654,
     820,   821,   109,   655,   110,   111,   112,   113,   114,   115,
     116,   117,   823,   118,   119,   422,   560,   661,   662,   676,
     563,   665,   666,   677,   120,   161,   263,   264,   121,   154,
     155,   122,   582,   201,   267,   308,   738,   739,   740,   939,
     846,   612,   694,   756,   763,   695,   696,   123,   124,   125,
     545,   863,   946,   126,   127,   306,   674,   128,   234,   587,
     130,   305,   673,   442,   577,   578,   579,   733,   830,   831,
     832,   899,   933,   934,   936,   960,   961,   517,   889,   833,
     834,   897,   835,   999,  1008,   967,   983,   836,   837,   361,
     690,   748,   911,   270,   390,   391,   641,  1024,  1004,   994,
    1014,   473,   474,   475,   838,   893,   894,   839,   896,   840,
     841,   895,   842,   495,   630,   631,   476,   477,   364,   237,
     134,   330,   853,   942
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -848
static const yytype_int16 yypact[] =
{
    -848,   264,  -848,  1011,  -848,  -848,    67,    30,    64,    89,
      68,    70,   240,   240,   240,   240,    74,   246,   240,   240,
     240,   240,   240,   240,   240,   240,   313,   143,  -848,  -848,
      26,   284,    56,   240,   240,   240,    46,   272,    49,    59,
     102,   102,  -848,  -848,  -848,   216,  -848,  -848,  -848,  -848,
    -848,  -848,  -848,  -848,  -848,  -848,  -848,  -848,  -848,   260,
     242,   300,   305,   313,  -848,   495,   339,  -848,  -848,    75,
      92,   495,   102,   186,  -848,    61,    63,    65,  -848,  -848,
    -848,  -848,  -848,  -848,  -848,  -848,  -848,  -848,  -848,  -848,
    -848,  -848,  -848,  -848,  -848,  -848,  -848,  -848,  -848,  -848,
    -848,  -848,  -848,  -848,  -848,  -848,  -848,  -848,  -848,  -848,
    -848,  -848,  -848,  -848,  -848,  -848,  -848,  -848,  -848,  -848,
    -848,  -848,  -848,   210,   286,  -848,  -848,  -848,  -848,   332,
    -848,  -848,  -848,    84,  -848,   279,   -21,  -848,  -848,   270,
    -848,  -848,   281,  -848,  -848,   289,  -848,  -848,   295,  -848,
    -848,   297,   240,  -848,  -848,   176,  -848,  -848,  -848,  -848,
      47,   240,   412,  -848,  -848,  -848,  -848,  -848,  -848,  -848,
    -848,   102,   290,  -848,  -848,   190,  -848,  -848,  -848,   320,
    -848,  -848,  -848,  -848,  -848,  -848,   195,   306,  -848,   361,
     362,   355,  -848,   378,   363,  -848,  -848,    22,  -848,  -848,
    -848,   440,  -848,  -848,  -848,   442,  -848,   286,  -848,    86,
    -848,   364,   365,  -848,   384,  -848,  -848,   212,   290,   371,
     372,   495,   388,  -848,  -848,   389,  -848,  -848,   390,  -848,
    -848,   495,   305,   313,   466,   -29,   150,   139,  -848,   379,
     383,   181,  -848,   385,   250,  -848,   386,   268,  -848,   387,
     291,  -848,   391,   296,  -848,   392,   298,  -848,  -848,  -848,
    -848,   393,   394,   301,  -848,  -848,  -848,   240,  -848,   497,
    -848,     2,   396,   398,   303,  -848,   399,   311,  -848,   400,
     401,   403,   404,   406,   407,   408,   410,   430,   316,  -848,
     257,  -848,   361,  -848,  -848,   435,   318,  -848,   247,  -848,
     437,   323,  -848,   247,  -848,   370,   370,  -848,   290,  -848,
     139,   290,   381,   377,   510,   511,   438,   325,  -848,   439,
     441,   444,   448,   328,  -848,  -848,  1253,   102,   290,  -848,
       8,   449,   330,  -848,   450,   333,  -848,   451,   336,  -848,
       9,  -848,   290,  -848,  1253,   411,   454,   141,   453,   455,
     456,   461,   462,   463,   464,  -848,    16,    95,   424,   465,
     468,   445,   513,  -848,  -848,   347,   515,  -848,   -21,   588,
    -848,   270,   589,  -848,   281,   590,  -848,   289,   593,  -848,
     295,   594,  -848,   297,   595,   528,  -848,    47,  -848,   480,
     134,  -848,   481,   476,   604,   544,   487,   482,   606,   376,
    -848,   190,   608,  -848,   320,   609,   610,   272,   376,   376,
     376,   376,   614,   538,  -848,   195,   498,    93,  -848,  -848,
      40,  -848,  -848,  -848,  -848,   272,  -848,   362,  -848,  -848,
      31,  -848,  -848,   272,  -848,   378,  -848,  -848,    35,  -848,
    -848,   358,   290,   290,  -848,  -848,   491,   617,   758,   494,
    -848,    28,  -848,  -848,    43,  -848,   347,  -848,   384,   618,
     619,   620,   621,  -848,   212,   440,   440,   440,   440,   440,
     501,   506,   440,   507,   514,  -848,   440,   512,   517,   509,
     495,  -848,   272,  -848,   388,   272,  -848,   389,   102,  -848,
     390,  -848,  -848,   519,   319,   516,  -848,   495,  -848,  -848,
    -848,  -848,  -848,  -848,  -848,  -848,  -848,   518,  -848,   520,
    -848,  -848,  -848,  1253,  -848,  -848,  -848,  -848,  -848,  -848,
    -848,  -848,  -848,  -848,  -848,  -848,  -848,  -848,  -848,  -848,
    -848,  -848,  -848,    97,   497,  -848,   235,  -848,  -848,    21,
    -848,   440,  -848,  -848,  -848,  -848,  -848,  -848,  -848,  -848,
    -848,  -848,  -848,  -848,  -848,  -848,  -848,  -848,  -848,   535,
    -848,  -848,   114,   240,   521,  -848,  -848,  -848,   522,  -848,
    -848,  -848,   523,  -848,  -848,  -848,  -848,   529,  -848,   102,
    -848,  -848,   526,   665,   532,  -848,  1093,  -848,  -848,  -848,
    -848,  -848,  -848,  -848,  -848,  -848,  -848,  -848,  -848,  -848,
     290,   290,   290,   290,   290,  1253,  1253,   290,   592,  1253,
     290,   534,  -848,  -848,    83,  -848,  -848,  -848,  -848,  -848,
    -848,  -848,  -848,   481,   592,   240,   240,   240,  -848,  -848,
      57,  -848,   672,   537,  -848,  -848,   539,   524,  -848,  -848,
    -848,  -848,  -848,   240,   240,   240,   240,   495,    55,  -848,
    -848,  -848,  -848,  -848,  -848,  -848,  -848,  -848,   657,  -848,
     540,   340,  -848,   542,   545,   342,  -848,  -848,  -848,  -848,
    -848,   358,  -848,   543,   543,   397,  -848,  -848,   549,  -848,
    -848,   512,   512,   512,  -848,  -848,   550,   551,  -848,  -848,
     576,  -848,  -848,   102,   207,  -848,   199,   240,    53,  -848,
    -848,  -848,  -848,  -848,  -848,   576,  -848,  -848,  -848,  -848,
    -848,   240,   687,   548,   592,   361,  -848,  -848,  -848,  -848,
     673,   552,  -848,  -848,   675,  -848,   535,   676,   677,  -848,
     114,  -848,  1192,   556,   557,   553,  -848,  -848,   559,   397,
    -848,   495,  -848,  -848,  -848,   440,   440,  -848,   598,   565,
    -848,  -848,  -848,  -848,  -848,  -848,   534,  -848,  -848,  -848,
    -848,  -848,  -848,  1314,  -848,   562,  -848,   598,  -848,   240,
     690,  -848,   576,   561,   570,  -848,  -848,  -848,  -848,  -848,
    -848,   240,   240,   571,   240,   240,   240,   240,   240,   240,
     240,   240,   240,   240,   240,   240,   654,   654,   654,   558,
    -848,  -848,   563,  -848,  -848,   578,  -848,  -848,  -848,  -848,
    -848,  -848,  -848,  -848,  -848,  -848,  -848,  -848,  -848,  -848,
    -848,  -848,  -848,  -848,  -848,  -848,  -848,  -848,  -848,  -848,
     877,  -848,  -848,  -848,  -848,  -848,   567,  -848,  -848,  -848,
    -848,  -848,  -848,  -848,  -848,   534,   290,   573,  -848,   345,
     290,   290,   582,   580,  -848,  -848,  -848,  -848,  -848,  -848,
    -848,   -17,  -848,  -848,   585,  -848,   580,  -848,   240,  -848,
     598,   361,  1253,  -848,  -848,  1253,  -848,  -848,  -848,  -848,
    -848,  -848,  -848,  -848,  -848,  -848,  -848,  -848,  -848,   569,
     574,   575,  -848,  1370,  1370,  1484,  1427,   694,   140,   583,
    -848,  -848,   706,  -848,   591,  -848,   290,  -848,  -848,   102,
     636,   290,   285,   290,  -848,   580,  -848,   596,   597,  -848,
    -848,  -848,  -848,  -848,  -848,  -848,    88,  -848,   571,  -848,
     599,   600,   602,   348,  -848,   247,  -848,   607,  -848,  -848,
     605,  -848,   350,  -848,   577,   207,   352,   577,   290,   592,
     598,   612,   712,   347,   714,  -848,   140,  -848,  -848,  -848,
      37,  -848,   613,   665,  -848,   102,  -848,   625,  -848,   534,
     626,   577,   598,   290,  -848,  -848,  -848,  -848,   627,  -848,
     598,  -848,  -848,   495,   702,   207,   665,   629,   290,   623,
    -848,   580,   615,   240,   704,   702,   702,   577,  -848,   630,
     290,  1253,  -848,   240,   721,   704,   704,   631,   642,   665,
     637,   643,  -848,   240,   713,   721,   721,   665,  1253,   702,
     702,   628,  -848,   240,  -848,   713,   713,   702,   644,   704,
     704,  -848,  -848,  -848,  -848,   704,   632,  -848,   721,  -848,
    -848,  -848
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -848,  -848,   761,  -848,  -848,  -397,  -848,  -848,  -848,   427,
    -848,  -848,  -848,   426,  -848,  -848,  -848,   425,  -848,  -848,
    -848,   428,  -848,  -848,  -848,   382,  -848,  -848,  -848,   402,
    -640,  -848,  -848,  -848,   106,  -848,  -848,  -848,  -848,  -848,
     182,  -848,   158,  -848,  -606,  -848,  -848,  -848,  -848,  -848,
    -848,   353,  -848,  -848,  -848,   351,  -848,  -848,  -848,   359,
    -266,  -848,  -848,  -848,  -264,  -848,  -848,  -848,   354,  -848,
    -848,  -848,   327,  -848,  -848,  -848,   329,  -848,  -848,  -848,
     322,  -848,  -848,  -848,   421,  -848,  -848,   395,  -848,  -848,
    -848,   380,  -848,  -848,   415,  -848,  -848,  -848,  -848,   405,
    -848,  -848,   409,     3,  -269,  -848,  -848,  -848,   413,  -848,
    -848,  -848,  -848,   417,  -848,  -848,  -848,  -848,  -848,  -848,
    -503,  -848,  -848,  -848,  -848,  -848,  -848,  -848,  -848,  -605,
    -848,  -848,  -848,  -583,  -848,  -848,  -848,  -848,  -848,  -848,
    -848,  -848,  -848,  -848,  -848,  -848,  -848,  -848,    98,  -273,
    -848,  -848,    96,  -726,  -848,  -848,  -848,   432,  -848,   -13,
    -848,  -490,  -848,  -848,  -848,  -233,  -848,  -848,   117,  -848,
    -848,  -240,  -799,  -848,  -848,  -646,  -848,   -27,   765,   633,
    -351,  -848,  -848,  -581,  -579,  -848,  -848,  -572,  -848,   825,
    -215,  -848,  -848,   554,  -848,   187,  -848,   183,  -848,    29,
    -848,  -848,  -848,   -95,  -848,  -848,   -98,  -428,  -290,  -848,
    -848,  -848,   -33,  -848,  -848,  -804,  -848,  -848,    12,  -848,
    -595,  -568,  -812,  -187,  -848,   334,  -848,  -516,  -731,  -743,
    -847,  -340,  -848,   261,  -848,  -848,  -848,  -379,  -848,   -25,
    -848,  -848,    15,  -848,  -848,   239,     0,  -848,   564,   664,
     -10,  -204,  -732,  -848
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -561
static const yytype_int16 yytable[] =
{
     156,   157,   158,   133,   493,   163,   164,   165,   166,   167,
     168,   169,   170,   196,   197,   131,   172,   423,   132,   343,
     181,   182,   183,   424,   418,   432,   419,   340,   593,   705,
     440,   325,   428,   653,   429,   866,   393,   436,   397,   437,
     189,   192,   195,   417,   416,   220,   656,   417,   345,   417,
     230,   585,   417,   208,   913,   657,  -560,   552,   553,   554,
     555,   174,   450,    27,    28,   209,   138,    27,    28,    27,
      28,   219,    27,    28,   625,   626,   627,   453,   224,   227,
     184,   185,   643,   184,   185,    27,    28,    27,    28,    27,
      28,   178,   806,   184,   185,   184,   185,   184,   185,    29,
     141,   135,   147,   948,   150,   644,   645,   646,   159,   213,
     855,   703,    35,   945,    35,    27,    28,   239,   235,   772,
     311,   444,   951,   344,   446,   144,   807,   819,   216,   561,
     240,   637,   185,   638,   269,  -558,    29,   767,   915,   258,
    -560,   479,    35,   970,   268,   653,    59,   423,   265,   822,
     231,   826,   647,   827,   418,   492,   419,   432,   656,    62,
     828,   394,   480,   480,   428,   440,   429,   987,  1025,  1026,
     985,   506,   436,   636,   437,   697,   481,   491,   175,  1000,
     658,   261,   139,   507,  -560,   589,  -560,   262,   568,   680,
     806,  1041,   572,  1007,   978,   703,    62,   564,   186,   903,
     591,   190,   236,   639,   870,   697,   236,   259,   179,   260,
     765,   193,   721,   222,   709,   225,   142,   228,   973,   136,
     148,   329,   151,   342,   807,   819,   160,   214,   628,   810,
     629,   329,   600,   601,   602,   603,   604,   981,   312,   607,
     988,   145,   825,   610,   217,   562,   663,   822,   991,   826,
     508,   827,  1005,  1006,   388,   580,   581,   664,   828,   417,
     995,   416,   643,   509,     4,   686,   687,    27,    28,   417,
     651,   153,   652,   930,  1015,  1016,  1029,  1030,   931,    27,
      28,   162,   932,  1019,  1035,   644,   645,   646,   534,    27,
      28,  1027,   198,   497,    35,   173,   498,   535,  1037,  1038,
     478,   362,   346,   363,  1039,   347,   184,   185,   659,   348,
     349,   350,   351,   352,   353,   354,   355,   356,   357,   177,
     358,   202,   359,   360,   199,   200,    59,   810,   272,   273,
     279,   280,   647,   281,   367,   368,   625,   626,   627,    29,
     825,   282,   283,   284,   285,   286,   287,    29,   701,   319,
     702,    27,    28,   221,   972,   320,   321,   322,   757,   758,
     759,   760,   233,   761,   628,   762,   629,   750,   751,   752,
     753,   754,   755,   210,   203,   204,   205,   231,   171,   206,
     232,   515,   651,   516,   652,   238,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,   370,   371,    60,    61,    62,   243,   736,
     551,   737,   862,   681,   682,   683,   684,   685,    64,   246,
     688,   373,   374,   692,   574,   575,   576,   249,   566,    27,
      28,   735,   701,   252,   702,   255,   570,   693,  -363,  -363,
      74,   742,   743,   744,   376,   377,   773,   266,   133,   379,
     380,   382,   383,   269,   386,   387,   400,   401,   276,   824,
     131,   621,   290,   132,   403,   404,   808,   293,   809,   414,
     415,   426,   427,   736,   307,   737,   434,   435,   457,   458,
     616,   463,   464,   483,   484,   617,   486,   487,   619,   489,
     490,   543,   544,   725,   726,   729,   730,   633,   906,   480,
     295,   955,   956,   964,   965,   968,   969,   890,   891,  1033,
    1034,   298,   850,   851,   923,   924,   300,   829,   309,   303,
     314,   315,   316,   326,   327,   975,   331,   334,   337,    29,
      40,   389,   917,   448,   365,   918,   640,   849,   366,   441,
     369,   372,   375,   447,   450,   453,   378,   381,   384,   385,
     667,   398,   672,   399,   402,   405,   406,   824,   407,   408,
     171,   409,   410,   411,   808,   412,   809,   494,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,   413,   133,    60,    61,    62,
     425,    63,   433,   456,   459,   510,   460,   513,   131,   461,
      64,   132,   916,   462,   482,   485,   488,   496,   499,   514,
     500,   501,   706,   707,   708,   829,   502,   503,   504,   505,
     511,   518,    74,   512,   520,   522,   524,   526,   528,   530,
     716,   717,   718,   719,   531,   533,  -316,   536,   538,   539,
     540,   542,   541,   547,   557,   549,   550,   720,   556,   583,
     559,   584,   588,   605,   595,   596,   597,   598,   606,   904,
     608,  1011,   959,   907,   908,   614,   749,   611,   609,   957,
     613,   958,   624,   634,   632,   635,   660,   417,  1028,   668,
     669,   670,   675,   671,   764,   678,   693,   959,   689,   711,
     713,   723,   714,   715,   957,   724,   958,   727,   768,   732,
     728,   741,   747,   745,   746,   769,   771,   774,   845,   868,
     775,   776,   778,   779,   843,   844,   847,   852,   854,   940,
     865,   871,   872,   875,   944,   888,   947,   892,   928,  -456,
     898,   905,   133,   902,   909,   910,   861,   912,   919,   935,
     937,   329,   943,   920,   921,   938,   974,   966,   976,   949,
     950,   993,  1003,   864,   952,   953,   867,   954,  1013,   962,
    1023,   971,     5,   963,   344,   529,   980,  1001,   873,   874,
     519,   876,   877,   878,   879,   880,   881,   882,   883,   884,
     885,   886,   887,   984,   986,   990,   989,   996,  1009,  1017,
      27,    28,    29,   998,  1018,  1020,  1021,  1036,   521,  1031,
     523,   997,   525,  1040,   766,   704,   722,   592,   527,   594,
     590,   618,   622,  1010,   546,   571,   620,    35,   599,   532,
     558,   548,    40,    41,   777,   569,   780,   207,   129,   565,
     133,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,   567,    59,
      60,    61,    62,   573,    63,   914,   848,   734,   731,   901,
     443,   977,   979,    64,   929,   341,    65,    66,   642,   710,
     691,   927,   310,     0,   445,     0,    71,     0,     0,    72,
       0,     0,   941,    73,     0,    74,     0,     0,   781,   417,
       0,     0,     0,   133,   133,   926,   133,     0,     0,     0,
     782,     0,     0,     0,   643,     0,     0,   925,     0,    27,
      28,   783,     0,     0,     0,   784,   785,   786,   787,   788,
     789,   790,   791,   792,     0,     0,     0,   644,   793,   794,
     795,     0,     0,     0,     0,     0,    35,     0,   982,     0,
       0,    40,    41,   796,   797,   798,   799,   800,     0,   801,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,   802,    59,    60,
      61,    62,     0,    63,   803,     0,     0,     0,     0,     0,
    1002,     0,    64,   992,     0,    65,    66,     0,     0,     0,
    1012,     0,     0,     0,     0,    71,     0,     0,    72,     0,
    1022,   804,    73,     0,    74,     0,     0,   805,     0,     0,
    1032,     0,     0,     0,     6,     0,     7,     8,     9,    10,
      11,    12,    13,     0,    14,    15,    16,    17,     0,     0,
       0,    18,    19,    20,   900,    21,    22,    23,     0,    24,
      25,    26,     0,    27,    28,    29,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,     0,     0,     0,
       0,     0,     0,     0,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,     0,    59,    60,    61,    62,     0,    63,     0,     0,
       0,     0,     0,     0,     0,     0,    64,     0,     0,    65,
      66,    67,    68,    69,    70,    27,    28,    29,     0,    71,
       0,     0,    72,     0,     0,     0,    73,     0,    74,    75,
      76,     0,    77,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    35,     0,     0,     0,     0,    40,    41,     0,
       0,     0,     0,     0,     0,     0,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,     0,    59,    60,    61,    62,     0,    63,
       0,     0,     0,     0,     0,     0,     0,     0,    64,     0,
       0,    65,    66,   781,   417,     0,     0,     0,     0,     0,
       0,    71,     0,     0,    72,   782,     0,     0,    73,   643,
      74,     0,     0,     0,    27,    28,   783,     0,     0,     0,
     784,   785,   786,   787,   788,   789,   790,   791,   792,     0,
       0,     0,   644,   793,   794,   795,     0,     0,     0,     0,
     679,    35,     0,     0,     0,     0,    40,    41,   796,   797,
     798,   799,   800,     0,   801,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,   802,    59,    60,    61,    62,    29,    63,   803,
       0,     0,     0,     0,     0,     0,     0,    64,     0,     0,
      65,    66,     0,     0,     0,     0,     0,     0,     0,     0,
      71,     0,     0,    72,     0,     0,   804,    73,   171,    74,
       0,     0,   805,     0,     0,     0,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,     0,     0,    60,    61,    62,    29,    63,
     856,   465,   466,   467,   468,   469,   470,   471,    64,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   472,     0,     0,     0,     0,     0,   171,
      74,     0,     0,     0,     0,     0,     0,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    29,     0,    60,    61,    62,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    64,
     857,   858,     0,     0,     0,     0,     0,     0,   859,   543,
     544,   860,     0,     0,     0,   171,     0,     0,     0,     0,
       0,    74,     0,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
     922,    29,    60,    61,    62,     0,    63,   803,     0,     0,
       0,     0,     0,     0,     0,    64,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    71,     0,
       0,     0,   171,     0,     0,     0,     0,    74,     0,     0,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,   922,    29,    60,
      61,    62,     0,    63,     0,     0,     0,     0,     0,     0,
       0,     0,    64,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    71,     0,     0,     0,   171,
       0,     0,     0,     0,    74,     0,     0,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,     0,     0,    60,    61,    62,     0,
      63,     0,     0,     0,     0,     0,     0,     0,     0,    64,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    71,     0,     0,     0,     0,     0,     0,     0,
       0,    74
};

static const yytype_int16 yycheck[] =
{
      13,    14,    15,     3,   344,    18,    19,    20,    21,    22,
      23,    24,    25,    40,    41,     3,    26,   290,     3,   234,
      33,    34,    35,   292,   290,   298,   290,   231,   456,   624,
     303,   218,   298,   536,   298,   767,    34,   303,   271,   303,
      37,    38,    39,    12,     4,    72,   536,    12,   235,    12,
      77,   448,    12,    63,   866,    34,    34,   408,   409,   410,
     411,    35,    34,    32,    33,    65,    36,    32,    33,    32,
      33,    71,    32,    33,    17,    18,    19,    34,    75,    76,
      34,    35,    27,    34,    35,    32,    33,    32,    33,    32,
      33,    35,   732,    34,    35,    34,    35,    34,    35,    34,
      36,    34,    34,   915,    34,    50,    51,    52,    34,    34,
     756,   614,    59,   912,    59,    32,    33,   138,    34,   714,
      34,   308,    34,   152,   311,    36,   732,   732,    36,    36,
     151,    34,    35,    36,   163,   152,    34,   705,   870,   152,
     118,   328,    59,   947,   171,   648,    91,   420,   161,   732,
     167,   732,    97,   732,   420,   342,   420,   430,   648,    94,
     732,   159,   154,   154,   430,   438,   430,   971,  1015,  1016,
     969,   155,   438,   513,   438,   122,   168,   168,   152,   991,
     159,   134,   152,   167,   162,   157,   164,   140,   157,   586,
     830,  1038,   157,   997,   157,   698,    94,   157,   152,   845,
     157,   152,   118,   106,   772,   122,   118,    31,   152,    33,
     157,   152,   157,   152,   157,   152,   152,   152,   950,   152,
     152,   221,   152,   233,   830,   830,   152,   152,   494,   732,
     494,   231,   465,   466,   467,   468,   469,   963,   152,   472,
     972,   152,   732,   476,   152,   152,   132,   830,   980,   830,
     155,   830,   995,   996,   267,   442,   443,   143,   830,    12,
     986,     4,    27,   168,     0,   605,   606,    32,    33,    12,
     536,    31,   536,   133,  1005,  1006,  1019,  1020,   138,    32,
      33,    35,   142,  1009,  1027,    50,    51,    52,   154,    32,
      33,  1017,    76,   152,    59,   152,   155,   163,  1029,  1030,
     327,   162,   152,   164,  1035,   155,    34,    35,   541,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,    35,
     170,    79,   172,   173,    64,    65,    91,   830,   138,   139,
     135,   136,    97,   138,   153,   154,    17,    18,    19,    34,
     830,   146,   147,   148,   149,   150,   151,    34,   614,   137,
     614,    32,    33,   167,   949,   143,   144,   145,   159,   160,
     161,   162,    30,   164,   630,   166,   630,   160,   161,   162,
     163,   164,   165,    34,    74,    75,    76,   167,    65,    79,
      94,    34,   648,    36,   648,   106,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,   153,   154,    92,    93,    94,   138,   675,
     407,   675,   763,   600,   601,   602,   603,   604,   105,   138,
     607,   153,   154,   610,    66,    67,    68,   138,   425,    32,
      33,    34,   698,   138,   698,   138,   433,   152,   153,   154,
     127,   681,   682,   683,   153,   154,   715,    35,   448,   153,
     154,   153,   154,   163,   153,   154,   153,   154,   138,   732,
     448,   488,   156,   448,   153,   154,   732,   106,   732,   153,
     154,   153,   154,   739,    34,   739,   153,   154,   153,   154,
     480,   153,   154,   153,   154,   482,   153,   154,   485,   153,
     154,   115,   116,   153,   154,   153,   154,   497,   153,   154,
     138,   153,   154,   153,   154,   153,   154,   797,   798,  1025,
    1026,   156,   745,   746,   893,   894,   138,   732,    76,   156,
     156,   156,   138,   152,   152,   953,   138,   138,   138,    34,
      64,    34,   872,   156,   155,   875,   533,   741,   155,   169,
     155,   155,   155,   162,    34,    34,   155,   155,   155,   155,
     563,   155,   579,   155,   155,   155,   155,   830,   155,   155,
      65,   155,   155,   155,   830,   155,   830,   156,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,   155,   586,    92,    93,    94,
     155,    96,   155,   155,   155,   171,   155,   152,   586,   155,
     105,   586,   871,   155,   155,   155,   155,   153,   155,    96,
     155,   155,   625,   626,   627,   830,   155,   155,   155,   155,
     155,   106,   127,   155,    36,    36,    36,    34,    34,    34,
     643,   644,   645,   646,   106,   155,   160,   156,    34,    95,
     153,    35,   160,    35,   106,    36,    36,   647,    34,   158,
     152,    34,   158,   152,    36,    36,    36,    36,   152,   846,
     153,  1001,   935,   850,   851,   156,   693,   155,   154,   935,
     153,   935,   153,   155,   158,   155,   141,    12,  1018,   158,
     158,   158,   156,   154,   697,   153,   152,   960,    96,    17,
     153,    34,   153,   169,   960,   155,   960,   155,   711,   156,
     155,   152,   126,   153,   153,    18,   158,    34,   155,    19,
     158,    36,    36,    36,   158,   158,   157,   119,   153,   906,
     158,   160,   152,   152,   911,    71,   913,   169,    34,   166,
     152,   158,   732,   166,   152,   155,   763,   152,   169,   156,
      34,   741,   106,   169,   169,   154,    34,   170,    34,   153,
     153,    49,    48,   763,   155,   155,   769,   155,    37,   152,
      47,   948,     1,   158,   152,   383,   153,   152,   781,   782,
     368,   784,   785,   786,   787,   788,   789,   790,   791,   792,
     793,   794,   795,   158,   158,   158,   973,   158,   158,   158,
      32,    33,    34,   170,   152,   158,   153,   153,   371,   171,
     374,   988,   377,   171,   698,   623,   648,   454,   380,   458,
     451,   484,   490,  1000,   401,   435,   487,    59,   464,   387,
     415,   404,    64,    65,   726,   430,   730,    62,     3,   420,
     830,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,   427,    91,
      92,    93,    94,   438,    96,   868,   739,   674,   671,   830,
     306,   956,   960,   105,   897,   232,   108,   109,   534,   630,
     609,   896,   208,    -1,   310,    -1,   118,    -1,    -1,   121,
      -1,    -1,   909,   125,    -1,   127,    -1,    -1,    11,    12,
      -1,    -1,    -1,   893,   894,   895,   896,    -1,    -1,    -1,
      23,    -1,    -1,    -1,    27,    -1,    -1,   895,    -1,    32,
      33,    34,    -1,    -1,    -1,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    -1,    -1,    -1,    50,    51,    52,
      53,    -1,    -1,    -1,    -1,    -1,    59,    -1,   965,    -1,
      -1,    64,    65,    66,    67,    68,    69,    70,    -1,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    -1,    96,    97,    -1,    -1,    -1,    -1,    -1,
     993,    -1,   105,   983,    -1,   108,   109,    -1,    -1,    -1,
    1003,    -1,    -1,    -1,    -1,   118,    -1,    -1,   121,    -1,
    1013,   124,   125,    -1,   127,    -1,    -1,   130,    -1,    -1,
    1023,    -1,    -1,    -1,     3,    -1,     5,     6,     7,     8,
       9,    10,    11,    -1,    13,    14,    15,    16,    -1,    -1,
      -1,    20,    21,    22,   157,    24,    25,    26,    -1,    28,
      29,    30,    -1,    32,    33,    34,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    -1,    91,    92,    93,    94,    -1,    96,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   105,    -1,    -1,   108,
     109,   110,   111,   112,   113,    32,    33,    34,    -1,   118,
      -1,    -1,   121,    -1,    -1,    -1,   125,    -1,   127,   128,
     129,    -1,   131,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    59,    -1,    -1,    -1,    -1,    64,    65,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    -1,    91,    92,    93,    94,    -1,    96,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,    -1,
      -1,   108,   109,    11,    12,    -1,    -1,    -1,    -1,    -1,
      -1,   118,    -1,    -1,   121,    23,    -1,    -1,   125,    27,
     127,    -1,    -1,    -1,    32,    33,    34,    -1,    -1,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    -1,
      -1,    -1,    50,    51,    52,    53,    -1,    -1,    -1,    -1,
     157,    59,    -1,    -1,    -1,    -1,    64,    65,    66,    67,
      68,    69,    70,    -1,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    34,    96,    97,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,    -1,    -1,
     108,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     118,    -1,    -1,   121,    -1,    -1,   124,   125,    65,   127,
      -1,    -1,   130,    -1,    -1,    -1,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    -1,    -1,    92,    93,    94,    34,    96,
      36,    98,    99,   100,   101,   102,   103,   104,   105,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    65,
     127,    -1,    -1,    -1,    -1,    -1,    -1,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    34,    -1,    92,    93,    94,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,
     106,   107,    -1,    -1,    -1,    -1,    -1,    -1,   114,   115,
     116,   117,    -1,    -1,    -1,    65,    -1,    -1,    -1,    -1,
      -1,   127,    -1,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    34,    92,    93,    94,    -1,    96,    97,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   105,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,
      -1,    -1,    65,    -1,    -1,    -1,    -1,   127,    -1,    -1,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    34,    92,
      93,    94,    -1,    96,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    65,
      -1,    -1,    -1,    -1,   127,    -1,    -1,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    -1,    -1,    92,    93,    94,    -1,
      96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   127
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   175,   176,   177,     0,   176,     3,     5,     6,     7,
       8,     9,    10,    11,    13,    14,    15,    16,    20,    21,
      22,    24,    25,    26,    28,    29,    30,    32,    33,    34,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    91,
      92,    93,    94,    96,   105,   108,   109,   110,   111,   112,
     113,   118,   121,   125,   127,   128,   129,   131,   178,   179,
     180,   184,   188,   192,   196,   200,   204,   210,   212,   218,
     222,   226,   230,   234,   238,   239,   243,   247,   251,   255,
     262,   269,   279,   283,   284,   291,   292,   293,   294,   306,
     308,   309,   310,   311,   312,   313,   314,   315,   317,   318,
     328,   332,   335,   351,   352,   353,   357,   358,   361,   363,
     364,   392,   416,   420,   424,    34,   152,   201,    36,   152,
     181,    36,   152,   185,    36,   152,   189,    34,   152,   193,
      34,   152,   197,    31,   333,   334,   333,   333,   333,    34,
     152,   329,    35,   333,   333,   333,   333,   333,   333,   333,
     333,    65,   424,   152,    35,   152,   280,    35,    35,   152,
     285,   333,   333,   333,    34,    35,   152,   270,   277,   277,
     152,   256,   277,   152,   263,   277,   351,   351,    76,    64,
      65,   337,    79,    74,    75,    76,    79,   352,   424,   420,
      34,   231,   223,    34,   152,   227,    36,   152,   240,   420,
     351,   167,   152,   244,   277,   152,   248,   277,   152,   252,
     351,   167,    94,    30,   362,    34,   118,   423,   106,   138,
     151,   202,   203,   138,   182,   183,   138,   186,   187,   138,
     190,   191,   138,   194,   195,   138,   198,   199,   333,    31,
      33,   134,   140,   330,   331,   333,    35,   338,   351,   163,
     397,   235,   138,   139,   281,   282,   138,   286,   287,   135,
     136,   138,   146,   147,   148,   149,   150,   151,   272,   273,
     156,   274,   271,   106,   278,   138,   257,   258,   156,   259,
     138,   264,   265,   156,   266,   365,   359,    34,   339,    76,
     423,    34,   152,   219,   156,   156,   138,   228,   229,   137,
     143,   144,   145,   241,   242,   397,   152,   152,   205,   420,
     425,   138,   245,   246,   138,   249,   250,   138,   253,   254,
     425,   353,   424,   364,   152,   397,   152,   155,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   170,   172,
     173,   393,   162,   164,   422,   155,   155,   153,   154,   155,
     153,   154,   155,   153,   154,   155,   153,   154,   155,   153,
     154,   155,   153,   154,   155,   155,   153,   154,   333,    34,
     398,   399,   211,    34,   159,   236,   237,   339,   155,   155,
     153,   154,   155,   153,   154,   155,   155,   155,   155,   155,
     155,   155,   155,   155,   153,   154,     4,    12,   234,   238,
     275,   276,   319,   323,   278,   155,   153,   154,   234,   238,
     260,   261,   323,   155,   153,   154,   234,   238,   267,   268,
     323,   169,   367,   367,   397,   422,   397,   162,   156,   220,
      34,   232,   233,    34,   224,   225,   155,   153,   154,   155,
     155,   155,   155,   153,   154,    98,    99,   100,   101,   102,
     103,   104,   120,   405,   406,   407,   420,   421,   351,   397,
     154,   168,   155,   153,   154,   155,   153,   154,   155,   153,
     154,   168,   397,   405,   156,   417,   153,   152,   155,   155,
     155,   155,   155,   155,   155,   155,   155,   167,   155,   168,
     171,   155,   155,   152,    96,    34,    36,   381,   106,   203,
      36,   183,    36,   187,    36,   191,    34,   195,    34,   199,
      34,   106,   331,   155,   154,   163,   156,   214,    34,    95,
     153,   160,    35,   115,   116,   354,   282,    35,   287,    36,
      36,   277,   354,   354,   354,   354,    34,   106,   273,   152,
     320,    36,   152,   324,   157,   276,   277,   258,   157,   261,
     277,   265,   157,   268,    66,    67,    68,   368,   369,   370,
     397,   397,   336,   158,    34,   179,   221,   363,   158,   157,
     233,   157,   225,   381,   229,    36,    36,    36,    36,   242,
     339,   339,   339,   339,   339,   152,   152,   339,   153,   154,
     339,   155,   345,   153,   156,   206,   420,   277,   246,   277,
     250,   351,   254,   213,   153,    17,    18,    19,   234,   238,
     418,   419,   158,   420,   155,   155,   405,    34,    36,   106,
     277,   400,   399,    27,    50,    51,    52,    97,   215,   216,
     217,   234,   238,   294,   303,   307,   335,    34,   159,   339,
     141,   321,   322,   132,   143,   325,   326,   333,   158,   158,
     158,   154,   351,   366,   360,   156,   323,   327,   153,   157,
     179,   397,   397,   397,   397,   397,   405,   405,   397,    96,
     394,   407,   397,   152,   346,   349,   350,   122,   207,   208,
     209,   234,   238,   294,   214,   394,   333,   333,   333,   157,
     419,    17,   288,   153,   153,   169,   333,   333,   333,   333,
     420,   157,   216,    34,   155,   153,   154,   155,   155,   153,
     154,   369,   156,   371,   371,    34,   234,   238,   340,   341,
     342,   152,   345,   345,   345,   153,   153,   126,   395,   351,
     160,   161,   162,   163,   164,   165,   347,   159,   160,   161,
     162,   164,   166,   348,   333,   157,   208,   395,   333,    18,
     289,   158,   394,   278,    34,   158,    36,   322,    36,    36,
     326,    11,    23,    34,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    51,    52,    53,    66,    67,    68,    69,
      70,    72,    90,    97,   124,   130,   204,   218,   234,   238,
     294,   295,   296,   297,   298,   299,   300,   301,   302,   303,
     304,   305,   307,   316,   323,   335,   357,   358,   361,   364,
     372,   373,   374,   383,   384,   386,   391,   392,   408,   411,
     413,   414,   416,   158,   158,   155,   344,   157,   342,   425,
     339,   339,   119,   426,   153,   349,    36,   106,   107,   114,
     117,   351,   354,   355,   424,   158,   426,   333,    19,   290,
     395,   160,   152,   333,   333,   152,   333,   333,   333,   333,
     333,   333,   333,   333,   333,   333,   333,   333,    71,   382,
     382,   382,   169,   409,   410,   415,   412,   385,   152,   375,
     157,   373,   166,   349,   397,   158,   153,   397,   397,   152,
     155,   396,   152,   396,   333,   426,   278,   405,   405,   169,
     169,   169,    90,   411,   411,   392,   420,   413,    34,   386,
     133,   138,   142,   376,   377,   156,   378,    34,   154,   343,
     397,   351,   427,   106,   397,   346,   356,   397,   396,   153,
     153,    34,   155,   155,   155,   153,   154,   234,   238,   323,
     379,   380,   152,   158,   153,   154,   170,   389,   153,   154,
     389,   397,   394,   426,    34,   381,    34,   377,   157,   380,
     153,   327,   351,   390,   158,   346,   158,   389,   426,   397,
     158,   426,   420,    49,   403,   327,   158,   397,   170,   387,
     396,   152,   333,    48,   402,   403,   403,   389,   388,   158,
     397,   405,   333,    37,   404,   402,   402,   158,   152,   327,
     158,   153,   333,    47,   401,   404,   404,   327,   405,   403,
     403,   171,   333,   401,   401,   403,   153,   402,   402,   402,
     171,   404
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
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
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
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
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
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
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



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

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
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

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
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

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

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
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

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
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
#line 580 "sip-4.19.12/sipgen/metasrc/parser.y"
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

  case 55:
#line 650 "sip-4.19.12/sipgen/metasrc/parser.y"
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

  case 56:
#line 663 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping())
                currentModule->defdocstringfmt = convertFormat((yyvsp[(2) - (2)].defdocstringfmt).name);
        }
    break;

  case 57:
#line 669 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            resetLexerState();

            (yyval.defdocstringfmt).name = (yyvsp[(1) - (1)].text);
        }
    break;

  case 58:
#line 674 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.defdocstringfmt) = (yyvsp[(2) - (3)].defdocstringfmt);
        }
    break;

  case 60:
#line 680 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.defdocstringfmt) = (yyvsp[(1) - (3)].defdocstringfmt);

            switch ((yyvsp[(3) - (3)].defdocstringfmt).token)
            {
            case TK_NAME: (yyval.defdocstringfmt).name = (yyvsp[(3) - (3)].defdocstringfmt).name; break;
            }
        }
    break;

  case 61:
#line 690 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.defdocstringfmt).token = TK_NAME;

            (yyval.defdocstringfmt).name = (yyvsp[(3) - (3)].text);
        }
    break;

  case 62:
#line 697 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping())
                currentModule->defdocstringsig = convertSignature((yyvsp[(2) - (2)].defdocstringsig).name);
        }
    break;

  case 63:
#line 703 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            resetLexerState();

            (yyval.defdocstringsig).name = (yyvsp[(1) - (1)].text);
        }
    break;

  case 64:
#line 708 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.defdocstringsig) = (yyvsp[(2) - (3)].defdocstringsig);
        }
    break;

  case 66:
#line 714 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.defdocstringsig) = (yyvsp[(1) - (3)].defdocstringsig);

            switch ((yyvsp[(3) - (3)].defdocstringsig).token)
            {
            case TK_NAME: (yyval.defdocstringsig).name = (yyvsp[(3) - (3)].defdocstringsig).name; break;
            }
        }
    break;

  case 67:
#line 724 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.defdocstringsig).token = TK_NAME;

            (yyval.defdocstringsig).name = (yyvsp[(3) - (3)].text);
        }
    break;

  case 68:
#line 731 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping())
            {
                if ((currentModule->encoding = convertEncoding((yyvsp[(2) - (2)].defencoding).name)) == no_type)
                    yyerror("The %DefaultEncoding name must be one of \"ASCII\", \"Latin-1\", \"UTF-8\" or \"None\"");
            }
        }
    break;

  case 69:
#line 740 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            resetLexerState();

            (yyval.defencoding).name = (yyvsp[(1) - (1)].text);
        }
    break;

  case 70:
#line 745 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.defencoding) = (yyvsp[(2) - (3)].defencoding);
        }
    break;

  case 72:
#line 751 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.defencoding) = (yyvsp[(1) - (3)].defencoding);

            switch ((yyvsp[(3) - (3)].defencoding).token)
            {
            case TK_NAME: (yyval.defencoding).name = (yyvsp[(3) - (3)].defencoding).name; break;
            }
        }
    break;

  case 73:
#line 761 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.defencoding).token = TK_NAME;

            (yyval.defencoding).name = (yyvsp[(3) - (3)].text);
        }
    break;

  case 74:
#line 768 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            /*
             * Note that %Plugin is internal in SIP v4.  The current thinking
             * is that it won't be needed for SIP v5.
             */

            if (notSkipping())
                appendString(&currentSpec->plugins, (yyvsp[(2) - (2)].plugin).name);
        }
    break;

  case 75:
#line 779 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            resetLexerState();

            (yyval.plugin).name = (yyvsp[(1) - (1)].text);
        }
    break;

  case 76:
#line 784 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.plugin) = (yyvsp[(2) - (3)].plugin);
        }
    break;

  case 78:
#line 790 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.plugin) = (yyvsp[(1) - (3)].plugin);

            switch ((yyvsp[(3) - (3)].plugin).token)
            {
            case TK_NAME: (yyval.plugin).name = (yyvsp[(3) - (3)].plugin).name; break;
            }
        }
    break;

  case 79:
#line 800 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.plugin).token = TK_NAME;

            (yyval.plugin).name = (yyvsp[(3) - (3)].text);
        }
    break;

  case 80:
#line 807 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if ((yyvsp[(2) - (3)].veh).name == NULL)
                yyerror("%VirtualErrorHandler must have a 'name' argument");

            if (notSkipping())
            {
                virtErrorHandler *veh, **tailp;

                /* Check there isn't already a handler with the same name. */
                for (tailp = &currentSpec->errorhandlers; (veh = *tailp) != NULL; tailp = &veh->next)
                    if (strcmp(veh->name, (yyvsp[(2) - (3)].veh).name) == 0)
                        break;

                if (veh != NULL)
                    yyerror("A virtual error handler with that name has already been defined");

                veh = sipMalloc(sizeof (virtErrorHandler));

                veh->name = (yyvsp[(2) - (3)].veh).name;
                appendCodeBlock(&veh->code, (yyvsp[(3) - (3)].codeb));
                veh->mod = currentModule;
                veh->index = -1;
                veh->next = NULL;

                *tailp = veh;
            }
        }
    break;

  case 81:
#line 836 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            resetLexerState();

            (yyval.veh).name = (yyvsp[(1) - (1)].text);
        }
    break;

  case 82:
#line 841 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.veh) = (yyvsp[(2) - (3)].veh);
        }
    break;

  case 84:
#line 847 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.veh) = (yyvsp[(1) - (3)].veh);

            switch ((yyvsp[(3) - (3)].veh).token)
            {
            case TK_NAME: (yyval.veh).name = (yyvsp[(3) - (3)].veh).name; break;
            }
        }
    break;

  case 85:
#line 857 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.veh).token = TK_NAME;

            (yyval.veh).name = (yyvsp[(3) - (3)].text);
        }
    break;

  case 86:
#line 864 "sip-4.19.12/sipgen/metasrc/parser.y"
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

  case 87:
#line 890 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            resetLexerState();

            deprecated("%API name and version number should be specified using the 'name' and 'version' arguments");

            (yyval.api).name = (yyvsp[(1) - (2)].text);
            (yyval.api).version = (yyvsp[(2) - (2)].number);
        }
    break;

  case 88:
#line 898 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.api) = (yyvsp[(2) - (3)].api);
        }
    break;

  case 90:
#line 904 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.api) = (yyvsp[(1) - (3)].api);

            switch ((yyvsp[(3) - (3)].api).token)
            {
            case TK_NAME: (yyval.api).name = (yyvsp[(3) - (3)].api).name; break;
            case TK_VERSION: (yyval.api).version = (yyvsp[(3) - (3)].api).version; break;
            }
        }
    break;

  case 91:
#line 915 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.api).token = TK_NAME;

            (yyval.api).name = (yyvsp[(3) - (3)].text);
            (yyval.api).version = 0;
        }
    break;

  case 92:
#line 921 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.api).token = TK_VERSION;

            (yyval.api).name = NULL;
            (yyval.api).version = (yyvsp[(3) - (3)].number);
        }
    break;

  case 93:
#line 929 "sip-4.19.12/sipgen/metasrc/parser.y"
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
                appendCodeBlock(&xd->iff->hdrcode, (yyvsp[(5) - (5)].exception).type_header_code);
                xd->pyname = pyname;
                xd->bibase = (yyvsp[(3) - (5)].exceptionbase).bibase;
                xd->base = (yyvsp[(3) - (5)].exceptionbase).base;
                appendCodeBlock(&xd->raisecode, (yyvsp[(5) - (5)].exception).raise_code);

                if (getOptFlag(&(yyvsp[(4) - (5)].optflags), "Default", bool_flag) != NULL)
                    currentModule->defexception = xd;
            }
        }
    break;

  case 94:
#line 976 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.exceptionbase).bibase = NULL;
            (yyval.exceptionbase).base = NULL;
        }
    break;

  case 95:
#line 980 "sip-4.19.12/sipgen/metasrc/parser.y"
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
                    "BaseException",
                    "Exception",
                    "StopIteration",
                    "GeneratorExit",
                    "ArithmeticError",
                    "LookupError",
                    "StandardError",                /* Python v2. */

                    "AssertionError",
                    "AttributeError",
                    "BufferError",
                    "EOFError",
                    "FloatingPointError",
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
                    "EnvironmentError",             /* Python v2. */
                    "IOError",                      /* Python v2. */
                    "WindowsError",                 /* Python v2. */
                    "VMSError",                     /* Python v2. */

                    "BlockingIOError",
                    "BrokenPipeError",
                    "ChildProcessError",
                    "ConnectionError",
                    "ConnectionAbortedError",
                    "ConnectionRefusedError",
                    "ConnectionResetError",
                    "FileExistsError",
                    "FileNotFoundError",
                    "InterruptedError",
                    "IsADirectoryError",
                    "NotADirectoryError",
                    "PermissionError",
                    "ProcessLookupError",
                    "TimeoutError",

                    "Warning",
                    "UserWarning",
                    "DeprecationWarning",
                    "PendingDeprecationWarning",
                    "SyntaxWarning",
                    "RuntimeWarning",
                    "FutureWarning",
                    "ImportWarning",
                    "UnicodeWarning",
                    "BytesWarning",
                    "ResourceWarning",

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

  case 96:
#line 1087 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.exception) = (yyvsp[(2) - (4)].exception);
        }
    break;

  case 98:
#line 1093 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.exception) = (yyvsp[(1) - (2)].exception);

            switch ((yyvsp[(2) - (2)].exception).token)
            {
            case TK_RAISECODE: (yyval.exception).raise_code = (yyvsp[(2) - (2)].exception).raise_code; break;
            case TK_TYPEHEADERCODE: (yyval.exception).type_header_code = (yyvsp[(2) - (2)].exception).type_header_code; break;
            }
        }
    break;

  case 99:
#line 1104 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.exception).token = TK_IF;
        }
    break;

  case 100:
#line 1107 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.exception).token = TK_END;
        }
    break;

  case 101:
#line 1110 "sip-4.19.12/sipgen/metasrc/parser.y"
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

            (yyval.exception).type_header_code = NULL;
        }
    break;

  case 102:
#line 1124 "sip-4.19.12/sipgen/metasrc/parser.y"
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

            (yyval.exception).raise_code = NULL;
        }
    break;

  case 103:
#line 1140 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.codeb) = (yyvsp[(2) - (2)].codeb);
        }
    break;

  case 104:
#line 1145 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping())
            {
                static const char *annos[] = {
                    "AllowNone",
                    "API",
                    "DocType",
                    "NoRelease",
                    "PyName",
                    "TypeHint",
                    "TypeHintIn",
                    "TypeHintOut",
                    "TypeHintValue",
                    NULL
                };

                checkAnnos(&(yyvsp[(3) - (3)].optflags), annos);

                currentMappedType = newMappedType(currentSpec, &(yyvsp[(2) - (3)].memArg), &(yyvsp[(3) - (3)].optflags));
            }
        }
    break;

  case 106:
#line 1168 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping())
            {
                static const char *annos[] = {
                    "AllowNone",
                    "DocType",
                    "NoRelease",
                    "TypeHint",
                    "TypeHintIn",
                    "TypeHintOut",
                    "TypeHintValue",
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

                (yyvsp[(3) - (4)].memArg).u.td->fqname  = fullyQualifiedName((yyvsp[(3) - (4)].memArg).u.td->fqname);

                /* Check a template hasn't already been provided. */
                for (mtt = currentSpec->mappedtypetemplates; mtt != NULL; mtt = mtt->next)
                    if (compareScopedNames(mtt->mt->type.u.td->fqname, (yyvsp[(3) - (4)].memArg).u.td->fqname ) == 0 && sameTemplateSignature(&mtt->mt->type.u.td->types, &(yyvsp[(3) - (4)].memArg).u.td->types, TRUE))
                        yyerror("%MappedType template for this type has already been defined");

                (yyvsp[(3) - (4)].memArg).nrderefs = 0;
                (yyvsp[(3) - (4)].memArg).argflags = 0;

                mtt = sipMalloc(sizeof (mappedTypeTmplDef));

                mtt->sig = (yyvsp[(1) - (4)].signature);
                mtt->mt = allocMappedType(currentSpec, &(yyvsp[(3) - (4)].memArg));
                mappedTypeAnnos(mtt->mt, &(yyvsp[(4) - (4)].optflags));
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

  case 108:
#line 1235 "sip-4.19.12/sipgen/metasrc/parser.y"
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

  case 113:
#line 1255 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping())
                appendCodeBlock(&currentMappedType->iff->hdrcode, (yyvsp[(1) - (1)].codeb));
        }
    break;

  case 114:
#line 1259 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping())
                appendCodeBlock(&currentMappedType->typecode, (yyvsp[(1) - (1)].codeb));
        }
    break;

  case 115:
#line 1263 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping())
            {
                if (currentMappedType->convfromcode != NULL)
                    yyerror("%MappedType has more than one %ConvertFromTypeCode directive");

                appendCodeBlock(&currentMappedType->convfromcode, (yyvsp[(2) - (2)].codeb));
            }
        }
    break;

  case 116:
#line 1272 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping())
            {
                if (currentMappedType->convtocode != NULL)
                    yyerror("%MappedType has more than one %ConvertToTypeCode directive");

                appendCodeBlock(&currentMappedType->convtocode, (yyvsp[(2) - (2)].codeb));
            }
        }
    break;

  case 117:
#line 1281 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping())
            {
                if (currentMappedType->instancecode != NULL)
                    yyerror("%MappedType has more than one %InstanceCode directive");

                appendCodeBlock(&currentMappedType->instancecode, (yyvsp[(1) - (1)].codeb));
            }
        }
    break;

  case 120:
#line 1294 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping())
            {
                applyTypeFlags(currentModule, &(yyvsp[(2) - (14)].memArg), &(yyvsp[(9) - (14)].optflags));

                (yyvsp[(5) - (14)].signature).result = (yyvsp[(2) - (14)].memArg);

                newFunction(currentSpec, currentModule, NULL, NULL,
                        currentMappedType, 0, TRUE, FALSE, FALSE, FALSE, (yyvsp[(3) - (14)].text),
                        &(yyvsp[(5) - (14)].signature), (yyvsp[(7) - (14)].number), FALSE, &(yyvsp[(9) - (14)].optflags), (yyvsp[(14) - (14)].codeb), NULL, NULL, (yyvsp[(8) - (14)].throwlist), (yyvsp[(10) - (14)].optsignature), (yyvsp[(12) - (14)].docstr),
                        FALSE, (yyvsp[(13) - (14)].codeb));
            }
        }
    break;

  case 121:
#line 1309 "sip-4.19.12/sipgen/metasrc/parser.y"
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
                        text2scopedName(scope, (yyvsp[(2) - (2)].text)), NULL, NULL, NULL, NULL);

                pushScope(ns);

                sectionFlags = 0;
            }
        }
    break;

  case 122:
#line 1330 "sip-4.19.12/sipgen/metasrc/parser.y"
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

  case 127:
#line 1354 "sip-4.19.12/sipgen/metasrc/parser.y"
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

  case 128:
#line 1364 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping())
            {
                qualDef *qd;
                int nrneeded;

                /* Check that exactly one platform in the set was requested. */
                nrneeded = 0;

                for (qd = currentModule->qualifiers; qd != NULL; qd = qd->next)
                    if (qd->qtype == platform_qualifier && selectedQualifier(neededQualifiers, qd))
                        ++nrneeded;

                if (nrneeded > 1)
                    yyerror("No more than one of these %Platforms must be specified with the -t flag");
            }
        }
    break;

  case 131:
#line 1387 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            newQualifier(currentModule, -1, -1, notSkipping(), (yyvsp[(1) - (1)].text),
                    platform_qualifier);
        }
    break;

  case 132:
#line 1393 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            newQualifier(currentModule, -1, -1, notSkipping(), (yyvsp[(2) - (2)].feature).name,
                    feature_qualifier);
        }
    break;

  case 133:
#line 1399 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            resetLexerState();

            (yyval.feature).name = (yyvsp[(1) - (1)].text);
        }
    break;

  case 134:
#line 1404 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.feature) = (yyvsp[(2) - (3)].feature);
        }
    break;

  case 136:
#line 1410 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.feature) = (yyvsp[(1) - (3)].feature);

            switch ((yyvsp[(3) - (3)].feature).token)
            {
            case TK_NAME: (yyval.feature).name = (yyvsp[(3) - (3)].feature).name; break;
            }
        }
    break;

  case 137:
#line 1420 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.feature).token = TK_NAME;

            (yyval.feature).name = (yyvsp[(3) - (3)].text);
        }
    break;

  case 138:
#line 1427 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            currentTimelineOrder = 0;
        }
    break;

  case 139:
#line 1430 "sip-4.19.12/sipgen/metasrc/parser.y"
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
                    if (qd->qtype == time_qualifier && selectedQualifier(neededQualifiers, qd))
                        ++nrneeded;

                if (nrneeded > 1)
                    yyerror("At most one of this %Timeline must be specified with the -t flag");

                currentModule->nrtimelines++;
            }
        }
    break;

  case 142:
#line 1457 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            newQualifier(currentModule, currentModule->nrtimelines,
                    currentTimelineOrder++, TRUE, (yyvsp[(1) - (1)].text), time_qualifier);
        }
    break;

  case 143:
#line 1463 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            currentPlatforms = NULL;
        }
    break;

  case 144:
#line 1465 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (stackPtr >= MAX_NESTED_IF)
                yyerror("Internal error: increase the value of MAX_NESTED_IF");

            /* Nested %Ifs are implicit logical ands. */

            if (stackPtr > 0)
                (yyvsp[(4) - (5)].boolean) = ((yyvsp[(4) - (5)].boolean) && skipStack[stackPtr - 1]);

            skipStack[stackPtr] = (yyvsp[(4) - (5)].boolean);

            platformStack[stackPtr] = currentPlatforms;

            ++stackPtr;
        }
    break;

  case 145:
#line 1482 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.boolean) = platOrFeature((yyvsp[(1) - (1)].text), FALSE);
        }
    break;

  case 146:
#line 1485 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.boolean) = platOrFeature((yyvsp[(2) - (2)].text), TRUE);
        }
    break;

  case 147:
#line 1488 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.boolean) = (platOrFeature((yyvsp[(3) - (3)].text), FALSE) || (yyvsp[(1) - (3)].boolean));
        }
    break;

  case 148:
#line 1491 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.boolean) = (platOrFeature((yyvsp[(4) - (4)].text), TRUE) || (yyvsp[(1) - (4)].boolean));
        }
    break;

  case 150:
#line 1497 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.boolean) = timePeriod((yyvsp[(1) - (3)].text), (yyvsp[(3) - (3)].text));
        }
    break;

  case 151:
#line 1502 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (stackPtr-- <= 0)
                yyerror("Too many %End directives");

            currentPlatforms = (stackPtr == 0 ? NULL : platformStack[stackPtr - 1]);
        }
    break;

  case 152:
#line 1510 "sip-4.19.12/sipgen/metasrc/parser.y"
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

  case 153:
#line 1547 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            resetLexerState();

            (yyval.license).type = NULL;
            (yyval.license).licensee = NULL;
            (yyval.license).signature = NULL;
            (yyval.license).timestamp = NULL;
        }
    break;

  case 154:
#line 1555 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.license).type = (yyvsp[(1) - (1)].text);
            (yyval.license).licensee = NULL;
            (yyval.license).signature = NULL;
            (yyval.license).timestamp = NULL;
        }
    break;

  case 155:
#line 1561 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.license) = (yyvsp[(2) - (3)].license);
        }
    break;

  case 157:
#line 1567 "sip-4.19.12/sipgen/metasrc/parser.y"
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

  case 158:
#line 1580 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.license).token = TK_NAME;

            (yyval.license).type = (yyvsp[(3) - (3)].text);
            (yyval.license).licensee = NULL;
            (yyval.license).signature = NULL;
            (yyval.license).timestamp = NULL;
        }
    break;

  case 159:
#line 1588 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.license).token = TK_LICENSEE;

            (yyval.license).type = NULL;
            (yyval.license).licensee = (yyvsp[(3) - (3)].text);
            (yyval.license).signature = NULL;
            (yyval.license).timestamp = NULL;
        }
    break;

  case 160:
#line 1596 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.license).token = TK_SIGNATURE;

            (yyval.license).type = NULL;
            (yyval.license).licensee = NULL;
            (yyval.license).signature = (yyvsp[(3) - (3)].text);
            (yyval.license).timestamp = NULL;
        }
    break;

  case 161:
#line 1604 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.license).token = TK_TIMESTAMP;

            (yyval.license).type = NULL;
            (yyval.license).licensee = NULL;
            (yyval.license).signature = NULL;
            (yyval.license).timestamp = (yyvsp[(3) - (3)].text);
        }
    break;

  case 162:
#line 1614 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping())
            {
                if (currentModule->defmetatype != NULL)
                    yyerror("%DefaultMetatype has already been defined for this module");

                currentModule->defmetatype = cacheName(currentSpec, (yyvsp[(2) - (2)].defmetatype).name);
            }
        }
    break;

  case 163:
#line 1625 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            resetLexerState();

            (yyval.defmetatype).name = (yyvsp[(1) - (1)].text);
        }
    break;

  case 164:
#line 1630 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.defmetatype) = (yyvsp[(2) - (3)].defmetatype);
        }
    break;

  case 166:
#line 1636 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.defmetatype) = (yyvsp[(1) - (3)].defmetatype);

            switch ((yyvsp[(3) - (3)].defmetatype).token)
            {
            case TK_NAME: (yyval.defmetatype).name = (yyvsp[(3) - (3)].defmetatype).name; break;
            }
        }
    break;

  case 167:
#line 1646 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.defmetatype).token = TK_NAME;

            (yyval.defmetatype).name = (yyvsp[(3) - (3)].text);
        }
    break;

  case 168:
#line 1653 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping())
            {
                if (currentModule->defsupertype != NULL)
                    yyerror("%DefaultSupertype has already been defined for this module");

                currentModule->defsupertype = cacheName(currentSpec, (yyvsp[(2) - (2)].defsupertype).name);
            }
        }
    break;

  case 169:
#line 1664 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            resetLexerState();

            (yyval.defsupertype).name = (yyvsp[(1) - (1)].text);
        }
    break;

  case 170:
#line 1669 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.defsupertype) = (yyvsp[(2) - (3)].defsupertype);
        }
    break;

  case 172:
#line 1675 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.defsupertype) = (yyvsp[(1) - (3)].defsupertype);

            switch ((yyvsp[(3) - (3)].defsupertype).token)
            {
            case TK_NAME: (yyval.defsupertype).name = (yyvsp[(3) - (3)].defsupertype).name; break;
            }
        }
    break;

  case 173:
#line 1685 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.defsupertype).token = TK_NAME;

            (yyval.defsupertype).name = (yyvsp[(3) - (3)].text);
        }
    break;

  case 174:
#line 1692 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping())
            {
                classDef *ns;

                ns = newClass(currentSpec, namespace_iface, NULL,
                        fullyQualifiedName((yyvsp[(2) - (2)].hiddenns).name), NULL, NULL, NULL, NULL);
                setHiddenNamespace(ns);
            }
        }
    break;

  case 175:
#line 1704 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            resetLexerState();

            (yyval.hiddenns).name = (yyvsp[(1) - (1)].scpvalp);
        }
    break;

  case 176:
#line 1709 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.hiddenns) = (yyvsp[(2) - (3)].hiddenns);
        }
    break;

  case 178:
#line 1715 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.hiddenns) = (yyvsp[(1) - (3)].hiddenns);

            switch ((yyvsp[(3) - (3)].hiddenns).token)
            {
            case TK_NAME: (yyval.hiddenns).name = (yyvsp[(3) - (3)].hiddenns).name; break;
            }
        }
    break;

  case 179:
#line 1725 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.hiddenns).token = TK_NAME;

            (yyval.hiddenns).name = (yyvsp[(3) - (3)].scpvalp);
        }
    break;

  case 180:
#line 1732 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            deprecated("%ConsolidatedModule is deprecated and will not be supported by SIP v5");

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

  case 181:
#line 1752 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            resetLexerState();

            (yyval.consmodule).name = (yyvsp[(1) - (1)].text);
        }
    break;

  case 182:
#line 1757 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.consmodule) = (yyvsp[(2) - (3)].consmodule);
        }
    break;

  case 184:
#line 1763 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.consmodule) = (yyvsp[(1) - (3)].consmodule);

            switch ((yyvsp[(3) - (3)].consmodule).token)
            {
            case TK_NAME: (yyval.consmodule).name = (yyvsp[(3) - (3)].consmodule).name; break;
            }
        }
    break;

  case 185:
#line 1773 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.consmodule).token = TK_NAME;

            (yyval.consmodule).name = (yyvsp[(3) - (3)].text);
        }
    break;

  case 186:
#line 1780 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.consmodule).token = 0;
            (yyval.consmodule).docstring = NULL;
        }
    break;

  case 187:
#line 1784 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.consmodule) = (yyvsp[(2) - (4)].consmodule);
        }
    break;

  case 189:
#line 1790 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.consmodule) = (yyvsp[(1) - (2)].consmodule);

            switch ((yyvsp[(2) - (2)].consmodule).token)
            {
            case TK_DOCSTRING: (yyval.consmodule).docstring = (yyvsp[(2) - (2)].consmodule).docstring; break;
            }
        }
    break;

  case 190:
#line 1800 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.consmodule).token = TK_IF;
        }
    break;

  case 191:
#line 1803 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.consmodule).token = TK_END;
        }
    break;

  case 192:
#line 1806 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping())
            {
                (yyval.consmodule).token = TK_DOCSTRING;
                (yyval.consmodule).docstring = (yyvsp[(1) - (1)].docstr);
            }
            else
            {
                (yyval.consmodule).token = 0;
                (yyval.consmodule).docstring = NULL;
            }
        }
    break;

  case 193:
#line 1820 "sip-4.19.12/sipgen/metasrc/parser.y"
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

  case 194:
#line 1838 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            resetLexerState();

            (yyval.compmodule).name = (yyvsp[(1) - (1)].text);
        }
    break;

  case 195:
#line 1843 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.compmodule) = (yyvsp[(2) - (3)].compmodule);
        }
    break;

  case 197:
#line 1849 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.compmodule) = (yyvsp[(1) - (3)].compmodule);

            switch ((yyvsp[(3) - (3)].compmodule).token)
            {
            case TK_NAME: (yyval.compmodule).name = (yyvsp[(3) - (3)].compmodule).name; break;
            }
        }
    break;

  case 198:
#line 1859 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.compmodule).token = TK_NAME;

            (yyval.compmodule).name = (yyvsp[(3) - (3)].text);
        }
    break;

  case 199:
#line 1866 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.compmodule).token = 0;
            (yyval.compmodule).docstring = NULL;
        }
    break;

  case 200:
#line 1870 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.compmodule) = (yyvsp[(2) - (4)].compmodule);
        }
    break;

  case 202:
#line 1876 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.compmodule) = (yyvsp[(1) - (2)].compmodule);

            switch ((yyvsp[(2) - (2)].compmodule).token)
            {
            case TK_DOCSTRING: (yyval.compmodule).docstring = (yyvsp[(2) - (2)].compmodule).docstring; break;
            }
        }
    break;

  case 203:
#line 1886 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.compmodule).token = TK_IF;
        }
    break;

  case 204:
#line 1889 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.compmodule).token = TK_END;
        }
    break;

  case 205:
#line 1892 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping())
            {
                (yyval.compmodule).token = TK_DOCSTRING;
                (yyval.compmodule).docstring = (yyvsp[(1) - (1)].docstr);
            }
            else
            {
                (yyval.compmodule).token = 0;
                (yyval.compmodule).docstring = NULL;
            }
        }
    break;

  case 206:
#line 1906 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if ((yyvsp[(2) - (3)].module).name == NULL)
                yyerror("%Module must have a 'name' argument");

            if (notSkipping())
                currentModule = configureModule(currentSpec, currentModule,
                        currentContext.filename, (yyvsp[(2) - (3)].module).name, (yyvsp[(2) - (3)].module).c_module,
                        (yyvsp[(2) - (3)].module).kwargs, (yyvsp[(2) - (3)].module).use_arg_names, (yyvsp[(2) - (3)].module).use_limited_api,
                        (yyvsp[(2) - (3)].module).call_super_init, (yyvsp[(2) - (3)].module).all_raise_py_exc,
                        (yyvsp[(2) - (3)].module).def_error_handler, (yyvsp[(3) - (3)].module).docstring);
        }
    break;

  case 207:
#line 1917 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            deprecated("%CModule is deprecated, use %Module and the 'language' argument instead");

            if (notSkipping())
                currentModule = configureModule(currentSpec, currentModule,
                        currentContext.filename, (yyvsp[(2) - (3)].text), TRUE, defaultKwArgs,
                        FALSE, FALSE, -1, FALSE, NULL, NULL);
        }
    break;

  case 208:
#line 1927 "sip-4.19.12/sipgen/metasrc/parser.y"
    {resetLexerState();}
    break;

  case 209:
#line 1927 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if ((yyvsp[(3) - (3)].number) >= 0)
                deprecated("%Module version number should be specified using the 'version' argument");

            (yyval.module).c_module = FALSE;
            (yyval.module).kwargs = defaultKwArgs;
            (yyval.module).name = (yyvsp[(1) - (3)].text);
            (yyval.module).use_arg_names = FALSE;
            (yyval.module).use_limited_api = FALSE;
            (yyval.module).all_raise_py_exc = FALSE;
            (yyval.module).call_super_init = -1;
            (yyval.module).def_error_handler = NULL;
        }
    break;

  case 210:
#line 1940 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.module) = (yyvsp[(2) - (3)].module);
        }
    break;

  case 212:
#line 1946 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.module) = (yyvsp[(1) - (3)].module);

            switch ((yyvsp[(3) - (3)].module).token)
            {
            case TK_KWARGS: (yyval.module).kwargs = (yyvsp[(3) - (3)].module).kwargs; break;
            case TK_LANGUAGE: (yyval.module).c_module = (yyvsp[(3) - (3)].module).c_module; break;
            case TK_NAME: (yyval.module).name = (yyvsp[(3) - (3)].module).name; break;
            case TK_USEARGNAMES: (yyval.module).use_arg_names = (yyvsp[(3) - (3)].module).use_arg_names; break;
            case TK_USELIMITEDAPI: (yyval.module).use_limited_api = (yyvsp[(3) - (3)].module).use_limited_api; break;
            case TK_ALLRAISEPYEXC: (yyval.module).all_raise_py_exc = (yyvsp[(3) - (3)].module).all_raise_py_exc; break;
            case TK_CALLSUPERINIT: (yyval.module).call_super_init = (yyvsp[(3) - (3)].module).call_super_init; break;
            case TK_DEFERRORHANDLER: (yyval.module).def_error_handler = (yyvsp[(3) - (3)].module).def_error_handler; break;
            }
        }
    break;

  case 213:
#line 1963 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.module).token = TK_KWARGS;

            (yyval.module).c_module = FALSE;
            (yyval.module).kwargs = convertKwArgs((yyvsp[(3) - (3)].text));
            (yyval.module).name = NULL;
            (yyval.module).use_arg_names = FALSE;
            (yyval.module).use_limited_api = FALSE;
            (yyval.module).all_raise_py_exc = FALSE;
            (yyval.module).call_super_init = -1;
            (yyval.module).def_error_handler = NULL;
        }
    break;

  case 214:
#line 1975 "sip-4.19.12/sipgen/metasrc/parser.y"
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
            (yyval.module).use_limited_api = FALSE;
            (yyval.module).all_raise_py_exc = FALSE;
            (yyval.module).call_super_init = -1;
            (yyval.module).def_error_handler = NULL;
        }
    break;

  case 215:
#line 1993 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.module).token = TK_NAME;

            (yyval.module).c_module = FALSE;
            (yyval.module).kwargs = defaultKwArgs;
            (yyval.module).name = (yyvsp[(3) - (3)].text);
            (yyval.module).use_arg_names = FALSE;
            (yyval.module).use_limited_api = FALSE;
            (yyval.module).all_raise_py_exc = FALSE;
            (yyval.module).call_super_init = -1;
            (yyval.module).def_error_handler = NULL;
        }
    break;

  case 216:
#line 2005 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.module).token = TK_USEARGNAMES;

            (yyval.module).c_module = FALSE;
            (yyval.module).kwargs = defaultKwArgs;
            (yyval.module).name = NULL;
            (yyval.module).use_arg_names = (yyvsp[(3) - (3)].boolean);
            (yyval.module).use_limited_api = FALSE;
            (yyval.module).all_raise_py_exc = FALSE;
            (yyval.module).call_super_init = -1;
            (yyval.module).def_error_handler = NULL;
        }
    break;

  case 217:
#line 2017 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.module).token = TK_USELIMITEDAPI;

            (yyval.module).c_module = FALSE;
            (yyval.module).kwargs = defaultKwArgs;
            (yyval.module).name = NULL;
            (yyval.module).use_arg_names = FALSE;
            (yyval.module).use_limited_api = (yyvsp[(3) - (3)].boolean);
            (yyval.module).all_raise_py_exc = FALSE;
            (yyval.module).call_super_init = -1;
            (yyval.module).def_error_handler = NULL;
        }
    break;

  case 218:
#line 2029 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.module).token = TK_ALLRAISEPYEXC;

            (yyval.module).c_module = FALSE;
            (yyval.module).kwargs = defaultKwArgs;
            (yyval.module).name = NULL;
            (yyval.module).use_arg_names = FALSE;
            (yyval.module).use_limited_api = FALSE;
            (yyval.module).all_raise_py_exc = (yyvsp[(3) - (3)].boolean);
            (yyval.module).call_super_init = -1;
            (yyval.module).def_error_handler = NULL;
        }
    break;

  case 219:
#line 2041 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.module).token = TK_CALLSUPERINIT;

            (yyval.module).c_module = FALSE;
            (yyval.module).kwargs = defaultKwArgs;
            (yyval.module).name = NULL;
            (yyval.module).use_arg_names = FALSE;
            (yyval.module).use_limited_api = FALSE;
            (yyval.module).all_raise_py_exc = FALSE;
            (yyval.module).call_super_init = (yyvsp[(3) - (3)].boolean);
            (yyval.module).def_error_handler = NULL;
        }
    break;

  case 220:
#line 2053 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.module).token = TK_DEFERRORHANDLER;

            (yyval.module).c_module = FALSE;
            (yyval.module).kwargs = defaultKwArgs;
            (yyval.module).name = NULL;
            (yyval.module).use_arg_names = FALSE;
            (yyval.module).use_limited_api = FALSE;
            (yyval.module).all_raise_py_exc = FALSE;
            (yyval.module).call_super_init = -1;
            (yyval.module).def_error_handler = (yyvsp[(3) - (3)].text);
        }
    break;

  case 221:
#line 2065 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            deprecated("%Module version numbers are deprecated and ignored");

            if ((yyvsp[(3) - (3)].number) < 0)
                yyerror("%Module 'version' argument cannot be negative");

            (yyval.module).token = TK_VERSION;

            (yyval.module).c_module = FALSE;
            (yyval.module).kwargs = defaultKwArgs;
            (yyval.module).name = NULL;
            (yyval.module).use_arg_names = FALSE;
            (yyval.module).use_limited_api = FALSE;
            (yyval.module).all_raise_py_exc = FALSE;
            (yyval.module).call_super_init = -1;
            (yyval.module).def_error_handler = NULL;
        }
    break;

  case 222:
#line 2084 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.module).token = 0;
            (yyval.module).docstring = NULL;
        }
    break;

  case 223:
#line 2088 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.module) = (yyvsp[(2) - (4)].module);
        }
    break;

  case 225:
#line 2094 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.module) = (yyvsp[(1) - (2)].module);

            switch ((yyvsp[(2) - (2)].module).token)
            {
            case TK_DOCSTRING: (yyval.module).docstring = (yyvsp[(2) - (2)].module).docstring; break;
            }
        }
    break;

  case 226:
#line 2104 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.module).token = TK_IF;
        }
    break;

  case 227:
#line 2107 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.module).token = TK_END;
        }
    break;

  case 228:
#line 2110 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.module).token = TK_AUTOPYNAME;
        }
    break;

  case 229:
#line 2113 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping())
            {
                (yyval.module).token = TK_DOCSTRING;
                (yyval.module).docstring = (yyvsp[(1) - (1)].docstr);
            }
            else
            {
                (yyval.module).token = 0;
                (yyval.module).docstring = NULL;
            }
        }
    break;

  case 231:
#line 2128 "sip-4.19.12/sipgen/metasrc/parser.y"
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

  case 232:
#line 2144 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.number) = -1;
        }
    break;

  case 234:
#line 2150 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if ((yyvsp[(2) - (2)].include).name == NULL)
                yyerror("%Include must have a 'name' argument");

            if (notSkipping())
                parseFile(NULL, (yyvsp[(2) - (2)].include).name, NULL, (yyvsp[(2) - (2)].include).optional);
        }
    break;

  case 235:
#line 2159 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            resetLexerState();

            (yyval.include).name = (yyvsp[(1) - (1)].text);
            (yyval.include).optional = FALSE;
        }
    break;

  case 236:
#line 2165 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.include) = (yyvsp[(2) - (3)].include);
        }
    break;

  case 238:
#line 2171 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.include) = (yyvsp[(1) - (3)].include);

            switch ((yyvsp[(3) - (3)].include).token)
            {
            case TK_NAME: (yyval.include).name = (yyvsp[(3) - (3)].include).name; break;
            case TK_OPTIONAL: (yyval.include).optional = (yyvsp[(3) - (3)].include).optional; break;
            }
        }
    break;

  case 239:
#line 2182 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.include).token = TK_NAME;

            (yyval.include).name = (yyvsp[(3) - (3)].text);
            (yyval.include).optional = FALSE;
        }
    break;

  case 240:
#line 2188 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.include).token = TK_OPTIONAL;

            (yyval.include).name = NULL;
            (yyval.include).optional = (yyvsp[(3) - (3)].boolean);
        }
    break;

  case 241:
#line 2196 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            deprecated("%OptionalInclude is deprecated, use %Include and the 'optional' argument instead");

            if (notSkipping())
                parseFile(NULL, (yyvsp[(2) - (2)].text), NULL, TRUE);
        }
    break;

  case 242:
#line 2204 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping())
                newImport((yyvsp[(2) - (2)].import).name);
        }
    break;

  case 243:
#line 2210 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            resetLexerState();

            (yyval.import).name = (yyvsp[(1) - (1)].text);
        }
    break;

  case 244:
#line 2215 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.import) = (yyvsp[(2) - (3)].import);
        }
    break;

  case 246:
#line 2221 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.import) = (yyvsp[(1) - (3)].import);

            switch ((yyvsp[(3) - (3)].import).token)
            {
            case TK_NAME: (yyval.import).name = (yyvsp[(3) - (3)].import).name; break;
            }
        }
    break;

  case 247:
#line 2231 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.import).token = TK_NAME;

            (yyval.import).name = (yyvsp[(3) - (3)].text);
        }
    break;

  case 248:
#line 2238 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.codeb) = NULL;
        }
    break;

  case 249:
#line 2241 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.codeb) = (yyvsp[(2) - (2)].codeb);
        }
    break;

  case 250:
#line 2246 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.codeb) = NULL;
        }
    break;

  case 251:
#line 2249 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.codeb) = (yyvsp[(2) - (2)].codeb);
        }
    break;

  case 252:
#line 2254 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.codeb) = NULL;
        }
    break;

  case 253:
#line 2257 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.codeb) = (yyvsp[(2) - (2)].codeb);
        }
    break;

  case 254:
#line 2262 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping())
                appendCodeBlock(&currentModule->copying, (yyvsp[(2) - (2)].codeb));
        }
    break;

  case 255:
#line 2268 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping())
                appendCodeBlock(&currentSpec->exphdrcode, (yyvsp[(2) - (2)].codeb));
        }
    break;

  case 256:
#line 2274 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping())
                appendCodeBlock(&currentModule->hdrcode, (yyvsp[(2) - (2)].codeb));
        }
    break;

  case 257:
#line 2280 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.codeb) = (yyvsp[(2) - (2)].codeb);
        }
    break;

  case 258:
#line 2285 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.codeb) = (yyvsp[(2) - (2)].codeb);
        }
    break;

  case 259:
#line 2290 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.codeb) = (yyvsp[(2) - (2)].codeb);
        }
    break;

  case 260:
#line 2295 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.codeb) = (yyvsp[(2) - (2)].codeb);
        }
    break;

  case 261:
#line 2300 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.codeb) = (yyvsp[(2) - (2)].codeb);
        }
    break;

  case 262:
#line 2305 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.codeb) = (yyvsp[(2) - (2)].codeb);
        }
    break;

  case 263:
#line 2310 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.codeb) = (yyvsp[(2) - (2)].codeb);
        }
    break;

  case 264:
#line 2315 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.codeb) = (yyvsp[(2) - (2)].codeb);
        }
    break;

  case 265:
#line 2320 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.codeb) = (yyvsp[(2) - (2)].codeb);
        }
    break;

  case 266:
#line 2325 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.codeb) = (yyvsp[(2) - (2)].codeb);
        }
    break;

  case 267:
#line 2330 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.codeb) = (yyvsp[(2) - (2)].codeb);
        }
    break;

  case 268:
#line 2335 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.codeb) = (yyvsp[(2) - (2)].codeb);
        }
    break;

  case 269:
#line 2340 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping())
                appendCodeBlock(&currentModule->cppcode, (yyvsp[(2) - (2)].codeb));
        }
    break;

  case 270:
#line 2346 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.codeb) = (yyvsp[(2) - (2)].codeb);
        }
    break;

  case 271:
#line 2351 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping())
                appendCodeBlock(&currentModule->preinitcode, (yyvsp[(2) - (2)].codeb));
        }
    break;

  case 272:
#line 2357 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping())
                appendCodeBlock(&currentModule->initcode, (yyvsp[(2) - (2)].codeb));
        }
    break;

  case 273:
#line 2363 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping())
                appendCodeBlock(&currentModule->postinitcode, (yyvsp[(2) - (2)].codeb));
        }
    break;

  case 274:
#line 2369 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping())
                appendCodeBlock(&currentModule->unitcode, (yyvsp[(2) - (2)].codeb));
        }
    break;

  case 275:
#line 2375 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping())
                appendCodeBlock(&currentModule->unitpostinccode, (yyvsp[(2) - (2)].codeb));
        }
    break;

  case 276:
#line 2381 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            /* Deprecated. */
        }
    break;

  case 277:
#line 2386 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping() && !inMainModule())
                appendCodeBlock(&currentSpec->exptypehintcode, (yyvsp[(2) - (2)].codeb));
        }
    break;

  case 278:
#line 2392 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping())
                appendCodeBlock(&currentModule->typehintcode, (yyvsp[(2) - (2)].codeb));
        }
    break;

  case 279:
#line 2398 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.codeb) = (yyvsp[(2) - (2)].codeb);
        }
    break;

  case 280:
#line 2403 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping() && inMainModule())
                appendCodeBlock(&currentSpec->docs, (yyvsp[(2) - (2)].codeb));
        }
    break;

  case 281:
#line 2409 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping())
                appendCodeBlock(&currentSpec->docs, (yyvsp[(2) - (2)].codeb));
        }
    break;

  case 282:
#line 2415 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping())
                addAutoPyName(currentModule, (yyvsp[(2) - (2)].autopyname).remove_leading);
        }
    break;

  case 283:
#line 2421 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.autopyname) = (yyvsp[(2) - (3)].autopyname);
        }
    break;

  case 285:
#line 2427 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.autopyname) = (yyvsp[(1) - (3)].autopyname);

            switch ((yyvsp[(3) - (3)].autopyname).token)
            {
            case TK_REMOVELEADING: (yyval.autopyname).remove_leading = (yyvsp[(3) - (3)].autopyname).remove_leading; break;
            }
        }
    break;

  case 286:
#line 2437 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.autopyname).token = TK_REMOVELEADING;

            (yyval.autopyname).remove_leading = (yyvsp[(3) - (3)].text);
        }
    break;

  case 287:
#line 2444 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.docstr) = sipMalloc(sizeof(docstringDef));

            (yyval.docstr)->signature = (yyvsp[(2) - (3)].docstring).signature;
            (yyval.docstr)->text = (yyvsp[(3) - (3)].codeb)->frag;
            free((yyvsp[(3) - (3)].codeb));

            /* Format the docstring. */
            if ((yyvsp[(2) - (3)].docstring).format == deindented)
            {
                const char *cp;
                char *dp;
                int min_indent, indent, skipping;

                /* Find the common indent. */
                min_indent = -1;
                indent = 0;
                skipping = FALSE;

                for (cp = (yyval.docstr)->text; *cp != '\0'; ++cp)
                {
                    if (skipping)
                    {
                        /*
                         * We have handled the indent and are just looking for
                         * the end of the line.
                         */
                        if (*cp == '\n')
                            skipping = FALSE;
                    }
                    else
                    {
                        if (*cp == ' ')
                        {
                            ++indent;
                        }
                        else if (*cp != '\n')
                        {
                            if (min_indent < 0 || min_indent > indent)
                                min_indent = indent;

                            /* Ignore the remaining characters of the line. */
                            skipping = TRUE;
                        }
                    }
                }

                /* In case the last line doesn't have a trailing newline. */
                if (min_indent < 0 || min_indent > indent)
                    min_indent = indent;

                /*
                 * Go through the text again removing the common indentation.
                 */
                cp = dp = (yyval.docstr)->text;

                while (*cp != '\0')
                {
                    const char *start = cp;
                    int non_blank = FALSE;

                    /* Find the end of the line. */
                    while (*cp != '\n' && *cp != '\0')
                        if (*cp++ != ' ')
                            non_blank = TRUE;

                    /* Find where we are copying from. */
                    if (non_blank)
                    {
                        start += min_indent;

                        while (*start != '\n' && *start != '\0')
                            *dp++ = *start++;
                    }

                    if (*cp == '\n')
                        *dp++ = *cp++;
                }

                *dp = '\0';
            }
        }
    break;

  case 288:
#line 2528 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.docstring).format = currentModule->defdocstringfmt;
            (yyval.docstring).signature = currentModule->defdocstringsig;
        }
    break;

  case 289:
#line 2532 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            resetLexerState();

            (yyval.docstring).format = convertFormat((yyvsp[(1) - (1)].text));
            (yyval.docstring).signature = currentModule->defdocstringsig;
        }
    break;

  case 290:
#line 2538 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.docstring) = (yyvsp[(2) - (3)].docstring);
        }
    break;

  case 292:
#line 2544 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.docstring) = (yyvsp[(1) - (3)].docstring);

            switch ((yyvsp[(3) - (3)].docstring).token)
            {
            case TK_FORMAT: (yyval.docstring).format = (yyvsp[(3) - (3)].docstring).format; break;
            case TK_SIGNATURE: (yyval.docstring).signature = (yyvsp[(3) - (3)].docstring).signature; break;
            }
        }
    break;

  case 293:
#line 2555 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.docstring).token = TK_FORMAT;

            (yyval.docstring).format = convertFormat((yyvsp[(3) - (3)].text));
            (yyval.docstring).signature = currentModule->defdocstringsig;
        }
    break;

  case 294:
#line 2561 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.docstring).token = TK_SIGNATURE;

            (yyval.docstring).format = currentModule->defdocstringfmt;
            (yyval.docstring).signature = convertSignature((yyvsp[(3) - (3)].text));
        }
    break;

  case 295:
#line 2569 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.docstr) = NULL;
        }
    break;

  case 297:
#line 2575 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if ((yyvsp[(2) - (3)].extract).id == NULL)
                yyerror("%Extract must have an 'id' argument");

            if (notSkipping())
                addExtractPart(currentSpec, (yyvsp[(2) - (3)].extract).id, (yyvsp[(2) - (3)].extract).order, (yyvsp[(3) - (3)].codeb));
        }
    break;

  case 298:
#line 2584 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            resetLexerState();

            (yyval.extract).id = (yyvsp[(1) - (1)].text);
            (yyval.extract).order = -1;
        }
    break;

  case 299:
#line 2590 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.extract) = (yyvsp[(2) - (3)].extract);
        }
    break;

  case 301:
#line 2596 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.extract) = (yyvsp[(1) - (3)].extract);

            switch ((yyvsp[(3) - (3)].extract).token)
            {
            case TK_ID: (yyval.extract).id = (yyvsp[(3) - (3)].extract).id; break;
            case TK_ORDER: (yyval.extract).order = (yyvsp[(3) - (3)].extract).order; break;
            }
        }
    break;

  case 302:
#line 2607 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.extract).token = TK_ID;

            (yyval.extract).id = (yyvsp[(3) - (3)].text);
            (yyval.extract).order = -1;
        }
    break;

  case 303:
#line 2613 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.extract).token = TK_ORDER;

            if ((yyvsp[(3) - (3)].number) < 0)
                yyerror("The 'order' of an %Extract directive must not be negative");

            (yyval.extract).id = NULL;
            (yyval.extract).order = (yyvsp[(3) - (3)].number);
        }
    break;

  case 304:
#line 2624 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            /* Deprecated. */
        }
    break;

  case 307:
#line 2633 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.codeb) = (yyvsp[(1) - (2)].codeb);

            append(&(yyval.codeb)->frag, (yyvsp[(2) - (2)].codeb)->frag);

            free((yyvsp[(2) - (2)].codeb)->frag);
            free((yyvsp[(2) - (2)].codeb));
        }
    break;

  case 308:
#line 2643 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping())
            {
                const char *annos[] = {
                    "NoScope",
                    "NoTypeHint",
                    "PyName",
                    NULL
                };

                checkAnnos(&(yyvsp[(4) - (4)].optflags), annos);

                if (sectionFlags != 0 && (sectionFlags & ~(SECT_IS_PUBLIC | SECT_IS_PROT)) != 0)
                    yyerror("Class enums must be in the public or protected sections");

                if (currentSpec->genc && (yyvsp[(2) - (4)].boolean))
                    yyerror("Scoped enums not allowed in a C module");

                currentEnum = newEnum(currentSpec, currentModule,
                        currentMappedType, (yyvsp[(3) - (4)].text), &(yyvsp[(4) - (4)].optflags), sectionFlags, (yyvsp[(2) - (4)].boolean));
            }
        }
    break;

  case 310:
#line 2667 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.boolean) = FALSE;
        }
    break;

  case 311:
#line 2670 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.boolean) = TRUE;
        }
    break;

  case 312:
#line 2673 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.boolean) = TRUE;
        }
    break;

  case 313:
#line 2678 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.text) = NULL;
        }
    break;

  case 314:
#line 2681 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.text) = (yyvsp[(1) - (1)].text);
        }
    break;

  case 315:
#line 2686 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.text) = NULL;
        }
    break;

  case 316:
#line 2689 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.text) = (yyvsp[(1) - (1)].text);
        }
    break;

  case 323:
#line 2704 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping())
            {
                const char *annos[] = {
                    "NoTypeHint",
                    "PyName",
                    NULL
                };

                enumMemberDef *emd, **tail;

                checkAnnos(&(yyvsp[(3) - (4)].optflags), annos);

                /* Note that we don't use the assigned value. */
                emd = sipMalloc(sizeof (enumMemberDef));

                emd->pyname = cacheName(currentSpec,
                        getPythonName(currentModule, &(yyvsp[(3) - (4)].optflags), (yyvsp[(1) - (4)].text)));
                emd->cname = (yyvsp[(1) - (4)].text);
                emd->no_typehint = getNoTypeHint(&(yyvsp[(3) - (4)].optflags));
                emd->ed = currentEnum;
                emd->platforms = currentPlatforms;
                emd->next = NULL;

                checkAttributes(currentSpec, currentModule, emd->ed->ecd,
                        emd->ed->emtd, emd->pyname->text, FALSE);

                /* Append to preserve the order. */
                for (tail = &currentEnum->members; *tail != NULL; tail = &(*tail)->next)
                    ;

                *tail = emd;

                if (inMainModule())
                    setIsUsedName(emd->pyname);
            }
        }
    break;

  case 328:
#line 2751 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.valp) = NULL;
        }
    break;

  case 329:
#line 2754 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.valp) = (yyvsp[(2) - (2)].valp);
        }
    break;

  case 331:
#line 2760 "sip-4.19.12/sipgen/metasrc/parser.y"
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

  case 332:
#line 2778 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.qchar) = '-';
        }
    break;

  case 333:
#line 2781 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.qchar) = '+';
        }
    break;

  case 334:
#line 2784 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.qchar) = '*';
        }
    break;

  case 335:
#line 2787 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.qchar) = '/';
        }
    break;

  case 336:
#line 2790 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.qchar) = '&';
        }
    break;

  case 337:
#line 2793 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.qchar) = '|';
        }
    break;

  case 338:
#line 2798 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.qchar) = '\0';
        }
    break;

  case 339:
#line 2801 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.qchar) = '!';
        }
    break;

  case 340:
#line 2804 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.qchar) = '~';
        }
    break;

  case 341:
#line 2807 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.qchar) = '-';
        }
    break;

  case 342:
#line 2810 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.qchar) = '+';
        }
    break;

  case 343:
#line 2813 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.qchar) = '*';
        }
    break;

  case 344:
#line 2816 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.qchar) = '&';
        }
    break;

  case 345:
#line 2821 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if ((yyvsp[(2) - (3)].qchar) != '\0' && (yyvsp[(3) - (3)].value).vtype == string_value)
                yyerror("Invalid unary operator for string");
 
            /* Convert the value to a simple expression on the heap. */
            (yyval.valp) = sipMalloc(sizeof (valueDef));
 
            *(yyval.valp) = (yyvsp[(3) - (3)].value);
            (yyval.valp)->vunop = (yyvsp[(2) - (3)].qchar);
            (yyval.valp)->vbinop = '\0';
            (yyval.valp)->cast = (yyvsp[(1) - (3)].scpvalp);
            (yyval.valp)->next = NULL;
        }
    break;

  case 346:
#line 2836 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.scpvalp) = NULL;
        }
    break;

  case 347:
#line 2839 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.scpvalp) = (yyvsp[(2) - (3)].scpvalp);
        }
    break;

  case 348:
#line 2844 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (currentSpec->genc)
                yyerror("Scoped names are not allowed in a C module");

            (yyval.scpvalp) = scopeScopedName(NULL, (yyvsp[(2) - (2)].scpvalp));
        }
    break;

  case 351:
#line 2854 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (currentSpec->genc)
                yyerror("Scoped names are not allowed in a C module");

            appendScopedName(&(yyvsp[(1) - (3)].scpvalp), (yyvsp[(3) - (3)].scpvalp));
        }
    break;

  case 352:
#line 2862 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.scpvalp) = text2scopePart((yyvsp[(1) - (1)].text));
        }
    break;

  case 353:
#line 2867 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.boolean) = TRUE;
        }
    break;

  case 354:
#line 2870 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.boolean) = FALSE;
        }
    break;

  case 355:
#line 2875 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            /*
             * We let the C++ compiler decide if the value is a valid one - no
             * point in building a full C++ parser here.
             */

            (yyval.value).vtype = scoped_value;
            (yyval.value).u.vscp = (yyvsp[(1) - (1)].scpvalp);
        }
    break;

  case 356:
#line 2884 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            fcallDef *fcd;

            fcd = sipMalloc(sizeof (fcallDef));
            *fcd = (yyvsp[(3) - (4)].fcall);
            fcd -> type = (yyvsp[(1) - (4)].memArg);

            (yyval.value).vtype = fcall_value;
            (yyval.value).u.fcd = fcd;
        }
    break;

  case 357:
#line 2894 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.value).vtype = real_value;
            (yyval.value).u.vreal = (yyvsp[(1) - (1)].real);
        }
    break;

  case 358:
#line 2898 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.value).vtype = numeric_value;
            (yyval.value).u.vnum = (yyvsp[(1) - (1)].number);
        }
    break;

  case 359:
#line 2902 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.value).vtype = numeric_value;
            (yyval.value).u.vnum = (yyvsp[(1) - (1)].boolean);
        }
    break;

  case 360:
#line 2906 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.value).vtype = numeric_value;
            (yyval.value).u.vnum = 0;
        }
    break;

  case 361:
#line 2910 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.value).vtype = string_value;
            (yyval.value).u.vstr = (yyvsp[(1) - (1)].text);
        }
    break;

  case 362:
#line 2914 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.value).vtype = qchar_value;
            (yyval.value).u.vqchar = (yyvsp[(1) - (1)].qchar);
        }
    break;

  case 363:
#line 2920 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            /* No values. */

            (yyval.fcall).nrArgs = 0;
        }
    break;

  case 364:
#line 2925 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            /* The single or first expression. */

            (yyval.fcall).args[0] = (yyvsp[(1) - (1)].valp);
            (yyval.fcall).nrArgs = 1;
        }
    break;

  case 365:
#line 2931 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            /* Check that it wasn't ...(,expression...). */

            if ((yyval.fcall).nrArgs == 0)
                yyerror("First argument to function call is missing");

            /* Check there is room. */

            if ((yyvsp[(1) - (3)].fcall).nrArgs == MAX_NR_ARGS)
                yyerror("Internal error - increase the value of MAX_NR_ARGS");

            (yyval.fcall) = (yyvsp[(1) - (3)].fcall);

            (yyval.fcall).args[(yyval.fcall).nrArgs] = (yyvsp[(3) - (3)].valp);
            (yyval.fcall).nrArgs++;
        }
    break;

  case 366:
#line 2949 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping())
            {
                const char *annos[] = {
                    "Capsule",
                    "DocType",
                    "Encoding",
                    "NoTypeName",
                    "PyInt",
                    "PyName",
                    "TypeHint",
                    "TypeHintIn",
                    "TypeHintOut",
                    NULL
                };

                checkAnnos(&(yyvsp[(4) - (6)].optflags), annos);

                applyTypeFlags(currentModule, &(yyvsp[(2) - (6)].memArg), &(yyvsp[(4) - (6)].optflags));
                newTypedef(currentSpec, currentModule, (yyvsp[(3) - (6)].text), &(yyvsp[(2) - (6)].memArg), &(yyvsp[(4) - (6)].optflags), (yyvsp[(6) - (6)].docstr));
            }
        }
    break;

  case 367:
#line 2971 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping())
            {
                const char *annos[] = {
                    "DocType",
                    "Encoding",
                    "NoTypeName",
                    "PyInt",
                    "PyName",
                    "TypeHint",
                    "TypeHintIn",
                    "TypeHintOut",
                    NULL
                };

                signatureDef *sig;
                argDef ftype;

                checkAnnos(&(yyvsp[(10) - (12)].optflags), annos);

                applyTypeFlags(currentModule, &(yyvsp[(2) - (12)].memArg), &(yyvsp[(10) - (12)].optflags));

                memset(&ftype, 0, sizeof (argDef));

                /* Create the full signature on the heap. */
                sig = sipMalloc(sizeof (signatureDef));
                *sig = (yyvsp[(8) - (12)].signature);
                sig->result = (yyvsp[(2) - (12)].memArg);

                /* Create the full type. */
                ftype.atype = function_type;
                ftype.nrderefs = 1;
                ftype.u.sa = sig;

                newTypedef(currentSpec, currentModule, (yyvsp[(5) - (12)].text), &ftype, &(yyvsp[(10) - (12)].optflags), (yyvsp[(12) - (12)].docstr));
            }
        }
    break;

  case 368:
#line 3010 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (currentSpec -> genc && (yyvsp[(2) - (2)].scpvalp)->next != NULL)
                yyerror("Namespaces not allowed in a C module");

            if (notSkipping())
                currentSupers = NULL;
        }
    break;

  case 369:
#line 3016 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping())
            {
                const char *annos[] = {
                    "Abstract",
                    "AllowNone",
                    "API",
                    "DelayDtor",
                    "Deprecated",
                    "ExportDerived",
                    "External",
                    "FileExtension",
                    "Metatype",
                    "Mixin",
                    "NoDefaultCtors",
                    "NoTypeHint",
                    "PyName",
                    "PyQtFlags",
                    "PyQtInterface",
                    "PyQtNoQMetaObject",
                    "Supertype",
                    "TypeHint",
                    "TypeHintIn",
                    "TypeHintOut",
                    "TypeHintValue",
                    "VirtualErrorHandler",
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

  case 370:
#line 3053 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping())
                completeClass((yyvsp[(2) - (8)].scpvalp), &(yyvsp[(5) - (8)].optflags), (yyvsp[(7) - (8)].boolean));
        }
    break;

  case 371:
#line 3059 "sip-4.19.12/sipgen/metasrc/parser.y"
    {currentIsTemplate = TRUE;}
    break;

  case 372:
#line 3059 "sip-4.19.12/sipgen/metasrc/parser.y"
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

  case 373:
#line 3085 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.signature) = (yyvsp[(3) - (4)].signature);
        }
    break;

  case 374:
#line 3090 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (currentSpec->genc)
                yyerror("Class definition not allowed in a C module");

            if (notSkipping())
                currentSupers = NULL;
        }
    break;

  case 375:
#line 3096 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping())
            {
                const char *annos[] = {
                    "Abstract",
                    "AllowNone",
                    "API",
                    "DelayDtor",
                    "Deprecated",
                    "ExportDerived",
                    "External",
                    "FileExtension",
                    "Metatype",
                    "Mixin",
                    "NoDefaultCtors",
                    "PyName",
                    "PyQtFlags",
                    "PyQtInterface",
                    "PyQtNoQMetaObject",
                    "Supertype",
                    "TypeHint",
                    "TypeHintIn",
                    "TypeHintOut",
                    "TypeHintValue",
                    "VirtualErrorHandler",
                    NULL
                };

                checkAnnos(&(yyvsp[(5) - (5)].optflags), annos);

                defineClass((yyvsp[(2) - (5)].scpvalp), currentSupers, &(yyvsp[(5) - (5)].optflags));
                sectionFlags = SECT_IS_PRIVATE;
            }
        }
    break;

  case 376:
#line 3129 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping())
                (yyval.klass) = completeClass((yyvsp[(2) - (8)].scpvalp), &(yyvsp[(5) - (8)].optflags), (yyvsp[(7) - (8)].boolean));
        }
    break;

  case 381:
#line 3143 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping() && (yyvsp[(1) - (2)].token) == TK_PUBLIC)
            {
                argDef ad;
                classDef *super;
                scopedNameDef *snd = (yyvsp[(2) - (2)].scpvalp);

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

                /*
                 * This is a bug because we should look in the local scope
                 * rather than assume it is in the global scope.
                 */
                if (snd->name[0] != '\0')
                    snd = scopeScopedName(NULL, snd);

                /*
                 * Note that passing NULL as the API is a bug.  Instead we
                 * should pass the API of the sub-class being defined,
                 * otherwise we cannot create sub-classes of versioned classes.
                 */
                super = findClass(currentSpec, class_iface, NULL, snd,
                        currentIsTemplate);
                appendToClassList(&currentSupers, super);
            }
        }
    break;

  case 382:
#line 3195 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
        (yyval.token) = TK_PUBLIC;
        }
    break;

  case 383:
#line 3198 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
        (yyval.token) = TK_PUBLIC;
        }
    break;

  case 384:
#line 3201 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
        (yyval.token) = TK_PROTECTED;
        }
    break;

  case 385:
#line 3204 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
        (yyval.token) = TK_PRIVATE;
        }
    break;

  case 386:
#line 3209 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.boolean) = FALSE;
        }
    break;

  case 387:
#line 3212 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.boolean) = TRUE;
        }
    break;

  case 401:
#line 3232 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping())
            {
                classDef *scope = currentScope();

                if (scope->docstring != NULL)
                    yyerror("%Docstring already given for class");

                scope->docstring = (yyvsp[(1) - (1)].docstr);
            }
        }
    break;

  case 402:
#line 3243 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping())
                appendCodeBlock(&currentScope()->cppcode, (yyvsp[(1) - (1)].codeb));
        }
    break;

  case 403:
#line 3247 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping())
                appendCodeBlock(&currentScope()->iff->hdrcode, (yyvsp[(1) - (1)].codeb));
        }
    break;

  case 404:
#line 3251 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping())
            {
                classDef *scope = currentScope();

                if (scope->travcode != NULL)
                    yyerror("%GCTraverseCode already given for class");

                appendCodeBlock(&scope->travcode, (yyvsp[(1) - (1)].codeb));
            }
        }
    break;

  case 405:
#line 3262 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping())
            {
                classDef *scope = currentScope();

                if (scope->clearcode != NULL)
                    yyerror("%GCClearCode already given for class");

                appendCodeBlock(&scope->clearcode, (yyvsp[(1) - (1)].codeb));
            }
        }
    break;

  case 406:
#line 3273 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping())
            {
                classDef *scope = currentScope();

                if (scope->getbufcode != NULL)
                    yyerror("%BIGetBufferCode already given for class");

                appendCodeBlock(&scope->getbufcode, (yyvsp[(1) - (1)].codeb));
            }
        }
    break;

  case 407:
#line 3284 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping())
            {
                classDef *scope = currentScope();

                if (scope->releasebufcode != NULL)
                    yyerror("%BIReleaseBufferCode already given for class");

                appendCodeBlock(&scope->releasebufcode, (yyvsp[(1) - (1)].codeb));
            }
        }
    break;

  case 408:
#line 3295 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping())
            {
                classDef *scope = currentScope();

                if (scope->readbufcode != NULL)
                    yyerror("%BIGetReadBufferCode already given for class");

                appendCodeBlock(&scope->readbufcode, (yyvsp[(1) - (1)].codeb));
            }
        }
    break;

  case 409:
#line 3306 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping())
            {
                classDef *scope = currentScope();

                if (scope->writebufcode != NULL)
                    yyerror("%BIGetWriteBufferCode already given for class");

                appendCodeBlock(&scope->writebufcode, (yyvsp[(1) - (1)].codeb));
            }
        }
    break;

  case 410:
#line 3317 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping())
            {
                classDef *scope = currentScope();

                if (scope->segcountcode != NULL)
                    yyerror("%BIGetSegCountCode already given for class");

                appendCodeBlock(&scope->segcountcode, (yyvsp[(1) - (1)].codeb));
            }
        }
    break;

  case 411:
#line 3328 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping())
            {
                classDef *scope = currentScope();

                if (scope->charbufcode != NULL)
                    yyerror("%BIGetCharBufferCode already given for class");

                appendCodeBlock(&scope->charbufcode, (yyvsp[(1) - (1)].codeb));
            }
        }
    break;

  case 412:
#line 3339 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping())
            {
                classDef *scope = currentScope();

                if (scope->instancecode != NULL)
                    yyerror("%InstanceCode already given for class");

                appendCodeBlock(&scope->instancecode, (yyvsp[(1) - (1)].codeb));
            }
        }
    break;

  case 413:
#line 3350 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping())
            {
                classDef *scope = currentScope();

                if (scope->picklecode != NULL)
                    yyerror("%PickleCode already given for class");

                appendCodeBlock(&scope->picklecode, (yyvsp[(1) - (1)].codeb));
            }
        }
    break;

  case 414:
#line 3361 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping())
            {
                classDef *scope = currentScope();

                if (scope->finalcode != NULL)
                    yyerror("%FinalisationCode already given for class");

                appendCodeBlock(&scope->finalcode, (yyvsp[(1) - (1)].codeb));
            }
        }
    break;

  case 415:
#line 3372 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping())
            {
                classDef *scope = currentScope();

                if (scope->typehintcode != NULL)
                    yyerror("%TypeHintCode already given for class");

                appendCodeBlock(&scope->typehintcode, (yyvsp[(1) - (1)].codeb));
            }
        }
    break;

  case 419:
#line 3386 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping())
            {
                classDef *scope = currentScope();

                if (scope->convtosubcode != NULL)
                    yyerror("Class has more than one %ConvertToSubClassCode directive");

                appendCodeBlock(&scope->convtosubcode, (yyvsp[(2) - (2)].codeb));
            }
        }
    break;

  case 420:
#line 3397 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping())
            {
                classDef *scope = currentScope();

                if (scope->convtocode != NULL)
                    yyerror("Class has more than one %ConvertToTypeCode directive");

                appendCodeBlock(&scope->convtocode, (yyvsp[(2) - (2)].codeb));
            }
        }
    break;

  case 421:
#line 3408 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping())
            {
                classDef *scope = currentScope();

                if (scope->convfromcode != NULL)
                    yyerror("Class has more than one %ConvertFromTypeCode directive");

                appendCodeBlock(&scope->convfromcode, (yyvsp[(2) - (2)].codeb));
            }
        }
    break;

  case 422:
#line 3419 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (currentSpec -> genc)
                yyerror("public section not allowed in a C module");

            if (notSkipping())
                sectionFlags = SECT_IS_PUBLIC | (yyvsp[(2) - (3)].number);
        }
    break;

  case 423:
#line 3426 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (currentSpec -> genc)
                yyerror("protected section not allowed in a C module");

            if (notSkipping())
                sectionFlags = SECT_IS_PROT | (yyvsp[(2) - (3)].number);
        }
    break;

  case 424:
#line 3433 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (currentSpec -> genc)
                yyerror("private section not allowed in a C module");

            if (notSkipping())
                sectionFlags = SECT_IS_PRIVATE | (yyvsp[(2) - (3)].number);
        }
    break;

  case 425:
#line 3440 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (currentSpec -> genc)
                yyerror("signals section not allowed in a C module");

            if (notSkipping())
                sectionFlags = SECT_IS_SIGNAL;
        }
    break;

  case 426:
#line 3449 "sip-4.19.12/sipgen/metasrc/parser.y"
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

  case 427:
#line 3462 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.property) = (yyvsp[(2) - (3)].property);
        }
    break;

  case 429:
#line 3468 "sip-4.19.12/sipgen/metasrc/parser.y"
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

  case 430:
#line 3480 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.property).token = TK_GET;

            (yyval.property).get = (yyvsp[(3) - (3)].text);
            (yyval.property).name = NULL;
            (yyval.property).set = NULL;
        }
    break;

  case 431:
#line 3487 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.property).token = TK_NAME;

            (yyval.property).get = NULL;
            (yyval.property).name = (yyvsp[(3) - (3)].text);
            (yyval.property).set = NULL;
        }
    break;

  case 432:
#line 3494 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.property).token = TK_SET;

            (yyval.property).get = NULL;
            (yyval.property).name = NULL;
            (yyval.property).set = (yyvsp[(3) - (3)].text);
        }
    break;

  case 433:
#line 3503 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.property).token = 0;
            (yyval.property).docstring = NULL;
        }
    break;

  case 434:
#line 3507 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.property) = (yyvsp[(2) - (4)].property);
        }
    break;

  case 436:
#line 3513 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.property) = (yyvsp[(1) - (2)].property);

            switch ((yyvsp[(2) - (2)].property).token)
            {
            case TK_DOCSTRING: (yyval.property).docstring = (yyvsp[(2) - (2)].property).docstring; break;
            }
        }
    break;

  case 437:
#line 3523 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.property).token = TK_IF;
        }
    break;

  case 438:
#line 3526 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.property).token = TK_END;
        }
    break;

  case 439:
#line 3529 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping())
            {
                (yyval.property).token = TK_DOCSTRING;
                (yyval.property).docstring = (yyvsp[(1) - (1)].docstr);
            }
            else
            {
                (yyval.property).token = 0;
                (yyval.property).docstring = NULL;
            }
        }
    break;

  case 442:
#line 3547 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.number) = 0;
        }
    break;

  case 443:
#line 3550 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.number) = SECT_IS_SLOT;
        }
    break;

  case 444:
#line 3555 "sip-4.19.12/sipgen/metasrc/parser.y"
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

                checkAnnos(&(yyvsp[(8) - (12)].optflags), annos);

                if (strcmp(classBaseName(cd),(yyvsp[(3) - (12)].text)) != 0)
                    yyerror("Destructor doesn't have the same name as its class");

                if (isDtor(cd))
                    yyerror("Destructor has already been defined");

                if (currentSpec -> genc && (yyvsp[(10) - (12)].codeb) == NULL)
                    yyerror("Destructor in C modules must include %MethodCode");


                appendCodeBlock(&cd->dealloccode, (yyvsp[(10) - (12)].codeb)); /* premethodcode */
                appendCodeBlock(&cd->dealloccode, (yyvsp[(11) - (12)].codeb)); /* methodcode */
                appendCodeBlock(&cd->dtorcode, (yyvsp[(12) - (12)].codeb));
                cd -> dtorexceptions = (yyvsp[(6) - (12)].throwlist);

                /*
                 * Note that we don't apply the protected/public hack to dtors
                 * as it (I think) may change the behaviour of the wrapped API.
                 */
                cd->classflags |= sectionFlags;

                if ((yyvsp[(7) - (12)].number))
                {
                    if (!(yyvsp[(1) - (12)].number))
                        yyerror("Abstract destructor must be virtual");

                    setIsAbstractClass(cd);
                }

                /*
                 * The class has a shadow if we have a virtual dtor or some
                 * dtor code.
                 */
                if ((yyvsp[(1) - (12)].number) || (yyvsp[(11) - (12)].codeb) != NULL)
                {
                    if (currentSpec -> genc)
                        yyerror("Virtual destructor or %VirtualCatcherCode not allowed in a C module");

                    setNeedsShadow(cd);
                }

                if (getReleaseGIL(&(yyvsp[(8) - (12)].optflags)))
                    setIsReleaseGILDtor(cd);
                else if (getHoldGIL(&(yyvsp[(8) - (12)].optflags)))
                    setIsHoldGILDtor(cd);
            }
        }
    break;

  case 445:
#line 3619 "sip-4.19.12/sipgen/metasrc/parser.y"
    {currentCtorIsExplicit = TRUE;}
    break;

  case 448:
#line 3623 "sip-4.19.12/sipgen/metasrc/parser.y"
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
                    "NoRaisesPyException",
                    "NoTypeHint",
                    "PostHook",
                    "PreHook",
                    "RaisesPyException",
                    "ReleaseGIL",
                    "Transfer",
                    NULL
                };

                checkAnnos(&(yyvsp[(6) - (11)].optflags), annos);

                if (currentSpec -> genc)
                {
                    if ((yyvsp[(10) - (11)].codeb) == NULL && (yyvsp[(3) - (11)].signature).nrArgs != 0)
                        yyerror("Constructors with arguments in C modules must include %MethodCode");

                    if (currentCtorIsExplicit)
                        yyerror("Explicit constructors not allowed in a C module");
                }

                if ((sectionFlags & (SECT_IS_PUBLIC | SECT_IS_PROT | SECT_IS_PRIVATE)) == 0)
                    yyerror("Constructor must be in the public, private or protected sections");

                newCtor(currentModule, (yyvsp[(1) - (11)].text), sectionFlags, &(yyvsp[(3) - (11)].signature), &(yyvsp[(6) - (11)].optflags), (yyvsp[(11) - (11)].codeb), (yyvsp[(5) - (11)].throwlist), (yyvsp[(7) - (11)].optsignature),
                        currentCtorIsExplicit, (yyvsp[(9) - (11)].docstr), (yyvsp[(10) - (11)].codeb));
            }

            free((yyvsp[(1) - (11)].text));

            currentCtorIsExplicit = FALSE;
        }
    break;

  case 449:
#line 3669 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.optsignature) = NULL;
        }
    break;

  case 450:
#line 3672 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            parsingCSignature = TRUE;
        }
    break;

  case 451:
#line 3674 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.optsignature) = sipMalloc(sizeof (signatureDef));

            *(yyval.optsignature) = (yyvsp[(4) - (6)].signature);

            parsingCSignature = FALSE;
        }
    break;

  case 452:
#line 3683 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.optsignature) = NULL;
        }
    break;

  case 453:
#line 3686 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            parsingCSignature = TRUE;
        }
    break;

  case 454:
#line 3688 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.optsignature) = sipMalloc(sizeof (signatureDef));

            *(yyval.optsignature) = (yyvsp[(5) - (7)].signature);
            (yyval.optsignature)->result = (yyvsp[(3) - (7)].memArg);

            parsingCSignature = FALSE;
        }
    break;

  case 455:
#line 3698 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.number) = FALSE;
        }
    break;

  case 456:
#line 3701 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.number) = TRUE;
        }
    break;

  case 457:
#line 3706 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping())
            {
                applyTypeFlags(currentModule, &(yyvsp[(1) - (17)].memArg), &(yyvsp[(10) - (17)].optflags));

                (yyvsp[(4) - (17)].signature).result = (yyvsp[(1) - (17)].memArg);

                newFunction(currentSpec, currentModule, currentScope(), NULL,
                        NULL, sectionFlags, currentIsStatic, currentIsSignal,
                        currentIsSlot, currentOverIsVirt, (yyvsp[(2) - (17)].text), &(yyvsp[(4) - (17)].signature), (yyvsp[(6) - (17)].number), (yyvsp[(9) - (17)].number),
                        &(yyvsp[(10) - (17)].optflags), (yyvsp[(15) - (17)].codeb), (yyvsp[(16) - (17)].codeb), (yyvsp[(17) - (17)].codeb), (yyvsp[(8) - (17)].throwlist), (yyvsp[(11) - (17)].optsignature), (yyvsp[(13) - (17)].docstr), (yyvsp[(7) - (17)].number), (yyvsp[(14) - (17)].codeb));
            }

            currentIsStatic = FALSE;
            currentIsSignal = FALSE;
            currentIsSlot = FALSE;
            currentOverIsVirt = FALSE;
        }
    break;

  case 458:
#line 3724 "sip-4.19.12/sipgen/metasrc/parser.y"
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

  case 459:
#line 3744 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping())
            {
                classDef *cd = currentScope();
                ifaceFileDef *ns_scope;

                /*
                 * If the scope is a namespace then make sure the operator is
                 * handled as a global, but remember it's C++ scope..
                 */
                if (cd != NULL && cd->iff->type == namespace_iface)
                {
                    ns_scope = cd->iff;
                    cd = NULL;
                }
                else
                {
                    ns_scope = NULL;
                }

                applyTypeFlags(currentModule, &(yyvsp[(1) - (17)].memArg), &(yyvsp[(11) - (17)].optflags));

                /* Handle the unary '+' and '-' operators. */
                if ((cd != NULL && (yyvsp[(5) - (17)].signature).nrArgs == 0) || (cd == NULL && (yyvsp[(5) - (17)].signature).nrArgs == 1))
                {
                    if (strcmp((yyvsp[(3) - (17)].text), "__add__") == 0)
                        (yyvsp[(3) - (17)].text) = "__pos__";
                    else if (strcmp((yyvsp[(3) - (17)].text), "__sub__") == 0)
                        (yyvsp[(3) - (17)].text) = "__neg__";
                }

                (yyvsp[(5) - (17)].signature).result = (yyvsp[(1) - (17)].memArg);

                newFunction(currentSpec, currentModule, cd, ns_scope, NULL,
                        sectionFlags, currentIsStatic, currentIsSignal,
                        currentIsSlot, currentOverIsVirt, (yyvsp[(3) - (17)].text), &(yyvsp[(5) - (17)].signature), (yyvsp[(7) - (17)].number), (yyvsp[(10) - (17)].number),
                        &(yyvsp[(11) - (17)].optflags), (yyvsp[(15) - (17)].codeb), (yyvsp[(16) - (17)].codeb), (yyvsp[(17) - (17)].codeb), (yyvsp[(9) - (17)].throwlist), (yyvsp[(12) - (17)].optsignature), NULL, (yyvsp[(8) - (17)].number), (yyvsp[(14) - (17)].codeb));
            }

            currentIsStatic = FALSE;
            currentIsSignal = FALSE;
            currentIsSlot = FALSE;
            currentOverIsVirt = FALSE;
        }
    break;

  case 460:
#line 3788 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping())
            {
                char *sname;
                classDef *scope = currentScope();

                if (scope == NULL || (yyvsp[(4) - (16)].signature).nrArgs != 0)
                    yyerror("Operator casts must be specified in a class and have no arguments");

                applyTypeFlags(currentModule, &(yyvsp[(2) - (16)].memArg), &(yyvsp[(10) - (16)].optflags));

                switch ((yyvsp[(2) - (16)].memArg).atype)
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
                    (yyvsp[(4) - (16)].signature).result = (yyvsp[(2) - (16)].memArg);

                    newFunction(currentSpec, currentModule, scope, NULL, NULL,
                            sectionFlags, currentIsStatic, currentIsSignal,
                            currentIsSlot, currentOverIsVirt, sname, &(yyvsp[(4) - (16)].signature), (yyvsp[(6) - (16)].number),
                            (yyvsp[(9) - (16)].number), &(yyvsp[(10) - (16)].optflags), (yyvsp[(14) - (16)].codeb), (yyvsp[(15) - (16)].codeb), (yyvsp[(16) - (16)].codeb), (yyvsp[(8) - (16)].throwlist), (yyvsp[(11) - (16)].optsignature), NULL, (yyvsp[(7) - (16)].number), (yyvsp[(13) - (16)].codeb));
                }
                else
                {
                    argList *al;

                    /* Check it doesn't already exist. */
                    for (al = scope->casts; al != NULL; al = al->next)
                        if (compareScopedNames((yyvsp[(2) - (16)].memArg).u.snd, al->arg.u.snd) == 0)
                            yyerror("This operator cast has already been specified in this class");

                    al = sipMalloc(sizeof (argList));
                    al->arg = (yyvsp[(2) - (16)].memArg);
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

  case 461:
#line 3869 "sip-4.19.12/sipgen/metasrc/parser.y"
    {(yyval.text) = "__add__";}
    break;

  case 462:
#line 3870 "sip-4.19.12/sipgen/metasrc/parser.y"
    {(yyval.text) = "__sub__";}
    break;

  case 463:
#line 3871 "sip-4.19.12/sipgen/metasrc/parser.y"
    {(yyval.text) = "__mul__";}
    break;

  case 464:
#line 3872 "sip-4.19.12/sipgen/metasrc/parser.y"
    {(yyval.text) = "__div__";}
    break;

  case 465:
#line 3873 "sip-4.19.12/sipgen/metasrc/parser.y"
    {(yyval.text) = "__mod__";}
    break;

  case 466:
#line 3874 "sip-4.19.12/sipgen/metasrc/parser.y"
    {(yyval.text) = "__and__";}
    break;

  case 467:
#line 3875 "sip-4.19.12/sipgen/metasrc/parser.y"
    {(yyval.text) = "__or__";}
    break;

  case 468:
#line 3876 "sip-4.19.12/sipgen/metasrc/parser.y"
    {(yyval.text) = "__xor__";}
    break;

  case 469:
#line 3877 "sip-4.19.12/sipgen/metasrc/parser.y"
    {(yyval.text) = "__lshift__";}
    break;

  case 470:
#line 3878 "sip-4.19.12/sipgen/metasrc/parser.y"
    {(yyval.text) = "__rshift__";}
    break;

  case 471:
#line 3879 "sip-4.19.12/sipgen/metasrc/parser.y"
    {(yyval.text) = "__iadd__";}
    break;

  case 472:
#line 3880 "sip-4.19.12/sipgen/metasrc/parser.y"
    {(yyval.text) = "__isub__";}
    break;

  case 473:
#line 3881 "sip-4.19.12/sipgen/metasrc/parser.y"
    {(yyval.text) = "__imul__";}
    break;

  case 474:
#line 3882 "sip-4.19.12/sipgen/metasrc/parser.y"
    {(yyval.text) = "__idiv__";}
    break;

  case 475:
#line 3883 "sip-4.19.12/sipgen/metasrc/parser.y"
    {(yyval.text) = "__imod__";}
    break;

  case 476:
#line 3884 "sip-4.19.12/sipgen/metasrc/parser.y"
    {(yyval.text) = "__iand__";}
    break;

  case 477:
#line 3885 "sip-4.19.12/sipgen/metasrc/parser.y"
    {(yyval.text) = "__ior__";}
    break;

  case 478:
#line 3886 "sip-4.19.12/sipgen/metasrc/parser.y"
    {(yyval.text) = "__ixor__";}
    break;

  case 479:
#line 3887 "sip-4.19.12/sipgen/metasrc/parser.y"
    {(yyval.text) = "__ilshift__";}
    break;

  case 480:
#line 3888 "sip-4.19.12/sipgen/metasrc/parser.y"
    {(yyval.text) = "__irshift__";}
    break;

  case 481:
#line 3889 "sip-4.19.12/sipgen/metasrc/parser.y"
    {(yyval.text) = "__invert__";}
    break;

  case 482:
#line 3890 "sip-4.19.12/sipgen/metasrc/parser.y"
    {(yyval.text) = "__call__";}
    break;

  case 483:
#line 3891 "sip-4.19.12/sipgen/metasrc/parser.y"
    {(yyval.text) = "__getitem__";}
    break;

  case 484:
#line 3892 "sip-4.19.12/sipgen/metasrc/parser.y"
    {(yyval.text) = "__lt__";}
    break;

  case 485:
#line 3893 "sip-4.19.12/sipgen/metasrc/parser.y"
    {(yyval.text) = "__le__";}
    break;

  case 486:
#line 3894 "sip-4.19.12/sipgen/metasrc/parser.y"
    {(yyval.text) = "__eq__";}
    break;

  case 487:
#line 3895 "sip-4.19.12/sipgen/metasrc/parser.y"
    {(yyval.text) = "__ne__";}
    break;

  case 488:
#line 3896 "sip-4.19.12/sipgen/metasrc/parser.y"
    {(yyval.text) = "__gt__";}
    break;

  case 489:
#line 3897 "sip-4.19.12/sipgen/metasrc/parser.y"
    {(yyval.text) = "__ge__";}
    break;

  case 490:
#line 3900 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.number) = FALSE;
        }
    break;

  case 491:
#line 3903 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.number) = TRUE;
        }
    break;

  case 492:
#line 3908 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.number) = FALSE;
        }
    break;

  case 493:
#line 3911 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.number) = TRUE;
        }
    break;

  case 494:
#line 3916 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.number) = 0;
        }
    break;

  case 495:
#line 3919 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if ((yyvsp[(2) - (2)].number) != 0)
                yyerror("Abstract virtual function '= 0' expected");

            (yyval.number) = TRUE;
        }
    break;

  case 496:
#line 3927 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.optflags).nrFlags = 0;
        }
    break;

  case 497:
#line 3930 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.optflags) = (yyvsp[(2) - (3)].optflags);
        }
    break;

  case 498:
#line 3936 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.optflags).flags[0] = (yyvsp[(1) - (1)].flag);
            (yyval.optflags).nrFlags = 1;
        }
    break;

  case 499:
#line 3940 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            /* Check there is room. */

            if ((yyvsp[(1) - (3)].optflags).nrFlags == MAX_NR_FLAGS)
                yyerror("Too many optional flags");

            (yyval.optflags) = (yyvsp[(1) - (3)].optflags);

            (yyval.optflags).flags[(yyval.optflags).nrFlags++] = (yyvsp[(3) - (3)].flag);
        }
    break;

  case 500:
#line 3952 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.flag).ftype = bool_flag;
            (yyval.flag).fname = (yyvsp[(1) - (1)].text);
        }
    break;

  case 501:
#line 3956 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.flag) = (yyvsp[(3) - (3)].flag);
            (yyval.flag).fname = (yyvsp[(1) - (3)].text);
        }
    break;

  case 502:
#line 3962 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.flag).ftype = (strchr((yyvsp[(1) - (1)].text), '.') != NULL) ? dotted_name_flag : name_flag;
            (yyval.flag).fvalue.sval = (yyvsp[(1) - (1)].text);
        }
    break;

  case 503:
#line 3966 "sip-4.19.12/sipgen/metasrc/parser.y"
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

  case 504:
#line 3989 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.flag).ftype = string_flag;
            (yyval.flag).fvalue.sval = convertFeaturedString((yyvsp[(1) - (1)].text));
        }
    break;

  case 505:
#line 3993 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.flag).ftype = integer_flag;
            (yyval.flag).fvalue.ival = (yyvsp[(1) - (1)].number);
        }
    break;

  case 506:
#line 3999 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.codeb) = NULL;
        }
    break;

  case 507:
#line 4002 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.codeb) = (yyvsp[(2) - (2)].codeb);
        }
    break;

  case 508:
#line 4007 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.codeb) = NULL;
        }
    break;

  case 509:
#line 4010 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.codeb) = (yyvsp[(2) - (2)].codeb);
        }
    break;

  case 510:
#line 4015 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.codeb) = NULL;
        }
    break;

  case 511:
#line 4018 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.codeb) = (yyvsp[(2) - (2)].codeb);
        }
    break;

  case 512:
#line 4023 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.codeb) = NULL;
        }
    break;

  case 513:
#line 4026 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.codeb) = (yyvsp[(2) - (2)].codeb);
        }
    break;

  case 514:
#line 4031 "sip-4.19.12/sipgen/metasrc/parser.y"
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

                /* Suppress a compiler warning. */
                default:
                    ;
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

  case 515:
#line 4083 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            /* No arguments. */

            (yyval.signature).nrArgs = 0;
        }
    break;

  case 516:
#line 4088 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            /* The single or first argument. */

            (yyval.signature).args[0] = (yyvsp[(1) - (1)].memArg);
            (yyval.signature).nrArgs = 1;
        }
    break;

  case 517:
#line 4094 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            /* Check that it wasn't ...(,arg...). */
            if ((yyvsp[(1) - (3)].signature).nrArgs == 0)
                yyerror("First argument of the list is missing");

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

  case 518:
#line 4117 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            deprecated("SIP_SIGNAL is deprecated\n");
            checkNoAnnos(&(yyvsp[(3) - (4)].optflags), "SIP_SIGNAL has no annotations");

            (yyval.memArg).atype = signal_type;
            (yyval.memArg).argflags = ARG_IS_CONST;
            (yyval.memArg).nrderefs = 0;
            (yyval.memArg).name = cacheName(currentSpec, (yyvsp[(2) - (4)].text));
            (yyval.memArg).defval = (yyvsp[(4) - (4)].valp);

            currentSpec -> sigslots = TRUE;
        }
    break;

  case 519:
#line 4129 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            deprecated("SIP_SLOT is deprecated\n");
            checkNoAnnos(&(yyvsp[(3) - (4)].optflags), "SIP_SLOT has no annotations");

            (yyval.memArg).atype = slot_type;
            (yyval.memArg).argflags = ARG_IS_CONST;
            (yyval.memArg).nrderefs = 0;
            (yyval.memArg).name = cacheName(currentSpec, (yyvsp[(2) - (4)].text));
            (yyval.memArg).defval = (yyvsp[(4) - (4)].valp);

            currentSpec -> sigslots = TRUE;
        }
    break;

  case 520:
#line 4141 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            deprecated("SIP_ANYSLOT is deprecated\n");
            checkNoAnnos(&(yyvsp[(3) - (4)].optflags), "SIP_ANYSLOT has no annotations");

            (yyval.memArg).atype = anyslot_type;
            (yyval.memArg).argflags = ARG_IS_CONST;
            (yyval.memArg).nrderefs = 0;
            (yyval.memArg).name = cacheName(currentSpec, (yyvsp[(2) - (4)].text));
            (yyval.memArg).defval = (yyvsp[(4) - (4)].valp);

            currentSpec -> sigslots = TRUE;
        }
    break;

  case 521:
#line 4153 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            const char *annos[] = {
                "SingleShot",
                NULL
            };

            deprecated("SIP_RXOBJ_CON is deprecated\n");
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

  case 522:
#line 4172 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            deprecated("SIP_RXOBJ_DIS is deprecated\n");
            checkNoAnnos(&(yyvsp[(3) - (3)].optflags), "SIP_RXOBJ_DIS has no annotations");

            (yyval.memArg).atype = rxdis_type;
            (yyval.memArg).argflags = 0;
            (yyval.memArg).nrderefs = 0;
            (yyval.memArg).name = cacheName(currentSpec, (yyvsp[(2) - (3)].text));

            currentSpec -> sigslots = TRUE;
        }
    break;

  case 523:
#line 4183 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            deprecated("SIP_SLOT_CON is deprecated\n");
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

  case 524:
#line 4200 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            deprecated("SIP_SLOT_DIS is deprecated\n");
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

  case 525:
#line 4217 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            deprecated("SIP_QOBJECT is deprecated\n");
            checkNoAnnos(&(yyvsp[(3) - (3)].optflags), "SIP_QOBJECT has no annotations");

            (yyval.memArg).atype = qobject_type;
            (yyval.memArg).argflags = 0;
            (yyval.memArg).nrderefs = 0;
            (yyval.memArg).name = cacheName(currentSpec, (yyvsp[(2) - (3)].text));
        }
    break;

  case 526:
#line 4226 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.memArg) = (yyvsp[(1) - (2)].memArg);
            (yyval.memArg).defval = (yyvsp[(2) - (2)].valp);
        }
    break;

  case 527:
#line 4233 "sip-4.19.12/sipgen/metasrc/parser.y"
    {currentIsSignal = TRUE;}
    break;

  case 529:
#line 4234 "sip-4.19.12/sipgen/metasrc/parser.y"
    {currentIsSlot = TRUE;}
    break;

  case 532:
#line 4239 "sip-4.19.12/sipgen/metasrc/parser.y"
    {currentIsStatic = TRUE;}
    break;

  case 537:
#line 4249 "sip-4.19.12/sipgen/metasrc/parser.y"
    {currentOverIsVirt = TRUE;}
    break;

  case 540:
#line 4253 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping())
            {
                const char *annos[] = {
                    "DocType",
                    "Encoding",
                    "NoSetter",
                    "NoTypeHint",
                    "PyInt",
                    "PyName",
                    "TypeHint",
                    NULL
                };

                checkAnnos(&(yyvsp[(3) - (8)].optflags), annos);

                if ((yyvsp[(6) - (8)].codeb) != NULL)
                {
                    if ((yyvsp[(4) - (8)].variable).access_code != NULL)
                        yyerror("%AccessCode already defined");

                    (yyvsp[(4) - (8)].variable).access_code = (yyvsp[(6) - (8)].codeb);

                    deprecated("%AccessCode should be used as a sub-directive");
                }

                if ((yyvsp[(7) - (8)].codeb) != NULL)
                {
                    if ((yyvsp[(4) - (8)].variable).get_code != NULL)
                        yyerror("%GetCode already defined");

                    (yyvsp[(4) - (8)].variable).get_code = (yyvsp[(7) - (8)].codeb);

                    deprecated("%GetCode should be used as a sub-directive");
                }

                if ((yyvsp[(8) - (8)].codeb) != NULL)
                {
                    if ((yyvsp[(4) - (8)].variable).set_code != NULL)
                        yyerror("%SetCode already defined");

                    (yyvsp[(4) - (8)].variable).set_code = (yyvsp[(8) - (8)].codeb);

                    deprecated("%SetCode should be used as a sub-directive");
                }

                newVar(currentSpec, currentModule, (yyvsp[(2) - (8)].text), currentIsStatic, &(yyvsp[(1) - (8)].memArg),
                        &(yyvsp[(3) - (8)].optflags), (yyvsp[(4) - (8)].variable).access_code, (yyvsp[(4) - (8)].variable).get_code, (yyvsp[(4) - (8)].variable).set_code,
                        sectionFlags);
            }

            currentIsStatic = FALSE;
        }
    break;

  case 541:
#line 4308 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.variable).token = 0;
            (yyval.variable).access_code = NULL;
            (yyval.variable).get_code = NULL;
            (yyval.variable).set_code = NULL;
        }
    break;

  case 542:
#line 4314 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.variable) = (yyvsp[(2) - (3)].variable);
        }
    break;

  case 544:
#line 4320 "sip-4.19.12/sipgen/metasrc/parser.y"
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

  case 545:
#line 4332 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.variable).token = TK_IF;
        }
    break;

  case 546:
#line 4335 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.variable).token = TK_END;
        }
    break;

  case 547:
#line 4338 "sip-4.19.12/sipgen/metasrc/parser.y"
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
            }

            (yyval.variable).get_code = NULL;
            (yyval.variable).set_code = NULL;
        }
    break;

  case 548:
#line 4353 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping())
            {
                (yyval.variable).token = TK_GETCODE;
                (yyval.variable).get_code = (yyvsp[(2) - (2)].codeb);
            }
            else
            {
                (yyval.variable).token = 0;
                (yyval.variable).get_code = NULL;
            }

            (yyval.variable).access_code = NULL;
            (yyval.variable).set_code = NULL;
        }
    break;

  case 549:
#line 4368 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (notSkipping())
            {
                (yyval.variable).token = TK_SETCODE;
                (yyval.variable).set_code = (yyvsp[(2) - (2)].codeb);
            }
            else
            {
                (yyval.variable).token = 0;
                (yyval.variable).set_code = NULL;
            }

            (yyval.variable).access_code = NULL;
            (yyval.variable).get_code = NULL;
        }
    break;

  case 550:
#line 4385 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.memArg) = (yyvsp[(2) - (4)].memArg);
            add_derefs(&(yyval.memArg), &(yyvsp[(3) - (4)].memArg));
            (yyval.memArg).argflags |= ARG_IS_CONST | (yyvsp[(4) - (4)].number);
        }
    break;

  case 551:
#line 4390 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.memArg) = (yyvsp[(1) - (3)].memArg);
            add_derefs(&(yyval.memArg), &(yyvsp[(2) - (3)].memArg));
            (yyval.memArg).argflags |= (yyvsp[(3) - (3)].number);

            /* PyObject * is a synonym for SIP_PYOBJECT. */
            if ((yyvsp[(1) - (3)].memArg).atype == defined_type && strcmp((yyvsp[(1) - (3)].memArg).u.snd->name, "PyObject") == 0 && (yyvsp[(1) - (3)].memArg).u.snd->next == NULL && (yyvsp[(2) - (3)].memArg).nrderefs == 1 && (yyvsp[(3) - (3)].number) == 0)
            {
                (yyval.memArg).atype = pyobject_type;
                (yyval.memArg).nrderefs = 0;
            }
        }
    break;

  case 552:
#line 4404 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            const char *annos[] = {
                "AllowNone",
                "Array",
                "ArraySize",
                "Constrained",
                "DisallowNone",
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
                "ScopesStripped",
                "Transfer",
                "TransferBack",
                "TransferThis",
                "TypeHint",
                "TypeHintIn",
                "TypeHintOut",
                "TypeHintValue",
                NULL
            };

            optFlag *of;

            checkAnnos(&(yyvsp[(3) - (3)].optflags), annos);

            (yyval.memArg) = (yyvsp[(1) - (3)].memArg);
            (yyval.memArg).name = cacheName(currentSpec, (yyvsp[(2) - (3)].text));

            handleKeepReference(&(yyvsp[(3) - (3)].optflags), &(yyval.memArg), currentModule);

            if ((of = getOptFlag(&(yyvsp[(3) - (3)].optflags), "ScopesStripped", opt_integer_flag)) != NULL)
                if (((yyval.memArg).scopes_stripped = of->fvalue.ival) <= 0)
                    yyerror("/ScopesStripped/ must be greater than 0");

            if (getAllowNone(&(yyvsp[(3) - (3)].optflags)))
                (yyval.memArg).argflags |= ARG_ALLOW_NONE;

            if (getDisallowNone(&(yyvsp[(3) - (3)].optflags)))
                (yyval.memArg).argflags |= ARG_DISALLOW_NONE;

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

                /* Suppress a compiler warning. */
                default:
                    ;
                }
            }

            applyTypeFlags(currentModule, &(yyval.memArg), &(yyvsp[(3) - (3)].optflags));
            (yyval.memArg).typehint_value = getTypeHintValue(&(yyvsp[(3) - (3)].optflags));
        }
    break;

  case 553:
#line 4514 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.number) = 0;
        }
    break;

  case 554:
#line 4517 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (currentSpec -> genc)
                yyerror("References not allowed in a C module");

            (yyval.number) = ARG_IS_REF;
        }
    break;

  case 555:
#line 4525 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.memArg).nrderefs = 0;
        }
    break;

  case 556:
#line 4528 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            add_new_deref(&(yyval.memArg), &(yyvsp[(1) - (3)].memArg), TRUE);
        }
    break;

  case 557:
#line 4531 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            add_new_deref(&(yyval.memArg), &(yyvsp[(1) - (2)].memArg), FALSE);
        }
    break;

  case 558:
#line 4536 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            memset(&(yyval.memArg), 0, sizeof (argDef));
            (yyval.memArg).atype = defined_type;
            (yyval.memArg).u.snd = (yyvsp[(1) - (1)].scpvalp);

            /* Try and resolve typedefs as early as possible. */
            resolveAnyTypedef(currentSpec, &(yyval.memArg));
        }
    break;

  case 559:
#line 4544 "sip-4.19.12/sipgen/metasrc/parser.y"
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

  case 560:
#line 4555 "sip-4.19.12/sipgen/metasrc/parser.y"
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

  case 561:
#line 4570 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            memset(&(yyval.memArg), 0, sizeof (argDef));
            (yyval.memArg).atype = ushort_type;
        }
    break;

  case 562:
#line 4574 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            memset(&(yyval.memArg), 0, sizeof (argDef));
            (yyval.memArg).atype = short_type;
        }
    break;

  case 563:
#line 4578 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            memset(&(yyval.memArg), 0, sizeof (argDef));
            (yyval.memArg).atype = uint_type;
        }
    break;

  case 564:
#line 4582 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            memset(&(yyval.memArg), 0, sizeof (argDef));
            (yyval.memArg).atype = uint_type;
        }
    break;

  case 565:
#line 4586 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            memset(&(yyval.memArg), 0, sizeof (argDef));
            (yyval.memArg).atype = int_type;
        }
    break;

  case 566:
#line 4590 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            memset(&(yyval.memArg), 0, sizeof (argDef));
            (yyval.memArg).atype = long_type;
        }
    break;

  case 567:
#line 4594 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            memset(&(yyval.memArg), 0, sizeof (argDef));
            (yyval.memArg).atype = ulong_type;
        }
    break;

  case 568:
#line 4598 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            memset(&(yyval.memArg), 0, sizeof (argDef));
            (yyval.memArg).atype = longlong_type;
        }
    break;

  case 569:
#line 4602 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            memset(&(yyval.memArg), 0, sizeof (argDef));
            (yyval.memArg).atype = ulonglong_type;
        }
    break;

  case 570:
#line 4606 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            memset(&(yyval.memArg), 0, sizeof (argDef));
            (yyval.memArg).atype = float_type;
        }
    break;

  case 571:
#line 4610 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            memset(&(yyval.memArg), 0, sizeof (argDef));
            (yyval.memArg).atype = double_type;
        }
    break;

  case 572:
#line 4614 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            memset(&(yyval.memArg), 0, sizeof (argDef));
            (yyval.memArg).atype = bool_type;
        }
    break;

  case 573:
#line 4618 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            memset(&(yyval.memArg), 0, sizeof (argDef));
            (yyval.memArg).atype = sstring_type;
        }
    break;

  case 574:
#line 4622 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            memset(&(yyval.memArg), 0, sizeof (argDef));
            (yyval.memArg).atype = ustring_type;
        }
    break;

  case 575:
#line 4626 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            memset(&(yyval.memArg), 0, sizeof (argDef));
            (yyval.memArg).atype = string_type;
        }
    break;

  case 576:
#line 4630 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            memset(&(yyval.memArg), 0, sizeof (argDef));
            (yyval.memArg).atype = wstring_type;
        }
    break;

  case 577:
#line 4634 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            memset(&(yyval.memArg), 0, sizeof (argDef));
            (yyval.memArg).atype = void_type;
        }
    break;

  case 578:
#line 4638 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            memset(&(yyval.memArg), 0, sizeof (argDef));
            (yyval.memArg).atype = pyobject_type;
        }
    break;

  case 579:
#line 4642 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            memset(&(yyval.memArg), 0, sizeof (argDef));
            (yyval.memArg).atype = pytuple_type;
        }
    break;

  case 580:
#line 4646 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            memset(&(yyval.memArg), 0, sizeof (argDef));
            (yyval.memArg).atype = pylist_type;
        }
    break;

  case 581:
#line 4650 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            memset(&(yyval.memArg), 0, sizeof (argDef));
            (yyval.memArg).atype = pydict_type;
        }
    break;

  case 582:
#line 4654 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            memset(&(yyval.memArg), 0, sizeof (argDef));
            (yyval.memArg).atype = pycallable_type;
        }
    break;

  case 583:
#line 4658 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            memset(&(yyval.memArg), 0, sizeof (argDef));
            (yyval.memArg).atype = pyslice_type;
        }
    break;

  case 584:
#line 4662 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            memset(&(yyval.memArg), 0, sizeof (argDef));
            (yyval.memArg).atype = pytype_type;
        }
    break;

  case 585:
#line 4666 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            memset(&(yyval.memArg), 0, sizeof (argDef));
            (yyval.memArg).atype = pybuffer_type;
        }
    break;

  case 586:
#line 4670 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            memset(&(yyval.memArg), 0, sizeof (argDef));
            (yyval.memArg).atype = ssize_type;
        }
    break;

  case 587:
#line 4674 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            memset(&(yyval.memArg), 0, sizeof (argDef));
            (yyval.memArg).atype = ellipsis_type;
        }
    break;

  case 588:
#line 4680 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            /* The single or first type. */

            (yyval.signature).args[0] = (yyvsp[(1) - (1)].memArg);
            (yyval.signature).nrArgs = 1;
        }
    break;

  case 589:
#line 4686 "sip-4.19.12/sipgen/metasrc/parser.y"
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

  case 590:
#line 4702 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            (yyval.throwlist) = NULL;
        }
    break;

  case 591:
#line 4705 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            if (currentSpec->genc)
                yyerror("Exceptions not allowed in a C module");

            (yyval.throwlist) = (yyvsp[(3) - (4)].throwlist);
        }
    break;

  case 592:
#line 4713 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            /* Empty list so use a blank. */

            (yyval.throwlist) = sipMalloc(sizeof (throwArgs));
            (yyval.throwlist) -> nrArgs = 0;
        }
    break;

  case 593:
#line 4719 "sip-4.19.12/sipgen/metasrc/parser.y"
    {
            /* The only or first exception. */

            (yyval.throwlist) = sipMalloc(sizeof (throwArgs));
            (yyval.throwlist) -> nrArgs = 1;
            (yyval.throwlist) -> args[0] = findException(currentSpec, (yyvsp[(1) - (1)].scpvalp), FALSE);
        }
    break;

  case 594:
#line 4726 "sip-4.19.12/sipgen/metasrc/parser.y"
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


/* Line 1267 of yacc.c.  */
#line 8389 "sip-4.19.12/sipgen/parser.c"
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
      /* If just tried and failed to reuse look-ahead token after an
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

  /* Else will try to reuse look-ahead token after shifting the error
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

  if (yyn == YYFINAL)
    YYACCEPT;

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

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
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


#line 4742 "sip-4.19.12/sipgen/metasrc/parser.y"



/*
 * Parse the specification.
 */
void parse(sipSpec *spec, FILE *fp, char *filename, int strict,
        stringList *tsl, stringList *bsl, stringList *xfl, KwArgs kwArgs,
        int protHack)
{
    classTmplDef *tcd;

    /* Initialise the spec. */
 
    memset(spec, 0, sizeof (sipSpec));
    spec->genc = -1;

    currentSpec = spec;
    strictParse = strict;
    backstops = bsl;
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
    stackPtr = 0;
    currentPlatforms = NULL;
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
void parserEOF(const char *name, parserContext *pc)
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
static void newModule(FILE *fp, const char *filename)
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

    newmod->defdocstringfmt = raw;
    newmod->encoding = no_type;
    newmod->next_key = -1;

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
static void parseFile(FILE *fp, const char *name, moduleDef *prevmod,
        int optional)
{
    parserContext pc;

    pc.filename = name;
    pc.ifdepth = stackPtr;
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

    /*
     * Note that we assume that the type (ie. class vs. mapped type vs.
     * exception) will be the same across all platforms.
     */
    iff->platforms = currentPlatforms;

    iff->type = iftype;
    iff->ifacenr = -1;
    iff->fqcname = fqname;
    iff->module = NULL;
    iff->hdrcode = NULL;
    iff->used = NULL;
    iff->file_extension = NULL;
    iff->next = pt->ifacefiles;

    pt->ifacefiles = iff;

    return iff;
}


/*
 * Find a class definition in a parse tree.
 */
static classDef *findClass(sipSpec *pt, ifaceFileType iftype,
        apiVersionRangeDef *api_range, scopedNameDef *fqname, int tmpl_arg)
{
    return findClassWithInterface(pt,
            findIfaceFile(pt, currentModule, fqname, iftype, api_range, NULL),
            tmpl_arg);
}


/*
 * Find a class definition given an existing interface file.
 */
static classDef *findClassWithInterface(sipSpec *pt, ifaceFileDef *iff,
        int tmpl_arg)
{
    classDef *cd;

    for (cd = pt->classes; cd != NULL; cd = cd->next)
        if (cd->iff == iff)
        {
            if (isTemplateArg(cd) && !tmpl_arg)
                resetTemplateArg(cd);

            return cd;
        }

    /* Create a new one. */
    cd = sipMalloc(sizeof (classDef));

    if (tmpl_arg)
        setTemplateArg(cd);

    cd->iff = iff;
    cd->pyname = cacheName(pt, classBaseName(cd));
    cd->next = pt->classes;

    pt->classes = cd;

    return cd;
}


/*
 * Add an interface file to an interface file list if it isn't already there.
 */
void appendToIfaceFileList(ifaceFileList **ifflp, ifaceFileDef *iff)
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
     * If it is an exception interface file then we have never seen this name
     * before.  We require that exceptions are defined before being used, but
     * don't make the same requirement of classes (for reasons of backwards
     * compatibility).  Therefore the name must be reinterpreted as a (as yet
     * undefined) class.
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

        cd = findClassWithInterface(pt, iff, FALSE);
    }

    /* Create a new one. */
    xd = sipMalloc(sizeof (exceptionDef));

    xd->exceptionnr = -1;
    xd->needed = FALSE;
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
        apiVersionRangeDef *api_range, scopedNameDef *fqname,
        const char *virt_error_handler, typeHintDef *typehint_in,
        typeHintDef *typehint_out, const char *typehint_value)
{
    int flags;
    classDef *cd, *scope;
    codeBlockList *hdrcode;

    if (sectionFlags & SECT_IS_PRIVATE)
        yyerror("Classes, structs and namespaces must be in the public or protected sections");

    flags = 0;

    if ((scope = currentScope()) != NULL)
    {
        if (sectionFlags & SECT_IS_PROT && !makeProtPublic)
        {
            flags = CLASS_IS_PROTECTED;

            if (scope->iff->type == class_iface)
                setNeedsShadow(scope);
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

    cd = findClass(pt, iftype, api_range, fqname, FALSE);

    /* Check it hasn't already been defined. */
    if (iftype != namespace_iface && cd->iff->module != NULL)
        yyerror("The struct/class has already been defined");

    /* Complete the initialisation. */
    cd->classflags |= flags;
    cd->ecd = scope;
    cd->iff->module = currentModule;
    cd->virt_error_handler = virt_error_handler;
    cd->typehint_in = typehint_in;
    cd->typehint_out = typehint_out;
    cd->typehint_value = typehint_value;

    if (currentIsTemplate)
        setIsTemplateClass(cd);

    appendCodeBlockList(&cd->iff->hdrcode, hdrcode);

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

            if (inMainModule())
                ns->iff->first_alt->needed = TRUE;

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
    cd->no_typehint = getNoTypeHint(of);

    if ((flg = getOptFlag(of, "Metatype", dotted_name_flag)) != NULL)
        cd->metatype = cacheName(pt, flg->fvalue.sval);

    if ((flg = getOptFlag(of, "Supertype", dotted_name_flag)) != NULL)
        cd->supertype = cacheName(pt, flg->fvalue.sval);

    if (getOptFlag(of, "ExportDerived", bool_flag) != NULL)
        setExportDerived(cd);

    if (getOptFlag(of, "Mixin", bool_flag) != NULL)
        setMixin(cd);

    if ((flg = getOptFlag(of, "FileExtension", string_flag)) != NULL)
        cd->iff->file_extension = flg->fvalue.sval;

    if ((flg = getOptFlag(of, "PyQtFlags", integer_flag)) != NULL)
        cd->pyqt_flags = flg->fvalue.ival;

    if (getOptFlag(of, "PyQtNoQMetaObject", bool_flag) != NULL)
        setPyQtNoQMetaObject(cd);

    if ((flg = getOptFlag(of, "PyQtInterface", string_flag)) != NULL)
        cd->pyqt_interface = flg->fvalue.sval;

    if (isOpaque(cd))
    {
        if (getOptFlag(of, "External", bool_flag) != NULL)
            setIsExternal(cd);
    }
    else
    {
        int seq_might, seq_not, default_to_sequence;
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
         * then we use add/multiply.  Otherwise, if the user has used the
         * /Sequence/ annotation or there are indexing operators then we use
         * concat/repeat.
         */
        seq_might = seq_not = FALSE;

        for (md = cd->members; md != NULL; md = md->next)
            switch (md->slot)
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

            /* Suppress a compiler warning. */
            default:
                ;
            }

        default_to_sequence = (!seq_not && seq_might);

        for (md = cd->members; md != NULL; md = md->next)
        {
            /* Ignore if it is explicitly numeric. */
            if (isNumeric(md))
                continue;

            if (isSequence(md) || default_to_sequence)
                switch (md->slot)
                {
                case add_slot:
                    md->slot = concat_slot;
                    break;

                case iadd_slot:
                    md->slot = iconcat_slot;
                    break;

                case mul_slot:
                    md->slot = repeat_slot;
                    break;

                case imul_slot:
                    md->slot = irepeat_slot;
                    break;

                /* Suppress a compiler warning. */
                default:
                    ;
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
        snd = ad->u.snd = fullyQualifiedName(ad->u.snd);
        cname = scopedNameTail(snd);
        break;

    case template_type:
        ad->u.td->fqname = fullyQualifiedName(ad->u.td->fqname);
        snd = encodedTemplateName(ad->u.td);
        cname = NULL;
        break;

    case struct_type:
        snd = ad->u.sname = fullyQualifiedName(ad->u.sname);
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

    mappedTypeAnnos(mtd, of);

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

    /* Keep track of the original definition as it gets copied. */
    mtd->real = mtd;

    return mtd;
}


/*
 * Create a new enum.
 */
static enumDef *newEnum(sipSpec *pt, moduleDef *mod, mappedTypeDef *mt_scope,
        char *name, optFlags *of, int flags, int isscoped)
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
            setNeedsShadow(c_scope);
        }
    }

    ed->enumflags = flags;
    ed->no_typehint = getNoTypeHint(of);
    ed->enumnr = -1;
    ed->ecd = c_scope;
    ed->emtd = mt_scope;
    ed->first_alt = first_alt;
    ed->next_alt = next_alt;
    ed->module = mod;
    ed->members = NULL;
    ed->slots = NULL;
    ed->overs = NULL;
    ed->platforms = currentPlatforms;
    ed->next = pt -> enums;

    pt->enums = ed;

    if (getOptFlag(of, "NoScope", bool_flag) != NULL)
        setIsNoScope(ed);

    if (isscoped)
        setIsScopedEnum(ed);

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

            /* We do want const. */
            if (isConstArg(sad))
            {
                char *const_val = sipStrdup("const ");

                append(&const_val, val);
                free(val);

                val = const_val;
            }

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

        case void_type:
            s = "void";
            break;

        case capsule_type:
            s = "void *";
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
 * Remove any explicit global scope.
 */
scopedNameDef *removeGlobalScope(scopedNameDef *snd)
{
    return ((snd != NULL && snd->name[0] == '\0') ? snd->next : snd);
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

    /*
     * We don't want the global scope (which probably should always be there,
     * but we check anyway).
     */
    name = removeGlobalScope(name);

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
        templateDef *td, const char *pyname, int use_template_name,
        docstringDef *docstring)
{
    scopedNameDef *type_names, *type_values;
    classDef *cd;
    ctorDef *oct, **cttail;
    argDef *ad;
    ifaceFileList *iffl, **used;
    classList *cl;

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

    if (docstring != NULL)
        cd->docstring = docstring;

    resetIsTemplateClass(cd);
    cd->pyname = cacheName(pt, pyname);
    cd->td = td;

    if (use_template_name)
        setUseTemplateName(cd);

    /* Handle the interface file. */
    cd->iff = findIfaceFile(pt, mod, fqname, class_iface,
            (scope != NULL ? scope->iff->api_range : NULL), NULL);
    cd->iff->module = mod;

    appendCodeBlockList(&cd->iff->hdrcode, tcd->cd->iff->hdrcode);

    /* Make a copy of the used list and add the enclosing scope. */
    used = &cd->iff->used;

    for (iffl = tcd->cd->iff->used; iffl != NULL; iffl = iffl->next)
        appendToIfaceFileList(used, iffl->iff);

    /* Include any scope header code. */
    if (scope != NULL)
        appendCodeBlockList(&cd->iff->hdrcode, scope->iff->hdrcode);

    if (inMainModule())
    {
        setIsUsedName(cd->iff->name);
        setIsUsedName(cd->pyname);
    }

    cd->ecd = currentScope();

    /* Handle any type hints. */
    if (cd->typehint_in != NULL)
        cd->typehint_in = newTypeHint(
                templateString(cd->typehint_in->raw_hint, type_names,
                        type_values));

    if (cd->typehint_out != NULL)
        cd->typehint_out = newTypeHint(
                templateString(cd->typehint_out->raw_hint, type_names,
                        type_values));

    /* Handle the super-classes. */
    for (cl = cd->supers; cl != NULL; cl = cl->next)
    {
        int a;
        scopedNameDef *unscoped;

        unscoped = removeGlobalScope(cl->cd->iff->fqcname);

        /* Ignore defined or scoped classes. */
        if (cl->cd->iff->module != NULL || unscoped->next != NULL)
            continue;

        for (a = 0; a < tcd->sig.nrArgs - 1; ++a)
            if (strcmp(unscoped->name, scopedNameTail(tcd->sig.args[a].u.snd)) == 0)
            {
                argDef *tad = &td->types.args[a];
                classDef *icd;

                if (tad->atype == defined_type)
                    icd = findClass(pt, class_iface, NULL, tad->u.snd, FALSE);
                else if (tad->atype == class_type)
                    icd = tad->u.cd;
                else
                    fatal("Template argument %s must expand to a class\n",
                            unscoped->name);

                cl->cd = icd;
            }
    }

    /* Handle the enums. */
    instantiateTemplateEnums(pt, tcd, td, cd, used, type_names, type_values);

    /* Handle the variables. */
    instantiateTemplateVars(pt, tcd, td, cd, used, type_names, type_values);

    /* Handle the typedefs. */
    instantiateTemplateTypedefs(pt, tcd, td, cd, type_names, type_values);

    /* Handle the ctors. */
    cd->ctors = NULL;
    cttail = &cd->ctors;

    for (oct = tcd->cd->ctors; oct != NULL; oct = oct->next)
    {
        ctorDef *nct = sipMalloc(sizeof (ctorDef));

        /* Start with a shallow copy. */
        *nct = *oct;

        templateSignature(&nct->pysig, FALSE, tcd, td, cd, type_names,
                type_values);

        if (oct->cppsig == NULL)
            nct->cppsig = NULL;
        else if (oct->cppsig == &oct->pysig)
            nct->cppsig = &nct->pysig;
        else
        {
            nct->cppsig = sipMalloc(sizeof (signatureDef));

            *nct->cppsig = *oct->cppsig;

            templateSignature(nct->cppsig, FALSE, tcd, td, cd, type_names,
                    type_values);
        }

        nct->methodcode = templateCode(pt, used, nct->methodcode, type_names, type_values);
        nct->premethodcode = templateCode(pt, used, nct->premethodcode, type_names, type_values);

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
    cd->instancecode = templateCode(pt, used, cd->instancecode, type_names, type_values);
    cd->picklecode = templateCode(pt, used, cd->picklecode, type_names, type_values);
    cd->finalcode = templateCode(pt, used, cd->finalcode, type_names, type_values);
    cd->typehintcode = templateCode(pt, used, cd->typehintcode, type_names, type_values);
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

        templateSignature(&nod->pysig, TRUE, tcd, td, cd, type_names,
                type_values);

        if (od->cppsig == &od->pysig)
            nod->cppsig = &nod->pysig;
        else
        {
            nod->cppsig = sipMalloc(sizeof (signatureDef));

            *nod->cppsig = *od->cppsig;

            templateSignature(nod->cppsig, TRUE, tcd, td, cd, type_names,
                    type_values);
        }

        nod->methodcode = templateCode(pt, used, nod->methodcode, type_names, type_values);
        nod->premethodcode = templateCode(pt, used, nod->premethodcode, type_names, type_values);
        nod->virtcallcode = templateCode(pt, used, nod->virtcallcode, type_names, type_values);
        nod->virtcode = templateCode(pt, used, nod->virtcode, type_names, type_values);

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

            templateType(&vd->type, tcd, td, cd, type_names, type_values);

            vd->accessfunc = templateCode(pt, used, vd->accessfunc, type_names, type_values);
            vd->getcode = templateCode(pt, used, vd->getcode, type_names, type_values);
            vd->setcode = templateCode(pt, used, vd->setcode, type_names, type_values);

            addVariable(pt, vd);
        }
}


/*
 * Instantiate the typedefs of a template class.
 */
static void instantiateTemplateTypedefs(sipSpec *pt, classTmplDef *tcd,
        templateDef *td, classDef *cd, scopedNameDef *type_names,
        scopedNameDef *type_values)
{
    typedefDef *tdd;

    for (tdd = pt->typedefs; tdd != NULL; tdd = tdd->next)
    {
        typedefDef *new_tdd;

        if (tdd->ecd != tcd->cd)
            continue;

        new_tdd = sipMalloc(sizeof (typedefDef));

        /* Start with a shallow copy. */
        *new_tdd = *tdd;

        new_tdd->fqname = text2scopedName(cd->iff,
                scopedNameTail(new_tdd->fqname));
        new_tdd->ecd = cd;
        new_tdd->module = cd->iff->module;

        templateType(&new_tdd->type, tcd, td, cd, type_names, type_values);

        addTypedef(pt, new_tdd);
    }
}


/*
 * Replace any template arguments in a signature.
 */
static void templateSignature(signatureDef *sd, int result, classTmplDef *tcd,
        templateDef *td, classDef *ncd, scopedNameDef *type_names,
        scopedNameDef *type_values)
{
    int a;

    if (result)
        templateType(&sd->result, tcd, td, ncd, type_names, type_values);

    for (a = 0; a < sd->nrArgs; ++a)
        templateType(&sd->args[a], tcd, td, ncd, type_names, type_values);
}


/*
 * Replace any template arguments in a type.
 */
static void templateType(argDef *ad, classTmplDef *tcd, templateDef *td,
        classDef *ncd, scopedNameDef *type_names, scopedNameDef *type_values)
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

        templateSignature(&ad->u.td->types, FALSE, tcd, td, ncd, type_names,
                type_values);

        return;
    }

    /* Handle any default value. */
    if (ad->defval != NULL && ad->defval->vtype == fcall_value)
    {
        /*
         * We only handle the subset where the value is an function call, ie. a
         * template ctor.
         */
        valueDef *vd = ad->defval;

        if (vd->vtype == fcall_value && vd->u.fcd->type.atype == defined_type)
        {
            valueDef *new_vd;
            fcallDef *fcd;
            scopedNameDef *snd, **tailp;

            fcd = sipMalloc(sizeof (fcallDef));
            *fcd = *vd->u.fcd;

            tailp = &fcd->type.u.snd;
            for (snd = vd->u.fcd->type.u.snd; snd != NULL; snd = snd->next)
            {
                *tailp = text2scopePart(
                        templateString(snd->name, type_names, type_values));
                tailp = &(*tailp)->next;
            }

            new_vd = sipMalloc(sizeof (valueDef));
            new_vd->vtype = fcall_value;
            new_vd->u.fcd = fcd;

            ad->defval = new_vd;
        }
    }

    /* Handle any type hints. */
    if (ad->typehint_in != NULL)
        ad->typehint_in = newTypeHint(
                templateString(ad->typehint_in->raw_hint, type_names,
                        type_values));

    if (ad->typehint_out != NULL)
        ad->typehint_out = newTypeHint(
                templateString(ad->typehint_out->raw_hint, type_names,
                        type_values));

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
codeBlockList *templateCode(sipSpec *pt, ifaceFileList **used,
        codeBlockList *ocbl, scopedNameDef *names, scopedNameDef *values)
{
    codeBlockList *ncbl = NULL;

    while (ocbl != NULL)
    {
        char *at = ocbl->block->frag;
        int start_of_line = TRUE;

        do
        {
            char *from = at, *first = NULL;
            codeBlock *cb;
            scopedNameDef *nam, *val, *nam_first, *val_first;

            /* Suppress a compiler warning. */
            val_first = NULL;

            /*
             * Don't do any substitution in lines that appear to be
             * preprocessor directives.  This prevents #include'd file names
             * being broken.
             */
            if (start_of_line)
            {
                /* Strip leading whitespace. */
                while (isspace(*from))
                    ++from;

                if (*from == '#')
                {
                    /* Skip to the end of the line. */
                    do
                        ++from;
                    while (*from != '\n' && *from != '\0');
                }
                else
                {
                    start_of_line = FALSE;
                }
            }

            /*
             * Go through the rest of this fragment looking for each of the
             * types and the name of the class itself.
             */
            nam = names;
            val = values;

            while (nam != NULL && val != NULL)
            {
                char *cp;

                if ((cp = strstr(from, nam->name)) != NULL)
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

            if (at == ocbl->block->frag)
            {
                cb->filename = ocbl->block->filename;
                cb->linenr = ocbl->block->linenr;
            }
            else
                cb->filename = NULL;

            appendCodeBlock(&ncbl, cb);

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
                 * SIP generated object then translate any "::" scoping to "_"
                 * and remove any const.
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

                    if (strlen(sp) > 6 && strncmp(sp, "const ", 6) == 0)
                        sp += 6;

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

                if (*at == '\n')
                    start_of_line = TRUE;
            }
        }
        while (at != NULL && *at != '\0');

        ocbl = ocbl->next;
    }

    return ncbl;
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
            appendToIfaceFileList(used, iff);
            return;
        }
    }

    for (ed = pt->enums; ed != NULL; ed = ed->next)
    {
        if (ed->ecd == NULL)
            continue;

        if (sameName(ed->fqcname, sname))
        {
            appendToIfaceFileList(used, ed->ecd->iff);
            return;
        }
    }
}


/*
 * Compare a scoped name with its string equivalent.
 */
static int sameName(scopedNameDef *snd, const char *sname)
{
    /* Handle any explicit scopes. */
    if (sname[0] == ':' && sname[1] == ':')
    {
        if (snd->name[0] != '\0')
            return FALSE;

        sname += 2;
    }

    snd = removeGlobalScope(snd);

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
        optFlags *optflgs, docstringDef *docstring)
{
    int no_type_name;
    typedefDef *td;
    scopedNameDef *fqname;
    classDef *scope;

    scope = currentScope();
    fqname = text2scopedName((scope != NULL ? scope->iff : NULL), name);
    no_type_name = (getOptFlag(optflgs, "NoTypeName", bool_flag) != NULL);

    /* See if we are instantiating a template class. */
    if (type->atype == template_type)
    {
        classTmplDef *tcd;
        templateDef *td = type->u.td;

        for (tcd = pt->classtemplates; tcd != NULL; tcd = tcd->next)
            if (foundInScope(tcd->cd->iff->fqcname, td->fqname) &&
                sameTemplateSignature(&tcd->sig, &td->types, FALSE))
            {
                instantiateClassTemplate(pt, mod, scope, fqname, tcd, td,
                        getPythonName(mod, optflgs, name), no_type_name,
                        docstring);

                /* All done. */
                return;
            }
    }

    td = sipMalloc(sizeof (typedefDef));

    td->tdflags = 0;
    td->fqname = fqname;
    td->ecd = scope;
    td->module = mod;
    td->platforms = currentPlatforms;
    td->type = *type;

    if (getOptFlag(optflgs, "Capsule", bool_flag) != NULL)
    {
        /* Make sure the type is void *. */
        if (type->atype != void_type || type->nrderefs != 1 || isConstArg(type) || isReference(type))
        {
            fatalScopedName(fqname);
            fatal(" must be a void* if /Capsule/ is specified\n");
        }

        td->type.atype = capsule_type;
        td->type.nrderefs = 0;
        td->type.u.cap = fqname;
    }

    if (no_type_name)
        setNoTypeName(td);

    addTypedef(pt, td);
}


/*
 * Add a typedef to the list so that the list remains sorted.
 */
static void addTypedef(sipSpec *pt, typedefDef *tdd)
{
    typedefDef **tdp;

    /*
     * Check it doesn't already exist (with a strict parse) and find the
     * position in the sorted list where it should be put.
     */
    for (tdp = &pt->typedefs; *tdp != NULL; tdp = &(*tdp)->next)
    {
        int res = compareScopedNames((*tdp)->fqname, tdd->fqname);

        if (res == 0 && strictParse)
        {
            fatalScopedName(tdd->fqname);
            fatal(" already defined\n");
        }

        if (res >= 0)
            break;
    }

    tdd->next = *tdp;
    *tdp = tdd;

    tdd->module->nrtypedefs++;
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

    /*
     * For the moment we don't support capsule variables because it needs the
     * API major version increasing.
     */
    if (type->atype == capsule_type)
        yyerror("Capsule variables not yet supported");

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
    var->no_typehint = getNoTypeHint(of);
    var->platforms = currentPlatforms;
    var->type = *type;
    appendCodeBlock(&var->accessfunc, acode);
    appendCodeBlock(&var->getcode, gcode);
    appendCodeBlock(&var->setcode, scode);

    if (isstatic || (escope != NULL && escope->iff->type == namespace_iface))
        setIsStaticVar(var);

    if (getOptFlag(of, "NoSetter", bool_flag) != NULL)
        setNoSetter(var);

    addVariable(pt, var);
}


/*
 * Create a new ctor.
 */
static void newCtor(moduleDef *mod, char *name, int sectFlags,
        signatureDef *args, optFlags *optflgs, codeBlock *methodcode,
        throwArgs *exceptions, signatureDef *cppsig, int explicit,
        docstringDef *docstring, codeBlock *premethodcode)
{
    ctorDef *ct, **ctp;
    classDef *cd = currentScope();

    /* Check the name of the constructor. */
    if (strcmp(classBaseName(cd), name) != 0)
        yyerror("Constructor doesn't have the same name as its class");

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

    ct->docstring = docstring;
    ct->ctorflags = sectFlags;
    ct->no_typehint = getNoTypeHint(optflgs);
    ct->api_range = getAPIRange(optflgs);
    ct->pysig = *args;
    ct->cppsig = (cppsig != NULL ? cppsig : &ct->pysig);
    ct->exceptions = exceptions;
    ct->platforms = currentPlatforms;
    appendCodeBlock(&ct->methodcode, methodcode);
    appendCodeBlock(&ct->premethodcode, premethodcode);

    if (!isPrivateCtor(ct))
        setCanCreate(cd);

    if (isProtectedCtor(ct))
        setNeedsShadow(cd);

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
        ct->kwargs = keywordArgs(mod, optflgs, &ct->pysig, FALSE);

    if (methodcode == NULL && getOptFlag(optflgs, "NoRaisesPyException", bool_flag) == NULL)
    {
        if (allRaisePyException(mod) || getOptFlag(optflgs, "RaisesPyException", bool_flag) != NULL)
            setRaisesPyExceptionCtor(ct);
    }

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
        ifaceFileDef *ns_scope, mappedTypeDef *mt_scope, int sflags,
        int isstatic, int issignal, int isslot, int isvirt, char *name,
        signatureDef *sig, int isconst, int isabstract, optFlags *optflgs,
        codeBlock *methodcode, codeBlock *vcode, codeBlock *virtcallcode,
        throwArgs *exceptions, signatureDef *cppsig, docstringDef *docstring,
        int isfinal, codeBlock *premethodcode)
{
    static const char *annos[] = {
        "__len__",
        "__imatmul__",
        "__matmul__",
        "AbortOnException",
        "AllowNone",
        "API",
        "AutoGen",
        "Deprecated",
        "DisallowNone",
        "DocType",
        "Encoding",
        "Factory",
        "HoldGIL",
        "KeywordArgs",
        "KeepReference",
        "NewThread",
        "NoArgParser",
        "NoCopy",
        "NoRaisesPyException",
        "NoTypeHint",
        "NoVirtualErrorHandler",
        "Numeric",
        "PostHook",
        "PreHook",
        "PyInt",
        "PyName",
        "PyQtSignalHack",
        "RaisesPyException",
        "ReleaseGIL",
        "Sequence",
        "VirtualErrorHandler",
        "Transfer",
        "TransferBack",
        "TransferThis",
        "TypeHint",
        NULL
    };

    const char *pyname, *virt_error_handler;
    int factory, xferback, no_arg_parser, no_virt_error_handler;
    overDef *od, **odp, **headp;
    optFlag *of;

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

        /* Handle C void prototypes. */
        if (sig->nrArgs == 1)
        {
            argDef *vad = &sig->args[0];

            if (vad->atype == void_type && vad->nrderefs == 0)
                sig->nrArgs = 0;
        }
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
            "__aenter__",
            "__aexit__",
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

    getSourceLocation(&od->sloc);

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

    od->no_typehint = getNoTypeHint(optflgs);

    if (isSignal(od))
        if ((of = getOptFlag(optflgs, "PyQtSignalHack", integer_flag)) != NULL)
            od->pyqt_signal_hack = of->fvalue.ival;

    if (factory)
        setIsFactory(od);

    if (xferback)
        setIsResultTransferredBack(od);

    if (getTransfer(optflgs))
        setIsResultTransferred(od);

    if (getOptFlag(optflgs, "TransferThis", bool_flag) != NULL)
        setIsThisTransferredMeth(od);

    if (methodcode == NULL && getOptFlag(optflgs, "NoRaisesPyException", bool_flag) == NULL)
    {
        if (allRaisePyException(mod) || getOptFlag(optflgs, "RaisesPyException", bool_flag) != NULL)
            setRaisesPyException(od);
    }

    if (isProtected(od))
        setNeedsShadow(c_scope);

    if ((isSlot(od) || isSignal(od)) && !isPrivate(od))
    {
        if (isSignal(od))
            setNeedsShadow(c_scope);

        pt->sigslots = TRUE;
    }

    if (isSignal(od) && (methodcode != NULL || vcode != NULL || virtcallcode != NULL))
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

    if (isfinal)
        setIsFinal(od);

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

    virt_error_handler = getVirtErrorHandler(optflgs);
    no_virt_error_handler = (getOptFlag(optflgs, "NoVirtualErrorHandler", bool_flag) != NULL);

    if (isvirt)
    {
        if (!isfinal)
        {
            setIsVirtual(od);
            setNeedsShadow(c_scope);
        }

        if (getOptFlag(optflgs, "AbortOnException", bool_flag) != NULL)
            setAbortOnException(od);

        if (no_virt_error_handler)
        {
            if (virt_error_handler != NULL)
                yyerror("/VirtualErrorHandler/ and /NoVirtualErrorHandler/ provided");

            setNoErrorHandler(od);
        }
        else
        {
            od->virt_error_handler = virt_error_handler;
        }
    }
    else
    {
        if (vcode != NULL)
            yyerror("%VirtualCatcherCode provided for non-virtual function");

        if (virt_error_handler != NULL)
            yyerror("/VirtualErrorHandler/ provided for non-virtual function");

        if (no_virt_error_handler)
            yyerror("/NoVirtualErrorHandler/ provided for non-virtual function");
    }

    od->cppname = name;
    od->pysig = *sig;
    od->cppsig = (cppsig != NULL ? cppsig : &od->pysig);
    od->exceptions = exceptions;
    od->platforms = currentPlatforms;
    appendCodeBlock(&od->methodcode, methodcode);
    appendCodeBlock(&od->premethodcode, premethodcode);
    appendCodeBlock(&od->virtcallcode, virtcallcode);
    appendCodeBlock(&od->virtcode, vcode);

    no_arg_parser = (getOptFlag(optflgs, "NoArgParser", bool_flag) != NULL);

    if (no_arg_parser)
    {
        if (methodcode == NULL)
            yyerror("%MethodCode must be supplied if /NoArgParser/ is specified");
    }
    else
    {
        /*
         * The argument parser requires that there is nothing after an
         * ellipsis.
         */
        checkEllipsis(sig);
    }

    if (cppsig != NULL)
        checkEllipsis(cppsig);

    if (getOptFlag(optflgs, "NoCopy", bool_flag) != NULL)
        setNoCopy(&od->pysig.result);

    if (getAllowNone(optflgs))
        setAllowNone(&od->pysig.result);

    if (getDisallowNone(optflgs))
        setDisallowNone(&od->pysig.result);

    handleKeepReference(optflgs, &od->pysig.result, mod);

    pyname = getPythonName(mod, optflgs, name);

    od->common = findFunction(pt, mod, c_scope, ns_scope, mt_scope, pyname,
            (methodcode != NULL), sig->nrArgs, no_arg_parser);

    if (isProtected(od))
        setHasProtected(od->common);

    if (strcmp(pyname, "__delattr__") == 0)
        setIsDelattr(od);

    od->docstring = docstring;

    od->api_range = getAPIRange(optflgs);

    if (od->api_range == NULL)
        setNotVersioned(od->common);

    if (getOptFlag(optflgs, "Numeric", bool_flag) != NULL)
    {
        if (isSequence(od->common))
            yyerror("/Sequence/ has already been specified");

        setIsNumeric(od->common);
    }

    if (getOptFlag(optflgs, "Sequence", bool_flag) != NULL)
    {
        if (isNumeric(od->common))
            yyerror("/Numeric/ has already been specified");

        setIsSequence(od->common);
    }

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
        od->kwargs = keywordArgs(mod, optflgs, &od->pysig, hasProtected(od->common));

        if (od->kwargs != NoKwArgs)
            setUseKeywordArgs(od->common);

        /*
         * If the overload is protected and defined in an imported module then
         * we need to make sure that any other overloads' keyword argument
         * names are marked as used.
         */
        if (isProtected(od) && !inMainModule())
        {
            overDef *kwod;

            for (kwod = c_scope->overs; kwod != NULL; kwod = kwod->next)
                if (kwod->common == od->common && kwod->kwargs != NoKwArgs)
                {
                    int a;

                    for (a = 0; a < kwod->pysig.nrArgs; ++a)
                    {
                        argDef *ad = &kwod->pysig.args[a];

                        if (kwod->kwargs == OptionalKwArgs && ad->defval == NULL)
                            continue;

                        if (ad->name != NULL)
                            setIsUsedName(ad->name);
                    }
                }
        }
    }

    od->next = NULL;

    /* See if we want to auto-generate some methods. */
    if (getOptFlag(optflgs, "__len__", bool_flag) != NULL)
    {
        overDef *len;

        len = sipMalloc(sizeof (overDef));

        len->cppname = "__len__";
        len->overflags = SECT_IS_PUBLIC;
        len->pysig.result.atype = ssize_type;
        len->pysig.nrArgs = 0;
        len->cppsig = &len->pysig;

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

            appendCodeBlock(&len->methodcode, code);
        }

        len->common = findFunction(pt, mod, c_scope, ns_scope, mt_scope,
                len->cppname, TRUE, 0, FALSE);

        len->platforms = od->platforms;
        len->next = od->next;
        od->next = len;
    }

    if (getOptFlag(optflgs, "__matmul__", bool_flag) != NULL)
    {
        overDef *matmul;

        matmul = sipMalloc(sizeof (overDef));

        matmul->cppname = "__matmul__";
        matmul->overflags = SECT_IS_PUBLIC;
        matmul->pysig = od->pysig;
        matmul->cppsig = (cppsig != NULL ? cppsig : &matmul->pysig);

        matmul->methodcode = od->methodcode;

        matmul->common = findFunction(pt, mod, c_scope, ns_scope, mt_scope,
                matmul->cppname, (matmul->methodcode != NULL),
                matmul->pysig.nrArgs, FALSE);

        matmul->platforms = od->platforms;
        matmul->next = od->next;
        od->next = matmul;
    }

    if (getOptFlag(optflgs, "__imatmul__", bool_flag) != NULL)
    {
        overDef *imatmul;

        imatmul = sipMalloc(sizeof (overDef));

        imatmul->cppname = "__imatmul__";
        imatmul->overflags = SECT_IS_PUBLIC;
        imatmul->pysig = od->pysig;
        imatmul->cppsig = (cppsig != NULL ? cppsig : &imatmul->pysig);

        imatmul->methodcode = od->methodcode;

        imatmul->common = findFunction(pt, mod, c_scope, ns_scope, mt_scope,
                imatmul->cppname, (imatmul->methodcode != NULL),
                imatmul->pysig.nrArgs, FALSE);

        imatmul->platforms = od->platforms;
        imatmul->next = od->next;
        od->next = imatmul;
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
        ifaceFileDef *ns_scope, mappedTypeDef *mt_scope, const char *pname,
        int hwcode, int nrargs, int no_arg_parser)
{
    static struct slot_map {
        const char *name;   /* The slot name. */
        slotType type;      /* The corresponding type. */
        int needs_hwcode;   /* Set if handwritten code is required. */
        int nrargs;         /* Nr. of arguments. */
    } slot_table[] = {
        {"__str__", str_slot, TRUE, 0},
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
        {"__matmul__", matmul_slot, FALSE, 1},
        {"__imatmul__", imatmul_slot, FALSE, 1},
        {"__await__", await_slot, TRUE, 0},
        {"__aiter__", aiter_slot, TRUE, 0},
        {"__anext__", anext_slot, TRUE, 0},
        {NULL, no_slot, FALSE, 0}
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
        md->ns_scope = ns_scope;
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
    if (mt_scope == NULL && c_scope == NULL && st != no_slot && st != neg_slot && st != pos_slot && !isNumberSlot(md) && !isInplaceNumberSlot(md) && !isRichCompareSlot(md))
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

    /* We don't do any check for a non-strict parse. */
    if (!strictParse)
        return;

    /* Check the enums. */
    for (ed = pt->enums; ed != NULL; ed = ed->next)
    {
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

        if (!isScopedEnum(ed))
        {
            enumMemberDef *emd;

            for (emd = ed->members; emd != NULL; emd = emd->next)
                if (strcmp(emd->pyname->text, attr) == 0)
                    yyerror("There is already an enum member in scope with the same Python name");
        }
    }

    /*
     * Only check the members if this attribute isn't a member because we can
     * handle members with the same name in the same scope.
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
 * Append a code block to a list of them.
 */
static void appendCodeBlock(codeBlockList **headp, codeBlock *cb)
{
    codeBlockList *cbl;

    /* Handle the trivial case. */
    if (cb == NULL)
        return;

    /* Find the end of the list. */
    while (*headp != NULL)
    {
        /* Ignore if the block is already in the list. */
        if ((*headp)->block == cb)
            return;

        headp = &(*headp)->next;
    }

    cbl = sipMalloc(sizeof (codeBlockList));
    cbl->block = cb;

    *headp = cbl;
}


/*
 * Append a code block list to an existing list.
 */
void appendCodeBlockList(codeBlockList **headp, codeBlockList *cbl)
{
    while (cbl != NULL)
    {
        appendCodeBlock(headp, cbl->block);
        cbl = cbl->next;
    }
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

    if (stackPtr > currentContext.ifdepth)
        fatal("Too many %%If statements in %s\n", previousFile);

    if (stackPtr < currentContext.ifdepth)
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

    if (from != NULL)
    {
        if (from->encoding == no_type)
            from->encoding = currentModule->encoding;

        if (isCallSuperInitUndefined(from))
        {
            if (isCallSuperInitYes(currentModule))
                setCallSuperInitYes(from);
            else
                setCallSuperInitNo(from);
        }
    }

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
                (major << 16) | (minor << 8) | patch, TRUE, name,
                time_qualifier);
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

    snd = (scope != NULL ? copyScopedName(scope->fqcname) : text2scopePart(""));

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

    while (snd->next != NULL)
        snd = snd->next;

    return snd->name;
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
    return (stackPtr == 0 ? TRUE : skipStack[stackPtr - 1]);
}


/*
 * Return the value of an expression involving a time period.
 */
static int timePeriod(const char *lname, const char *uname)
{
    int line;
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
        if (lower != NULL && SIP_VERSION < lower->order)
            return FALSE;

        if (upper != NULL && SIP_VERSION >= upper->order)
            return FALSE;

        return TRUE;
    }

    for (qd = mod->qualifiers; qd != NULL; qd = qd->next)
    {
        if (qd->qtype != time_qualifier || qd->line != line)
            continue;

        if (selectedQualifier(neededQualifiers, qd))
        {
            if (lower != NULL && qd->order < lower->order)
                return FALSE;

            if (upper != NULL && qd->order >= upper->order)
                return FALSE;

            return TRUE;
        }
    }

    /*
     * If there is no upper bound then assume the expression is true unless
     * the lower bound is a backstop.
     */
    if (upper == NULL)
        return !isBackstop(lower);

    /*
     * If the upper limit corresponds to a backstop then assume the expression
     * is true.
     */
    return isBackstop(upper);
}


/*
 * See if a qualifier is a backstop.
 */
static int isBackstop(qualDef *qd)
{
    stringList *sl;

    for (sl = backstops; sl != NULL; sl = sl->next)
        if (strcmp(qd->name, sl->s) == 0)
            return TRUE;

    return FALSE;
}


/*
 * Return the value of an expression involving a single platform or feature.
 */
static int platOrFeature(char *name, int optnot)
{
    int this;
    qualDef *qd;

    if ((qd = findQualifier(name)) == NULL || qd->qtype == time_qualifier)
        yyerror("No such platform or feature");

    /* Assume this sub-expression is false. */

    this = FALSE;

    if (qd->qtype == feature_qualifier)
    {
        if (!excludedFeature(excludedQualifiers, qd))
            this = TRUE;
    }
    else
    {
        if (!strictParse)
        {
            if (optnot)
            {
                moduleDef *mod;

                /* Add every platform except the one we have. */
                for (mod = currentSpec->modules; mod != NULL; mod = mod->next)
                {
                    qualDef *not_qd;

                    for (not_qd = mod->qualifiers; not_qd != NULL; not_qd = not_qd->next)
                        if (not_qd->qtype == platform_qualifier && strcmp(not_qd->name, qd->name) != 0)
                            addPlatform(not_qd);
                }
            }
            else
            {
                addPlatform(qd);
            }

            /*
             * If it is a non-strict parse then this is always TRUE, ie. we
             * never skip because of the platform.
             */
            return TRUE;
        }

        if (selectedQualifier(neededQualifiers, qd))
            this = TRUE;
    }

    if (optnot)
        this = !this;

    return this;
}


/*
 * Add a platform to the current list of platforms if it is not already there.
 */
static void addPlatform(qualDef *qd)
{
    platformDef *pd;

    for (pd = currentPlatforms; pd != NULL; pd = pd->next)
        if (pd->qualifier == qd)
            return;

    pd = sipMalloc(sizeof (platformDef));
    pd->qualifier = qd;
    pd->next = currentPlatforms;
    currentPlatforms = pd;
}


/*
 * Return TRUE if the given qualifier is excluded.
 */
int excludedFeature(stringList *xsl, qualDef *qd)
{
    while (xsl != NULL)
    {
        if (strcmp(qd->name, xsl->s) == 0)
            return TRUE;

        xsl = xsl->next;
    }

    return !qd->default_enabled;
}


/*
 * Return TRUE if the given qualifier is needed.
 */
int selectedQualifier(stringList *needed_qualifiers, qualDef *qd)
{
    stringList *sl;

    for (sl = needed_qualifiers; sl != NULL; sl = sl->next)
        if (strcmp(qd->name, sl->s) == 0)
            return qd->default_enabled;

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
static void newQualifier(moduleDef *mod, int line, int order,
        int default_enabled, const char *name, qualType qt)
{
    /* Check it doesn't already exist. */
    if (findQualifier(name) != NULL)
        yyerror("Version is already defined");

    allocQualifier(mod, line, order, default_enabled, name, qt);
}


/*
 * Allocate a new qualifier.
 */
static qualDef *allocQualifier(moduleDef *mod, int line, int order,
        int default_enabled, const char *name, qualType qt)
{
    qualDef *qd;

    qd = sipMalloc(sizeof (qualDef));

    qd->name = name;
    qd->qtype = qt;
    qd->module = mod;
    qd->line = line;
    qd->order = order;
    qd->default_enabled = default_enabled;
    qd->next = mod->qualifiers;

    mod->qualifiers = qd;

    return qd;
}


/*
 * Create a new imported module.
 */
static void newImport(const char *filename)
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
 * Get the /DisallowNone/ option flag.
 */
static int getDisallowNone(optFlags *optflgs)
{
    return (getOptFlag(optflgs, "DisallowNone", bool_flag) != NULL);
}


/*
 * Get the /VirtualErrorHandler/ option flag.
 */
static const char *getVirtErrorHandler(optFlags *optflgs)
{
    optFlag *of = getOptFlag(optflgs, "VirtualErrorHandler", name_flag);

    if (of == NULL)
        return NULL;

    return of->fvalue.sval;
}


/*
 * Get the /DocType/ option flag.
 */
static const char *getDocType(optFlags *optflgs)
{
    optFlag *of = getOptFlag(optflgs, "DocType", string_flag);

    if (of == NULL)
        return NULL;

    deprecated("/DocType/ is deprecated\n");

    return of->fvalue.sval;
}


/*
 * Get the /TypeHintValue/ option flag.
 */
static const char *getTypeHintValue(optFlags *optflgs)
{
    optFlag *of = getOptFlag(optflgs, "TypeHintValue", string_flag);

    if (of != NULL)
        return of->fvalue.sval;

    if ((of = getOptFlag(optflgs, "DocValue", string_flag)) != NULL)
    {
        deprecated("/DocValue/ is deprecated\n");

        return of->fvalue.sval;
    }

    return NULL;
}


/*
 * Get the /TypeHint/, /TypeHintIn/ and /TypeHintOut/ option flags.
 */
static void getTypeHints(optFlags *optflgs, typeHintDef **in,
        typeHintDef **out)
{
    optFlag *of;
    typeHintDef *thd;

    if ((of = getOptFlag(optflgs, "TypeHint", string_flag)) != NULL)
        thd = newTypeHint(of->fvalue.sval);
    else
        thd = NULL;

    if ((of = getOptFlag(optflgs, "TypeHintIn", string_flag)) != NULL)
    {
        if (thd != NULL)
            yywarning("/TypeHintIn/ overrides /TypeHint/");

        *in = newTypeHint(of->fvalue.sval);
    }
    else
    {
        *in = thd;
    }

    if ((of = getOptFlag(optflgs, "TypeHintOut", string_flag)) != NULL)
    {
        if (thd != NULL)
            yywarning("/TypeHintOut/ overrides /TypeHint/");

        *out = newTypeHint(of->fvalue.sval);
    }
    else
    {
        *out = thd;
    }
}


/*
 * Get the /NoTypeHint/ option flag.
 */
static int getNoTypeHint(optFlags *optflgs)
{
    return (getOptFlag(optflgs, "NoTypeHint", bool_flag) != NULL);
}


/*
 * Return TRUE if the PyQt4 plugin was specified.
 */
int pluginPyQt4(sipSpec *pt)
{
    return stringFind(pt->plugins, "PyQt4");
}


/*
 * Return TRUE if the PyQt5 plugin was specified.
 */
int pluginPyQt5(sipSpec *pt)
{
    return stringFind(pt->plugins, "PyQt5");
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
    classDef *cd;
    typeHintDef *in, *out;

    getTypeHints(of, &in, &out);

    cd = newClass(currentSpec, class_iface, getAPIRange(of),
            fullyQualifiedName(snd), getVirtErrorHandler(of), in, out,
            getTypeHintValue(of));
    cd->supers = supers;

    pushScope(cd);
}


/*
 * Return a fully qualified scoped name.
 */
static scopedNameDef *fullyQualifiedName(scopedNameDef *snd)
{
    classDef *scope = currentScope();

    return scopeScopedName((scope != NULL ? scope->iff : NULL), snd);
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
    getTypeHints(flags, &ad->typehint_in, &ad->typehint_out);

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
 * Return the Format for a string.
 */
static Format convertFormat(const char *format)
{
    if (strcmp(format, "raw") == 0)
        return raw;

    if (strcmp(format, "deindented") == 0)
        return deindented;

    yyerror("The docstring format must be either \"raw\" or \"deindented\"");
}


/*
 * Return the Signature for a string.
 */
static Signature convertSignature(const char *signature)
{
    if (strcmp(signature, "discarded") == 0)
        return discarded;

    if (strcmp(signature, "prepended") == 0)
        return prepended;

    if (strcmp(signature, "appended") == 0)
        return appended;

    yyerror("The docstring signature must be either \"discarded\", \"prepended\" or \"appended\"");
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
static KwArgs keywordArgs(moduleDef *mod, optFlags *optflgs, signatureDef *sd,
        int need_name)
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
                if (need_name || inMainModule())
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
        docstringDef *docstring)
{
    propertyDef *pd;

    checkAttributes(pt, mod, cd, NULL, name, FALSE);

    pd = sipMalloc(sizeof (propertyDef));

    pd->name = cacheName(pt, name);
    pd->get = get;
    pd->set = set;
    pd->docstring = docstring;
    pd->platforms = currentPlatforms;
    pd->next = cd->properties;

    cd->properties = pd;

    if (inMainModule())
        setIsUsedName(pd->name);
}


/*
 * Configure a module and return the (possibly new) current module.
 */
static moduleDef *configureModule(sipSpec *pt, moduleDef *module,
        const char *filename, const char *name, int c_module, KwArgs kwargs,
        int use_arg_names, int use_limited_api, int call_super_init,
        int all_raise_py_exc, const char *def_error_handler,
        docstringDef *docstring)
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
    module->virt_error_handler = def_error_handler;
    module->docstring = docstring;

    if (all_raise_py_exc)
        setAllRaisePyException(module);

    if (use_arg_names)
        setUseArgNames(module);

    if (use_limited_api)
        setUseLimitedAPI(module);

    if (call_super_init == 0)
        setCallSuperInitNo(module);
    else if (call_super_init > 0)
        setCallSuperInitYes(module);

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
                yywarning("Annotation is unknown");
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


/*
 * Handle any /KeepReference/ annotation for a type.
 */
static void handleKeepReference(optFlags *optflgs, argDef *ad, moduleDef *mod)
{
    optFlag *of;

    if ((of = getOptFlag(optflgs, "KeepReference", opt_integer_flag)) != NULL)
    {
        setKeepReference(ad);

        if ((ad->key = of->fvalue.ival) < -1)
            yyerror("/KeepReference/ key cannot be negative");

        /* If there was no explicit key then auto-allocate one. */
        if (ad->key == -1)
            ad->key = mod->next_key--;
    }
}


/*
 * Configure the mapped type annotations that are also valid with mapped type
 * templates.
 */
static void mappedTypeAnnos(mappedTypeDef *mtd, optFlags *optflgs)
{
    if (getOptFlag(optflgs, "NoRelease", bool_flag) != NULL)
        setNoRelease(mtd);

    if (getAllowNone(optflgs))
        setHandlesNone(mtd);

    mtd->doctype = getDocType(optflgs);
    getTypeHints(optflgs, &mtd->typehint_in, &mtd->typehint_out);
    mtd->typehint_value = getTypeHintValue(optflgs);
}


/*
 * Initialise an argument with the derefences of another, plus a new one.
 */
static void add_new_deref(argDef *new, argDef *orig, int isconst)
{
    if ((new->nrderefs = orig->nrderefs + 1) >= MAX_NR_DEREFS)
        yyerror("Internal error - increase the value of MAX_NR_DEREFS");

    memcpy(&new->derefs[0], &orig->derefs[0], sizeof (new->derefs));
    new->derefs[orig->nrderefs] = isconst;
}


/*
 * Add the dereferences from one type to another.
 */
static void add_derefs(argDef *dst, argDef *src)
{
    int i;

    for (i = 0; i < src->nrderefs; ++i)
    {
        if (dst->nrderefs >= MAX_NR_DEREFS - 1)
            fatal("Internal error - increase the value of MAX_NR_DEREFS\n");

        dst->derefs[dst->nrderefs++] = src->derefs[i];
    }
}


/*
 * Check if a word is a Python keyword (or has been at any time).
 */
int isPyKeyword(const char *word)
{
    static const char *kwds[] = {
        "False", "None", "True", "and", "as", "assert", "break", "class",
        "continue", "def", "del", "elif", "else", "except", "finally", "for",
        "from", "global", "if", "import", "in", "is", "lambda", "nonlocal",
        "not", "or", "pass", "raise", "return", "try", "while", "with'"
        "yield",
        /* Historical keywords. */
        "exec", "print",
        NULL
    };

    const char **kwd;

    for (kwd = kwds; *kwd != NULL; ++kwd)
        if (strcmp(*kwd, word) == 0)
            return TRUE;

    return FALSE;
}


/*
 * Check there is nothing after an ellipsis.
 */
static void checkEllipsis(signatureDef *sd)
{
    int a;

    for (a = 0; a < sd->nrArgs; ++a)
        if (sd->args[a].atype == ellipsis_type && a < sd->nrArgs - 1)
            yyerror("An ellipsis must be at the end of the argument list if /NoArgParser/ is not specified");
}

