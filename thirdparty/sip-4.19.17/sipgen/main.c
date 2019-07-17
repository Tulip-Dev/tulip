/*
 * The main module for SIP.
 *
 * Copyright (c) 2017 Riverbank Computing Limited <info@riverbankcomputing.com>
 *
 * This file is part of SIP.
 *
 * This copy of SIP is licensed for use under the terms of the SIP License
 * Agreement.  See the file LICENSE for more details.
 *
 * This copy of SIP may also used under the terms of the GNU General Public
 * License v2 or v3 as published by the Free Software Foundation which can be
 * found in the files LICENSE-GPL2 and LICENSE-GPL3 included in this package.
 *
 * SIP is supplied WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

#include "sip.h"


#ifndef PACKAGE
#define PACKAGE "sip"
#endif


/* Global variables - see sip.h for their meaning. */
char *sipVersion;
stringList *includeDirList;

static char *sipPackage = PACKAGE;
static int warnings = FALSE;
static int warnings_are_fatal = FALSE;


SIP_NORETURN static void help(void);
SIP_NORETURN static void version(void);
SIP_NORETURN static void usage(void);
static char parseopt(int,char **,char *,char **,int *,char **);
static int parseInt(char *,char);


int main(int argc, char **argv)
{
    char *filename, *docFile, *codeDir, *srcSuffix, *flagFile, *consModule;
    char arg, *optarg, *buildFile, *apiFile, *xmlFile, *pyiFile;
    const char *sipName;
    int optnr, exceptions, tracing, releaseGIL, parts, protHack, docs;
    int timestamp, was_flagFile, py_debug, strict;
    KwArgs kwArgs;
    FILE *file;
    sipSpec spec;
    stringList *versions, *backstops, *xfeatures, *extracts;

    /* Initialise. */
    sipVersion = SIP_VERSION_STR;
    includeDirList = NULL;
    versions = NULL;
    backstops = NULL;
    xfeatures = NULL;
    buildFile = NULL;
    codeDir = NULL;
    docFile = NULL;
    srcSuffix = NULL;
    flagFile = NULL;
    was_flagFile = FALSE;
    apiFile = NULL;
    xmlFile = NULL;
    pyiFile = NULL;
    sipName = "sip";
    consModule = NULL;
    extracts = NULL;
    exceptions = FALSE;
    tracing = FALSE;
    releaseGIL = FALSE;
    parts = 0;
    kwArgs = NoKwArgs;
    protHack = FALSE;
    docs = FALSE;
    timestamp = TRUE;
    py_debug = FALSE;
    strict = TRUE;

    /* Parse the command line. */
    optnr = 1;

    while ((arg = parseopt(argc, argv, "hVa:b:B:ec:d:DfgI:j:km:n:op:Prs:t:Twx:X:y:z:", &flagFile, &optnr, &optarg)) != '\0')
        switch (arg)
        {
        case 'o':
            /* Generate docstrings. */
            docs = TRUE;
            break;

        case 'p':
            /* The name of the consolidated module. */
            consModule = optarg;
            break;

        case 'P':
            /* Enable the protected/public hack. */
            protHack = TRUE;
            break;

        case 'a':
            /* Where to generate the API file. */
            apiFile = optarg;
            break;

        case 'm':
            /* Where to generate the XML file. */
            xmlFile = optarg;
            break;

        case 'n':
            /* The qualified name of the sip module. */
            sipName = optarg;
            break;

        case 'y':
            /* Where to generate the .pyi file. */
            pyiFile = optarg;
            break;

        case 'b':
            /* Generate a build file. */
            buildFile = optarg;
            break;

        case 'B':
            /* Define a backstop. */
            appendString(&backstops, optarg);
            break;

        case 'e':
            /* Enable exceptions. */
            exceptions = TRUE;
            break;

        case 'g':
            /* Always release the GIL. */
            releaseGIL = TRUE;
            break;

        case 'j':
            /* Generate the code in this number of parts. */
            parts = parseInt(optarg,'j');
            break;

        case 'z':
            /* Read a file for the next flags. */
            if (flagFile != NULL)
                fatal("The -z flag cannot be specified in an argument file\n");

            flagFile = optarg;
            was_flagFile = TRUE;
            break;

        case 'c':
            /* Where to generate the code. */
            codeDir = optarg;
            break;

        case 'd':
            /* Where to generate the documentation. */
            docFile = optarg;
            break;

        case 'D':
            /* Generate code for a debug build of Python. */
            py_debug = TRUE;
            break;

        case 't':
            /* Which platform or version to generate code for. */
            appendString(&versions,optarg);
            break;

        case 'T':
            /*
             * Disable the timestamp in the header of generated files.  It is
             * now ignored apart from triggering a deprecation warning.
             */
            timestamp = FALSE;
            break;

        case 'x':
            /* Which features are disabled. */
            appendString(&xfeatures,optarg);
            break;

        case 'X':
            /* Which extracts are to be created. */
            appendString(&extracts, optarg);
            break;

        case 'I':
            /* Where to get included files from. */
            appendString(&includeDirList,optarg);
            break;

        case 'r':
            /* Enable tracing. */
            tracing = TRUE;
            break;

        case 's':
            /* The suffix to use for source files. */
            srcSuffix = optarg;
            break;

        case 'w':
            /* Enable warning messages. */
            warnings = TRUE;
            break;

        case 'f':
            /* Warning messages are fatal. */
            warnings_are_fatal = TRUE;
            break;

        case 'k':
            /* Allow keyword arguments in functions and methods. */
            kwArgs = AllKwArgs;
            break;

        case 'h':
            /* Help message. */
            help();
            break;

        case 'V':
            /* Display the version number. */
            version();
            break;

        default:
            usage();
        }

    if (optnr < argc)
    {
        file = NULL;
        filename = argv[optnr++];

        if (optnr < argc)
            usage();
    }
    else
    {
        file = stdin;
        filename = "stdin";
    }

    /* Issue warnings after they (might) have been enabled. */
    if (docFile != NULL)
        warning(DeprecationWarning, "the -d flag is deprecated\n");

    if (kwArgs != NoKwArgs)
        warning(DeprecationWarning, "the -k flag is deprecated\n");

    if (!timestamp)
        warning(DeprecationWarning, "the -T flag is ignored and deprecated\n");

    if (was_flagFile)
        warning(DeprecationWarning, "the -z flag is deprecated\n");

    /* Handle conflicting arguments. */
    if (xmlFile != NULL)
    {
        if (codeDir != NULL || docFile != NULL || buildFile != NULL)
            fatal("The -m flag cannot be specified with either the -b, -c or -d flags\n");

        strict = FALSE;
    }

    /* Parse the input file. */
    parse(&spec, file, filename, strict, versions, backstops, xfeatures,
            kwArgs, protHack);

    /* Verify and transform the parse tree. */
    transform(&spec, strict);

    /* Generate code. */
    generateCode(&spec, codeDir, buildFile, docFile, srcSuffix, exceptions,
            tracing, releaseGIL, parts, versions, xfeatures, consModule, docs,
            py_debug, sipName);

    /* Generate any extracts. */
    generateExtracts(&spec, extracts);

    /* Generate the API file. */
    if (apiFile != NULL)
        generateAPI(&spec, spec.module, apiFile);

    /* Generate the XML export. */
    if (xmlFile != NULL)
        generateXML(&spec, spec.module,  xmlFile);

    /* Generate the .pyi file. */
    if (pyiFile != NULL)
        generateTypeHints(&spec, spec.module,  pyiFile);

    /* All done. */
    return 0;
}


