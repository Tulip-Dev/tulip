/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

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
     TK_SIZET = 361,
     TK_NUMBER_VALUE = 362,
     TK_REAL_VALUE = 363,
     TK_TYPEDEF = 364,
     TK_NAMESPACE = 365,
     TK_TIMELINE = 366,
     TK_PLATFORMS = 367,
     TK_FEATURE = 368,
     TK_LICENSE = 369,
     TK_QCHAR_VALUE = 370,
     TK_TRUE_VALUE = 371,
     TK_FALSE_VALUE = 372,
     TK_NULL_VALUE = 373,
     TK_OPERATOR = 374,
     TK_THROW = 375,
     TK_QOBJECT = 376,
     TK_EXCEPTION = 377,
     TK_RAISECODE = 378,
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
#define TK_SIZET 361
#define TK_NUMBER_VALUE 362
#define TK_REAL_VALUE 363
#define TK_TYPEDEF 364
#define TK_NAMESPACE 365
#define TK_TIMELINE 366
#define TK_PLATFORMS 367
#define TK_FEATURE 368
#define TK_LICENSE 369
#define TK_QCHAR_VALUE 370
#define TK_TRUE_VALUE 371
#define TK_FALSE_VALUE 372
#define TK_NULL_VALUE 373
#define TK_OPERATOR 374
#define TK_THROW 375
#define TK_QOBJECT 376
#define TK_EXCEPTION 377
#define TK_RAISECODE 378
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




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 202 "sip-4.19.17/sipgen/metasrc/parser.y"
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
/* Line 1529 of yacc.c.  */
#line 396 "sip-4.19.17/sipgen/parser.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

