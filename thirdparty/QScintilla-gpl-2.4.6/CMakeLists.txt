INCLUDE( ${QT_USE_FILE} )

INCLUDE("${PROJECT_SOURCE_DIR}/${TulipUseFile}")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} ${QT_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include/ ${CMAKE_CURRENT_SOURCE_DIR}/src/ ${CMAKE_CURRENT_SOURCE_DIR}/Qt4)

SET(LIB_SRCS
    Qt4/qsciscintilla.cpp
    Qt4/qsciscintillabase.cpp    
    Qt4/qsciabstractapis.cpp    
    Qt4/qsciapis.cpp    
    Qt4/qscicommand.cpp    
    Qt4/qscicommandset.cpp    
    Qt4/qscidocument.cpp    
    Qt4/qscilexer.cpp    
    Qt4/qscilexerpython.cpp    
    Qt4/qscimacro.cpp    
    Qt4/qsciprinter.cpp    
    Qt4/qscistyle.cpp    
    Qt4/qscistyledtext.cpp    
    Qt4/SciClasses.cpp    
    Qt4/ListBoxQt.cpp    
    Qt4/PlatQt.cpp    
    Qt4/ScintillaQt.cpp    
    src/AutoComplete.cpp
    src/CallTip.cpp
    src/CellBuffer.cpp
    src/CharClassify.cpp
    src/ContractionState.cpp
    src/Decoration.cpp
    src/Document.cpp
    src/DocumentAccessor.cpp
    src/Editor.cpp
    src/ExternalLexer.cpp
    src/Indicator.cpp
    src/KeyMap.cpp
    src/KeyWords.cpp
    src/LexAbaqus.cpp
    src/LexAPDL.cpp
    src/LexAU3.cpp
    src/LexAVE.cpp
    src/LexAda.cpp
    src/LexAsm.cpp
    src/LexAsn1.cpp
    src/LexASY.cpp
    src/LexBaan.cpp
    src/LexBash.cpp
    src/LexBasic.cpp
    src/LexBullant.cpp
    src/LexCLW.cpp
    src/LexCPP.cpp
    src/LexCSS.cpp
    src/LexCaml.cpp
    src/LexCmake.cpp
    src/LexCOBOL.cpp
    src/LexConf.cpp
    src/LexCrontab.cpp
    src/LexCsound.cpp
    src/LexD.cpp
    src/LexEScript.cpp
    src/LexEiffel.cpp
    src/LexErlang.cpp
    src/LexFlagship.cpp
    src/LexForth.cpp
    src/LexFortran.cpp
    src/LexGAP.cpp
    src/LexGui4Cli.cpp
    src/LexHTML.cpp
    src/LexHaskell.cpp
    src/LexInno.cpp
    src/LexKix.cpp
    src/LexLisp.cpp
    src/LexLout.cpp
    src/LexLua.cpp
    src/LexMagik.cpp
    src/LexMMIXAL.cpp
    src/LexMPT.cpp
    src/LexMSSQL.cpp
    src/LexMatlab.cpp
    src/LexMetapost.cpp
    src/LexMySQL.cpp
    src/LexNimrod.cpp
    src/LexNsis.cpp
    src/LexOpal.cpp
    src/LexOthers.cpp
    src/LexPB.cpp
    src/LexPLM.cpp
    src/LexPOV.cpp
    src/LexPowerPro.cpp
    src/LexPowerShell.cpp
    src/LexProgress.cpp
    src/LexPS.cpp
    src/LexPascal.cpp
    src/LexPerl.cpp
    src/LexPython.cpp
    src/LexR.cpp
    src/LexRebol.cpp
    src/LexRuby.cpp
    src/LexSQL.cpp
    src/LexScriptol.cpp
    src/LexSmalltalk.cpp
    src/LexSML.cpp
    src/LexSorcus.cpp
    src/LexSpecman.cpp
    src/LexSpice.cpp
    src/LexTACL.cpp
    src/LexTADS3.cpp
    src/LexTAL.cpp
    src/LexTCL.cpp
    src/LexTeX.cpp
    src/LexVB.cpp
    src/LexVHDL.cpp
    src/LexVerilog.cpp
    src/LexYAML.cpp
    src/LineMarker.cpp
    src/PerLine.cpp
    src/PositionCache.cpp
    src/PropSet.cpp
    src/RESearch.cpp
    src/RunStyles.cpp
    src/ScintillaBase.cpp
    src/Style.cpp
    src/StyleContext.cpp
    src/ViewStyle.cpp
    src/XPM.cpp
)
    
SET(LIB_MOC
    Qt4/Qsci/qsciscintilla.h
    Qt4/Qsci/qsciscintillabase.h
    Qt4/Qsci/qsciabstractapis.h
    Qt4/Qsci/qsciapis.h
    Qt4/Qsci/qscilexer.h
    Qt4/Qsci/qscilexerpython.h
    Qt4/Qsci/qscimacro.h
    Qt4/SciClasses.h
)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w -DSCI_LEXER -DQT -DPLAT_QT -DQSCINTILLA_MAKE_DLL")
IF(UNIX)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
ENDIF(UNIX)

TULIP_QT4_WRAP_CPP(LIB_MOC_SRCS ${LIB_MOC})
ADD_LIBRARY(qscintilla2 SHARED ${LIB_SRCS} ${LIB_MOC_SRCS})
TARGET_LINK_LIBRARIES(qscintilla2 ${QT_LIBRARIES})

INSTALL(TARGETS qscintilla2
       RUNTIME DESTINATION ${TulipBinInstallDir} COMPONENT qscintilla2
       LIBRARY DESTINATION ${TulipLibInstallDir} COMPONENT qscintilla2
       ARCHIVE DESTINATION ${TulipLibInstallDir} COMPONENT qscintilla2)

INSTALL(FILES 
        ${CMAKE_CURRENT_SOURCE_DIR}/qsci/api/python/Python-2.4.api
        ${CMAKE_CURRENT_SOURCE_DIR}/qsci/api/python/Python-2.5.api
        ${CMAKE_CURRENT_SOURCE_DIR}/qsci/api/python/Python-2.6.api
        ${CMAKE_CURRENT_SOURCE_DIR}/qsci/api/python/Python-2.7.api
        ${CMAKE_CURRENT_SOURCE_DIR}/qsci/api/python/Python-3.1.api
        DESTINATION ${TulipShareInstallDir}/apiFiles 
        COMPONENT qscintilla2)