/*
 * Parse the next command line argument - similar to UNIX getopts().  Allow a
 * flag to specify that a file contains further arguments.
 */
static char parseopt(int argc, char **argv, char *opts, char **flags,
        int *optnrp, char **optargp)
{
    char arg, *op, *fname;
    int optnr;
    static FILE *fp = NULL;

    optnr = *optnrp;

    /* Deal with any file first. */

    fname = *flags;

    /* Support the sip5 method of passing arguments in a file. */
    if (fname == NULL && optnr < argc && argv[optnr][0] == '@')
    {
        fname = *flags = &argv[optnr][1];
        *optnrp = ++optnr;
    }

    if (fname != NULL && fp == NULL && (fp = fopen(fname,"r")) == NULL)
        fatal("Unable to open %s\n",fname);

    if (fp != NULL)
    {
        char buf[200], *cp, *fname;
        int ch;

        fname = *flags;
        cp = buf;

        while ((ch = fgetc(fp)) != EOF)
        {
            /* Skip leading whitespace. */

            if (cp == buf && isspace(ch))
                continue;

            if (ch == '\n')
                break;

            if (cp == &buf[sizeof (buf) - 1])
                fatal("A flag in %s is too long\n",fname);

            *cp++ = (char)ch;
        }

        *cp = '\0';

        if (ch == EOF)
        {
            fclose(fp);
            fp = NULL;
            *flags = NULL;
        }

        /*
         * Get the option character and any optional argument from the
         * line.
         */

        if (buf[0] != '\0')
        {
            if (buf[0] != '-' || buf[1] == '\0')
                fatal("An non-flag was given in %s\n",fname);

            arg = buf[1];

            /* Find any optional argument. */

            for (cp = &buf[2]; *cp != '\0'; ++cp)
                if (!isspace(*cp))
                    break;

            if (*cp == '\0')
                cp = NULL;
            else
                cp = sipStrdup(cp);

            *optargp = cp;

            if ((op = strchr(opts,arg)) == NULL)
                fatal("An invalid flag was given in %s\n",fname);

            if (op[1] == ':' && cp == NULL)
                fatal("Missing flag argument in %s\n",fname);

            if (op[1] != ':' && cp != NULL)
                fatal("Unexpected flag argument in %s\n",fname);

            return arg;
        }
    }

    /* Check there is an argument and it is a switch. */

    if (optnr >= argc || argv[optnr] == NULL || argv[optnr][0] != '-')
        return '\0';

    /* Check it is a valid switch. */

    arg = argv[optnr][1];

    if (arg == '\0' || (op = strchr(opts,arg)) == NULL)
        usage();

    /* Check for the switch parameter, if any. */

    if (op[1] == ':')
    {
        if (argv[optnr][2] != '\0')
        {
            *optargp = &argv[optnr][2];
            ++optnr;
        }
        else if (optnr + 1 >= argc || argv[optnr + 1] == NULL)
            usage();
        else
        {
            *optargp = argv[optnr + 1];
            optnr += 2;
        }
    }
    else if (argv[optnr][2] != '\0')
        usage();
    else
    {
        *optargp = NULL;
        ++optnr;
    }

    *optnrp = optnr;

    return arg;
}


