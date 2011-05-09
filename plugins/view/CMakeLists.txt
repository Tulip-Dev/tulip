ADD_SUBDIRECTORY(spreadsheet)

#Commented until SIP bindings are updated to fit with latest Tulip API changes
#IF(PYTHONLIBS_FOUND)
#IF(SIP_OK)
#ADD_SUBDIRECTORY(pythonscriptview)
#ENDIF(SIP_OK)
#ENDIF(PYTHONLIBS_FOUND)
