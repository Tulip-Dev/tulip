all : 

docs : html

check :
	$(XMLLINT) --valid -noout index.docbook

clean :
	rm -f *.html

html : index.html

index.html : index.docbook ../common/tulip-html.xsl ../common/CatalogManager.properties
	$(JAVA) -cp "$(top_builddir)/thirdparty/saxon/saxon653.jar:$(top_builddir)/thirdparty/saxon/saxon.jar:../common:$(top_builddir)/thirdparty/xml-commons/resolver.jar"\
	com.icl.saxon.StyleSheet \
	-x org.apache.xml.resolver.tools.ResolvingXMLReader \
	-y org.apache.xml.resolver.tools.ResolvingXMLReader \
	-r org.apache.xml.resolver.tools.CatalogResolver -u\
	index.docbook  ../common/tulip-html.xsl\
	html.stylesheet="../common/tulip-default.css" \
	use.extensions=1 use.callouts=1 \
	callout.graphics.path="../common/"

pdf :
# # 	$(JAVA) -cp "$(top_srcdir)/thirdparty/saxon/saxon653.jar:$(top_srcdir)/thirdparty/saxon/saxon.jar:../common:/usr/share/java/xml-commons-resolver.jar"\
# # 	com.icl.saxon.StyleSheet -r org.apache.xml.resolver.tools.CatalogResolver -o devHandbook.fo \
# # 	index.docbook  ../common/tulip-fo.xsl html.stylesheet="../common/tulip-default.css" use.extensions=1 use.callouts=1 callout.graphics.path="../common/"  && \
# # 	$(JAVA) -cp "/home/bardet/fop-0.20.5/build/fop.jar:/home/bardet/fop-0.20.5/lib/batik.jar:/home/bardet/fop-0.20.5/lib/xalan-2.4.1.jar:/home/bardet/fop-0.20.5/lib/xercesImpl-2.2.1.jar:/home/bardet/fop-0.20.5/lib/xml-apis.jar:/home/bardet/fop-0.20.5/lib/avalon-framework-cvs-20020806.jar" \
# # 	org.apache.fop.apps.Fop -fo devHandbook.fo -pdf devHandbook.pdf



