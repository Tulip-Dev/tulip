// This module implements the QsciLexerCPP class.
//
// Copyright (c) 2010 Riverbank Computing Limited <info@riverbankcomputing.com>
// 
// This file is part of QScintilla.
// 
// This file may be used under the terms of the GNU General Public
// License versions 2.0 or 3.0 as published by the Free Software
// Foundation and appearing in the files LICENSE.GPL2 and LICENSE.GPL3
// included in the packaging of this file.  Alternatively you may (at
// your option) use any later version of the GNU General Public
// License if such license has been publicly approved by Riverbank
// Computing Limited (or its successors, if any) and the KDE Free Qt
// Foundation. In addition, as a special exception, Riverbank gives you
// certain additional rights. These rights are described in the Riverbank
// GPL Exception version 1.1, which can be found in the file
// GPL_EXCEPTION.txt in this package.
// 
// Please review the following information to ensure GNU General
// Public Licensing requirements will be met:
// http://trolltech.com/products/qt/licenses/licensing/opensource/. If
// you are unsure which license is appropriate for your use, please
// review the following information:
// http://trolltech.com/products/qt/licenses/licensing/licensingoverview
// or contact the sales department at sales@riverbankcomputing.com.
// 
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.


#include "Qsci/qscilexercpp.h"

#include <qcolor.h>
#include <qfont.h>
#include <qsettings.h>


// The ctor.
QsciLexerCPP::QsciLexerCPP(QObject *parent, const char *name,
        bool caseInsensitiveKeywords)
    : QsciLexer(parent, name),
      fold_atelse(false), fold_comments(false), fold_compact(true),
      fold_preproc(true), style_preproc(false), dollars(true),
      nocase(caseInsensitiveKeywords)
{
}


// The dtor.
QsciLexerCPP::~QsciLexerCPP()
{
}


// Returns the language name.
const char *QsciLexerCPP::language() const
{
    return "C++";
}


// Returns the lexer name.
const char *QsciLexerCPP::lexer() const
{
    return (nocase ? "cppnocase" : "cpp");
}


// Return the set of character sequences that can separate auto-completion
// words.
QStringList QsciLexerCPP::autoCompletionWordSeparators() const
{
    QStringList wl;

    wl << "::" << "->" << ".";

    return wl;
}


// Return the list of keywords that can start a block.
const char *QsciLexerCPP::blockStartKeyword(int *style) const
{
    if (style)
        *style = Keyword;

    return "case catch class default do else finally for if private "
           "protected public struct try union while";
}


// Return the list of characters that can start a block.
const char *QsciLexerCPP::blockStart(int *style) const
{
    if (style)
        *style = Operator;

    return "{";
}


// Return the list of characters that can end a block.
const char *QsciLexerCPP::blockEnd(int *style) const
{
    if (style)
        *style = Operator;

    return "}";
}


// Return the style used for braces.
int QsciLexerCPP::braceStyle() const
{
    return Operator;
}


// Return the string of characters that comprise a word.
const char *QsciLexerCPP::wordCharacters() const
{
    return "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_#";
}


// Returns the foreground colour of the text for a style.
QColor QsciLexerCPP::defaultColor(int style) const
{
    switch (style)
    {
    case Default:
        return QColor(0x80,0x80,0x80);

    case Comment:
    case CommentLine:
        return QColor(0x00,0x7f,0x00);

    case CommentDoc:
    case CommentLineDoc:
        return QColor(0x3f,0x70,0x3f);

    case Number:
        return QColor(0x00,0x7f,0x7f);

    case Keyword:
        return QColor(0x00,0x00,0x7f);

    case DoubleQuotedString:
    case SingleQuotedString:
        return QColor(0x7f,0x00,0x7f);

    case PreProcessor:
        return QColor(0x7f,0x7f,0x00);

    case Operator:
    case UnclosedString:
        return QColor(0x00,0x00,0x00);

    case Identifier:
        break;

    case Regex:
        return QColor(0x3f,0x7f,0x3f);

    case CommentDocKeyword:
        return QColor(0x30,0x60,0xa0);

    case CommentDocKeywordError:
        return QColor(0x80,0x40,0x20);
    }

    return QsciLexer::defaultColor(style);
}


// Returns the end-of-line fill for a style.
bool QsciLexerCPP::defaultEolFill(int style) const
{
    if (style == UnclosedString)
        return true;

    return QsciLexer::defaultEolFill(style);
}


