
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
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

/* Line 1676 of yacc.c  */
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



/* Line 1676 of yacc.c  */
#line 358 "/home/phil/hg/sip/sip-4.12.1/sipgen/parser.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