/*
 * Parse an integer option.
 */
static int parseInt(char *arg, char opt)
{
    char *endptr;
    int val;

    val = strtol(arg, &endptr, 10);

    if (*arg == '\0' || *endptr != '\0')
        fatal("Invalid integer argument for -%c flag\n", opt);

    return val;
}


/*
 * Append a string to a list of them.
 */
void appendString(stringList **headp, const char *s)
{
    stringList *sl;

    /* Create the new entry. */

    sl = sipMalloc(sizeof (stringList));

    sl -> s = s;
    sl -> next = NULL;

    /* Append it to the list. */

    while (*headp != NULL)
        headp = &(*headp) -> next;

    *headp = sl;
}


/*
 * Display a warning message.
 */
void warning(Warning w, const char *fmt, ...)
{
    static int start = TRUE;

    va_list ap;

    /* Don't allow deprecation warnings to be suppressed. */
    if (!warnings && w != DeprecationWarning)
        return;

    if (start)
    {
        const char *wstr;

        switch (w)
        {
        case ParserWarning:
            wstr = "Parser warning";
            break;

        case DeprecationWarning:
            wstr = "Deprecation warning";
            break;
        }

        fprintf(stderr, "%s: %s: ", sipPackage, wstr);
        start = FALSE;
    }

    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);

    if (strchr(fmt, '\n') != NULL)
    {
        if (warnings_are_fatal)
            exit(1);

        start = TRUE;
    }
}