// Returns the font of the text for a style.
QFont QsciLexerCPP::defaultFont(int style) const
{
    QFont f;

    switch (style)
    {
    case Comment:
    case CommentLine:
    case CommentDoc:
    case CommentLineDoc:
    case CommentDocKeyword:
    case CommentDocKeywordError:
#if defined(Q_OS_WIN)
        f = QFont("Comic Sans MS",9);
#else
        f = QFont("Bitstream Vera Serif",9);
#endif
        break;

    case Keyword:
    case Operator:
        f = QsciLexer::defaultFont(style);
        f.setBold(true);
        break;

    case DoubleQuotedString:
    case SingleQuotedString:
    case UnclosedString:
#if defined(Q_OS_WIN)
        f = QFont("Courier New",10);
#else
        f = QFont("Bitstream Vera Sans Mono",9);
#endif
        break;

    default:
        f = QsciLexer::defaultFont(style);
    }

    return f;
}


// Returns the set of keywords.
const char *QsciLexerCPP::keywords(int set) const
{
    if (set == 1)
        return
            "and and_eq asm auto bitand bitor bool break case "
            "catch char class compl const const_cast continue "
            "default delete do double dynamic_cast else enum "
            "explicit export extern false float for friend goto if "
            "inline int long mutable namespace new not not_eq "
            "operator or or_eq private protected public register "
            "reinterpret_cast return short signed sizeof static "
            "static_cast struct switch template this throw true "
            "try typedef typeid typename union unsigned using "
            "virtual void volatile wchar_t while xor xor_eq";

    if (set == 3)
        return
            "a addindex addtogroup anchor arg attention author b "
            "brief bug c class code date def defgroup deprecated "
            "dontinclude e em endcode endhtmlonly endif "
            "endlatexonly endlink endverbatim enum example "
            "exception f$ f[ f] file fn hideinitializer "
            "htmlinclude htmlonly if image include ingroup "
            "internal invariant interface latexonly li line link "
            "mainpage name namespace nosubgrouping note overload "
            "p page par param post pre ref relates remarks return "
            "retval sa section see showinitializer since skip "
            "skipline struct subsection test throw todo typedef "
            "union until var verbatim verbinclude version warning "
            "weakgroup $ @ \\ & < > # { }";

    return 0;
}


// Returns the user name of a style.
QString QsciLexerCPP::description(int style) const
{
    switch (style)
    {
    case Default:
        return tr("Default");

    case Comment:
        return tr("C comment");

    case CommentLine:
        return tr("C++ comment");

    case CommentDoc:
        return tr("JavaDoc style C comment");

    case Number:
        return tr("Number");

    case Keyword:
        return tr("Keyword");

    case DoubleQuotedString:
        return tr("Double-quoted string");

    case SingleQuotedString:
        return tr("Single-quoted string");

    case UUID:
        return tr("IDL UUID");

    case PreProcessor:
        return tr("Pre-processor block");

    case Operator:
        return tr("Operator");

    case Identifier:
        return tr("Identifier");

    case UnclosedString:
        return tr("Unclosed string");

    case CommentLineDoc:
        return tr("JavaDoc style C++ comment");

    case KeywordSet2:
        return tr("Secondary keywords and identifiers");

    case CommentDocKeyword:
        return tr("JavaDoc keyword");

    case CommentDocKeywordError:
        return tr("JavaDoc keyword error");

    case GlobalClass:
        return tr("Global classes and typedefs");
    }

    return QString();
}


// Returns the background colour of the text for a style.
QColor QsciLexerCPP::defaultPaper(int style) const
{
    if (style == UnclosedString)
        return QColor(0xe0,0xc0,0xe0);

    return QsciLexer::defaultPaper(style);
}


// Refresh all properties.
void QsciLexerCPP::refreshProperties()
{
    setAtElseProp();
    setCommentProp();
    setCompactProp();
    setPreprocProp();
    setStylePreprocProp();
    setDollarsProp();
}


