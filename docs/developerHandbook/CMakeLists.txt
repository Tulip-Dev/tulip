ADD_CUSTOM_TARGET(dev_images ALL ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_SOURCE_DIR}/images ${CMAKE_CURRENT_BINARY_DIR}/images)

ADD_CUSTOM_TARGET(dev_html ALL java -cp "${PROJECT_SOURCE_DIR}/thirdparty/saxon/saxon653.jar:${PROJECT_SOURCE_DIR}/thirdparty/saxon/saxon.jar:${PROJECT_SOURCE_DIR}/docs/common:${PROJECT_SOURCE_DIR}/thirdparty/xml-commons/resolver.jar" com.icl.saxon.StyleSheet -x org.apache.xml.resolver.tools.ResolvingXMLReader -y org.apache.xml.resolver.tools.ResolvingXMLReader -r org.apache.xml.resolver.tools.CatalogResolver -u ${PROJECT_SOURCE_DIR}/docs/developerHandbook/index.docbook ${PROJECT_SOURCE_DIR}/docs/common/tulip-html.xsl)

ADD_DEPENDENCIES(dev_html dev_images)

ADD_CUSTOM_TARGET(dev_toc ALL java -cp "${PROJECT_SOURCE_DIR}/thirdparty/saxon/saxon653.jar:${PROJECT_SOURCE_DIR}/thirdparty/saxon/saxon.jar:${PROJECT_SOURCE_DIR}/docs/common:${PROJECT_SOURCE_DIR}/thirdparty/xml-commons/resolver.jar" com.icl.saxon.StyleSheet -x org.apache.xml.resolver.tools.ResolvingXMLReader -y org.apache.xml.resolver.tools.ResolvingXMLReader -r org.apache.xml.resolver.tools.CatalogResolver -o toc.dcf ${PROJECT_SOURCE_DIR}/docs/developerHandbook/index.docbook ${PROJECT_SOURCE_DIR}/docs/common/tulip-toc.xsl install.path="${CMAKE_CURRENT_BINARY_DIR}/html/")

ADD_DEPENDENCIES(dev_toc dev_html) 

ADD_CUSTOM_TARGET(dev_keywords ALL java -cp "${PROJECT_SOURCE_DIR}/thirdparty/saxon/saxon653.jar:${PROJECT_SOURCE_DIR}/thirdparty/saxon/saxon.jar:${PROJECT_SOURCE_DIR}/docs/common:${PROJECT_SOURCE_DIR}/thirdparty/xml-commons/resolver.jar" com.icl.saxon.StyleSheet -x org.apache.xml.resolver.tools.ResolvingXMLReader -y org.apache.xml.resolver.tools.ResolvingXMLReader -r org.apache.xml.resolver.tools.CatalogResolver -o keywords.dcf ${PROJECT_SOURCE_DIR}/docs/developerHandbook/index.docbook ${PROJECT_SOURCE_DIR}/docs/common/tulip-keywords.xsl install.path="${CMAKE_CURRENT_BINARY_DIR}/html/")

ADD_DEPENDENCIES(dev_keywords dev_html)

ADD_CUSTOM_COMMAND(OUTPUT pdf COMMAND mkdir ARGS ${CMAKE_CURRENT_BINARY_DIR}/pdf)
ADD_CUSTOM_COMMAND(OUTPUT devHandbook.fo COMMAND java ARGS -cp "${PROJECT_SOURCE_DIR}/thirdparty/saxon/saxon653.jar:${PROJECT_SOURCE_DIR}/thirdparty/saxon/saxon.jar:${PROJECT_SOURCE_DIR}/docs/common:${PROJECT_SOURCE_DIR}/thirdparty/xml-commons/resolver.jar" com.icl.saxon.StyleSheet -x org.apache.xml.resolver.tools.ResolvingXMLReader -y org.apache.xml.resolver.tools.ResolvingXMLReader -r org.apache.xml.resolver.tools.CatalogResolver -u  -o devHandbook.fo ${PROJECT_SOURCE_DIR}/docs/developerHandbook/index.docbook ${PROJECT_SOURCE_DIR}/docs/common/tulip-pdf.xsl)

ADD_CUSTOM_TARGET(dev_pdf ALL pdfxmltex --output-directory="pdf/" devHandbook.fo DEPENDS pdf devHandbook.fo)

ADD_DEPENDENCIES(dev_pdf dev_images)

INSTALL(DIRECTORY
  ${CMAKE_CURRENT_BINARY_DIR}/html
  ${CMAKE_CURRENT_BINARY_DIR}/images
DESTINATION share/tulip/developerHandbook
COMPONENT tulip_doc)

INSTALL(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/pdf/devHandbook.pdf
DESTINATION share/tulip/developerHandbook
COMPONENT tulip_doc)
