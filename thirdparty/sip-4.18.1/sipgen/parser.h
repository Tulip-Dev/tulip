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
     TK_INSTANCECODE = 303,
     TK_FROMTYPE = 304,
     TK_TOTYPE = 305,
     TK_TOSUBCLASS = 306,
     TK_INCLUDE = 307,
     TK_OPTINCLUDE = 308,
     TK_IMPORT = 309,
     TK_EXPHEADERCODE = 310,
     TK_MODHEADERCODE = 311,
     TK_TYPEHEADERCODE = 312,
     TK_MODULE = 313,
     TK_CMODULE = 314,
     TK_CONSMODULE = 315,
     TK_COMPOMODULE = 316,
     TK_CLASS = 317,
     TK_STRUCT = 318,
     TK_PUBLIC = 319,
     TK_PROTECTED = 320,
     TK_PRIVATE = 321,
     TK_SIGNALS = 322,
     TK_SIGNAL_METHOD = 323,
     TK_SLOTS = 324,
     TK_SLOT_METHOD = 325,
     TK_BOOL = 326,
     TK_SHORT = 327,
     TK_INT = 328,
     TK_LONG = 329,
     TK_FLOAT = 330,
     TK_DOUBLE = 331,
     TK_CHAR = 332,
     TK_WCHAR_T = 333,
     TK_VOID = 334,
     TK_PYOBJECT = 335,
     TK_PYTUPLE = 336,
     TK_PYLIST = 337,
     TK_PYDICT = 338,
     TK_PYCALLABLE = 339,
     TK_PYSLICE = 340,
     TK_PYTYPE = 341,
     TK_PYBUFFER = 342,
     TK_VIRTUAL = 343,
     TK_ENUM = 344,
     TK_SIGNED = 345,
     TK_UNSIGNED = 346,
     TK_SCOPE = 347,
     TK_LOGICAL_OR = 348,
     TK_CONST = 349,
     TK_STATIC = 350,
     TK_SIPSIGNAL = 351,
     TK_SIPSLOT = 352,
     TK_SIPANYSLOT = 353,
     TK_SIPRXCON = 354,
     TK_SIPRXDIS = 355,
     TK_SIPSLOTCON = 356,
     TK_SIPSLOTDIS = 357,
     TK_SIPSSIZET = 358,
     TK_NUMBER_VALUE = 359,
     TK_REAL_VALUE = 360,
     TK_TYPEDEF = 361,
     TK_NAMESPACE = 362,
     TK_TIMELINE = 363,
     TK_PLATFORMS = 364,
     TK_FEATURE = 365,
     TK_LICENSE = 366,
     TK_QCHAR_VALUE = 367,
     TK_TRUE_VALUE = 368,
     TK_FALSE_VALUE = 369,
     TK_NULL_VALUE = 370,
     TK_OPERATOR = 371,
     TK_THROW = 372,
     TK_QOBJECT = 373,
     TK_EXCEPTION = 374,
     TK_RAISECODE = 375,
     TK_VIRTERRORCODE = 376,
     TK_EXPLICIT = 377,
     TK_TEMPLATE = 378,
     TK_ELLIPSIS = 379,
     TK_DEFMETATYPE = 380,
     TK_DEFSUPERTYPE = 381,
     TK_PROPERTY = 382,
     TK_FORMAT = 383,
     TK_GET = 384,
     TK_ID = 385,
     TK_KWARGS = 386,
     TK_LANGUAGE = 387,
     TK_LICENSEE = 388,
     TK_NAME = 389,
     TK_OPTIONAL = 390,
     TK_ORDER = 391,
     TK_REMOVELEADING = 392,
     TK_SET = 393,
     TK_SIGNATURE = 394,
     TK_TIMESTAMP = 395,
     TK_TYPE = 396,
     TK_USEARGNAMES = 397,
     TK_ALLRAISEPYEXC = 398,
     TK_CALLSUPERINIT = 399,
     TK_DEFERRORHANDLER = 400,
     TK_VERSION = 401
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
#define TK_INSTANCECODE 303
#define TK_FROMTYPE 304
#define TK_TOTYPE 305
#define TK_TOSUBCLASS 306
#define TK_INCLUDE 307
#define TK_OPTINCLUDE 308
#define TK_IMPORT 309
#define TK_EXPHEADERCODE 310
#define TK_MODHEADERCODE 311
#define TK_TYPEHEADERCODE 312
#define TK_MODULE 313
#define TK_CMODULE 314
#define TK_CONSMODULE 315
#define TK_COMPOMODULE 316
#define TK_CLASS 317
#define TK_STRUCT 318
#define TK_PUBLIC 319
#define TK_PROTECTED 320
#define TK_PRIVATE 321
#define TK_SIGNALS 322
#define TK_SIGNAL_METHOD 323
#define TK_SLOTS 324
#define TK_SLOT_METHOD 325
#define TK_BOOL 326
#define TK_SHORT 327
#define TK_INT 328
#define TK_LONG 329
#define TK_FLOAT 330
#define TK_DOUBLE 331
#define TK_CHAR 332
#define TK_WCHAR_T 333
#define TK_VOID 334
#define TK_PYOBJECT 335
#define TK_PYTUPLE 336
#define TK_PYLIST 337
#define TK_PYDICT 338
#define TK_PYCALLABLE 339
#define TK_PYSLICE 340
#define TK_PYTYPE 341
#define TK_PYBUFFER 342
#define TK_VIRTUAL 343
#define TK_ENUM 344
#define TK_SIGNED 345
#define TK_UNSIGNED 346
#define TK_SCOPE 347
#define TK_LOGICAL_OR 348
#define TK_CONST 349
#define TK_STATIC 350
#define TK_SIPSIGNAL 351
#define TK_SIPSLOT 352
#define TK_SIPANYSLOT 353
#define TK_SIPRXCON 354
#define TK_SIPRXDIS 355
#define TK_SIPSLOTCON 356
#define TK_SIPSLOTDIS 357
#define TK_SIPSSIZET 358
#define TK_NUMBER_VALUE 359
#define TK_REAL_VALUE 360
#define TK_TYPEDEF 361
#define TK_NAMESPACE 362
#define TK_TIMELINE 363
#define TK_PLATFORMS 364
#define TK_FEATURE 365
#define TK_LICENSE 366
#define TK_QCHAR_VALUE 367
#define TK_TRUE_VALUE 368
#define TK_FALSE_VALUE 369
#define TK_NULL_VALUE 370
#define TK_OPERATOR 371
#define TK_THROW 372
#define TK_QOBJECT 373
#define TK_EXCEPTION 374
#define TK_RAISECODE 375
#define TK_VIRTERRORCODE 376
#define TK_EXPLICIT 377
#define TK_TEMPLATE 378
#define TK_ELLIPSIS 379
#define TK_DEFMETATYPE 380
#define TK_DEFSUPERTYPE 381
#define TK_PROPERTY 382
#define TK_FORMAT 383
#define TK_GET 384
#define TK_ID 385
#define TK_KWARGS 386
#define TK_LANGUAGE 387
#define TK_LICENSEE 388
#define TK_NAME 389
#define TK_OPTIONAL 390
#define TK_ORDER 391
#define TK_REMOVELEADING 392
#define TK_SET 393
#define TK_SIGNATURE 394
#define TK_TIMESTAMP 395
#define TK_TYPE 396
#define TK_USEARGNAMES 397
#define TK_ALLRAISEPYEXC 398
#define TK_CALLSUPERINIT 399
#define TK_DEFERRORHANDLER 400
#define TK_VERSION 401




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 193 "sip-4.18.1/sipgen/metasrc/parser.y"
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
#line 383 "sip-4.18.1/sipgen/parser.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

