ADD_CUSTOM_TARGET(tulip-doc-html ALL sh ${PROJECT_SOURCE_DIR}/docs/common/gen-index.sh)

ADD_CUSTOM_TARGET(tulip-lib-doc ALL doxygen tulip-lib.doxygen)

ADD_CUSTOM_TARGET(tulip-libogl-doc ALL doxygen tulip-ogl-lib.doxygen)

ADD_CUSTOM_TARGET(tulip-libqt-doc ALL doxygen tulip-qt-lib.doxygen)

ADD_CUSTOM_TARGET(tulip-plugins-doc ALL doxygen tulip-plugins.doxygen)

ADD_CUSTOM_TARGET(tulipPlugins-html ALL java -cp "${PROJECT_SOURCE_DIR}/thirdparty/saxon/saxon653.jar:${PROJECT_SOURCE_DIR}/thirdparty/saxon/saxon.jar:${PROJECT_SOURCE_DIR}/docs/common:${PROJECT_SOURCE_DIR}/thirdparty/xml-commons/resolver.jar" com.icl.saxon.StyleSheet -x org.apache.xml.resolver.tools.ResolvingXMLReader -y org.apache.xml.resolver.tools.ResolvingXMLReader -r org.apache.xml.resolver.tools.CatalogResolver -o tulipPlugins.html tulip-plugins.tag  ${PROJECT_SOURCE_DIR}/docs/doxygen/dox-plugins.xsl)

ADD_DEPENDENCIES(tulipPlugins-html tulip-plugins-doc)

ADD_CUSTOM_TARGET(allPluginsBody-html ALL java -cp "${PROJECT_SOURCE_DIR}/thirdparty/saxon/saxon653.jar:${PROJECT_SOURCE_DIR}/thirdparty/saxon/saxon.jar:${PROJECT_SOURCE_DIR}/docs/common:${PROJECT_SOURCE_DIR}/thirdparty/xml-commons/resolver.jar" com.icl.saxon.StyleSheet -x org.apache.xml.resolver.tools.ResolvingXMLReader -y org.apache.xml.resolver.tools.ResolvingXMLReader -r org.apache.xml.resolver.tools.CatalogResolver -o allPluginsBody.html tulip-plugins.tag  ${PROJECT_SOURCE_DIR}/docs/doxygen/dox-html.xsl)

ADD_DEPENDENCIES(allPluginsBody-html tulip-plugins-doc)

ADD_CUSTOM_TARGET(allPlugins-html ALL sh ${PROJECT_SOURCE_DIR}/docs/doxygen/gen-allPlugins.sh)

ADD_DEPENDENCIES(allPlugins-html allPluginsBody-html)

INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/tulip.html
	      ${CMAKE_CURRENT_BINARY_DIR}/tulipdoc.html
	      ${CMAKE_CURRENT_BINARY_DIR}/allPluginsBody.html
	      ${CMAKE_CURRENT_BINARY_DIR}/allPlugins.html
	      ${CMAKE_CURRENT_BINARY_DIR}/tulipPlugins.html
  DESTINATION share/tulip/doxygen
  COMPONENT tulip_doc)

INSTALL(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tulip-lib
		  ${CMAKE_CURRENT_BINARY_DIR}/tulip-ogl-lib
		  ${CMAKE_CURRENT_BINARY_DIR}/tulip-qt-lib
  DESTINATION share/tulip/doxygen
  COMPONENT tulip_doc)
