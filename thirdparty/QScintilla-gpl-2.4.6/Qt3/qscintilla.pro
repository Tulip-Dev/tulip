# The project file for the QScintilla library.
#
# Copyright (c) 2010 Riverbank Computing Limited <info@riverbankcomputing.com>
# 
# This file is part of QScintilla.
# 
# This file may be used under the terms of the GNU General Public
# License versions 2.0 or 3.0 as published by the Free Software
# Foundation and appearing in the files LICENSE.GPL2 and LICENSE.GPL3
# included in the packaging of this file.  Alternatively you may (at
# your option) use any later version of the GNU General Public
# License if such license has been publicly approved by Riverbank
# Computing Limited (or its successors, if any) and the KDE Free Qt
# Foundation. In addition, as a special exception, Riverbank gives you
# certain additional rights. These rights are described in the Riverbank
# GPL Exception version 1.1, which can be found in the file
# GPL_EXCEPTION.txt in this package.
# 
# Please review the following information to ensure GNU General
# Public Licensing requirements will be met:
# http://trolltech.com/products/qt/licenses/licensing/opensource/. If
# you are unsure which license is appropriate for your use, please
# review the following information:
# http://trolltech.com/products/qt/licenses/licensing/licensingoverview
# or contact the sales department at sales@riverbankcomputing.com.
# 
# This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
# WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.


# This must be kept in sync with configure.py.
!win32:VERSION = 5.5.0

TEMPLATE = lib
TARGET = qscintilla2
CONFIG += qt warn_off release dll thread
INCLUDEPATH = . ../include ../src
DEFINES = QSCINTILLA_MAKE_DLL QT SCI_LEXER

# Handle both Qt v4 and v3.
target.path = $$[QT_INSTALL_LIBS]
isEmpty(target.path) {
	target.path = $(QTDIR)/lib
}

header.path = $$[QT_INSTALL_HEADERS]
header.files = Qsci
isEmpty(header.path) {
	header.path = $(QTDIR)/include/Qsci
	header.files = Qsci/qsci*.h
}

trans.path = $$[QT_INSTALL_TRANSLATIONS]
trans.files = qscintilla_*.qm
isEmpty(trans.path) {
	trans.path = $(QTDIR)/translations
}

qsci.path = $$[QT_INSTALL_DATA]
qsci.files = ../qsci
isEmpty(qsci.path) {
	qsci.path = $(QTDIR)
}

INSTALLS += header trans qsci target

HEADERS = \
	./Qsci/qsciglobal.h \
	./Qsci/qsciscintilla.h \
	./Qsci/qsciscintillabase.h \
	./Qsci/qsciabstractapis.h \
	./Qsci/qsciapis.h \
	./Qsci/qscicommand.h \
	./Qsci/qscicommandset.h \
	./Qsci/qscidocument.h \
	./Qsci/qscilexer.h \
	./Qsci/qscilexerbash.h \
	./Qsci/qscilexerbatch.h \
	./Qsci/qscilexercmake.h \
	./Qsci/qscilexercpp.h \
	./Qsci/qscilexercsharp.h \
	./Qsci/qscilexercss.h \
	./Qsci/qscilexercustom.h \
	./Qsci/qscilexerd.h \
	./Qsci/qscilexerdiff.h \
	./Qsci/qscilexerfortran.h \
	./Qsci/qscilexerfortran77.h \
	./Qsci/qscilexerhtml.h \
	./Qsci/qscilexeridl.h \
	./Qsci/qscilexerjava.h \
	./Qsci/qscilexerjavascript.h \
	./Qsci/qscilexerlua.h \
	./Qsci/qscilexermakefile.h \
	./Qsci/qscilexerpascal.h \
	./Qsci/qscilexerperl.h \
	./Qsci/qscilexerpostscript.h \
	./Qsci/qscilexerpov.h \
	./Qsci/qscilexerproperties.h \
	./Qsci/qscilexerpython.h \
	./Qsci/qscilexerruby.h \
	./Qsci/qscilexerspice.h \
	./Qsci/qscilexersql.h \
	./Qsci/qscilexertcl.h \
	./Qsci/qscilexertex.h \
	./Qsci/qscilexerverilog.h \
	./Qsci/qscilexervhdl.h \
	./Qsci/qscilexerxml.h \
	./Qsci/qscilexeryaml.h \
	./Qsci/qscimacro.h \
	./Qsci/qsciprinter.h \
	./Qsci/qscistyle.h \
	./Qsci/qscistyledtext.h \
	ListBoxQt.h \
	SciClasses.h \
	ScintillaQt.h \
	../include/Accessor.h \
	../include/KeyWords.h \
	../include/Platform.h \
	../include/PropSet.h \
	../include/SString.h \
	../include/SciLexer.h \
	../include/Scintilla.h \
	../include/ScintillaWidget.h \
	../src/AutoComplete.h \
	../src/CallTip.h \
	../src/CellBuffer.h \
	../src/CharClassify.h \
	../src/ContractionState.h \
	../src/Decoration.h \
	../src/Document.h \
	../src/DocumentAccessor.h \
	../src/Editor.h \
	../src/ExternalLexer.h \
	../src/Indicator.h \
	../src/KeyMap.h \
	../src/LineMarker.h \
	../src/Partitioning.h \
	../src/PerLine.h \
	../src/PositionCache.h \
	../src/RESearch.h \
	../src/RunStyles.h \
	../src/SVector.h \
	../src/ScintillaBase.h \
	../src/SplitVector.h \
	../src/Style.h \
	../src/StyleContext.h \
	../src/ViewStyle.h \
	../src/XPM.h