// Read properties from the settings.
bool QsciLexerCPP::readProperties(QSettings &qs,const QString &prefix)
{
    int rc = true;

    bool ok, flag;

    flag = qs.readBoolEntry(prefix + "foldatelse", false, &ok);

    if (ok)
        fold_atelse = flag;
    else
        rc = false;

    flag = qs.readBoolEntry(prefix + "foldcomments", false, &ok);

    if (ok)
        fold_comments = flag;
    else
        rc = false;

    flag = qs.readBoolEntry(prefix + "foldcompact", true, &ok);

    if (ok)
        fold_compact = flag;
    else
        rc = false;

    flag = qs.readBoolEntry(prefix + "foldpreprocessor", true, &ok);

    if (ok)
        fold_preproc = flag;
    else
        rc = false;

    flag = qs.readBoolEntry(prefix + "stylepreprocessor", false, &ok);

    if (ok)
        style_preproc = flag;
    else
        rc = false;

    flag = qs.readBoolEntry(prefix + "dollars", true, &ok);

    if (ok)
        dollars = flag;
    else
        rc = false;

    return rc;
}


// Write properties to the settings.
bool QsciLexerCPP::writeProperties(QSettings &qs,const QString &prefix) const
{
    int rc = true;

    if (!qs.writeEntry(prefix + "foldatelse", fold_atelse))
        rc = false;

    if (!qs.writeEntry(prefix + "foldcomments", fold_comments))
        rc = false;

    if (!qs.writeEntry(prefix + "foldcompact", fold_compact))
        rc = false;

    if (!qs.writeEntry(prefix + "foldpreprocessor", fold_preproc))
        rc = false;

    if (!qs.writeEntry(prefix + "stylepreprocessor", style_preproc))
        rc = false;

    if (!qs.writeEntry(prefix + "dollars", dollars))
        rc = false;

    return rc;
}


// Return true if else can be folded.
bool QsciLexerCPP::foldAtElse() const
{
    return fold_atelse;
}


// Set if else can be folded.
void QsciLexerCPP::setFoldAtElse(bool fold)
{
    fold_atelse = fold;

    setAtElseProp();
}


// Set the "fold.at.else" property.
void QsciLexerCPP::setAtElseProp()
{
    emit propertyChanged("fold.at.else",(fold_atelse ? "1" : "0"));
}


// Return true if comments can be folded.
bool QsciLexerCPP::foldComments() const
{
    return fold_comments;
}


// Set if comments can be folded.
void QsciLexerCPP::setFoldComments(bool fold)
{
    fold_comments = fold;

    setCommentProp();
}


// Set the "fold.comment" property.
void QsciLexerCPP::setCommentProp()
{
    emit propertyChanged("fold.comment",(fold_comments ? "1" : "0"));
}


// Return true if folds are compact.
bool QsciLexerCPP::foldCompact() const
{
    return fold_compact;
}


// Set if folds are compact
void QsciLexerCPP::setFoldCompact(bool fold)
{
    fold_compact = fold;

    setCompactProp();
}


// Set the "fold.compact" property.
void QsciLexerCPP::setCompactProp()
{
    emit propertyChanged("fold.compact",(fold_compact ? "1" : "0"));
}


// Return true if preprocessor blocks can be folded.
bool QsciLexerCPP::foldPreprocessor() const
{
    return fold_preproc;
}


// Set if preprocessor blocks can be folded.
void QsciLexerCPP::setFoldPreprocessor(bool fold)
{
    fold_preproc = fold;

    setPreprocProp();
}


// Set the "fold.preprocessor" property.
void QsciLexerCPP::setPreprocProp()
{
    emit propertyChanged("fold.preprocessor",(fold_preproc ? "1" : "0"));
}


// Return true if preprocessor lines are styled.
bool QsciLexerCPP::stylePreprocessor() const
{
    return style_preproc;
}


// Set if preprocessor lines are styled.
void QsciLexerCPP::setStylePreprocessor(bool style)
{
    style_preproc = style;

    setStylePreprocProp();
}


// Set the "styling.within.preprocessor" property.
void QsciLexerCPP::setStylePreprocProp()
{
    emit propertyChanged("styling.within.preprocessor",(style_preproc ? "1" : "0"));
}


// Return true if '$' characters are allowed.
bool QsciLexerCPP::dollarsAllowed() const
{
    return dollars;
}


// Set if '$' characters are allowed.
void QsciLexerCPP::setDollarsAllowed(bool allowed)
{
    dollars = allowed;

    setDollarsProp();
}


// Set the "lexer.cpp.allow.dollars" property.
void QsciLexerCPP::setDollarsProp()
{
    emit propertyChanged("lexer.cpp.allow.dollars",(dollars ? "1" : "0"));
}
