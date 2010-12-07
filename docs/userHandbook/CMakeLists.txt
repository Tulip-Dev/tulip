ADD_CUSTOM_TARGET(user_html ALL java -cp "${PROJECT_SOURCE_DIR}/thirdparty/saxon/saxon653.jar:${PROJECT_SOURCE_DIR}/thirdparty/saxon/saxon.jar:${PROJECT_SOURCE_DIR}/docs/common:${PROJECT_SOURCE_DIR}/thirdparty/xml-commons/resolver.jar" com.icl.saxon.StyleSheet -x org.apache.xml.resolver.tools.ResolvingXMLReader -y org.apache.xml.resolver.tools.ResolvingXMLReader -r org.apache.xml.resolver.tools.CatalogResolver -u ${PROJECT_SOURCE_DIR}/docs/userHandbook/index.docbook ${PROJECT_SOURCE_DIR}/docs/common/tulip-html.xsl)

ADD_CUSTOM_COMMAND(OUTPUT pdf COMMAND mkdir ARGS ${CMAKE_CURRENT_BINARY_DIR}/pdf)
ADD_CUSTOM_COMMAND(OUTPUT images COMMAND ln ARGS -s ${PROJECT_SOURCE_DIR}/docs/userHandbook/images)

ADD_CUSTOM_COMMAND(OUTPUT userManual.fo COMMAND java ARGS -cp "${PROJECT_SOURCE_DIR}/thirdparty/saxon/saxon653.jar:${PROJECT_SOURCE_DIR}/thirdparty/saxon/saxon.jar:${PROJECT_SOURCE_DIR}/docs/common:${PROJECT_SOURCE_DIR}/thirdparty/xml-commons/resolver.jar" com.icl.saxon.StyleSheet -x org.apache.xml.resolver.tools.ResolvingXMLReader -y org.apache.xml.resolver.tools.ResolvingXMLReader -r org.apache.xml.resolver.tools.CatalogResolver -u  -o userManual.fo ${PROJECT_SOURCE_DIR}/docs/userHandbook/index.docbook ${PROJECT_SOURCE_DIR}/docs/common/tulip-pdf.xsl)

ADD_CUSTOM_TARGET(user_pdf ALL pdfxmltex --output-directory="pdf/" userManual.fo DEPENDS images pdf userManual.fo)

INSTALL(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html
		  ${CMAKE_CURRENT_BINARY_DIR}/images 
  DESTINATION share/tulip/userHandbook
  COMPONENT tulip_doc)

INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/pdf/userManual.pdf
  DESTINATION share/tulip/developerHandbook
  COMPONENT tulip_doc)
