ADD_CUSTOM_TARGET(user_images ALL ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/images ${CMAKE_CURRENT_BINARY_DIR}/images)

ADD_CUSTOM_TARGET(user_html ALL java -cp "${PROJECT_SOURCE_DIR}/thirdparty/saxon/saxon653.jar:${PROJECT_SOURCE_DIR}/thirdparty/saxon/saxon.jar:${PROJECT_SOURCE_DIR}/docs/common:${PROJECT_SOURCE_DIR}/thirdparty/xml-commons/resolver.jar" com.icl.saxon.StyleSheet -x org.apache.xml.resolver.tools.ResolvingXMLReader -y org.apache.xml.resolver.tools.ResolvingXMLReader -r org.apache.xml.resolver.tools.CatalogResolver -u ${PROJECT_SOURCE_DIR}/docs/userHandbook/index.docbook ${PROJECT_SOURCE_DIR}/docs/common/tulip-html.xsl)

ADD_DEPENDENCIES(user_html user_images)

ADD_CUSTOM_TARGET(user_toc ALL java -cp "${PROJECT_SOURCE_DIR}/thirdparty/saxon/saxon653.jar:${PROJECT_SOURCE_DIR}/thirdparty/saxon/saxon.jar:${PROJECT_SOURCE_DIR}/docs/common:${PROJECT_SOURCE_DIR}/thirdparty/xml-commons/resolver.jar" com.icl.saxon.StyleSheet -x org.apache.xml.resolver.tools.ResolvingXMLReader -y org.apache.xml.resolver.tools.ResolvingXMLReader -r org.apache.xml.resolver.tools.CatalogResolver -o toc.dcf ${PROJECT_SOURCE_DIR}/docs/developerHandbook/index.docbook ${PROJECT_SOURCE_DIR}/docs/common/tulip-toc.xsl install.path="userHandbook/html/")

ADD_DEPENDENCIES(user_toc user_html) 

ADD_CUSTOM_TARGET(user_keywords ALL java -cp "${PROJECT_SOURCE_DIR}/thirdparty/saxon/saxon653.jar:${PROJECT_SOURCE_DIR}/thirdparty/saxon/saxon.jar:${PROJECT_SOURCE_DIR}/docs/common:${PROJECT_SOURCE_DIR}/thirdparty/xml-commons/resolver.jar" com.icl.saxon.StyleSheet -x org.apache.xml.resolver.tools.ResolvingXMLReader -y org.apache.xml.resolver.tools.ResolvingXMLReader -r org.apache.xml.resolver.tools.CatalogResolver -o keywords.dcf ${PROJECT_SOURCE_DIR}/docs/developerHandbook/index.docbook ${PROJECT_SOURCE_DIR}/docs/common/tulip-keywords.xsl install.path="userHandbook/html/")

ADD_DEPENDENCIES(user_keywords user_html) 

INSTALL(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html
		  ${CMAKE_CURRENT_BINARY_DIR}/images 
  DESTINATION share/tulip/userHandbook
  COMPONENT tulip_doc)
