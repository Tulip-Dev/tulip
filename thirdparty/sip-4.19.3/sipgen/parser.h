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
     TK_DEFDOCSTRING = 260,
     TK_DEFENCODING = 261,
     TK_PLUGIN = 262,
     TK_VIRTERRORHANDLER = 263,
     TK_EXPTYPEHINTCODE = 264,
     TK_TYPEHINTCODE = 265,
     TK_DOCSTRING = 266,
     TK_DOC = 267,
     TK_EXPORTEDDOC = 268,
     TK_EXTRACT = 269,
     TK_MAKEFILE = 270,
     TK_ACCESSCODE = 271,
     TK_GETCODE = 272,
     TK_SETCODE = 273,
     TK_PREINITCODE = 274,
     TK_INITCODE = 275,
     TK_POSTINITCODE = 276,
     TK_FINALCODE = 277,
     TK_UNITCODE = 278,
     TK_UNITPOSTINCLUDECODE = 279,
     TK_MODCODE = 280,
     TK_TYPECODE = 281,
     TK_PREPYCODE = 282,
     TK_COPYING = 283,
     TK_MAPPEDTYPE = 284,
     TK_CODELINE = 285,
     TK_IF = 286,
     TK_END = 287,
     TK_NAME_VALUE = 288,
     TK_PATH_VALUE = 289,
     TK_STRING_VALUE = 290,
     TK_VIRTUALCATCHERCODE = 291,
     TK_TRAVERSECODE = 292,
     TK_CLEARCODE = 293,
     TK_GETBUFFERCODE = 294,
     TK_RELEASEBUFFERCODE = 295,
     TK_READBUFFERCODE = 296,
     TK_WRITEBUFFERCODE = 297,
     TK_SEGCOUNTCODE = 298,
     TK_CHARBUFFERCODE = 299,
     TK_PICKLECODE = 300,
     TK_VIRTUALCALLCODE = 301,
     TK_METHODCODE = 302,
     TK_PREMETHODCODE = 303,
     TK_INSTANCECODE = 304,
     TK_FROMTYPE = 305,
     TK_TOTYPE = 306,
     TK_TOSUBCLASS = 307,
     TK_INCLUDE = 308,
     TK_OPTINCLUDE = 309,
     TK_IMPORT = 310,
     TK_EXPHEADERCODE = 311,
     TK_MODHEADERCODE = 312,
     TK_TYPEHEADERCODE = 313,
     TK_MODULE = 314,
     TK_CMODULE = 315,
     TK_CONSMODULE = 316,
     TK_COMPOMODULE = 317,
     TK_CLASS = 318,
     TK_STRUCT = 319,
     TK_PUBLIC = 320,
     TK_PROTECTED = 321,
     TK_PRIVATE = 322,
     TK_SIGNALS = 323,
     TK_SIGNAL_METHOD = 324,
     TK_SLOTS = 325,
     TK_SLOT_METHOD = 326,
     TK_BOOL = 327,
     TK_SHORT = 328,
     TK_INT = 329,
     TK_LONG = 330,
     TK_FLOAT = 331,
     TK_DOUBLE = 332,
     TK_CHAR = 333,
     TK_WCHAR_T = 334,
     TK_VOID = 335,
     TK_PYOBJECT = 336,
     TK_PYTUPLE = 337,
     TK_PYLIST = 338,
     TK_PYDICT = 339,
     TK_PYCALLABLE = 340,
     TK_PYSLICE = 341,
     TK_PYTYPE = 342,
     TK_PYBUFFER = 343,
     TK_VIRTUAL = 344,
     TK_ENUM = 345,
     TK_SIGNED = 346,
     TK_UNSIGNED = 347,
     TK_SCOPE = 348,
     TK_LOGICAL_OR = 349,
     TK_CONST = 350,
     TK_STATIC = 351,
     TK_SIPSIGNAL = 352,
     TK_SIPSLOT = 353,
     TK_SIPANYSLOT = 354,
     TK_SIPRXCON = 355,
     TK_SIPRXDIS = 356,
     TK_SIPSLOTCON = 357,
     TK_SIPSLOTDIS = 358,
     TK_SIPSSIZET = 359,
     TK_NUMBER_VALUE = 360,
     TK_REAL_VALUE = 361,
     TK_TYPEDEF = 362,
     TK_NAMESPACE = 363,
     TK_TIMELINE = 364,
     TK_PLATFORMS = 365,
     TK_FEATURE = 366,
     TK_LICENSE = 367,
     TK_QCHAR_VALUE = 368,
     TK_TRUE_VALUE = 369,
     TK_FALSE_VALUE = 370,
     TK_NULL_VALUE = 371,
     TK_OPERATOR = 372,
     TK_THROW = 373,
     TK_QOBJECT = 374,
     TK_EXCEPTION = 375,
     TK_RAISECODE = 376,
     TK_VIRTERRORCODE = 377,
     TK_EXPLICIT = 378,
     TK_TEMPLATE = 379,
     TK_FINAL = 380,
     TK_ELLIPSIS = 381,
     TK_DEFMETATYPE = 382,
     TK_DEFSUPERTYPE = 383,
     TK_PROPERTY = 384,
     TK_HIDE_NS = 385,
     TK_FORMAT = 386,
     TK_GET = 387,
     TK_ID = 388,
     TK_KWARGS = 389,
     TK_LANGUAGE = 390,
     TK_LICENSEE = 391,
     TK_NAME = 392,
     TK_OPTIONAL = 393,
     TK_ORDER = 394,
     TK_REMOVELEADING = 395,
     TK_SET = 396,
     TK_SIGNATURE = 397,
     TK_TIMESTAMP = 398,
     TK_TYPE = 399,
     TK_USEARGNAMES = 400,
     TK_USELIMITEDAPI = 401,
     TK_ALLRAISEPYEXC = 402,
     TK_CALLSUPERINIT = 403,
     TK_DEFERRORHANDLER = 404,
     TK_VERSION = 405
   };