SOURCES = \
	qsciscintilla.cpp \
	qsciscintillabase.cpp \
	qsciabstractapis.cpp \
	qsciapis.cpp \
	qscicommand.cpp \
	qscicommandset.cpp \
	qscidocument.cpp \
	qscilexer.cpp \
	qscilexerbash.cpp \
	qscilexerbatch.cpp \
	qscilexercmake.cpp \
	qscilexercpp.cpp \
	qscilexercsharp.cpp \
	qscilexercss.cpp \
	qscilexercustom.cpp \
	qscilexerd.cpp \
	qscilexerdiff.cpp \
	qscilexerfortran.cpp \
	qscilexerfortran77.cpp \
	qscilexerhtml.cpp \
	qscilexeridl.cpp \
	qscilexerjava.cpp \
	qscilexerjavascript.cpp \
	qscilexerlua.cpp \
	qscilexermakefile.cpp \
	qscilexerpascal.cpp \
	qscilexerperl.cpp \
	qscilexerpostscript.cpp \
	qscilexerpov.cpp \
	qscilexerproperties.cpp \
	qscilexerpython.cpp \
	qscilexerruby.cpp \
	qscilexerspice.cpp \
	qscilexersql.cpp \
	qscilexertcl.cpp \
	qscilexertex.cpp \
	qscilexerverilog.cpp \
	qscilexervhdl.cpp \
	qscilexerxml.cpp \
	qscilexeryaml.cpp \
	qscimacro.cpp \
	qsciprinter.cpp \
	qscistyle.cpp \
	qscistyledtext.cpp \
	SciClasses.cpp \
	ListBoxQt.cpp \
	PlatQt.cpp \
	ScintillaQt.cpp \
	../src/AutoComplete.cpp \
	../src/CallTip.cpp \
	../src/CellBuffer.cpp \
	../src/CharClassify.cpp \
	../src/ContractionState.cpp \
	../src/Decoration.cpp \
	../src/Document.cpp \
	../src/DocumentAccessor.cpp \
	../src/Editor.cpp \
	../src/ExternalLexer.cpp \
	../src/Indicator.cpp \
    ../src/KeyMap.cpp \
	../src/KeyWords.cpp \
	../src/LexAbaqus.cpp \
	../src/LexAPDL.cpp \
	../src/LexAU3.cpp \
	../src/LexAVE.cpp \
	../src/LexAda.cpp \
	../src/LexAsm.cpp \
	../src/LexAsn1.cpp \
	../src/LexASY.cpp \
	../src/LexBaan.cpp \
	../src/LexBash.cpp \
	../src/LexBasic.cpp \
	../src/LexBullant.cpp \
	../src/LexCLW.cpp \
	../src/LexCPP.cpp \
	../src/LexCSS.cpp \
	../src/LexCaml.cpp \
	../src/LexCmake.cpp \
	../src/LexCOBOL.cpp \
	../src/LexConf.cpp \
	../src/LexCrontab.cpp \
	../src/LexCsound.cpp \
	../src/LexD.cpp \
	../src/LexEScript.cpp \
	../src/LexEiffel.cpp \
	../src/LexErlang.cpp \
	../src/LexFlagship.cpp \
	../src/LexForth.cpp \
	../src/LexFortran.cpp \
	../src/LexGAP.cpp \
	../src/LexGui4Cli.cpp \
	../src/LexHTML.cpp \
	../src/LexHaskell.cpp \
	../src/LexInno.cpp \
	../src/LexKix.cpp \
	../src/LexLisp.cpp \
	../src/LexLout.cpp \
	../src/LexLua.cpp \
	../src/LexMagik.cpp \
	../src/LexMMIXAL.cpp \
	../src/LexMPT.cpp \
	../src/LexMSSQL.cpp \
	../src/LexMatlab.cpp \
	../src/LexMetapost.cpp \
	../src/LexMySQL.cpp \
	../src/LexNimrod.cpp \
	../src/LexNsis.cpp \
	../src/LexOpal.cpp \
	../src/LexOthers.cpp \
	../src/LexPB.cpp \
	../src/LexPLM.cpp \
	../src/LexPOV.cpp \
	../src/LexPowerPro.cpp \
	../src/LexPowerShell.cpp \
	../src/LexProgress.cpp \
	../src/LexPS.cpp \
	../src/LexPascal.cpp \
	../src/LexPerl.cpp \
	../src/LexPython.cpp \
	../src/LexR.cpp \
	../src/LexRebol.cpp \
	../src/LexRuby.cpp \
	../src/LexSQL.cpp \
	../src/LexScriptol.cpp \
	../src/LexSmalltalk.cpp \
	../src/LexSML.cpp \
	../src/LexSorcus.cpp \
	../src/LexSpecman.cpp \
	../src/LexSpice.cpp \
	../src/LexTACL.cpp \
	../src/LexTADS3.cpp \
	../src/LexTAL.cpp \
	../src/LexTCL.cpp \
	../src/LexTeX.cpp \
	../src/LexVB.cpp \
	../src/LexVHDL.cpp \
	../src/LexVerilog.cpp \
	../src/LexYAML.cpp \
	../src/LineMarker.cpp \
	../src/PerLine.cpp \
	../src/PositionCache.cpp \
	../src/PropSet.cpp \
    ../src/RESearch.cpp \
	../src/RunStyles.cpp \
    ../src/ScintillaBase.cpp \
	../src/Style.cpp \
	../src/StyleContext.cpp \
	../src/ViewStyle.cpp \
	../src/XPM.cpp

TRANSLATIONS = \
	qscintilla_cs.ts \
	qscintilla_de.ts \
	qscintilla_es.ts \
	qscintilla_fr.ts \
	qscintilla_pt_br.ts \
	qscintilla_ru.ts