/*
 * Display all or part of a one line error message describing a fatal error.
 */
void fatal(const char *fmt, ...)
{
    va_list ap;

    fatalStart();

    va_start(ap,fmt);
    vfprintf(stderr,fmt,ap);
    va_end(ap);

    exit(1);
}


/*
 * Make sure the start of a fatal message is handled.
 */
void fatalStart()
{
    static int start = TRUE;

    if (start)
    {
        fprintf(stderr, "%s: ", sipPackage);
        start = FALSE;
    }
}


/*
 * Display the SIP version number on stdout and exit with zero exit status.
 */
static void version(void)
{
    printf("%s\n",sipVersion);
    exit(0);
}


/*
 * Display the help message on stdout and exit with zero exit status.
 */
static void help(void)
{
    printf(
"Usage:\n"
"    %s [-h] [-V] [-a file] [-b file] [-B tag] [-c dir] [-d file] [-D] [-e] [-f] [-g] [-I dir] [-j #] [-k] [-m file] [-n name] [-o] [-p module] [-P] [-r] [-s suffix] [-t tag] [-T] [-w] [-x feature] [-X id:file] [-z file] [@file] [file]\n"
"where:\n"
"    -h          display this help message\n"
"    -V          display the %s version number\n"
"    -a file     the name of the QScintilla API file [default not generated]\n"
"    -b file     the name of the build file [default none generated]\n"
"    -B tag      add tag to the list of timeline backstops\n"
"    -c dir      the name of the code directory [default not generated]\n"
"    -d file     the name of the documentation file (deprecated) [default not generated]\n"
"    -D          generate code for a debug build of Python\n"
"    -e          enable support for exceptions [default disabled]\n"
"    -f          warnings are handled as errors\n"
"    -g          always release and reacquire the GIL [default only when specified]\n"
"    -I dir      look in this directory when including files\n"
"    -j #        split the generated code into # files [default 1 per class]\n"
"    -k          support keyword arguments in functions and methods\n"
"    -m file     the name of the XML export file [default not generated]\n"
"    -n name     the qualified name of the private copy of the sip module\n"
"    -o          enable the automatic generation of docstrings [default disabled]\n"
"    -p module   the name of the consolidated module that this is a component of\n"
"    -P          enable the protected/public hack\n"
"    -r          generate code with tracing enabled [default disabled]\n"
"    -s suffix   the suffix to use for C or C++ source files [default \".c\" or \".cpp\"]\n"
"    -t tag      the version/platform to generate code for\n"
"    -w          enable warning messages\n"
"    -x feature  this feature is disabled\n"
"    -X id:file  create the extracts for an id in a file\n"
"    -y file     the name of the .pyi stub file [default not generated]\n"
"    -z file     the name of a file containing more command line flags\n"
"    @file       the name of a file containing more command line flags\n"
"    file        the name of the specification file [default stdin]\n"
        , sipPackage, sipPackage);

    exit(0);
}


/*
 * Display the usage message.
 */
static void usage(void)
{
    fatal("Usage: %s [-h] [-V] [-a file] [-b file] [-B tag] [-c dir] "
            "[-d file] [-D] [-e] [-f] [-g] [-I dir] [-j #] [-k] [-m file] "
            "[-n name] [-o] [-p module] [-P] [-r] [-s suffix] [-t tag] [-w] "
            "[-x feature] [-X id:file] [-y file] [-z file] [@file] [file]\n",
            sipPackage);
}