#endif
/* Tokens.  */
#define TK_API 258
#define TK_AUTOPYNAME 259
#define TK_DEFDOCSTRING 260
#define TK_DEFENCODING 261
#define TK_PLUGIN 262
#define TK_VIRTERRORHANDLER 263
#define TK_EXPTYPEHINTCODE 264
#define TK_TYPEHINTCODE 265
#define TK_DOCSTRING 266
#define TK_DOC 267
#define TK_EXPORTEDDOC 268
#define TK_EXTRACT 269
#define TK_MAKEFILE 270
#define TK_ACCESSCODE 271
#define TK_GETCODE 272
#define TK_SETCODE 273
#define TK_PREINITCODE 274
#define TK_INITCODE 275
#define TK_POSTINITCODE 276
#define TK_FINALCODE 277
#define TK_UNITCODE 278
#define TK_UNITPOSTINCLUDECODE 279
#define TK_MODCODE 280
#define TK_TYPECODE 281
#define TK_PREPYCODE 282
#define TK_COPYING 283
#define TK_MAPPEDTYPE 284
#define TK_CODELINE 285
#define TK_IF 286
#define TK_END 287
#define TK_NAME_VALUE 288
#define TK_PATH_VALUE 289
#define TK_STRING_VALUE 290
#define TK_VIRTUALCATCHERCODE 291
#define TK_TRAVERSECODE 292
#define TK_CLEARCODE 293
#define TK_GETBUFFERCODE 294
#define TK_RELEASEBUFFERCODE 295
#define TK_READBUFFERCODE 296
#define TK_WRITEBUFFERCODE 297
#define TK_SEGCOUNTCODE 298
#define TK_CHARBUFFERCODE 299
#define TK_PICKLECODE 300
#define TK_VIRTUALCALLCODE 301
#define TK_METHODCODE 302
#define TK_PREMETHODCODE 303
#define TK_INSTANCECODE 304
#define TK_FROMTYPE 305
#define TK_TOTYPE 306
#define TK_TOSUBCLASS 307
#define TK_INCLUDE 308
#define TK_OPTINCLUDE 309
#define TK_IMPORT 310
#define TK_EXPHEADERCODE 311
#define TK_MODHEADERCODE 312
#define TK_TYPEHEADERCODE 313
#define TK_MODULE 314
#define TK_CMODULE 315
#define TK_CONSMODULE 316
#define TK_COMPOMODULE 317
#define TK_CLASS 318
#define TK_STRUCT 319
#define TK_PUBLIC 320
#define TK_PROTECTED 321
#define TK_PRIVATE 322
#define TK_SIGNALS 323
#define TK_SIGNAL_METHOD 324
#define TK_SLOTS 325
#define TK_SLOT_METHOD 326
#define TK_BOOL 327
#define TK_SHORT 328
#define TK_INT 329
#define TK_LONG 330
#define TK_FLOAT 331
#define TK_DOUBLE 332
#define TK_CHAR 333
#define TK_WCHAR_T 334
#define TK_VOID 335
#define TK_PYOBJECT 336
#define TK_PYTUPLE 337
#define TK_PYLIST 338
#define TK_PYDICT 339
#define TK_PYCALLABLE 340
#define TK_PYSLICE 341
#define TK_PYTYPE 342
#define TK_PYBUFFER 343
#define TK_VIRTUAL 344
#define TK_ENUM 345
#define TK_SIGNED 346
#define TK_UNSIGNED 347
#define TK_SCOPE 348
#define TK_LOGICAL_OR 349
#define TK_CONST 350
#define TK_STATIC 351
#define TK_SIPSIGNAL 352
#define TK_SIPSLOT 353
#define TK_SIPANYSLOT 354
#define TK_SIPRXCON 355
#define TK_SIPRXDIS 356
#define TK_SIPSLOTCON 357
#define TK_SIPSLOTDIS 358
#define TK_SIPSSIZET 359
#define TK_NUMBER_VALUE 360
#define TK_REAL_VALUE 361
#define TK_TYPEDEF 362
#define TK_NAMESPACE 363
#define TK_TIMELINE 364
#define TK_PLATFORMS 365
#define TK_FEATURE 366
#define TK_LICENSE 367
#define TK_QCHAR_VALUE 368
#define TK_TRUE_VALUE 369
#define TK_FALSE_VALUE 370
#define TK_NULL_VALUE 371
#define TK_OPERATOR 372
#define TK_THROW 373
#define TK_QOBJECT 374
#define TK_EXCEPTION 375
#define TK_RAISECODE 376
#define TK_VIRTERRORCODE 377
#define TK_EXPLICIT 378
#define TK_TEMPLATE 379
#define TK_FINAL 380
#define TK_ELLIPSIS 381
#define TK_DEFMETATYPE 382
#define TK_DEFSUPERTYPE 383
#define TK_PROPERTY 384
#define TK_HIDE_NS 385
#define TK_FORMAT 386
#define TK_GET 387
#define TK_ID 388
#define TK_KWARGS 389
#define TK_LANGUAGE 390
#define TK_LICENSEE 391
#define TK_NAME 392
#define TK_OPTIONAL 393
#define TK_ORDER 394
#define TK_REMOVELEADING 395
#define TK_SET 396
#define TK_SIGNATURE 397
#define TK_TIMESTAMP 398
#define TK_TYPE 399
#define TK_USEARGNAMES 400
#define TK_USELIMITEDAPI 401
#define TK_ALLRAISEPYEXC 402
#define TK_CALLSUPERINIT 403
#define TK_DEFERRORHANDLER 404
#define TK_VERSION 405




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 200 "sip-4.19.3/sipgen/metasrc/parser.y"
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
    defDocstringCfg defdocstring;
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
#line 392 "sip-4.19.3/sipgen/parser.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

